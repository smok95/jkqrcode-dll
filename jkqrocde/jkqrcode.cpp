#include "jkqrcode.h"
#include <Windows.h>
#include "qrencode.h"
#include "png.h"
#include <comutil.h>
#pragma comment(lib,"comsuppw.lib")

#define INCHES_PER_METER (100.0/2.54)

static int gVersion = 0;
static QRecLevel glevel = QR_ECLEVEL_L;
static QRencodeMode gHint = QR_MODE_8;
static int gCasesensitive = 1;

static unsigned int fg_color[4] = {0, 0, 0, 255};
static unsigned int bg_color[4] = {255, 255, 255, 255};

static bool writePNG(QRcode* qrcode, const char* outfile, int margin, int size, int dpi);


///////////////////////////////////////////////////////////////////////////////////////////////////

int __stdcall jkqrcode_encode(const char* text, const char* outfile, int margin, int size, int dpi)
{	
	if(margin < 0)	margin = 4;
	if(size < 0)	size = 3;
	if(dpi < 0)		dpi = 72;

	QRcode *qrcode = QRcode_encodeString((char*)text, gVersion, glevel, gHint, gCasesensitive);
	if(!qrcode)
		return -1;
	bool bOk = writePNG(qrcode, outfile, margin, size, dpi);
	
	QRcode_free(qrcode);
	return bOk ? 0:-2;
}

BSTR __stdcall jkqrcode_encode2(const char* text, int margin, int size, int dpi)
{
	if(margin < 0)	margin = 4;
	if(size < 0)	size = 3;
	if(dpi < 0)		dpi = 72;

	static int nIndex =0;
	
	QRcode *qrcode = QRcode_encodeString((char*)text, gVersion, glevel, gHint, gCasesensitive);
	if(!qrcode)
		return NULL;

	char buf[MAX_PATH] = {0,};
	GetTempPath(sizeof(buf), buf);


	if(nIndex >= 100)
		nIndex = 0;

	char szFile[512] = {0,};
	sprintf_s(szFile, "%sjkqrcode%03d.png", buf, nIndex++);	
	bool bOk = writePNG(qrcode, szFile, margin, size, dpi);

	QRcode_free(qrcode);

	BSTR sRet = NULL;

	if(bOk){
		const size_t sLen = strlen(szFile);
		int wslen = MultiByteToWideChar(CP_ACP, 0, szFile, sLen, 0, 0);
		sRet = SysAllocStringLen(0, wslen);
		MultiByteToWideChar(CP_ACP, 0, szFile, sLen, sRet, wslen);		
	}

	{
		char sDbg[1024];
		sprintf(sDbg, "%s, bOk=%d", szFile, bOk);
		OutputDebugString(sDbg);
	}

	return sRet;
}


static bool writePNG(QRcode *qrcode, const char *outfile, int margin, int size, int dpi)
{	
	png_colorp palette = nullptr;
	unsigned char *row = nullptr;
	FILE *fp = nullptr;
	png_structp png_ptr = nullptr;
	png_infop info_ptr = nullptr;
		
	png_byte alpha_values[2];	
	unsigned char *p, *q;
	int x, y, xx, yy, bit;
	int realwidth;

	bool bOk = false;

	realwidth = (qrcode->width + margin * 2) * size;
	row = (unsigned char *)malloc((realwidth + 7) / 8);
	if(!row){
		OutputDebugString("Failed to allocate memory.\n");
		goto clean;
	}

	// avoid clobbering by setjmp.
	errno_t err = fopen_s(&fp, outfile, "wb");
	if(err != 0){
		char buf[1024]={0,};
		sprintf_s(buf, "Failed to create file: %s\n", outfile);
		OutputDebugString(buf);
		goto clean;
	}
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		OutputDebugString("Failed to initialize PNG writer.\n");
		goto clean;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		OutputDebugString("Failed to initialize PNG write.\n");
		goto clean;
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		OutputDebugString("Failed to write PNG image.\n");
		goto clean;
	}

	palette = (png_colorp) malloc(sizeof(png_color) * 2);
	if(!palette){
		OutputDebugString("Failed to allocate memory.\n");
		goto clean;
	}

	palette[0].red   = fg_color[0];
	palette[0].green = fg_color[1];
	palette[0].blue  = fg_color[2];
	palette[1].red   = bg_color[0];
	palette[1].green = bg_color[1];
	palette[1].blue  = bg_color[2];
	alpha_values[0] = fg_color[3];
	alpha_values[1] = bg_color[3];
	png_set_PLTE(png_ptr, info_ptr, palette, 2);
	png_set_tRNS(png_ptr, info_ptr, alpha_values, 2, NULL);

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr,
		realwidth, realwidth,
		1,
		PNG_COLOR_TYPE_PALETTE,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	png_set_pHYs(png_ptr, info_ptr, (png_uint_32)(dpi * INCHES_PER_METER), (png_uint_32)(dpi * INCHES_PER_METER),PNG_RESOLUTION_METER);
	png_write_info(png_ptr, info_ptr);

	/* top margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	/* data */
	p = qrcode->data;
	for(y=0; y<qrcode->width; y++) {
		bit = 7;
		memset(row, 0xff, (realwidth + 7) / 8);
		q = row;
		q += margin * size / 8;
		bit = 7 - (margin * size % 8);
		for(x=0; x<qrcode->width; x++) {
			for(xx=0; xx<size; xx++) {
				*q ^= (*p & 1) << bit;
				bit--;
				if(bit < 0) {
					q++;
					bit = 7;
				}
			}
			p++;
		}
		for(yy=0; yy<size; yy++) {
			png_write_row(png_ptr, row);
		}
	}
	/* bottom margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin * size; y++) {
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	bOk = true;
clean:
	if(fp)		fclose(fp);
	if(row)		free(row);
	if(palette) free(palette);

	return bOk;
}
