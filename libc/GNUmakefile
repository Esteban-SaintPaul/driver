include ../GNUmakefile.cfg

DRIVER = libc

all : $(DRIVER).a

$(DRIVER).a : $(DRIVER).o ../include/cr1.o
	ar -r $(DRIVER).a ../include/cr1.o ../include/cr1.o
	cp libc.a ../lib/libc.a

$(DRIVER).o : $(DRIVER).c
	gcc -c -o $(DRIVER).o $(DRIVER).c $(FLAGS)

clean :
	rm $(DRIVER).o
	rm $(DRIVER).a
