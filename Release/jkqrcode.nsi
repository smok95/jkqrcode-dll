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
SectionEnd
