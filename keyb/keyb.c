#include "entry.h"

#define SIZE_BUF 512
#define SIZE_DAT 5

//static unsigned long c[SIZE_DAT];
//static unsigned long b;
//void man_int();

extern void int_keyb();
extern unsigned long key_dato;
extern unsigned long c_base[SIZE_DAT];
extern unsigned long b_des;


int main(void){

	char arch[]="keyb";	//defino el nombre del dispositivo
	sys_param sys;		//defino la estructura de traspaso de datos
	int i,indice,buf_i;	//variables varias
	char buf[SIZE_BUF];	//buffer de transferencia
	char aux;

	sys.arch = arch;	//puntero a nombre
	sys.size = SIZE_BUF;	//tamaño de buffer
	sys.buf = buf;		//puntero a buffer

	// coloco en cero el indice presente en la interrupcion
//	b=0;
	b_des = 0;

	// limpio el buffer de teclado
//	for( i=0; i < SIZE_DAT; i++){c[i]=0;}
	for( i=0; i < SIZE_DAT; i++){c_base[i]=0;}

	// Pido el puerto 0x60 para manejar el teclado
	i = sys_call_speed( SYS_RPORT, 0x60, 0, 0);

	// registro la funcion main_int() en la int 17
	i = sys_call_speed( SYS_RINT, 17 , (unsigned long) int_keyb, 0);
//	i = sys_call_speed( SYS_RINT, 17 , (unsigned long) man_int, 0);
	//sys_call_speed( SYS_DEBUG, (unsigned long) man_int , 0, 0);

	// Registro el dispositivo
	i = sys_call( SYS_REG, (unsigned long) &sys , 0, 0);

	indice=0;

	// si i < 0 entonces no pudo reistrarse
	if( i > 0 ){
		do{
			switch (sys.call){
			case SYS_WRITE:
				// no permito escribir
				sys.count=0;
				break;
			case SYS_READ:
				buf_i = 0;
				// recorro el buffer
//				while(c[indice]!=0){
				while(c_base[indice]!=0){
//sys_call_speed( SYS_DEBUG, (unsigned long)c[indice] , 0, 0);
//sys_call_speed( SYS_DEBUG, (unsigned long)c[indice] , 0, 0);
//sys_call_speed( SYS_DEBUG, key_dato , 0, 0);

//					buf[buf_i]=(char)c[indice];
					aux = (char) c_base[indice] & 0x000000ff;
					buf[buf_i] = aux;
//					c[indice]= 0;
					c_base[indice]= 0;
					indice++;
					buf_i++;
					if(indice == SIZE_DAT)indice=0;
				}
				sys.count=buf_i;
				break;
			}

			// Retoramos el resultado de la llamada y nos bloqueamos 
			// hasta la siguiente llamada.
			//valor que se retornara al proceso llamador
			sys.ret = sys.count;
			i = sys_call(SYS_RET, (unsigned long) &sys, 0, 0);
		}while(1);
		// No hay que preocuparce, no estara cilclando gastando pulsos reloj
		// el kernel suspende este proceso hasta que tenga otra llamada en
		//SYS_RET
	}
	//	sys_call( SYS_DEBUG, -1 , 0, 0);

	// solo llega aca si no se registra "Que mal!" no podra comunicase.
	while(1);
	return(0);
}


/*
void man_int(){
unsigned long m;



	__asm__ __volatile__("	movl 	$0, %%eax;	\
				inb 	$0x60;		"
				:"=g" (m):: "%eax" );

	if(c[b]==0){
		c[b]=m ;
		b++;
		if(b==SIZE_DAT)b=0;
	}

	return;
}
*/
