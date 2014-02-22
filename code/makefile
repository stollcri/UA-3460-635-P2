CC = c++

default: svd

all: clean test

svd:
	${CC} ${CFLAGS} -o svd svd.cpp pgm.cpp

test:
	${CC} ${CFLAGS} -o svd svd.cpp pgm.cpp
	./svd 1 image.pgm
	./svd 2 image_b.pgm
	./svd 3 header.txt SVD.txt 2
	./svd 4 image_b.pgm.SVD

clean:
	-rm *.o
	-rm svd
