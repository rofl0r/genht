CFLAGS=-Wall -pedantic -g

BIN=main

all: $(BIN) htss.o

clean:
	rm -f $(BIN) *.o

%.o:%.c
	$(CC) -o $@ -c $^ $(CFLAGS)

main: main.o htsi.o
	$(CC) -o $@ $^ $(LDFLAGS)

