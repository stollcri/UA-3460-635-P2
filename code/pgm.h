#ifndef PGM_H
#define PGM_H
#define pgmBinary 0
#define pgmASCII 1

class pgm {
private:
	unsigned char ** imageMatrix;
	unsigned short width;
	unsigned short height;
	unsigned char depth;
public:
	pgm(char * filename, int fileType);
	int toBinary(char * filename);
	int toASCII(char * filename);
	~pgm();
};

#endif