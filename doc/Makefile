CFLAGS = -I../src

all: example examplev

# you can use -lgenht instead of the ../src/ stuff when it's installed
example: example.o ../src/htsi.o ../src/hash.o
	$(CC) -o example $(LDFLAGS) example.o ../src/htsi.o ../src/hash.o

example.o: example.c
	$(CC) -o example.o -c $(CFLAGS) example.c


examplev: examplev.o
	$(CC) -o examplev $(LDFLAGS) -lm examplev.o

examplev.o: examplev.c
	$(CC) -o examplev.o -c $(CFLAGS) examplev.c

clean:
	-rm example.o example examplev.o examplev
