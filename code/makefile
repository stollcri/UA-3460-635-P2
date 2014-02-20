CC = c++

default: svd

all: clean test

svd:
	${CC} ${CFLAGS} -o svd svd.cpp pgm.cpp

test:
	${CC} ${CFLAGS} -o svd svd.cpp pgm.cpp
	./svd

clean:
	-rm *.o
	-rm svd
