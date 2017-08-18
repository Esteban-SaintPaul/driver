#include "entry.h"

int sys_call(	unsigned long a,	\
			unsigned long b,	\
			unsigned long c,	\
			unsigned long d		 ){

	int ret;
	unsigned long r1,r2,r3;	

	__asm__ __volatile__ (" movl	%%ebx, %0;	\
				movl	%%ecx, %1;	\
				movl	%%edx, %2;"
				:"=g"(r1), "=g"(r2), "=g"(r3));

	__asm__ __volatile__ ("	movl	%1,	%%eax;	\
				movl	%2,	%%ebx;	\
				movl	%3,	%%ecx;	\
				movl	%4,	%%edx;	\
				int 	$0x40;		\
				movl	%%eax,	%0" 	\
				:"=g" (ret)		\
				:"g" (a), "g" (b), "g" (c),"g" (d));

	__asm__ __volatile__ (" movl	%0, %%ebx;	\
				movl	%1, %%ecx;	\
				movl	%2, %%edx;"
				::"g"(r1), "g"(r2), "g"(r3));

	return(ret);
}

int sys_call_speed(	unsigned long a,	\
			unsigned long b,	\
			unsigned long c,	\
			unsigned long d		 ){

	int ret;
	unsigned long r1,r2,r3;	

	__asm__ __volatile__ (" movl	%%ebx, %0;	\
				movl	%%ecx, %1;	\
				movl	%%edx, %2;"
				:"=g"(r1), "=g"(r2), "=g"(r3));

	__asm__ __volatile__ ("	movl	%1,	%%eax;	\
				movl	%2,	%%ebx;	\
				movl	%3,	%%ecx;	\
				movl	%4,	%%edx;	\
				int 	$0x41;		\
				movl	%%eax,	%0" 	\
				:"=g" (ret)		\
				:"g" (a), "g" (b), "g" (c),"g" (d));

	__asm__ __volatile__ (" movl	%0, %%ebx;	\
				movl	%1, %%ecx;	\
				movl	%2, %%edx;"
				::"g"(r1), "g"(r2), "g"(r3));

	return(ret);
}
