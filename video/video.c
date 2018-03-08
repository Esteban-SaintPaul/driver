#include "libc.h"

#define SIZE_BUF 512
#define MAX_STRING 1024

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

typedef struct {
	char *addr; // apuntador a tabla de bmp
	unsigned long x; // alto de letra en pixels
	unsigned long y; // ancho de letra
} s_font;

/*para comenzar a crear caracteres*/
/* Tabla de fuentes bmp 8x16 bits, 256 caracteres representables*/
//static char f[16 * 256];
static s_font font;



void vbe_cls ( void );
static char* dir;
static int color;
static int x;
static int y;
static int xy;
static int xpos;
static int ypos;

/*función que carga una fuente para modo grafico*/
void vbe_font(s_font *fo);
void vbe_memcpy(char *orig, char *des, unsigned long cant);
void vbe_memset(char *des,unsigned long cant, char dat);
void vbe_bmp2graf(int c, char *tab, unsigned long x,unsigned long \
		y,unsigned long r, unsigned long g, unsigned long b);
unsigned long vbe_strlen(char *str);
void vbe_putc ( int c );
static char* f;


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

//i = 28;
//sys_call_speed( SYS_DEBUG,i , 0, 0);

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

	// Pido una página de memoria (4096 bites)
	i = sys_call( SYS_MEM, 0, 0, 0); // En i tenemos el número de página
//sys_call_speed( SYS_DEBUG,i , 0, 0);
	i = i << 12;			// lo convertimos en direccón
	f = (char *) i;			// y se lo asignamos al puntero f
