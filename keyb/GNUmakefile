include ../GNUmakefile.cfg

DRIVER = keyb-es

all : $(DRIVER).bin

$(DRIVER).bin : $(DRIVER).o int_keyb.o
	ld -T link.ld --oformat binary -o $(DRIVER).bin ../include/cr1.o $(DRIVER).o int_keyb.o -L../lib -lc
	ld -T link.ld -o $(DRIVER).elf ../include/cr1.o $(DRIVER).o int_keyb.o -L../lib -lc

$(DRIVER).o : $(DRIVER).c
	$(CC) -c -o $(DRIVER).o $(DRIVER).c $(FLAGS)

int_keyb.o : int_keyb.S
	nasm -f elf -o int_keyb.o int_keyb.S

cp :
	cp $(DRIVER).bin ../../Silicio/iso-Silicio/iso/boot/

clean :
	rm int_keyb.o
	rm $(DRIVER).o
	rm $(DRIVER).elf
	rm $(DRIVER).bin
