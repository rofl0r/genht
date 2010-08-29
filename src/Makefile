CFLAGS=-Wall -pedantic -g

BIN=main1

all: $(BIN)

clean:
	rm -f $(BIN) *.o

%.o:%.c
	$(CC) -o $@ -c $^ $(CFLAGS)

main1: main1.o htsi1.o
	$(CC) -o $@ $^ $(LDFLAGS)

