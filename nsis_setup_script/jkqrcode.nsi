;--------------------------------------------------------------------------------------------------
;	2018.03.21 kim,jk	1.1	msvcr100.dll 설치파일에 추가
;--------------------------------------------------------------------------------------------------

!define PRODUCT_VERSION "1.1.0.0"
!define VERSION "1.1.0.0"

VIProductVersion "${PRODUCT_VERSION}"
VIFileVersion "${VERSION}"
VIAddVersionKey "FileVersion" "${VERSION}"

RequestExecutionLevel		admin
XPStyle 					on									; xp manifest 사용여부
SetOverwrite 				on									; 파일 복사시 덮어쓰기
; The name of the installer
Name "jkqrcode"

; The file to write
OutFile "qrcode_setup.exe"

InstallDir $WINDIR

Section install
	SetOutpath $INSTDIR
	File jkqrcode.dll
	File msvcr100.dll
SectionEnd
