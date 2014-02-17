/* pgm.cpp
	Implementation for the pgm class
	Created by Michael Crouse, 2/16/14
*/
#include "pgm.h"

/* pgm constructor
	creates a pgm objecct from a pgm file, either binary or ASCII
	fileType is expecting pgmBinary or pgmASCII, which are defined in the header
*/
pgm::pgm(const char * filename, int fileType) {
	
}

/* toBinary outputs the object to a binary .pgm file
	this file will NOT be readable by graphics editors
*/
int pgm::toBinary(const char * filename) {
	return 0;
}

/* toASCII outputs the object to an ASCII .pgm file
	this file will be readable by graphics editors, but any comments
	will be replaced with a watermark for our project
*/
int pgm::toASCII(const char * filename) {
	return 0;
}

//destructor - clears up the image matrix safely
pgm::~pgm() {
	
}