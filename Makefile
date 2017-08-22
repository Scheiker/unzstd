CC      = cc
LDFLAGS = -lzstd
CFLAGS  =

all: unzstd

unzstd:
	$(CC) unzstd.c -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm unzstd
