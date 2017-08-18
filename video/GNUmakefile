include ../GNUmakefile.cfg

DRIVER = video

all : $(DRIVER).bin

$(DRIVER).bin : $(DRIVER).o
	ld -T link.ld --oformat binary -o $(DRIVER).bin ../include/cr1.o $(DRIVER).o -L../lib -lc
	ld -T link.ld -o $(DRIVER).elf ../include/cr1.o $(DRIVER).o -L../libc/ -lc

$(DRIVER).o : $(DRIVER).c
	gcc -c -o $(DRIVER).o -I../include $(DRIVER).c $(FLAGS)

clean :
	rm *.o
	rm *.bin
	rm *.elf

cp :
	cp video.bin ../../Silicio/iso-Silicio/iso/boot/
