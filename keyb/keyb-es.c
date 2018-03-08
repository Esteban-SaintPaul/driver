#include "libc.h"
//#include "entry.h"

#define SIZE_BUF	512	// tamaño de buffer de transferencia a programas
#define SIZE_DAT	128	// tamaño de buffer de teclado
#define SIZE_KEY_CHAR	128	// tamaño de array de códigos de teclado

// Dfinición de códigos a enviar en caso de teclas especiales
#define KEY_ESPECIAL	0	//17 o 0, Código atepuesto a cualquier código especial

#define KEY_ESCAPE	1	// Ejemplo	tecla escape 	: 17 01
#define KEY_F1 		2	// 		tecla F1 	: 17 02
#define KEY_F2		3
#define KEY_F3		4
#define KEY_F4		5
#define KEY_F5		6
#define KEY_F6		7
#define KEY_F7		8
#define KEY_F8		9
#define KEY_F9		10
#define KEY_F10		11
#define KEY_F11		12
#define KEY_F12		13
#define KEY_IMPRIMIR	14
#define KEY_INSERTAR	15
#define KEY_SUPRIMIR	16
#define KEY_RETORNO	17
#define KEY_TAB		18
#define KEY_ENTER		10
#define KEY_ARRIBA	20
#define KEY_ABAJO	21
#define KEY_DERECHA	22
#define KEY_IZQUIERDA	23
#define KEY_CONTROL	24
#define KEY_NO_CONTROL	25
#define KEY_CONTROL_DER	24
#define KEY_NO_CONTROL_DER	25
#define KEY_ALT		26
#define KEY_NO_ALT	27
#define KEY_ALTGR	28
#define KEY_NO_ALTGR	29
#define KEY_INICIO	30
#define KEY_FIN		31
#define KEY_REPAG	32
#define KEY_AVPAG	33
#define KEY_SHIFT	34
#define KEY_N		164
#define KEY_N_MAY	165

// Codigos de escaneo de teclado
#define CODE_ESPECIAL		224

#define CODE_ESCAPE	1	// Código de teclado
#define CODE_BLOQ_MAY 	58
#define CODE_NO_BLOQ_MAY 	186
#define CODE_F1 	59
#define CODE_F2		60
#define CODE_F3		61
#define CODE_F4		62
#define CODE_F5		63
#define CODE_F6		64
#define CODE_F7		65
#define CODE_F8		66
#define CODE_F9		67
#define CODE_F10	68
#define CODE_F11	87
#define CODE_F12	88
#define CODE_IMPRIMIR	14
#define CODE_INSERTAR	82
#define CODE_NO_INSERTAR	210
#define CODE_SUPRIMIR	83
#define CODE_NO_SUPRIMIR	211
#define CODE_RETORNO	14
#define CODE_TAB	15
#define CODE_ENTER	28
#define CODE_ARRIBA	72
#define CODE_NO_ARRIBA	200
#define CODE_ABAJO	80
#define CODE_NO_ABAJO	208
#define CODE_DERECHA	77
#define CODE_NO_DERECHA	205
#define CODE_IZQUIERDA	75
#define CODE_NO_IZQUIERDA 203
#define CODE_CONTROL	29
#define CODE_NO_CONTROL	157
#define CODE_CONTROL_DER	29
#define CODE_NO_CONTROL_DER	157
#define CODE_ALT	56
#define CODE_NO_ALT	184
#define CODE_ALTGR	56
#define CODE_NO_ALTGR	184
#define CODE_INICIO	71
#define CODE_NO_INICIO	199
#define CODE_FIN	79
#define CODE_NO_FIN	207
#define CODE_REPAG	73
#define CODE_NO_REPAG	201
#define CODE_AVPAG	81
#define CODE_NO_AVPAG	209
#define CODE_SHIFT	42
#define CODE_NO_SHIFT	170
#define CODE_SHIFT_D	54
#define CODE_NO_SHIFT_D	182
#define CODE_N		39
#define CODE_NO_N	167
#define CODE_Q		16
#define CODE_NO_Q	144
#define CODE_MAYOR	86
#define CODE_NO_MAYOR	214
#define CODE_GUION	53
#define CODE_NO_GUION	181
#define CODE_PUNTO	52
#define CODE_NO_PUNTO	180
#define CODE_COMA	51
#define CODE_NO_COMA	179
#define CODE_LLAVE_O	40
#define CODE_NO_LLAVE_O	168
#define CODE_LLAVE_C	43
#define CODE_NO_LLAVE_C	171
#define CODE_MAS	27
#define CODE_NO_MAS	155
#define CODE_ACENTO	26
#define CODE_NO_ACENTO	154
#define CODE_BARRA_V	41
#define CODE_NO_BARRA_V	169
#define CODE_PREGUNTA	13
#define CODE_NO_PREGUNTA	141
#define CODE_COMILLA	12
#define CODE_NO_COMILLA	140
#define CODE_UNO	2
#define CODE_NO_UNO	130
#define CODE_DOS	3
#define CODE_NO_DOS	131
#define CODE_TRES	4
#define CODE_NO_TRES	132
#define CODE_CUATRO	5
#define CODE_NO_CUATRO	133
#define CODE_CINCO	6
#define CODE_NO_CINCO	134
#define CODE_SEIS	7
#define CODE_NO_SEIS	135
#define CODE_SIETE	8
#define CODE_NO_SIETE	136
#define CODE_OCHO	9
#define CODE_NO_OCHO	137
#define CODE_NUEVE	10
#define CODE_NO_NUEVE	138
#define CODE_CERO	11
#define CODE_NO_CERO	139



