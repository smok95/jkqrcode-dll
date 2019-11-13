# jkqrcode-dll
윈도우용 QRCode 생성 C/C++ 라이브러리  

libqrencode 기반으로 만들어졌습니다. 아래 페이지를 참고해 주세요!  
https://fukuchi.org/works/qrencode/  
https://code.google.com/archive/p/qrencode-win32/  
  
    
      
  
  
  
## Usage  
```cpp
#include "jkqrcode.h"  
// 생성할 QRCode 값   
char* value = "hello world!";  
// QRCode 이미지파일 저장경로  
char* outfile = "c:\\temp\\qrcode.png";  

jkqrcode_encode(value, outfile, -1, -1, -1);  
```
