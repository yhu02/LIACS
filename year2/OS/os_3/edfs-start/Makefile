CC = cc
CFLAGS = -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -g
FUSE_CFLAGS = `pkg-config fuse --cflags`
FUSE_LDFLAGS = `pkg-config fuse --libs`

TARGETS = edfuse

OBJS = \
	edfs-common.o

HEADERS = \
	edfs.h		\
	edfs-common.h


all:	$(TARGETS)

edfuse:		edfuse.o $(OBJS)
		$(CC) $(CFLAGS) $(FUSE_CFLAGS) -o $@ $^ $(FUSE_LDFLAGS)

%.o:		%.c $(HEADERS)
		$(CC) $(CFLAGS) $(FUSE_CFLAGS) -c $<

clean:
		rm -f $(TARGETS) *.o
