#
# Skeleton code for use with Computer Architecture 2021 assignment 3,
# LIACS, Leiden University.
#

CC = gcc

# Do not touch the "-f" options, these are here to help you not being
# misled by the compiler.
CFLAGS = -O3 -g -Wall -std=gnu99 -I../common \
	-fno-tree-vectorize -fno-unroll-loops -fno-inline
LDFLAGS = -lm
PNGFLAGS = `pkg-config --cflags --libs libpng`

# Comment these lines to disable the timing code. (macOS does not
# provide clock_gettime().
CFLAGS += -DENABLE_TIMING
LDFLAGS += -lrt


# Register additional targets here
TARGETS = \
		matrixmul-r1-n512	\
		matrixmul-r50-n512	\
		grayscale		\
		tilecomposite		\
		pipeline1


all:		$(TARGETS)

# Define a target for matrixmul, 1 repeat, N=512 matrix size
matrixmul-r1-n512:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=512 -DN_REPEAT=1 -o $@ $^ $(LDFLAGS)

# Define a target for matrixmul, 50 repeat, N=512 matrix size
matrixmul-r50-n512:	matrixmul.c ../common/timing.c
			$(CC) $(CFLAGS) -DN=512 -DN_REPEAT=50 -o $@ $^ $(LDFLAGS)

# TODO: define more targets here for your experiments.
# In this case, you also want to include a compiler flag to define the
# block size, e.g. -DB=32.
# Don't forget to add these targets to TARGETS above.

# Common target for the test programs based on images
%:		%.c ../common/image.c ../common/timing.c
		$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

clean:
		rm -f $(TARGETS)
