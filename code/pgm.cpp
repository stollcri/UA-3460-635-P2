/* pgm.cpp
	Implementation for the pgm class
	Created by Michael Crouse, 2/16/14
*/
#include "pgm.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;



/* pgm constructor
	creates a pgm objecct from a pgm file, either binary or ASCII
	fileType is expecting pgmBinary or pgmASCII, which are defined in the header
*/
//TODO: break two types into helper private methods
pgm::pgm(const char * filename, int fileType) {
	ifstream inFile(filename);
	string in;
	char item[MAX_ITEM_LENGTH];
	int tempValue;
	if (fileType == pgmASCII) {
		//TODO: check to make sure file is well-formed and opened properly
		getline(inFile, in, '\n'); //ignore first "P2"
		getline(inFile, in, '\n'); //get second line
		while (in[0] == '#') getline(inFile, in); //get any further comments
		//we should now have a line with two ints..
		readTwoShorts(in, &width, &height);
		getline(inFile, in, '\n'); //get the depth line
		readOneChar(in, &depth);
		//we have our x, y and z, so now we know the size of the matrix we must make
		imageMatrix = new unsigned char *[height]; //make Y rows
		for (int i = 0; i < height; i++) imageMatrix[i] = new unsigned char[width];
		if (DBGPGM) cout << "Image matrix: \n";
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				inFile >> item;
				tempValue = atoi(item);
				imageMatrix[i][j] = (unsigned char) tempValue;
				if (DBGPGM) cout << (int) imageMatrix[i][j] << ' ';
			}
			if (DBGPGM) cout << "\n";
		}
		inFile.close();
	}
	else if (fileType == pgmBinary) {
	}
	else {
		cerr << "Invalid file type provided to pgm class\n";
		width = height = 0;
		depth = (unsigned char) 0;
	}
	if (DBGPGM) cout << "height " << height << " width " << width << " depth " << (int) depth << "\n";
}

/* toBinary outputs the object to a binary .pgm file
	this file will NOT be readable by graphics editors
*/
int pgm::toBinary(const char * filename) {
	FILE* pFile;
    pFile = fopen(filename, "wb");

    // write header information
    fwrite(&width, sizeof(width), 1, pFile);
    fwrite(&height, sizeof(height), 1, pFile);
    fwrite(&depth, sizeof(depth), 1, pFile);
	
	// write image data
	unsigned char currentChar;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			currentChar = imageMatrix[i][j];
			fwrite(&currentChar, sizeof(currentChar), 1, pFile);
		}
	}
    fclose(pFile);

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

/* Get two space seperated numbers from the begining of a string of characters
	Borrowed from P1 to simplify implementation
*/
void readTwoShorts(string line, unsigned short *x, unsigned short *y) {
	unsigned short tmpX = 0, tmpY = 0;
	unsigned short nextDigit = 0, currentNumber = 0;
	int loopCount = 0;
	while (line[loopCount]) {
		if (isspace(line[loopCount]) || line[loopCount] == '\n') {
			tmpX = currentNumber;
			*x = tmpX;
			currentNumber = 0;
		} else {
			nextDigit = line[loopCount] - '0';
			currentNumber *= 10;
			currentNumber += nextDigit;
		}
		++loopCount;
	}
	tmpY = currentNumber;
	*y = tmpY;
}

/* Get a single short from a line
	Borrowed from P1 to simplify implementation
*/
void readOneChar(string line, unsigned char * z) {
	unsigned char tmpZ = (unsigned char) 0;
	int nextDigit = 0, currentNumber = 0;
	int loopCount = 0;
	while (line[loopCount]) {
		nextDigit = line[loopCount] - '0';
		currentNumber *= 10;
		currentNumber += nextDigit;
		++loopCount;
	}
	tmpZ = (unsigned char) currentNumber;
	*z = tmpZ;
}