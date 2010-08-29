CFLAGS=-Wall -pedantic -g

BIN=main

all: $(BIN)

clean:
	rm -f $(BIN) *.o

%.o:%.c
	$(CC) -o $@ -c $^ $(CFLAGS)

main: main1.o htsi.o
	$(CC) -o $@ $^ $(LDFLAGS)

