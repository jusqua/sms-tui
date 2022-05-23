# sms-project makefile

# after build, this will remove some trash
all: build

build: main.o user.o auxiliary.o tools.o
	gcc -o project main.o auxiliary.o user.o tools.o

main.o: main.c helper.h
	gcc -o main.o main.c -c -W -Wall -pedantic

auxiliary.o: auxiliary.c helper.h tools.h
	gcc -o auxiliary.o auxiliary.c -c -W -Wall -pedantic

user.o: user.c helper.h tools.h
	gcc -o user.o user.c -c -W -Wall -pedantic

tools.o: tools.c tools.h
	gcc -o tools.o tools.c -c -W -Wall -pedantic

clear:
	rm -rf *.o *~ project