//sys_call_speed( SYS_DEBUG,i , 0, 0);
	vbe_font(&font);		// cargamos los mapas de bits de cada letra y símbolo imprimible

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

	// Registra el dispositivo
	i = sys_call( SYS_REG, (unsigned long) &sys , 0, 0);

	// si i < 0 entonces no pudo reistrarse
	if( i > 0 ){
		do{
			switch (sys.call){
			case SYS_WRITE:
				// recorro el buffer
				for(n=0;n < sys.count ;n++){
//sys_call_speed( SYS_DEBUG,(unsigned long) buf[n] & 0xff , 0, 0);
					vbe_putc( buf[n] & 0xff );

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

void vbe_putc ( int c )
{
	unsigned long lx, ly, z, a, b;

	if(color != 0){
		lx = x / font.x;
		ly = y / font.y;
	}else{
		lx = x;
		ly = y;
	}
	if ( c == '\n' )
	{
		newline:
			xpos = 0;
			ypos++;
			if ( ypos >= ly ){
				ypos--;

			    if(color != 0){
				a= (unsigned long) dir;
				b= font.y;
				c= x * color / 8;
				z= a + (c * b);
				b= y - font.y;
				vbe_memcpy((void *)z , dir ,( b * c));

				z= a + (b * c);
				b= font.y * x;
				c= color / 8;
				a= b * c;
				vbe_memset( (char *)z , a,	0);
			    }
			    else{
				a= (unsigned long) dir;
				b= x * 2;
				z= a + b;
				c= y - 1;
				vbe_memcpy((void *)z , dir ,( b * c * 2));
			    }
		}
		return;
	}
	if ( c == '\r' )
	{
			xpos = 0;
		return;
	}
	if(color == 0){
	*( dir + (xpos + ypos * x) * 2 ) = c & 0xff;
	*( dir + (xpos + ypos * x) * 2 +1 ) = 0x7;
	}else{
		vbe_bmp2graf( c, font.addr , (xpos * font.x), (ypos * font.y),\
				0xf0,0xf0,0x80 );
	}
	xpos++;
	if (xpos >= lx)
		goto newline;

}

void vbe_bmp2graf(int c, char *tab, unsigned long lx,unsigned long \
		ly,unsigned long r, unsigned long g, unsigned long b){

	unsigned long y_aux, x_aux, cor;
	char *let;
	rgb24 *pixel24;
	rgb32 *pixel32;

	let = &tab[c * font.y];
	cor = (ly * x) + lx;

	if(color == 24){
		for(y_aux=0; y_aux < font.y; y_aux++){
			pixel24=(rgb24 *)dir + \
				(y_aux * x) + cor;
			for(x_aux=0;x_aux < font.x;x_aux++){
				if( ((let[y_aux]>> x_aux) & 0x1) == 0x1){
					pixel24->blue=b;
					pixel24->green=g;
					pixel24->red=r;
				}
				pixel24++;;
			}
		}
	}else if(color == 32){
		for(y_aux=0; y_aux < font.y; y_aux++){
			pixel32=(rgb32 *)dir + \
				(y_aux * x) + cor;
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



void vbe_font(s_font *fo){
	unsigned long i;
	fo->addr = f;
	fo->x= 8;
	fo->y=16;
i=12;

	for( i=0; i< (256*16); i+=16 ){
//if( i == 16*30 )sys_call_speed( SYS_DEBUG,i , 0, 0);

		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x0;
		f[i+8]=0x3f;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x0;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	}


	i=16 * 'a';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1e;
		f[i+6]=0x21;
		f[i+7]=0x20;
		f[i+8]=0x2e;
		f[i+9]=0x31;
		f[i+10]=0x21;
		f[i+11]=0x31;
		f[i+12]=0x2e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'b';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x01;
		f[i+4]=0x01;
		f[i+5]=0x1d;
		f[i+6]=0x23;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x23;
		f[i+12]=0x1d;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'c';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1e;
		f[i+6]=0x21;
		f[i+7]=0x01;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'd';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x20;
		f[i+4]=0x20;
		f[i+5]=0x2e;
		f[i+6]=0x31;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x31;
		f[i+12]=0x2e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'e';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1e;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x3f;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'f';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x18;
		f[i+4]=0x24;
		f[i+5]=0x04;
		f[i+6]=0x0e;
		f[i+7]=0x04;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x04;
		f[i+11]=0x04;
		f[i+12]=0x04;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'g';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x2e;
		f[i+6]=0x31;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x31;
		f[i+12]=0x2e;
		f[i+13]=0x20;
		f[i+14]=0x21;
		f[i+15]=0x1e;
	i=16 * 'h';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x01;
		f[i+4]=0x01;
		f[i+5]=0x1d;
		f[i+6]=0x23;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;

	i=16 * 'i';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x8;
		f[i+4]=0x0;
		f[i+5]=0x0c;
		f[i+6]=0x8;
		f[i+7]=0x8;
		f[i+8]=0x8;
		f[i+9]=0x8;
		f[i+10]=0x8;
		f[i+11]=0x8;
		f[i+12]=0x1c;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'j';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x20;
		f[i+4]=0x0;
		f[i+5]=0x30;
		f[i+6]=0x20;
		f[i+7]=0x20;
		f[i+8]=0x20;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x20;
		f[i+12]=0x20;
		f[i+13]=0x21;
		f[i+14]=0x21;
		f[i+15]=0x1e;
	i=16 * 'k';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x1;
		f[i+4]=0x1;
		f[i+5]=0x9;
		f[i+6]=0x5;
		f[i+7]=0x3;
		f[i+8]=0x3;
		f[i+9]=0x5;
		f[i+10]=0x09;
		f[i+11]=0x11;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'l';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x2;
		f[i+4]=0x2;
		f[i+5]=0x2;
		f[i+6]=0x2;
		f[i+7]=0x2;
		f[i+8]=0x2;
		f[i+9]=0x2;
		f[i+10]=0x2;
		f[i+11]=0x2;
		f[i+12]=0x1c;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'm';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1d;
		f[i+6]=0x2a;
		f[i+7]=0x2a;
		f[i+8]=0x2a;
		f[i+9]=0x2a;
		f[i+10]=0x2a;
		f[i+11]=0x2a;
		f[i+12]=0x2a;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'n';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1d;
		f[i+6]=0x22;
		f[i+7]=0x22;
		f[i+8]=0x22;
		f[i+9]=0x22;
		f[i+10]=0x22;
		f[i+11]=0x22;
		f[i+12]=0x22;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'o';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1e;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'p';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1d;
		f[i+6]=0x23;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x23;
		f[i+12]=0x1d;
		f[i+13]=0x1;
		f[i+14]=0x1;
		f[i+15]=0x1;
	i=16 * 'q';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x2e;
		f[i+6]=0x31;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x31;
		f[i+12]=0x2e;
		f[i+13]=0x20;
		f[i+14]=0x20;
		f[i+15]=0x20;
	i=16 * 'r';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1d;
		f[i+6]=0x23;
		f[i+7]=0x01;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x01;
		f[i+12]=0x01;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 's';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x1e;
		f[i+6]=0x21;
		f[i+7]=0x01;
		f[i+8]=0x06;
		f[i+9]=0x18;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 't';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x04;
		f[i+4]=0x04;
		f[i+5]=0x3f;
		f[i+6]=0x04;
		f[i+7]=0x04;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x04;
		f[i+11]=0x04;
		f[i+12]=0x18;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'u';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'v';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x12;
		f[i+12]=0x0c;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'w';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x2d;
		f[i+11]=0x2d;
		f[i+12]=0x12;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'x';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x12;
		f[i+8]=0x0c;
		f[i+9]=0x0c;
		f[i+10]=0x12;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'y';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x31;
		f[i+12]=0x2e;
		f[i+13]=0x20;
		f[i+14]=0x21;
		f[i+15]=0x1e;
	i=16 * 'z';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x3f;
		f[i+6]=0x20;
		f[i+7]=0x10;
		f[i+8]=0x08;
		f[i+9]=0x04;
		f[i+10]=0x02;
		f[i+11]=0x01;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'A';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0c;
		f[i+3]=0x12;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x3f;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'B';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1f;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x1f;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'C';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x01;
		f[i+5]=0x01;
		f[i+6]=0x01;
		f[i+7]=0x01;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'D';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1f;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'E';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x3f;
		f[i+3]=0x01;
		f[i+4]=0x01;
		f[i+5]=0x01;
		f[i+6]=0x01;
		f[i+7]=0x0f;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x01;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'F';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x3f;
		f[i+3]=0x01;
		f[i+4]=0x01;
		f[i+5]=0x01;
		f[i+6]=0x01;
		f[i+7]=0x0f;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x01;
		f[i+12]=0x01;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'G';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x01;
		f[i+5]=0x01;
		f[i+6]=0x01;
		f[i+7]=0x39;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x3e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'H';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x3f;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'I';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0e;
		f[i+3]=0x04;
		f[i+4]=0x04;
		f[i+5]=0x04;
		f[i+6]=0x04;
		f[i+7]=0x04;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x04;
		f[i+11]=0x04;
		f[i+12]=0x0e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'J';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x3f;
		f[i+3]=0x21;
		f[i+4]=0x20;
		f[i+5]=0x20;
		f[i+6]=0x20;
		f[i+7]=0x20;
		f[i+8]=0x20;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'K';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x11;
		f[i+4]=0x09;
		f[i+5]=0x05;
		f[i+6]=0x03;
		f[i+7]=0x01;
		f[i+8]=0x03;
		f[i+9]=0x05;
		f[i+10]=0x09;
		f[i+11]=0x11;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'L';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x01;
		f[i+3]=0x01;
		f[i+4]=0x01;
		f[i+5]=0x01;
		f[i+6]=0x01;
		f[i+7]=0x01;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x01;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'M';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x33;
		f[i+4]=0x2d;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'N';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x23;
		f[i+4]=0x23;
		f[i+5]=0x25;
		f[i+6]=0x25;
		f[i+7]=0x29;
		f[i+8]=0x29;
		f[i+9]=0x031;
		f[i+10]=0x31;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'O';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x01e;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'P';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1f;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x1f;
		f[i+8]=0x01;
		f[i+9]=0x01;
		f[i+10]=0x01;
		f[i+11]=0x01;
		f[i+12]=0x01;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'Q';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x29;
		f[i+11]=0x11;
		f[i+12]=0x2e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'R';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1f;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x1f;
		f[i+8]=0x03;
		f[i+9]=0x05;
		f[i+10]=0x09;
		f[i+11]=0x11;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'S';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x01;
		f[i+5]=0x01;
		f[i+6]=0x01;
		f[i+7]=0x1e;
		f[i+8]=0x20;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'T';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x3f;
		f[i+3]=0x04;
		f[i+4]=0x04;
		f[i+5]=0x04;
		f[i+6]=0x04;
		f[i+7]=0x04;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x04;
		f[i+11]=0x04;
		f[i+12]=0x04;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'U';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'V';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x12;
		f[i+12]=0x0c;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'X';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x12;
		f[i+5]=0x12;
		f[i+6]=0x0a;
		f[i+7]=0x0c;
		f[i+8]=0x14;
		f[i+9]=0x12;
		f[i+10]=0x12;
		f[i+11]=0x21;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'Y';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x31;
		f[i+9]=0x2e;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'Z';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x3f;
		f[i+3]=0x20;
		f[i+4]=0x10;
		f[i+5]=0x10;
		f[i+6]=0x08;
		f[i+7]=0x08;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x02;
		f[i+11]=0x01;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * 'W';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x2d;
		f[i+11]=0x33;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;

	i=16 * '1';//numeros
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x08;
		f[i+3]=0x0c;
		f[i+4]=0x0a;
		f[i+5]=0x09;
		f[i+6]=0x08;
		f[i+7]=0x08;
		f[i+8]=0x08;
		f[i+9]=0x08;
		f[i+10]=0x08;
		f[i+11]=0x08;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '2';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x20;
		f[i+5]=0x20;
		f[i+6]=0x10;
		f[i+7]=0x08;
		f[i+8]=0x04;
		f[i+9]=0x02;
		f[i+10]=0x01;
		f[i+11]=0x01;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '3';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x20;
		f[i+5]=0x20;
		f[i+6]=0x10;
		f[i+7]=0x0c;
		f[i+8]=0x10;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '4';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x21;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x21;
		f[i+8]=0x3f;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x20;
		f[i+12]=0x20;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '5';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1f;
		f[i+3]=0x01;
		f[i+4]=0x01;
		f[i+5]=0x1d;
		f[i+6]=0x23;
		f[i+7]=0x20;
		f[i+8]=0x20;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '6';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x01;
		f[i+5]=0x1d;
		f[i+6]=0x23;
		f[i+7]=0x21;
		f[i+8]=0x21;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '7';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x3f;
		f[i+3]=0x21;
		f[i+4]=0x10;
		f[i+5]=0x10;
		f[i+6]=0x08;
		f[i+7]=0x08;
		f[i+8]=0x08;
		f[i+9]=0x04;
		f[i+10]=0x04;
		f[i+11]=0x04;
		f[i+12]=0x04;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '8';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x12;
		f[i+7]=0x0c;
		f[i+8]=0x12;
		f[i+9]=0x21;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '9';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x21;
		f[i+7]=0x31;
		f[i+8]=0x2e;
		f[i+9]=0x20;
		f[i+10]=0x20;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '0';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x21;
		f[i+4]=0x21;
		f[i+5]=0x21;
		f[i+6]=0x31;
		f[i+7]=0x29;
		f[i+8]=0x25;
		f[i+9]=0x23;
		f[i+10]=0x21;
		f[i+11]=0x21;
		f[i+12]=0x1e;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;

	i=16 * ' ';//espacio
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x0;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x0;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '[';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x02;
		f[i+4]=0x02;
		f[i+5]=0x02;
		f[i+6]=0x02;
		f[i+7]=0x02;
		f[i+8]=0x02;
		f[i+9]=0x02;
		f[i+10]=0x02;
		f[i+11]=0x02;
		f[i+12]=0x02;
		f[i+13]=0x02;
		f[i+14]=0x02;
		f[i+15]=0x1e;
	i=16 * ']';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x1e;
		f[i+3]=0x10;
		f[i+4]=0x10;
		f[i+5]=0x10;
		f[i+6]=0x10;
		f[i+7]=0x10;
		f[i+8]=0x10;
		f[i+9]=0x10;
		f[i+10]=0x10;
		f[i+11]=0x10;
		f[i+12]=0x10;
		f[i+13]=0x10;
		f[i+14]=0x10;
		f[i+15]=0x1e;
	i=16 * '-';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x1e;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x0;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * ':';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x08;
		f[i+6]=0x0;
		f[i+7]=0x0;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x08;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '=';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x3f;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x3f;
		f[i+11]=0x0;
		f[i+12]=0x0;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '"';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x12;
		f[i+3]=0x12;
		f[i+4]=0x12;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x0;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x0;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '.';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x0;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x08;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '/';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x20;
		f[i+3]=0x20;
		f[i+4]=0x10;
		f[i+5]=0x10;
		f[i+6]=0x08;
		f[i+7]=0x08;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x02;
		f[i+11]=0x02;
		f[i+12]=0x01;
		f[i+13]=0x01;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '\\';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x01;
		f[i+3]=0x01;
		f[i+4]=0x02;
		f[i+5]=0x02;
		f[i+6]=0x04;
		f[i+7]=0x04;
		f[i+8]=0x08;
		f[i+9]=0x08;
		f[i+10]=0x10;
		f[i+11]=0x10;
		f[i+12]=0x20;
		f[i+13]=0x20;
		f[i+14]=0x0;
		f[i+15]=0x0;
	i=16 * '\(';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x18;
		f[i+3]=0x04;
		f[i+4]=0x02;
		f[i+5]=0x02;
		f[i+6]=0x02;
		f[i+7]=0x02;
		f[i+8]=0x02;
		f[i+9]=0x02;
		f[i+10]=0x02;
		f[i+11]=0x02;
		f[i+12]=0x02;
		f[i+13]=0x02;
		f[i+14]=0x04;
		f[i+15]=0x18;
	i=16 * ')';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x06;
		f[i+3]=0x08;
		f[i+4]=0x10;
		f[i+5]=0x10;
		f[i+6]=0x10;
		f[i+7]=0x10;
		f[i+8]=0x10;
		f[i+9]=0x10;
		f[i+10]=0x10;
		f[i+11]=0x10;
		f[i+12]=0x10;
		f[i+13]=0x10;
		f[i+14]=0x08;
		f[i+15]=0x06;
	i=16 * '\?';
		f[i]=0x1e;
		f[i+1]=0x21;
		f[i+2]=0x21;
		f[i+3]=0x20;
		f[i+4]=0x10;
		f[i+5]=0x10;
		f[i+6]=0x08;
		f[i+7]=0x08;
		f[i+8]=0x04;
		f[i+9]=0x04;
		f[i+10]=0x00;
		f[i+11]=0x04;
		f[i+12]=0x00;
		f[i+13]=0x00;
		f[i+14]=0x00;
		f[i+15]=0x00;

	i=16 * '{';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x18;
		f[i+3]=0x04;
		f[i+4]=0x04;
		f[i+5]=0x04;
		f[i+6]=0x08;
		f[i+7]=0x08;
		f[i+8]=0x06;
		f[i+9]=0x06;
		f[i+10]=0x08;
		f[i+11]=0x08;
		f[i+12]=0x04;
		f[i+13]=0x04;
		f[i+14]=0x04;
		f[i+15]=0x18;
	i=16 * '}';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x06;
		f[i+3]=0x08;
		f[i+4]=0x08;
		f[i+5]=0x08;
		f[i+6]=0x04;
		f[i+7]=0x04;
		f[i+8]=0x18;
		f[i+9]=0x18;
		f[i+10]=0x04;
		f[i+11]=0x04;
		f[i+12]=0x08;
		f[i+13]=0x08;
		f[i+14]=0x08;
		f[i+15]=0x06;

	i=16 * '_';
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x0;
		f[i+3]=0x0;
		f[i+4]=0x0;
		f[i+5]=0x0;
		f[i+6]=0x0;
		f[i+7]=0x0;
		f[i+8]=0x0;
		f[i+9]=0x0;
		f[i+10]=0x0;
		f[i+11]=0x0;
		f[i+12]=0x3f;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;

	i=16 * 164;//ñ
		f[i]=0x0;
		f[i+1]=0x0;
		f[i+2]=0x26;
		f[i+3]=0x19;
		f[i+4]=0x0;
		f[i+5]=0x1d;
		f[i+6]=0x22;
		f[i+7]=0x22;
		f[i+8]=0x22;
		f[i+9]=0x22;
		f[i+10]=0x22;
		f[i+11]=0x22;
		f[i+12]=0x22;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;

	i=16 * 165;//Ñ
		f[i]=0x0;
		f[i+1]=0x26;
		f[i+2]=0x19;
		f[i+3]=0x0;
		f[i+4]=0x21;
		f[i+5]=0x23;
		f[i+6]=0x23;
		f[i+7]=0x25;
		f[i+8]=0x25;
		f[i+9]=0x29;
		f[i+10]=0x31;
		f[i+11]=0x31;
		f[i+12]=0x21;
		f[i+13]=0x0;
		f[i+14]=0x0;
		f[i+15]=0x0;

}

unsigned long vbe_strcmp(char* buf, char* str)
{
	unsigned long ret,lb,ls,i;

	lb = vbe_strlen( buf );
	ls = vbe_strlen( str );
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


/* strlen() funcion que devuelve el largo de una cadena o cero en caso \
		de error o cadena nula */

unsigned long vbe_strlen(char *str)
{
	unsigned long cont=0;
	while( str[cont] != 0 && cont < 0xffffffff )
	{
		cont++;
	}
	if(cont == 0xffffffff) cont = 0;
	return (cont);
}

unsigned long vbe_strcpy( char* origen, char* destino){
	unsigned long largo;
	unsigned long i;

	largo=vbe_strlen(origen);
	if(largo == 0) return(0);
	if(largo > MAX_STRING) return(0);
	for( i=0; i < largo; i++){
		destino[i] = origen[i];
	}
	return(largo);
}

unsigned long vbe_strcat( char* base, char* agr){
	unsigned long a,b;
	a=vbe_strlen(base);
	b=vbe_strlen(agr);
	if( (a+b) > MAX_STRING) return(1);
	vbe_strcpy( agr, &base[a]);
	return(0);
}

void vbe_memcpy(char *orig, char *des, unsigned long cant){
	while(cant > 0){
		*des = *orig;
		cant--;
		orig++;
		des++;
	}
}

void vbe_memset(char *des,unsigned long cant, char dat){
	while(cant > 0){
		*des = dat;
		des++;
		cant--;
	}
}
