#include "stdio.h"
#include "unistd.h"

int main(void){
	char file[]="/dev/video";
	char file2[]="/dev/keyb";
	char modo[]="w";
	char text[200];
	int stdout,i,stdin;

	char textstart[]="Proceso Start [ iniciado ]\n";

	// Retardo para dar tiempo a el registro de los drivers, esto
	// es para no tener que llamar  a open en loop hasta que esten listos
	for(i=0;i<6500000;i++){;}//retardo

	stdin = open (file2, modo);

	stdout = open (file, modo);
//	sys_call_speed( SYS_DEBUG, arch, 0, 0 );

	// Imprimo un texto para saber que se inició Start.bin
	write( stdout, textstart, 27);
text[0]=166;
text[1]=236;
text[2]=168;
text[3]=44;
text[4]=182;
text[5]=0;
i = write( stdout, text, 5);

	while(1){
		for(i=0;i<65000;i++){;}//retardo

		i = read (stdin, text, 2);
//sys_call_speed( SYS_HLT,0, 0, 0 );

		if(i != 0){
			i = write( stdout, text, i);
		}

	}
	return(0);
}

