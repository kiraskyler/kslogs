all:libkslogs.a

libkslogs.a:kslogs_basic.o kslogs_file.o kslogs.o kslogs_config.o
	mkdir -p include/kslogs lib 
	cp src/*.h include/kslogs/
	ar rcs libkslogs.a kslogs_basic.o kslogs_file.o kslogs.o kslogs_config.o
	cp libkslogs.a lib/

kslogs_basic.o:src/kslogs_basic.h src/kslogs_basic.c 
	gcc -c src/kslogs_basic.c -O2 -Wall

kslogs_file.o:src/kslogs_file.h src/kslogs_file.c 
	gcc -c src/kslogs_file.c -O2 -Wall

kslogs.o:src/kslogs.h src/kslogs.c 
	gcc -c src/kslogs.c -O2 -Wall

kslogs_config.o:src/kslogs_config.h src/kslogs_config.c 
	gcc -c src/kslogs_config.c -O2 -Wall

clean:
	rm -rfv include lib *.o *.a | true
	rm src/kslogs src/*.orig src/*.log | true
	rm example/*.out example/*.log example/*.orig 

install:
	cp -r include/kslogs /usr/include/ 
	cp lib/libkslogs.a /usr/lib/

uninstall:
	rm -rfv /usr/include/kslogs/ /usr/lib/libkslogs.a
