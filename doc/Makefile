CFLAGS = -I../src -Wall

# you can use -lgenht instead of the ../src/ stuff when it's installed
example: example.o ../src/htsi.o ../src/hash.o
	$(CC) -o example $(LDFLAGS) example.o ../src/htsi.o ../src/hash.o

example.o: example.c
	$(CC) -o example.o -c $(CFLAGS) example.c

clean:
	-rm example.o example
