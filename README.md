# jkqrcode-dll
윈도우용 QRCode 생성 라이브러리  

libqrencode 기반으로 만들어졌습니다. 아래 페이지를 참고해 주세요!  
https://fukuchi.org/works/qrencode/  
https://code.google.com/archive/p/qrencode-win32/  
  
    
      
  
  
  
## Usage  
> #include "jkqrcode.h"  
> char* value = "hello world!";  
> char* outfile = "c:\\temp\\qrcode.png";  
> jkqrcode_encode(value, outfile, -1, -1, -1);  
