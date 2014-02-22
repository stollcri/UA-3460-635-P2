CC = c++

default: svd

all: clean test

svd:
	${CC} ${CFLAGS} -o svd svd.cpp pgm.cpp

test:
	${CC} ${CFLAGS} -o svd svd.cpp pgm.cpp

	./svd 1 image.pgm		# => image_b.pgm
	./svd 2 image_b.pgm		# => image2.pgm
	./svd 5 image2.pgm		# => headers.txt ; SVD.txt
	./svd 3 header.txt SVD.txt 2	# => image_b.pgm.SVD
	./svd 4 image_b.pgm.svd 	# => image_k.pgm

clean:
	-rm *.o
	-rm svd
