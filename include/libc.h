#include "entry.h"

#ifndef __LIBC_SILICIO__
#define __LIBC_SILICIO__

void init_libc();
unsigned long vbe_get_x();
unsigned long vbe_get_y();
unsigned long vbe_get_color();
char* vbe_get_addr();

#endif
