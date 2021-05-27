all:libkslogs.a

libkslogs.a:kslogs_basic.o kslogs_file.o kslogs.o
	mkdir -p include/kslogs lib 
	cp src/*.h include/kslogs/
	ar rcs libkslogs.a kslogs_basic.o kslogs_file.o kslogs.o
	cp libkslogs.a lib/

kslogs_basic.o:src/kslogs_basic.h src/kslogs_basic.c 
	gcc -c src/kslogs_basic.c -O2 -Wall

kslogs_file.o:src/kslogs_file.h src/kslogs_file.c 
	gcc -c src/kslogs_file.c -O2 -Wall

kslogs.o:src/kslogs.h src/kslogs.c 
	gcc -c src/kslogs.c -O2 -Wall

clean:
	rm -rfv include lib *.o *.a
	./src/clean.sh

install:
	cp -r include/kslogs /usr/include/ 
	cp lib/libkslogs.a /usr/lib/

uninstall:
	rm -rfv /usr/include/kslogs/ /usr/lib/libkslogs.a