extern void int_keyb();
extern unsigned long key_dato;
extern unsigned long c_base[SIZE_DAT];
extern unsigned long b_des;
char key_code[SIZE_KEY_CHAR]; // tabla de caracteres
char key_code_shift[SIZE_KEY_CHAR]; // tabla de caracteres + shift
char key_code_alt[SIZE_KEY_CHAR]; // tabla de caracteres + Alt  o AltGr
void set_key_char(char* buffer, int size, int set);

int main(void){

  char arch[]="keyb";	//defino el nombre del dispositivo
  sys_param sys;		//defino la estructura de traspaso de datos
  int i,indice,buf_i;	//variables varias
  char buf[SIZE_BUF];	//buffer de transferencia
  int aux;
  char bloq_may;
  char shift;
  char alt_gr;

  bloq_may = 0;	// may en 0 significa minúscula
  shift = 0;		// shift 0 significa minúscula
  alt_gr = 0;	// alt_gr 0 significa sin presionar

//alt_gr = alt_gr +20;
//alt_gr = 0;

  set_key_char( key_code, SIZE_KEY_CHAR, 0 ); // Cargo los códigos de teclas

  sys.arch = arch;	//puntero a nombre
  sys.size = SIZE_BUF;	//tamaño de buffer
  sys.buf = buf;	//puntero a buffer

  // coloco en cero el indice presente en la interrupcion
  b_des = 0;

  // limpio el buffer de teclado
  for( i=0; i < SIZE_DAT; i++){c_base[i]=0;}

  // Pido el puerto 0x60 para manejar el teclado
  i = sys_call_speed( SYS_RPORT, 0x60, 0, 0);

  // registro la funcion int_keyb() en la int 17
  i = sys_call_speed( SYS_RINT, 17 , (unsigned long) int_keyb, 0);

  // Registro el dispositivo
  i = sys_call( SYS_REG, (unsigned long) &sys , 0, 0);

  indice=0;

  // si i < 0 entonces no pudo reistrarse
  if ( i > 0 ){
   do {
    switch (sys.call){
     case SYS_WRITE:
       // no permito escribir
       sys.count=0;
     break;
     case SYS_READ:
       buf_i = 0;
       while(c_base[indice]!=0){		/* Recorremos el buffer   */
//sys_call_speed( SYS_DEBUG, (unsigned long)c_base[indice] , 0, 0);
         switch (c_base[indice]){
           case CODE_ESPECIAL:
//sys_call_speed( SYS_DEBUG, (unsigned long)c_base[indice] , 0, 0);

             c_base[indice]= 0;	/* borramos el buffer de teclado */
             indice++;		/* y nos paramos en la siguiente */
             if( indice == SIZE_DAT ) indice=0;	/* si era el último volvemos a 0 */

//sys_call_speed( SYS_DEBUG, (unsigned long)c_base[indice] , 0, 0);

             switch ( c_base[indice] ) {
               case CODE_INSERTAR :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_INSERTAR;
                 buf_i++;
               break;
               case CODE_NO_INSERTAR :
			;
               break;
               case CODE_SUPRIMIR :
//                 buf[buf_i] = KEY_ESPECIAL;
//                 buf_i++;
                 buf[buf_i] = 127;
                 buf_i++;
               break;
               case CODE_NO_SUPRIMIR :
			;
               break;
               case CODE_CONTROL_DER :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_CONTROL_DER;
                 buf_i++;
               break;
               case CODE_NO_CONTROL_DER :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_NO_CONTROL_DER;
                 buf_i++;
               break;
               case CODE_ALTGR :
		 alt_gr = 1;
               break;
               case CODE_NO_ALTGR :
		 alt_gr = 0;
               break;
               case CODE_ARRIBA :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_ARRIBA;
                 buf_i++;
               break;
               case CODE_NO_ARRIBA :
                 ;
               break;
               case CODE_ABAJO :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_ABAJO;
                 buf_i++;
               break;
               case CODE_NO_ABAJO :
                 ;
               break;
               case CODE_IZQUIERDA :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_IZQUIERDA;
                 buf_i++;
               break;
               case CODE_NO_IZQUIERDA :
                 ;
               break;
               case CODE_DERECHA :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_DERECHA;
                 buf_i++;
               break;
               case CODE_NO_DERECHA :
                 ;
               break;
               case CODE_INICIO :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_INICIO;
                 buf_i++;
               break;
               case CODE_NO_INICIO :
                 ;
               break;
               case CODE_FIN :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_FIN;
                 buf_i++;
               break;
               case CODE_NO_FIN :
                 ;
               break;
               case CODE_REPAG :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_REPAG;
                 buf_i++;
               break;
               case CODE_NO_REPAG :
                 ;
               break;
               case CODE_AVPAG :
                 buf[buf_i] = KEY_ESPECIAL;
                 buf_i++;
                 buf[buf_i] = KEY_AVPAG;
                 buf_i++;
               break;
               case CODE_NO_AVPAG :
                 ;
               break;

               default :
//sys_call_speed( SYS_DEBUG, (unsigned long)c_base[indice] , 0, 0);
;

             }
           break;
           case CODE_ESCAPE:
//             buf[buf_i] = KEY_ESPECIAL;
//             buf_i++;
             buf[buf_i] = 1;
             buf_i++;
            break;
           case CODE_F1:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F1;
             buf_i++;
            break;
           case CODE_F2:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F2;
             buf_i++;
            break;
           case CODE_F3:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F3;
             buf_i++;
            break;
           case CODE_F4:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F4;
             buf_i++;
            break;
           case CODE_F5:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F5;
             buf_i++;
            break;
           case CODE_F6:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F6;
             buf_i++;
            break;
           case CODE_F7:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F7;
             buf_i++;
            break;
           case CODE_F8:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F8;
             buf_i++;
            break;
           case CODE_F9:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F9;
             buf_i++;
            break;
           case CODE_F10:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F10;
             buf_i++;
            break;
           case CODE_F11:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F11;
             buf_i++;
            break;
           case CODE_F12:
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_F12;
             buf_i++;
            break;
           case CODE_RETORNO:
//             buf[buf_i] = KEY_ESPECIAL;
//             buf_i++;
             buf[buf_i] = 8;
             buf_i++;
           break;
           case CODE_TAB :
//             buf[buf_i] = KEY_ESPECIAL;
//             buf_i++;
             buf[buf_i] = 9;
             buf_i++;
           break;
           case CODE_ENTER :
//             buf[buf_i] = KEY_ESPECIAL;
//             buf_i++;
             buf[buf_i] = 10;
             buf_i++;
           break;
           case CODE_CONTROL :
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_CONTROL;
             buf_i++;
           break;
           case CODE_NO_CONTROL :
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_NO_CONTROL;
             buf_i++;
           break;
           case CODE_ALT :
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_ALT;
             buf_i++;
           break;
           case CODE_NO_ALT :
             buf[buf_i] = KEY_ESPECIAL;
             buf_i++;
             buf[buf_i] = KEY_NO_ALT;
             buf_i++;
           break;
           case CODE_SHIFT :
           case CODE_SHIFT_D :
		shift=1;
           break;
           case CODE_NO_SHIFT :
           case CODE_NO_SHIFT_D :
		shift=0;
           break;

           case CODE_BLOQ_MAY :
		if( bloq_may == 1){ bloq_may = 0 ;} else {bloq_may = 1;}
           break;
           case CODE_NO_BLOQ_MAY :
		;
           break;
           case CODE_N:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = KEY_N_MAY;} else {buf[buf_i] = KEY_N;}
             buf_i++;
            break;
           case CODE_NO_N:
		;
            break;
           case CODE_Q:
		if(alt_gr == 1){
			// alt_gr + q = @
			buf[buf_i] = 64;	//mandamos codigo arroba cuando presionan alt_gr
		} else {
			// sinó mandamos 'Q' o 'q'
			if( bloq_may == 1 || shift == 1){ buf[buf_i] = 81;} else {buf[buf_i] = 113;}
		}
             buf_i++;
            break;
           case CODE_NO_Q:
		;
            break;
           case CODE_MAYOR:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 62;} else {buf[buf_i] = 60;}
             buf_i++;
            break;
           case CODE_NO_MAYOR:
		;
            break;
           case CODE_GUION:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 95;} else {buf[buf_i] = 45;}
             buf_i++;
            break;
           case CODE_NO_GUION:
		;
            break;
           case CODE_PUNTO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 58;} else {buf[buf_i] = 46;}
             buf_i++;
            break;
           case CODE_NO_PUNTO:
		;
            break;
           case CODE_COMA:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 59;} else {buf[buf_i] = 44;}
             buf_i++;
            break;
           case CODE_NO_COMA:
		;
            break;
           case CODE_LLAVE_O:
		if(alt_gr == 1){
			// alt_gr + q = ^
			buf[buf_i] = 94;	//mandamos codigo arroba cuando presionan alt_gr
		} else {
			// sinó mandamos '[' o '{'
			if( bloq_may == 1 || shift == 1){ buf[buf_i] = 91;} else {buf[buf_i] = 123;}
		}
             buf_i++;
            break;
           case CODE_NO_LLAVE_O:
		;
            break;
           case CODE_LLAVE_C:
		if(alt_gr == 1){
			// alt_gr + q = `
			buf[buf_i] = 96;
		} else {
			// sinó mandamos ']' o '}'
			if( bloq_may == 1 || shift == 1){ buf[buf_i] = 93;} else {buf[buf_i] = 125;}
		}
             buf_i++;
            break;
           case CODE_NO_LLAVE_C:
		;
            break;
           case CODE_MAS:
		if(alt_gr == 1){
			// alt_gr + q = ~
			buf[buf_i] = 126;
		} else {
			// sinó mandamos '*' o '+'
			if( bloq_may == 1 || shift == 1){ buf[buf_i] = 42;} else {buf[buf_i] = 43;}
		}
             buf_i++;
            break;
           case CODE_NO_MAS:
		;
            break;
           case CODE_ACENTO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 249;} else {buf[buf_i] = 239;}
             buf_i++;
            break;
           case CODE_NO_ACENTO:
		;
            break;

           case CODE_BARRA_V:
		if(alt_gr == 1){
			// alt_gr + q = ¬
			buf[buf_i] = 170;
		} else {
			// sinó mandamos '°' o '¬'
			if( bloq_may == 1 || shift == 1){ buf[buf_i] = 167;} else {buf[buf_i] = 124;}
		}
             buf_i++;
            break;
           case CODE_NO_BARRA_V:
		;
            break;
           case CODE_PREGUNTA:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 173;} else {buf[buf_i] = 168;}
             buf_i++;
            break;
           case CODE_NO_PREGUNTA:
		;
            break;
           case CODE_COMILLA:
		if(alt_gr == 1){
			// alt_gr + q = '\'
			buf[buf_i] = 92;
		} else {
			// sinó mandamos '?' o '\''
			if( bloq_may == 1 || shift == 1){ buf[buf_i] = 63;} else {buf[buf_i] = 39;}
		}
             buf_i++;
            break;
           case CODE_NO_COMILLA:
		;
            break;
           case CODE_UNO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 33;} else {buf[buf_i] = 49;}
             buf_i++;
            break;
           case CODE_NO_UNO:
		;
            break;
           case CODE_DOS:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 34;} else {buf[buf_i] = 50;}
             buf_i++;
            break;
           case CODE_NO_DOS:
		;
            break;
           case CODE_TRES:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 35;} else {buf[buf_i] = 51;}
             buf_i++;
            break;
           case CODE_NO_TRES:
		;
            break;
           case CODE_CUATRO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 36;} else {buf[buf_i] = 52;}
             buf_i++;
            break;
           case CODE_NO_CUATRO:
		;
            break;
           case CODE_CINCO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 37;} else {buf[buf_i] = 53;}
             buf_i++;
            break;
           case CODE_NO_CINCO:
		;
            break;
           case CODE_SEIS:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 38;} else {buf[buf_i] = 54;}
             buf_i++;
            break;
           case CODE_NO_SEIS:
		;
            break;
           case CODE_SIETE:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 47;} else {buf[buf_i] = 55;}
             buf_i++;
            break;
           case CODE_NO_SIETE:
		;
            break;
           case CODE_OCHO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 40;} else {buf[buf_i] = 56;}
             buf_i++;
            break;
           case CODE_NO_OCHO:
		;
            break;
           case CODE_NUEVE:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 41;} else {buf[buf_i] = 57;}
             buf_i++;
            break;
           case CODE_NO_NUEVE:
		;
            break;
           case CODE_CERO:
		if( bloq_may == 1 || shift == 1){ buf[buf_i] = 61;} else {buf[buf_i] = 48;}
             buf_i++;
            break;
           case CODE_NO_CERO:
		;
            break;

          default:
         if(c_base[indice] < 128 && c_base[indice] != 42 && c_base[indice] != 54 ){	/* Solo código de tecla al presionar  */
           if( c_base[indice] == 255 ){
             if( bloq_may == 1){ bloq_may = 0 ;} else {bloq_may = 1;}
           }

           else {
             aux = c_base[indice] & 0x000000ff;
//---------------------------------------------------------
//          buf[buf_i] =  (char) aux; //código en bruto
             buf[buf_i] =  key_code[aux];
//---------------------------------------------------------
             if ( buf[buf_i] > 96 && buf[buf_i] < 123  ) {
               if ( bloq_may == 1 || shift == 1 ){
//---------------------------------------------------------
                 buf[buf_i] -= 32; // cambio a  mayúscula
//;
//---------------------------------------------------------
               }
             }
             buf_i++;
           }
         }
}
         c_base[indice]= 0;	/* borramos el buffer de teclado */
         indice++;		/* y nos paramos en la siguiente */
         if( indice == SIZE_DAT ) indice=0;	/* si era el último volvemos a 0 */

       }
       sys.count=buf_i;
     break;
     }

      // Retoramos el resultado de la llamada y nos bloqueamos
      // hasta la siguiente llamada.
      //valor que se retornara al proceso llamador
     sys.ret = sys.count;

     // Si sys.count es 0 , quiere decir que no hay ninguna tecla en el fuffer
     // de teclado que enviar al proceso llamador, por lo que le vamos a pedir
     // al microkernel que nos bloquee hasta la próxima interrupción.
     // con esto aliviamos termicamente al procesador porque quedará sus-
     // pendido.
     if(sys.count == 0){
         sys_call_speed( SYS_HLT,0, 0, 0 );
     }

     i = sys_call(SYS_RET, (unsigned long) &sys, 0, 0);
   } while(1);
     // No hay que preocuparce, no estara ciclando gastando pulsos reloj
     // el kernel suspende este proceso hasta que tenga otra llamada en
     // SYS_RET

	//	sys_call( SYS_DEBUG, -1 , 0, 0);

    // solo llega aca si no se registra "Que mal!" no podra comunicase.
  }while(1);
  return(0);
}

