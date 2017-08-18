include ../GNUmakefile.cfg

DRIVER = start

all : $(DRIVER).bin

$(DRIVER).bin : $(DRIVER).o
#	ld -T link.ld --oformat binary -o start.bin -L../lib -lc start.o
	ld -T link.ld --oformat binary -o $(DRIVER).bin ../include/cr1.o $(DRIVER).o -L../lib -lc
	ld -T link.ld -o $(DRIVER).elf ../include/cr1.o $(DRIVER).o -L../lib -lc

$(DRIVER).o : $(DRIVER).c
	gcc -c -o $(DRIVER).o $(DRIVER).c $(FLAGS)

clean :
	rm $(DRIVER).bin
	rm $(DRIVER).elf
	rm $(DRIVER).o

cp :
	cp start.bin ../../Silicio/iso-Silicio/iso/boot/
