.PHONY: clean

#Declaration Constantes
CC = gcc
CFLAGS = -w -Wall
LDFLAGS = -s

#EXECUTABLE
../server.exe: toto.o
	$(CC) $(LDFLAGS) -o $@ $^

#COMPILATION GENERIQUE
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^


clean:
	rm -f *.o
