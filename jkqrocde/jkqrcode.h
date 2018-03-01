#ifndef __JKQRCODE_H__

#include <WTypes.h>

#ifdef JKQROCDE_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif /* JKQROCDE_EXPORTS */

#ifdef __cplusplus
extern "C"{
#endif

/*
@brief	qrcode 이미지(png) 생성
@param	text	qrcode로 변환할 문자열
@param	outfile	출력png이미지 파일
@param	margin	여백크기, -1인경우 4로 설정됨.
@param	size	module size in dots(pixels) , -1설정시 기본값 3.
@param	dpi		DPI of the generated PNG, -1 설정시 기본값 72
@return
		0 : 성공
		-1: qrcode생성 실패
		-2: 이미지파일 생성 실패
*/
extern int __stdcall jkqrcode_encode(const char* text, const char* outfile, int margin, int size, int dpi);

/*
@brief	qrcode 이미지(png) 생성
@param	text	qrcode로 변환할 문자열
@param	margin	여백크기, -1인경우 4로 설정됨.
@param	size	module size in dots(pixels) , -1설정시 기본값 3.
@param	dpi		DPI of the generated PNG, -1 설정시 기본값 72
@return
		성공시 생성된 qrcode이미지 파일 경로
		실패시 null
*/
extern BSTR __stdcall jkqrcode_encode2(const char* text, int margin, int size, int dpi);

#ifdef __cplusplus
}
#endif

#endif /* __JKQRCODE_H__ */
