#include "entry.h"	// entry()
#include "syscall.h"	// syscall()
#include "vbe.h"	//struct vbe_header_s
#include "libc.h"	// init_libc()
#include "unistd.h"	// read() write()

s_vbe_header vbe_header;

void init_libc(){
        // Consulto dirección base de video VGA
        vbe_header.addr = (char*) sys_call_speed( SYS_VBE_ADDR,0 , 0, 0);
//        sys_call_speed( SYS_DEBUG, (unsigned long) vbe_header.addr , 0, 0);

        // Consulto resolución en X
        vbe_header.x = sys_call_speed( SYS_VBE_X,0 , 0, 0);
//        sys_call_speed( SYS_DEBUG, vbe_header.x , 0, 0);

        // Consulto resolución en Y
        vbe_header.y = sys_call_speed( SYS_VBE_Y,0 , 0, 0);
//        sys_call_speed( SYS_DEBUG, vbe_header.y , 0, 0);

        // Consulto resolución de color
        vbe_header.color = sys_call_speed( SYS_VBE_COLOR,0 , 0, 0);
//        sys_call_speed( SYS_DEBUG, vbe_header.color , 0, 0);

	return;
}

unsigned long vbe_get_x(){
	return(vbe_header.x);
}

unsigned long vbe_get_y(){
	return(vbe_header.y);
}

unsigned long vbe_get_color(){
	return(vbe_header.color);
}

char* vbe_get_addr(){
	return(vbe_header.addr);
}


int open( char* arch, char* mode){
	int ret;
	do{
		ret = sys_call( SYS_OPEN, (unsigned long) arch, (unsigned long) mode,0);
	}while(ret < 0);
	return(ret);
}

int read (int fd, void *buf, int count){
	int ret;
	ret = sys_call(SYS_READ, (unsigned long) fd, (unsigned long) buf, count);
	return(ret);
}

int write (int fd, void *buf, int count){
	int ret;
	ret = sys_call( SYS_WRITE, (unsigned long)fd, (unsigned long)buf, count);
	return (ret);
}
