#include<stdio.h>


typedef struct {
	union lin {
		unsigned char
		b0 :1,
		b1 :1,
	};
} font;
#define B00000000 0
#define B00000001 1
#define B00000010 2
#define B00000100 4
#define B00001000 8
#define B00010000 16
#define B00100000 32
#define B01000000 64
#define B10000000 128
font letra[256];

int main(){
letra['A'].lin.b0 =0;
return(0);
}
