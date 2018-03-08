include ../GNUmakefile.cfg

DRIVER = libc

all : $(DRIVER).a

$(DRIVER).a : $(DRIVER).o entry.o
	$(AR) -r $(DRIVER).a $(DRIVER).o entry.o
	cp libc.a ../lib/libc.a

$(DRIVER).o : $(DRIVER).c
	$(CC) -c -o $(DRIVER).o $(DRIVER).c $(FLAGS)

entry.o : entry.c
	$(CC) -c -o entry.o entry.c $(FLAGS)

clean :
	rm *.o
	rm *.a
