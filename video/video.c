#include "libc.h"

#define SIZE_BUF 512

	typedef struct {
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		unsigned char opt;
	}rgb32;

	typedef struct {
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	}rgb24;

void vbe_cls ( void );
static char* dir;
static int color;
static int x;
static int y;
static int xy;
static int xpos;
static int ypos;

int main(void){
	char arch[]="video";
	unsigned long pagina;

	sys_param sys;
	int i;
	char buf[SIZE_BUF];
	int n;
	int pag;


	sys.arch = arch;
	sys.size = SIZE_BUF;
	sys.buf = buf;
	x = 0;
	y = 0;
	xpos = 0;
	ypos = 0;


	// Iniciamos las variables de la librería de C, esto se deberá incluir
	// en un archivo que sea transparente al programador en un futuro
	init_libc();
	dir = vbe_get_addr();
//	sys_call_speed( SYS_DEBUG,(unsigned long) dir , 0, 0);
	color = vbe_get_color();
//	sys_call_speed( SYS_DEBUG,(unsigned long) color , 0, 0);
	x = vbe_get_x();
//	sys_call_speed( SYS_DEBUG,(unsigned long) x , 0, 0);
	y = vbe_get_y();
//	sys_call_speed( SYS_DEBUG,(unsigned long) y , 0, 0);
	xy = x * y;

	// Calculo la cantidad de páginas
	pag = x * y * (color / 8) / 4096;
	pag = pag + 1;
	pagina = (unsigned long ) dir / 4096;

	/* Tomar páginas de memoria de video para poder escribir */
	for( n=0; n < pag ; n++, pagina++){
		i = sys_call( SYS_MEM, pagina, 0, 0);
	}

	// limpiemos la pantalla
	vbe_cls();

//x=0;

	// Registra el dispositivo
	i = sys_call( SYS_REG, (unsigned long) &sys , 0, 0);

	// si i < 0 entonces no pudo reistrarse
	if( i > 0 ){
		do{
			switch (sys.call){
			case SYS_WRITE:
				// recorro el buffer
				for(n=0;n < sys.count ;n++){
sys_call_speed( SYS_DEBUG,(unsigned long) buf[n] & 0xff , 0, 0);
//					vbe_putc( buf[n] );
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

/*#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vbe.h>
#include <font.h>
*/

//extern s_vbe_header vbe_header;
//extern s_font font;

//#define MAX_STRING 1024
//#define VIDEO 0xb8000
/*
//static volatile unsigned char *video = (unsigned char *) VIDEO;

void vbe_bmp2graf(int c, char *tab, unsigned long x,unsigned long \
		y,unsigned long r, unsigned long g, unsigned long b);


void vbe_itoa (char *buf, int base, int d)
{
	char a[20];
	char *p;
	unsigned long ud; // unsigned long original
	int divisor = 10;
	int resto;
	int i;

	ud = d;
	p = buf;
	i = 0;

	if (base == 'd' && d < 0) // cambio el signo
	{
		ud = -d;
	}
	else if (base == 'x')
	{
		divisor = 16;
	}

	do
	{
		resto = ud % divisor;
		ud = ud / divisor;
		if (resto < 10)
		{
			 resto = resto + '0';
		}
		else
		{
			 resto = resto + 'a' - 10;
		}
		a[i] = resto;
		i++;
	}while ( ud != 0);
	if (base == 'd' && d < 0) // cambio el signo
	{
		a[i] = '-';
		i++;
	}
	while (i != 0)
	{
		i--;
		*p = a[i];
		p++;
	}
	*p = 0;
}
*/
void vbe_cls ( void )
{
	int i;
	rgb24 *p24;
	rgb32 *p32;
	char *video;

	if(color == 0){
		video =  dir;
		for ( i=0; i< xy * 2;i++ )
		{
			*video = 0;
			video++;
		}
	}
	if(color == 24){
		p24 = (rgb24 *) dir;
		for ( i=0; i< xy ;i++ )
		{
			p24->red = 0;
			p24->green = 0xf;
			p24->blue = 0;
			p24++;
		}
	}
	if(color == 32){
		p32 = (rgb32 *) dir;
		for ( i=0; i< xy ;i++ )
		{
			p32->red = 0;
			p32->green = 0x0f;
			p32->blue = 0;
			p32++;
		}
	}
	xpos = 0;
	ypos = 0;
}
/*
unsigned long strcmp(char* buf, char* str)
{
	unsigned long ret,lb,ls,i;

	lb = strlen( buf );
	ls = strlen( str );
	if ( lb != 0 && ls != 0 )
	{
		if ( lb == ls )
		{
			i = 0;
			while(buf[i] != 0 && (buf[i] - str[i]) == 0)
			{
				i++;
			}
			if ( i == lb ) ret = 0; 
		}
		else
		{
			ret = 1;
		}
	}
	else
	{
		ret = 1;
	}
	return ret;
}


// strlen() funcion que devuelve el largo de una cadena o cero en caso \
//		de error o cadena nula

unsigned long strlen(char *str)
{
	unsigned long cont=0;
	while( str[cont] != 0 && cont < 0xffffffff )
	{
		cont++;
	}
	if(cont == 0xffffffff) cont = 0;
	return (cont);
}

unsigned long strcpy( char* origen, char* destino){
	unsigned long largo;
	unsigned long i;

	largo=strlen(origen);
	if(largo == 0) return(0);
	if(largo > MAX_STRING) return(0);
	for( i=0; i < largo; i++){
		destino[i] = origen[i];
	}
	return(largo);
}

unsigned long strcat( char* base, char* agr){
	unsigned long a,b;
	a=strlen(base);
	b=strlen(agr);
	if( (a+b) > MAX_STRING) return(1);
	strcpy( agr, &base[a]);
	return(0);
}


void vbe_putc ( int c )
{
	unsigned long x, y, z, a, b;

	if(vbe_header.color != 0){
		x = vbe_header.x / font.x;
		y = vbe_header.y / font.y;
	}else{
		x = vbe_header.x;
		y = vbe_header.y;
	}
	if ( c == '\n' )
	{
		newline:
			xpos = 0;
			ypos++;
			if ( ypos >= y ){
				ypos--;

			    if(vbe_header.color != 0){
				a= (unsigned long) vbe_header.addr;
				b= font.y;
				c= vbe_header.x * vbe_header.color / 8;
				z= a + (c * b);
				b= vbe_header.y - font.y;
				memcpy((void *)z , vbe_header.addr ,( b * c));

				z= a + (b * c);
				b= font.y * vbe_header.x;
				c= vbe_header.color / 8;
				a= b * c;
				memset( (char *)z , a,	0);
			    }
			    else{
				a= (unsigned long) vbe_header.addr;
				b= vbe_header.x * 2;
				z= a + b;
				c= vbe_header.y - 1;
				memcpy((void *)z , vbe_header.addr ,( b * c * 2));
			    }
		}
		return;
	}
	if ( c == '\r' )
	{
			xpos = 0;
		return;
	}
	if(vbe_header.color == 0){
	*( vbe_header.addr + (xpos + ypos * x) * 2 ) = c & 0xff;
	*( vbe_header.addr + (xpos + ypos * x) * 2 +1 ) = 0x7;
	}else{
		bmp2graf( c, font.addr , (xpos * font.x), (ypos * font.y),\
				0xf0,0xf0,0x80 );
	}
	xpos++;
	if (xpos >= x)
		goto newline;

}

void bmp2graf(int c, char *tab, unsigned long x,unsigned long \
		y,unsigned long r, unsigned long g, unsigned long b){

	unsigned long y_aux, x_aux, cor;
	char *let;
	rgb24 *pixel24;
	rgb32 *pixel32;

	let = &tab[c * font.y];
	cor = (y * vbe_header.x) + x;

	if(vbe_header.color == 24){
		for(y_aux=0; y_aux < font.y; y_aux++){
			pixel24=(rgb24 *)vbe_header.addr + \
				(y_aux * vbe_header.x) + cor;
			for(x_aux=0;x_aux < font.x;x_aux++){
				if( ((let[y_aux]>> x_aux) & 0x1) == 0x1){
					pixel24->blue=b;
					pixel24->green=g;
					pixel24->red=r;
				}
				pixel24++;;
			}
		}
	}else if(vbe_header.color == 32){
		for(y_aux=0; y_aux < font.y; y_aux++){
			pixel32=(rgb32 *)vbe_header.addr + \
				(y_aux * vbe_header.x) + cor;
			for(x_aux=0;x_aux < font.x;x_aux++){
				if( ((let[y_aux]>> x_aux) & 0x1) == 0x1){
					pixel32->blue=b;
					pixel32->green=g;
					pixel32->red=r;
				}
				pixel32++;;
			}
		}
	}
}

void memcpy(char *orig, char *des, unsigned long cant){
	while(cant > 0){
		*des = *orig;
		cant--;
		orig++;
		des++;
	}
}

void memset(char *des,unsigned long cant, char dat){
	while(cant > 0){
		*des = dat;
		des++;
		cant--;
	}
}
*/