void set_key_char(char* buffer, int size, int set){
	buffer[0]=0;
	buffer[1]=27;
	buffer[2]='1';
	buffer[3]='2';
	buffer[4]='3';
	buffer[5]='4';
	buffer[6]='5';
	buffer[7]='6';
	buffer[8]='7';
	buffer[9]='8';
	buffer[10]='9';
	buffer[11]='0';
	buffer[12]='\'';
	buffer[13]=0;
	buffer[14]=8;
	buffer[15]=9;
	buffer[16]='q';
	buffer[17]='w';
	buffer[18]='e';
	buffer[19]='r';
	buffer[20]='t';
	buffer[21]='y';
	buffer[22]='u';
	buffer[23]='i';
	buffer[24]='o';
	buffer[25]='p';
	buffer[26]=0;
	buffer[27]=0;
	buffer[28]=10;
	buffer[29]=0;
	buffer[30]='a';
	buffer[31]='s';
	buffer[32]='d';
	buffer[33]='f';
	buffer[34]='g';
	buffer[35]='h';
	buffer[36]='j';
	buffer[37]='k';
	buffer[38]='l';
	buffer[39]=164;
	buffer[40]='0';
	buffer[41]='0';
	buffer[42]='0';
	buffer[43]='0';
	buffer[44]='z';
	buffer[45]='x';
	buffer[46]='c';
	buffer[47]='v';
	buffer[48]='b';
	buffer[49]='n';
	buffer[50]='m';
	buffer[51]='m';
	buffer[52]='m';
	buffer[53]='m';
	buffer[54]='m';
	buffer[55]='m';
	buffer[56]='m';
	buffer[57]=32;
	buffer[58]=0;	//mayúscula
	buffer[59]='m';

	return;
}
