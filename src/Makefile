CFLAGS=-Wall -pedantic -g

BIN=mainsi

all: $(BIN) htss.o hash.o

clean:
	rm -f $(BIN) *.o

%.o:%.c
	$(CC) -o $@ -c $^ $(CFLAGS)

mainsi: mainsi.o htsi.o
	$(CC) -o $@ $^ $(LDFLAGS)

