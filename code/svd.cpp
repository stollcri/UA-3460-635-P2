#include "pgm.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char ** argv) {
	pgm * image;
	int rvalue = 0;
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " option filename svd? k?\n";
		rvalue = 1;
	}
	else if (atoi(argv[1]) == 1)  {
		image = new pgm(argv[2], pgmASCII);
		image->toBinary("image_b.pgm");
	}
	else if (atoi(argv[1]) == 2) {
		image = new pgm(argv[2], pgmBinary);
		image->toASCII("image2.pgm");
	}
	else if (atoi(argv[1]) == 3) {
	}
	else if (atoi(argv[1]) == 4) {
	}
	else {
		cout << "Usage: " << argv[0] << " option filename svd? k?\n";
		rvalue = 1;
	}
	return rvalue;
}