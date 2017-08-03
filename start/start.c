//#include <entry.h>
#include "stdio.h"

int main(void){
	char file[]="/dev/video";
	char file2[]="/dev/keyb";
	char modo[]="w";
	char text[200];
	int arch,i,arch2;
//	unsigned long aux;

//	char textstart[]="Iniciando start.bin";

	// Retardo para dar tiempo a el registro de los drivers, esto
	// es para no tener que llamar  a open en loop hasta que esten listos

	do{
		arch2 = sys_call( SYS_OPEN, (unsigned long)file2, (unsigned long)modo, 0);	
	}while(arch2 < 0);
//	sys_call_speed( SYS_DEBUG, arch2, 0, 0 );

	do{
		arch = sys_call( SYS_OPEN, (unsigned long)file, (unsigned long)modo, 0);
	}while(arch < 0);
//	sys_call_speed( SYS_DEBUG, arch, 0, 0 );

//	i = sys_call( SYS_WRITE, (unsigned long)arch, (unsigned long)textstart, 20);
//	sys_call_speed( SYS_DEBUG, i, 0, 0 );

//	i=sys_call_speed( SYS_VBE_ADDR, 0, 0, 0 );
//	sys_call( SYS_WRITE, (unsigned long)arch, (unsigned long)textstart, 19);
//	sys_call_speed( SYS_DEBUG, i, 0, 0 );

	for(i=0;i<6500000;i++){;}//retardo

	while(1){
		for(i=0;i<65000;i++){;}//retardo

		i = sys_call( SYS_READ, (unsigned long)arch2, \
				(unsigned long)text, 2 );
sys_call_speed( SYS_HLT,0, 0, 0 );

		if(i != 0){
			i = sys_call( SYS_WRITE,\
				(unsigned long)arch, (unsigned long)text, i );
//aux = 0;
//aux = text[0];
		}

	}
	return(0);
}

