CC = cc
CPP = c++
CPPFLAGS = -I ./eigen/

default: svd

all: clean test

pcatest:
	${CPP} ${CPPFLAGS} -o pcatest testPCA.cpp dataGraph.cpp

svd:
	${CPP} ${CPPFLAGS} -o svd svd.cpp pgm.cpp

pcpp:
	${CPP} ${CPPFLAGS} -o pca pca.cpp
	./pca

pcc:
	${CC} -o pca-c pca.c

test:
	${CPP} ${CPPFLAGS} -o svd svd.cpp pgm.cpp

	./svd 1 image.pgm	# => image_b.pgm
	./svd 2 image_b.pgm		# => image2.pgm
	./svd 5 image2.pgm		# => headers.txt ; SVD.txt
	./svd 3 header.txt SVD.txt 30	# => image_b.pgm.SVD
	./svd 4 image_b.pgm.SVD 	# => image_k.pgm

clean:
	-rm *.o

	-rm svd
	-rm pca
	-rm pcatest

	-rm image_b.pgm
	-rm image2.pgm
	-rm header.txt
	-rm SVD.txt
	-rm image_b.pgm.SVD
	-rm image_k.pgm
