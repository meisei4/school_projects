#include <stdio.h>

void
b2Bin(unsigned char data, char * rep) {
  for (int i = 0; i < 8; i++) {
    rep[7-i] = (data & 0x1) ? '1' : '0';
    data = data >> 1;
  }
  rep[8] = '\0';
}

int
main(int argc, char * argv[]) {
  
  char bstr[9];
  
  printf("            Dec Hex Binary\n");
  
  printf("unsigned\n");
  
  unsigned char c = 0x9c;
  b2Bin(c,bstr);
  printf("            %3d  %2x %s\n",c,c,bstr);

  c = c >> 3;
  b2Bin(c,bstr);
  printf(">> 3        %3d  %2x %s\n",c,c,bstr);

  c = ~c;
  b2Bin(c,bstr);
  printf("~           %3d  %2x %s\n",c,c,bstr);
  
  printf("\nsigned\n");
  
  signed char d = 0xb9;
  b2Bin(d,bstr);
  printf("            %3d  %2x %s\n",d,d&0xFF,bstr);
  d >>= 1;
  b2Bin(d,bstr);
  printf(">> 1        %3d  %2x %s\n",d,d&0xff,bstr);
  
  printf("\nmasks\n");
  
  unsigned char e = 0x01 | 0x04 | 0x10 | 0x40 ;
  b2Bin(e,bstr);
  printf("            %3d  %2x %s\n",e,e,bstr);
  
  e = e | 0x08;
  b2Bin(e,bstr);
  printf("set | 0x08  %3d  %2x %s\n",e,e,bstr);
  
  e = e & 0x08;
  b2Bin(e,bstr);
  printf("test & 0x08 %3d  %2x %s\n",e,e,bstr);
  
  printf("\n");
  
  e = 0x01 | 0x04 | 0x10 | 0x40 ;
  b2Bin(e,bstr);
  printf("            %3d  %2x %s\n",e,e,bstr);
  
  e = e & 0x0f;
  b2Bin(e,bstr);
  printf("& 0x0f      %3d  %2x %s\n",e,e,bstr);
  
  e = e | 0x0f;
  b2Bin(e,bstr);
  printf("| 0x0f      %3d  %2x %s\n",e,e,bstr);
  
  e = e ^ 0xff;
  b2Bin(e,bstr);
  printf("^ 0xff      %3d  %2x %s\n",e,e,bstr);
  
  e = e ^ 0x00;
  b2Bin(e,bstr);
  printf("^ 0x00      %3d  %2x %s\n",e,e,bstr);
  
  
  printf("\n");
  unsigned char a = 0x45;
  unsigned char b = 0x91;
  b2Bin(a,bstr);
  printf("a           %3d  %2x %s\n",a,a,bstr);
  b2Bin(b,bstr);
  printf("b           %3d  %2x %s\n",b,b,bstr);
  unsigned char x = a ^ b;
  b2Bin(x,bstr);
  printf("a ^ b       %3d  %2x %s\n",x,x,bstr);
  x = x ^ b;
  b2Bin(x,bstr);
  printf("prev ^ b    %3d  %2x %s\n",x,x,bstr);

}












