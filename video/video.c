#include "entry.h"

#define SIZE_BUF 512


int main(void){
	char arch[]="video";
	sys_param sys;
	int i;
//	char *c;
	char buf[SIZE_BUF];
	int x;
	int y;
	int n;
	int color;
	int addr;
	int pag;

	sys.arch = arch;
	sys.size = SIZE_BUF;
	sys.buf = buf;
//	c = (char*)0xb8000;
	x = 0;
	y = 0;

	// Consulto dirección base de video VGA
	addr = sys_call_speed( SYS_VBE_ADDR,0 , 0, 0);
	sys_call_speed( SYS_DEBUG, addr , 0, 0);

	// Consulto resolución en X
	x = sys_call_speed( SYS_VBE_X,0 , 0, 0);
	sys_call_speed( SYS_DEBUG, x , 0, 0);

	// Consulto resolución en Y
	y = sys_call_speed( SYS_VBE_Y,0 , 0, 0);
	sys_call_speed( SYS_DEBUG, y , 0, 0);

	// Consulto resolución de color
	color = sys_call_speed( SYS_VBE_COLOR,0 , 0, 0);
	sys_call_speed( SYS_DEBUG, color , 0, 0);

/* Tomar páginas de memoria de video para poder escribir */

	// Calculo la cantidad de páginas
	pag = x * y * (color / 8) / 1024;
	pag = pag + 1;
	sys_call_speed( SYS_DEBUG, pag , 0, 0);

	// Toma memoria, en particular 0xb8000 a 0xb8fff
	i = sys_call( SYS_MEM, 0xb8, 0, 0);

	// Toma memoria, en particular 0xb9000 a 0xb9fff
	i = sys_call( SYS_MEM, 0xb9, 0, 0);
x=0;
	// limpiemos la pantalla
/*	for( n=x; n < 4000; n++){
		// caracter de video
		c[n*2]= 0;
		// atributos de video
		c[(n*2)+1]= 7;
	}
*/
	// Registra el dispositivo
	i = sys_call( SYS_REG, (unsigned long) &sys , 0, 0);

	// si i < 0 entonces no pudo reistrarse
	if( i > 0 ){
		do{
			switch (sys.call){
			case SYS_WRITE:
				// recorro el buffer
				for(n=x;n < sys.count ;n++){
sys_call_speed( SYS_DEBUG,(unsigned long) buf[n] & 0xff , 0, 0);
					// caracter de video
//					c[n*2]= buf[n];
					// atributos de video
//					c[(n*2)+1]= 7;
				}
				break;
			case SYS_READ:
				// no realizo accion
				sys.count=0;
				break;
			case SYS_SEEK:
				// no realizo accion
				sys.count=0;
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

