#include "syscall.h"

#ifndef __ENTRY__

#define __ENTRY__

int sys_call(	unsigned long a,	\
			unsigned long b,	\
			unsigned long c,	\
			unsigned long d		 );


int sys_call_speed(	unsigned long a,	\
			unsigned long b,	\
			unsigned long c,	\
			unsigned long d		 );


#endif
