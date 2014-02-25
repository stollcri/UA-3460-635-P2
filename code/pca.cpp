/**
 * Principal Component Analysis
 * Christopher Stoll, 2014
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <Eigen/Dense>
#include <Eigen/SVD>

#include <Eigen/Core>
#include <Eigen/LU>

#define DBGPGM 0

using namespace std;

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

void runPCA()
{
	unsigned char ** imageMatrix;
	unsigned short width;
	unsigned short height;
	unsigned char depth;

	/*===================
	 * pgm::pgmFromASCII
	 */
	ifstream inFile("testimg/IMG_2208.pgm");
	string in;
	if (!inFile.is_open()) {
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
	/* pgm::pgmFromASCII
	 *===================
	 */

	/*==============
	 * Matrix setup
	 */
	int total = 0.0;
	int mean = 0.0;
	Eigen::MatrixXd M(height, width);
	for (int i = 0; i < height; i++) {
		total = 0;
		for (int j = 0; j < width; j++) {
			total += (int)imageMatrix[i][j];
		}
		mean = total / width;

		for (int j = 0; j < width; j++) {
			//M(i, j) = imageMatrix[i][j];
			M(i, j) = (int)imageMatrix[i][j] - mean;
		}
	}
	//cout << M << endl << endl;

	/*========
	 * Test 1
	 *//*
	Eigen::MatrixXd centered = M.rowwise() - M.colwise().mean();
	Eigen::MatrixXd cov = centered.adjoint() * centered;
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(M);
	cout << eig.eigenvalues() << endl << endl;
	cout << eig.eigenvectors() << endl << endl;
	*/

	/*========
	 * Test 2
	 *//*
	int numData = M.cols();
	int numDim  = M.rows();

	Eigen::Matrix<Eigen::MatrixXd::Scalar,Eigen::Dynamic,1> means;
	means = M.rowwise().sum() / (numData - 1);
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(((M.colwise() - means).transpose() / sqrt(numData - 1)), Eigen::ComputeFullU | Eigen::ComputeFullV);

	Eigen::MatrixXd mPCT = svd.matrixV().transpose();

	Eigen::MatrixXd mV = (svd.singularValues()).array().square();

	Eigen::MatrixXd S;
	S.setZero(numDim,numData);
	S.block(0,0,numDim,numData) = svd.singularValues().asDiagonal();

	cout << svd.matrixV() * S * svd.matrixU().transpose();
	*/

	/*
	 * =================
	 * EXPERIMENTAL CODE
	 */
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Eigen::MatrixXd U = svd.matrixU();
	Eigen::MatrixXd S = svd.singularValues();
	Eigen::MatrixXd V = svd.matrixV();
	Eigen::MatrixXd Ut = U.transpose();
	Eigen::MatrixXd Vt = V.transpose();

	FILE* outFile;
	outFile = fopen("image_pca.pgm", "w");
	fprintf(outFile, "P2\n");
	fprintf(outFile, "# Created by Crouse and Stoll\n");
	fprintf(outFile, "%d %d\n", width, height);
	fprintf(outFile, "%d\n", depth);

	int k=8;//S.size();
	int currentVal;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			currentVal = 0;
			for (int currK = 0; currK < k; ++currK){
				//currentVal += U(i, currK) * S(currK) * Vt(currK, j);
				//currentVal += V(currK, j) * S(currK) * S(currK) * Vt(currK, j);
				currentVal += U(currK, i) * S(currK) * S(currK) * Ut(currK, i);
			}
			fprintf(outFile, "%d ", currentVal);
		}
		fprintf(outFile, "\n");
	}
	fclose(outFile);

	/*
	 * END EXPERIMENTAL CODE
	 * =====================
	 */
}

int main(int argc, char const *argv[])
{
	runPCA();
	return 0;
}
