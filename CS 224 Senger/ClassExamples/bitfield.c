#include <stdio.h>

int
main(int argc, char * argv[]) {
  
  union {
    unsigned int i;
    struct {
      unsigned int read : 1;
      unsigned int write : 1;
      unsigned int exec : 1;
    };
  } flags;
  
  printf("flags %0x\n",flags.i);
  
  flags.write = 1;
  printf("flags %0x\n",flags.i);
  printf("flags.write %0x\n",flags.write);
  
  flags.exec = 1;
  printf("flags %0x\n",flags.i);
  printf("flags.write %0x\n",flags.write);
  printf("flags.exec %0x\n",flags.exec);
  
  union {
    float f;
    struct {
      unsigned int mantissa : 23;
      unsigned int exponent : 8;
      unsigned int sign : 1;
    };
  } fval;
  
  fval.sign = 1;            // neg
  fval.exponent = 128;      // exp 2^1
  fval.mantissa = 1 << 22;  // 1.1b = 1.5
  
  printf("float %f\n",fval.f);
  printf("mantissa %x exp %x sign %x\n",fval.mantissa,fval.exponent,fval.sign);
  printf("mantissa %d shifted exp %d sign %d\n",fval.mantissa,fval.exponent - 127,fval.sign);
  
  fval.f = 0.171875; // 1/8 + 1/32 + 1/64, 0.001011, 1.011 * 2^-3, 01100.., 0x3
  
  printf("float %f\n",fval.f);
  printf("mantissa %x exp %x sign %x\n",fval.mantissa,fval.exponent,fval.sign);
  printf("mantissa %d shifted exp %d sign %d\n",fval.mantissa,fval.exponent - 127,fval.sign);
  
}
