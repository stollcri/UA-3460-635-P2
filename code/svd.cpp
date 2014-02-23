/* svd.cpp
	main program for part I of the project - SVD compression
	Created by Michael Crouse, 2/16/14
*/

#include "pgm.h"
#include <iostream>
#include <stdlib.h>
const char * image_b = "image_b.pgm";
const char * image2 = "image2.pgm";
const char * headerFile = "header.txt";
const char * svdFile = "SVD.txt";

using namespace std;

int main(int argc, char ** argv) {
	pgm * image;
	int rvalue = 0;
	if (argc < 3) {
		rvalue = 1;
	}
	else if (atoi(argv[1]) == 1)  {
		image = new pgm(argv[2], pgmASCII);
		image->toBinary(image_b);
	}
	else if (atoi(argv[1]) == 2) {
		image = new pgm(argv[2], pgmBinary);
		image->toASCII(image2);
	}
	else if (atoi(argv[1]) == 3) {
	}
	else if (atoi(argv[1]) == 4) {
	}
	else if (atoi(argv[1]) == 5) {
		image = new pgm(argv[2], pgmASCII);
		image->toSDVfiles(headerFile, svdFile);
	}
	else {
		rvalue = 1;
	}
	
	if (rvalue == 1) {
		cout << "Usage: " << argv[0] << " option filename svd? k?\n";
	}
	if (image != NULL) delete image;
	return rvalue;
}