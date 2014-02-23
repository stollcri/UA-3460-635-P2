/* pgm.h
	class definition for pgm objects
	Created by Michael Crouse, 2/16/14
	NOTE: this implementation assumes an 8 bit greyscale depth per
	the program requirements listed in the pdf
*/
#ifndef PGM_H
#define PGM_H
#include <string>
//pgmBinary and pgmASCII are definitions for use with the constructor, denoting
//the file type from which we are creating the object
#define pgmBinary 0
#define pgmASCII 1
//max length for any given pixel, used for constructor
#define MAX_ITEM_LENGTH 80
//set to 0 for quiet operation, 1 to debug
#define DBGPGM 0

using namespace std;

class pgm {
private:
	unsigned char ** imageMatrix;
	unsigned short width;
	unsigned short height;
	unsigned char depth;
	int generateSVDheader(const char *headerFileName);
	int generateSVD(const char *filename);
	int generatePGM(const char *filename);
public:
	pgm(const char * filename, int fileType);
	int toBinary(const char * filename);
	int toASCII(const char * filename);
	int toSDVfiles(const char *headerFileName, const char *svdFileName);
	~pgm();
};

//helper functions
void readTwoShorts(string line, unsigned short *x, unsigned short *y);
void readOneChar(string line, unsigned char * z);

#endif