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
@brief	qrcode �̹���(png) ����
@param	text	qrcode�� ��ȯ�� ���ڿ�
@param	outfile	���png�̹��� ����
@param	margin	����ũ��, -1�ΰ�� 4�� ������.
@param	size	module size in dots(pixels) , -1������ �⺻�� 3.
@param	dpi		DPI of the generated PNG, -1 ������ �⺻�� 72
@return
		0 : ����
		-1: qrcode���� ����
		-2: �̹������� ���� ����
*/
extern int __stdcall jkqrcode_encode(const char* text, const char* outfile, int margin, int size, int dpi);

/*
@brief	qrcode �̹���(png) ����
@param	text	qrcode�� ��ȯ�� ���ڿ�
@param	margin	����ũ��, -1�ΰ�� 4�� ������.
@param	size	module size in dots(pixels) , -1������ �⺻�� 3.
@param	dpi		DPI of the generated PNG, -1 ������ �⺻�� 72
@return
		������ ������ qrcode�̹��� ���� ���
		���н� null
*/
extern BSTR __stdcall jkqrcode_encode2(const char* text, int margin, int size, int dpi);

#ifdef __cplusplus
}
#endif

#endif /* __JKQRCODE_H__ */
