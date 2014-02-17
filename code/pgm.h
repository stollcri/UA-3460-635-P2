/* pgm.h
	class definition for pgm objects
	Created by Michael Crouse, 2/16/14
	NOTE: this implementation assumes an 8 bit greyscale depth per
	the program requirements listed in the pdf
*/

#ifndef PGM_H
#define PGM_H
//pgmBinary and pgmASCII are definitions for use with the constructor, denoting
//the file type from which we are creating the object
#define pgmBinary 0
#define pgmASCII 1

class pgm {
private:
	unsigned char ** imageMatrix;
	unsigned short width;
	unsigned short height;
	unsigned char depth;
public:
	pgm(const char * filename, int fileType);
	int toBinary(const char * filename);
	int toASCII(const char * filename);
	~pgm();
};

#endif