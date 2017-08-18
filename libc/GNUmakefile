include ../GNUmakefile.cfg

DRIVER = libc

all : $(DRIVER).a

$(DRIVER).a : $(DRIVER).o entry.o
	ar -r $(DRIVER).a $(DRIVER).o entry.o
	cp libc.a ../lib/libc.a

$(DRIVER).o : $(DRIVER).c
	gcc -c -o $(DRIVER).o $(DRIVER).c $(FLAGS)

entry.o : entry.c
	gcc -c -o entry.o entry.c $(FLAGS)

clean :
	rm *.o
	rm *.a
