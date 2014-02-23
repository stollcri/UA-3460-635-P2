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
#include <Eigen/Dense>
#include <Eigen/SVD>

using namespace std;

/*	helper method for pgm constructor
	creates a pgm object from an ASCII (normal) pgm file
*/
void pgm::pgmFromASCII(const char * filename) {
	ifstream inFile(filename);
	string in;
	if (!inFile.is_open()) {
		height = width = 0;
		depth = (unsigned char) 0;
		imageMatrix = NULL;
		cerr << "cannot open file for reading\n";
	}
	else {
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
				inFile >> in;
				imageMatrix[i][j] = (unsigned char) atoi(in.c_str());
				if (DBGPGM) cout << (int) imageMatrix[i][j] << ' ';
			}
			if (DBGPGM) cout << "\n";
		}
		inFile.close();
	}
}

/*	helper method for pgm constructor
	creates a pgm object from a binary pgm file
*/
void pgm::pgmFromBinary(const char* filename) {
	FILE* inFile;
	inFile = fopen(filename, "rb");
	if (inFile == NULL) {
		height = width = 0;
		depth = (unsigned char) 0;
		imageMatrix = NULL;
		cerr << "cannot open file for reading\n";
	}
	else {
		// read header infromation
		fread(&width, sizeof(width), 1, inFile);
		fread(&height, sizeof(height), 1, inFile);
		fread(&depth, sizeof(depth), 1, inFile);
		//we have our x, y and z, so now we know the size of the matrix we must make
		imageMatrix = new unsigned char *[height]; //make Y rows
		for (int i = 0; i < height; i++) imageMatrix[i] = new unsigned char[width];	
		// read image data
		unsigned char currentChar;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (!feof(inFile)) {
					fread(&currentChar, sizeof(currentChar), 1, inFile);
					imageMatrix[i][j] = currentChar;
				}
			}
		}
		fclose(inFile);
	}
}

/* pgm constructor
	creates a pgm objecct from a pgm file, either binary or ASCII
	fileType is expecting pgmBinary or pgmASCII, which are defined in the header
*/
pgm::pgm(const char * filename, int fileType) {
	if (fileType == pgmASCII) {
		pgmFromASCII(filename);
	}
	else if (fileType == pgmBinary) {
		pgmFromBinary(filename);
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
	FILE* outFile;
	outFile = fopen(filename, "wb");

	// write header information
	fwrite(&width, sizeof(width), 1, outFile);
	fwrite(&height, sizeof(height), 1, outFile);
	fwrite(&depth, sizeof(depth), 1, outFile);
	
	// write image data
	unsigned char currentChar;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			currentChar = imageMatrix[i][j];
			fwrite(&currentChar, sizeof(currentChar), 1, outFile);
		}
	}
	fclose(outFile);

	return 0;
}

/* toASCII outputs the object to an ASCII .pgm file
	this file will be readable by graphics editors, but any comments
	will be replaced with a watermark for our project
*/
int pgm::toASCII(const char * filename) {
	FILE* outFile;
	outFile = fopen(filename, "w");

	// write header information
	fprintf(outFile, "P2\n");
	fprintf(outFile, "# Created by Crouse and Stoll\n");
	fprintf(outFile, "%d %d\n", width, height);
	fprintf(outFile, "%d\n", depth);
	
	// write image data
	unsigned char currentChar;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			currentChar = imageMatrix[i][j];
			fprintf(outFile, "%d ", currentChar);
		}
		fprintf(outFile, "\n");
	}
	fclose(outFile);

	return 0;
}

int pgm::generateSVDheader(const char *headerFileName) {
	FILE* outFile;
	outFile = fopen(headerFileName, "w");

	// write header information
	fprintf(outFile, "%d ", width);
	fprintf(outFile, "%d ", height);
	fprintf(outFile, "%d", depth);
	fclose(outFile);

	return 0;
}

int pgm::generateSVD(const char *filename) {
	return 0;
}

int pgm::generatePGM(const char *filename) {
	return 0;
}

int pgm::toSVDfiles(const char *headerFileName, const char *svdFileName) {
	generateSVDheader(headerFileName);

	Eigen::MatrixXd M(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			M(i, j) = imageMatrix[i][j];
		}
	}

	Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::MatrixXd U = svd.matrixU();
	Eigen::MatrixXd S = svd.singularValues();
	Eigen::MatrixXd V = svd.matrixV();
	
	//cout << "===== M =====" << endl << M << endl;
	//cout << "===== U =====" << endl << U << endl;
	//cout << "===== S =====" << endl << S << endl;
	//cout << "===== V =====" << endl << V << endl;
	

	FILE* outFile;
	outFile = fopen(svdFileName, "w");
	// write the U matrix
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < height; j++) {
			fprintf(outFile, "%f ", U(i, j));
		}
		fprintf(outFile, "\n");
	}
	
	// write the S matrix
	for (int i = 0; i < S.size(); i++) {
		fprintf(outFile, "%f ", S(i));
	}
	fprintf(outFile, "\n");

	// write the V matrix
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			fprintf(outFile, "%f ", V(i, j));
		}
		fprintf(outFile, "\n");
	}
	fclose(outFile);


	/*
	 * =================
	 * EXPERIMENTAL CODE
	 */

	outFile = fopen("image_k.pgm", "w");
	fprintf(outFile, "P2\n");
	fprintf(outFile, "# Created by Crouse and Stoll\n");
	fprintf(outFile, "%d %d\n", width, height);
	fprintf(outFile, "%d\n", depth);

	Eigen::MatrixXd S2(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i==j) {
				S2(i, j) = S(i);
			} else {
				S2(i, j) = 0;
			}
		}
	}
	M = U * S2 * V.transpose(); // <= this should reverse the SVD

	int k=0, kmax=S.size();
	//cout << "U size " << U.size() << " and V size " << V.size() << " and M size "  << M.size() << endl;
	//cout << S2 << endl;
	int currentVal;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			
			//currentVal = abs( U(i, j) * S(k) * V(i, j) );
			currentVal = M(i, j);

			//cout << currentVal << " ";
			fprintf(outFile, "%d ", currentVal);

			++k;
			if (k >= kmax) {
				k = 0;
			}
		}
		//cout << endl;
		fprintf(outFile, "\n");
	}
	fclose(outFile);

	/*
	 * END EXPERIMENTAL CODE
	 * =====================
	 */

	return 0;
}

//destructor - clears up the image matrix safely
pgm::~pgm() {
	if (imageMatrix != NULL) {
		for (int i = 0; i < height; i++) {
			delete imageMatrix[i];
		}
		delete imageMatrix;
	}
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