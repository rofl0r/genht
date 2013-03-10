# use -Dinline if compiling with c89
INCDIR=$(install_root)/usr/include/genht
LIBDIR=$(install_root)/usr/lib

CFLAGS=-Wall -pedantic -g

BIN=mainsi
OBJS=htss.o htsp.o hash.o

all: $(BIN) $(OBJS)

clean:
	rm -f $(BIN) *.o

%.o:%.c
	$(CC) -o $@ -c $^ $(CFLAGS)

mainsi: mainsi.o htsi.o
	$(CC) -o $@ $^ $(LDFLAGS)

genht_std.so: $(OBJS)
	$(CC) -shared -dynamic -rdynamic -o $@ $^

genht_std.a: $(OBJS)
	ar rvu $@ $^

install_: genht_std.so genht_std.a
	mkdir -p $(INCDIR) $(LIBDIR)
	$(CP) `pwd`/hash.h $(INCDIR)/hash.h
	$(CP) `pwd`/ht.h $(INCDIR)/ht.h
	$(CP) `pwd`/ht_inlines.h $(INCDIR)/ht_inlines.h
	$(CP) `pwd`/htip.h $(INCDIR)/htip.h
	$(CP) `pwd`/htsi.h $(INCDIR)/htsi.h
	$(CP) `pwd`/htsp.h $(INCDIR)/htsp.h
	$(CP) `pwd`/htss.h $(INCDIR)/htss.h
	$(CP) `pwd`/genht_std.so $(LIBDIR)/genht_std.so
	$(CP) `pwd`/genht_std.a $(LIBDIR)/genht_std.a

uninstall:
	rm -rf $(INCDIR)
	rm $(LIBDIR)/genht_std.so  $(LIBDIR)/genht_std.a

install:
	make install_ CP="cp"

linstall:
	make install_ CP="ln -s"
