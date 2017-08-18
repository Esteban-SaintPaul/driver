#ifndef __ENTRY__
#include "entry.h"
#endif

#ifndef __SYSCALL__
#include "syscall.h"
#endif

#ifndef __STDIO__
#define __STDIO__

int open(char* arch, char* mode);
void cls ( void );
void putc ( int c );
void printk ( char *format , ... );
void itoa ( char *buf, int format, int dat );
void memcpy( char *orig, char *des, unsigned long cant);
void memset( char *des, unsigned long cant, char dat);

#endif
