include ../GNUmakefile.cfg

DRIVER = video

all : $(DRIVER).bin

$(DRIVER).bin : $(DRIVER).o
	ld -T link.ld --oformat binary -o $(DRIVER).bin ../include/cr1.o $(DRIVER).o
	ld -T link.ld -o $(DRIVER).elf ../include/cr1.o $(DRIVER).o

$(DRIVER).o : $(DRIVER).c
	gcc -c -o $(DRIVER).o $(DRIVER).c $(FLAGS)

clean :
	rm $(DRIVER).bin
	rm $(DRIVER).elf
	rm $(DRIVER).o

cp :
	cp video.bin ../../Silicio/iso-Silicio/iso/boot/
