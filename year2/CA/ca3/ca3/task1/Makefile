#
# Skeleton code for use with Computer Architecture 1021 assignment 3,
# LIACS, Leiden University.
#

CC = gcc-7

# Do not touch the "-f" options, these are here to help you not being
# misled by the compiler.
CFLAGS = -O3 -g -std=gnu99 -I../common \
	-fno-tree-vectorize -fno-unroll-loops -fno-inline
LDFLAGS = -lm
PNGFLAGS = `pkg-config --cflags --libs libpng`

# Comment these lines to disable the timing code. (macOS does not
# provide clock_gettime().
CFLAGS += -DENABLE_TIMING
LDFLAGS += -lrt

# Register additional targets here
TARGETS = \
		matrixmul-r50-block1	\
		matrixmul-r50-block2	\
		matrixmul-r50-block4	\
		matrixmul-r50-block8	\
		matrixmul-r50-block16	\
		matrixmul-r50-block32	\
		matrixmul-r50-block64	\
		matrixmul-r50-block128	\
		matrixmul-r50-block256	\
		grayscale		\
		tilecomposite		\
		pipeline1


all:		$(TARGETS)

#namen veranderen van targets

# Define a target for matrixmul, 50 repeat, N=128 matrix size
matrixmul-r50-block1:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=1 -o $@ $^ $(LDFLAGS)

#Parameter for block size and amount of repeats (Might make more of these for experiments)
matrixmul-r50-block2:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=2 -o $@ $^ $(LDFLAGS)

matrixmul-r50-block4:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=4 -o $@ $^ $(LDFLAGS)

matrixmul-r50-block8:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=8 -o $@ $^ $(LDFLAGS)

matrixmul-r50-block16:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=16 -o $@ $^ $(LDFLAGS)

matrixmul-r50-block32:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=32 -o $@ $^ $(LDFLAGS)

matrixmul-r50-block64:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=64 -o $@ $^ $(LDFLAGS)

matrixmul-r50-block128:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=128 -o $@ $^ $(LDFLAGS)
			
matrixmul-r50-block256:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=128 -DN_REPEAT=1 -Dblock=256 -o $@ $^ $(LDFLAGS)

# TODO: define more targets here for your experiments.
# In this case, you also want to include a compiler flag to define the
# block size, e.g. -DB=32.
# Don't forget to add these targets to TARGETS above.

# Common target for the test programs based on images
%:		%.c ../common/image.c ../common/timing.c
		$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

clean:
		rm -f $(TARGETS)
