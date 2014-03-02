/* pgm.cpp
	Implementation for the pgm class
	Created by Michael Crouse, 2/16/14
*/
#include "pgm.h"
#include "half.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <Eigen/Dense>
#include <Eigen/SVD>

using namespace std;
using half_float::half;

/*	helper method for pgm constructor
	creates a pgm object from an ASCII (normal) pgm file
*/
void pgm::pgmFromASCII(const char * filename) {
	ifstream inFile(filename);
	string in;
	if (!inFile.is_open()) {
		cerr << "cannot open file for reading\n";
		failedConstructor();
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
		cerr << "cannot open file for reading\n";
		failedConstructor();
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

void pgm::pgmFromCompressed(const char* filename) {
	FILE* inFile;
	inFile = fopen(filename, "rb");
	if (inFile == NULL) {
		cerr << "cannot open file for reading\n";
		failedConstructor();
	}
	else {
		unsigned short k;
		half tempHalf;
		// read header infromation
		fread(&width, sizeof(width), 1, inFile);
		fread(&height, sizeof(height), 1, inFile);
		fread(&depth, sizeof(depth), 1, inFile);
		fread(&k, sizeof(k), 1, inFile);
		//we have our x, y and z, so now we know the size of the matrix we must make
		imageMatrix = new unsigned char *[height]; //make Y rows
		for (int i = 0; i < height; i++) imageMatrix[i] = new unsigned char[width];	
		Eigen::MatrixXd M(height, width);
		Eigen::MatrixXd U(height, k);
		Eigen::MatrixXd S(k, k);
		Eigen::MatrixXd V(width, k);
		//read in U first..		
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < k; j++) {
				fread(&tempHalf, sizeof(half), 1, inFile);
				U(i, j) = tempHalf;
			}
		}
		//then S..
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				if (i==j) {
					fread(&(S(i,j)), sizeof(double), 1, inFile);
				}
				else {
					S(i, j) = 0;
				}
			}
		}
		//finally V
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < k; j++) {
				fread(&tempHalf, sizeof(half), 1, inFile);
				V(i, j) = tempHalf;
			}
		}
		M = U * S * V.transpose();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (M(i,j) > 255) {
					imageMatrix[i][j] = (unsigned char) 255;
				}
				else {
					imageMatrix[i][j] = (unsigned char) M(i, j);
				}
			}
		}
		fclose(inFile);
	}
}

int pgm:: toCompressedBinary(const char * filename, unsigned short k) {
	Eigen::MatrixXd M(height, width);
	half tempHalf;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			M(i, j) = imageMatrix[i][j];
		}
	}
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::MatrixXd U = svd.matrixU();
	Eigen::MatrixXd S = svd.singularValues();
	Eigen::MatrixXd V = svd.matrixV();
	//TODO: make sure file opens for writing
	FILE * outFile = fopen(filename, "wb");
	//first write in the header.  Note that we need K, unlike an uncompressed binary, to unpack
	fwrite(&width, sizeof(width), 1, outFile);
	fwrite(&height, sizeof(height), 1, outFile);
	fwrite(&depth, sizeof(depth), 1, outFile);
	fwrite(&k, sizeof(k), 1, outFile);
	//now we store the shortened U.
	for (int i = 0; i < height; i++) { //first k rows of U
		for (int j = 0; j < k; j++) {
			tempHalf = (half) U(i, j);
			fwrite(&tempHalf, sizeof(half), 1, outFile);
		}
	}
	//next we store S
	for (int i = 0; i < k; i++) { //first k S-values
			fwrite(&(S(i)), sizeof(double), 1, outFile);
	}
	//finally shortened V
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < k; j++) { //first k columns of V.transpose(), first k rows of V
			tempHalf = (half) V(i, j);
			fwrite(&tempHalf, sizeof(half), 1, outFile);
		}
	}
	fclose(outFile);
	return 0;
}

//failed constructor, to be used if bad filenames were passed to the "real" constructors
void pgm::failedConstructor() {
	width = height = 0;
	depth = (unsigned char) 0;
	imageMatrix= NULL;
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
	else if (fileType == pgmCompressed) {
		pgmFromCompressed(filename);
	}
	else {
		cerr << "Invalid file type provided to pgm class\n";
		failedConstructor();
	}
	if (DBGPGM) cout << "height " << height << " width " << width << " depth " << (int) depth << "\n";
}

/*	pgm constructor (from SVD file)
	generates a pgm object from a header and svd file
*/
pgm::pgm(const char * headerFile, const char * svdFile) {
	int tempValue;
	double tempDouble;
	ifstream header(headerFile), svd(svdFile);
	if (!header.is_open() || !svd.is_open()) {
		cerr << "Couldn't open header or svd file\n";
		failedConstructor();	
	}
	else {
		header >> width >> height >> tempValue;
		depth = (unsigned char) tempValue;
		header.close();
		Eigen::MatrixXd M(height, width);
		Eigen::MatrixXd U(height, height);
		Eigen::MatrixXd S(height, width);
		Eigen::MatrixXd V(width, width);
		//pull in U
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < height; j++) {
				svd >> tempDouble;
				U(i, j) = tempDouble;
			}
		}
		//now S
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				svd >> tempDouble;
				S(i, j) = tempDouble;
			}
		}
		//finally V
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < width; j++) {
				svd >> tempDouble;
				V(i, j) = tempDouble;
			}
		}
		svd.close();
		M = U * S * V.transpose(); // <= this should reverse the SVD
		//generate imageMatrix
		imageMatrix = new unsigned char *[height];
		for (int i = 0; i < height; i++){
			imageMatrix[i] = new unsigned char[width];
			for (int j = 0; j < width; j++) {
				imageMatrix[i][j] = (unsigned char) M(i, j); 
			}
		}
	}
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
	FILE* outFile;
	outFile = fopen(filename, "w");
	// write the U matrix
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < height; j++) {
			fprintf(outFile, "%f ", U(i, j));
		}
		fprintf(outFile, "\n");
	}
	
	// write the S matrix
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i==j) {
				fprintf(outFile, "%f ", S(i));
			}
			else {
				fprintf(outFile, "0 ");
			}
		}
		fprintf(outFile, "\n");
	}

	// write the V matrix
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			fprintf(outFile, "%f ", V(i, j));
		}
		fprintf(outFile, "\n");
	}
	fclose(outFile);
	return 0;
}

int pgm::toSVDfiles(const char *headerFileName, const char *svdFileName) {
	generateSVDheader(headerFileName);
	generateSVD(svdFileName);
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
