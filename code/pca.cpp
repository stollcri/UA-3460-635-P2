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
	ifstream inFile("testimg/face.pgm");
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
	int total = 0;
	int mean = 0;
	Eigen::MatrixXd M(height, width);
	
	// data points are columns, and dimensions are rows
	/*
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
	*/
	
	// data points are rows, and dimensions are columns
	for (int j = 0; j < width; j++) {
		
		total = 0;
		for (int i = 0; i < height; i++) {
			total += (int)imageMatrix[i][j];
		}
		mean = total / height;
		
		for (int i = 0; i < height; i++) {
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

	//Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeFullU | Eigen::ComputeFullV);

	Eigen::MatrixXd Mcov = (1/(double)width) * (M * M.transpose());
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(Mcov, Eigen::ComputeFullU | Eigen::ComputeFullV);

	Eigen::MatrixXd U = svd.matrixU();
	Eigen::MatrixXd S = svd.singularValues();
	Eigen::MatrixXd V = svd.matrixV();
	Eigen::MatrixXd Ut = U.transpose();
	Eigen::MatrixXd Vt = V.transpose();

	Eigen::MatrixXd Un(height, width);
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			if (i==j) {
				Un(i, j) = 1;
			} else {
				Un(i, j) = 0;
			}
		}
	}
	Eigen::MatrixXd Unt = Un.transpose();

	FILE* outFileMean;
	outFileMean = fopen("image_pca_mean.pgm", "w");
	fprintf(outFileMean, "P2\n");
	fprintf(outFileMean, "# Created by Crouse and Stoll\n");
	fprintf(outFileMean, "%d %d\n", width, height);
	fprintf(outFileMean, "%d\n", depth);

	FILE* outFileKa;
	outFileKa = fopen("image_pca_ka.pgm", "w");
	fprintf(outFileKa, "P2\n");
	fprintf(outFileKa, "# Created by Crouse and Stoll\n");
	fprintf(outFileKa, "%d %d\n", width, height);
	fprintf(outFileKa, "%d\n", depth);

	FILE* outFileKb;
	outFileKb = fopen("image_pca_kb.pgm", "w");
	fprintf(outFileKb, "P2\n");
	fprintf(outFileKb, "# Created by Crouse and Stoll\n");
	fprintf(outFileKb, "%d %d\n", width, height);
	fprintf(outFileKb, "%d\n", depth);

	int k=24;//S.size();
	int thisKa=0, thisKb=0;
	int currentValMean, currentValA, currentValB;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			currentValMean = 0;
			currentValA = 0;
			currentValB = 0;

			// principal components?
			currentValA += U(i, thisKa) * S(thisKa) * Vt(thisKa, j);
			currentValA += 127;

			currentValB += U(i, thisKb) * S(thisKb) * Vt(thisKb, j);
			currentValB += U(i, thisKb+1) * S(thisKb+1) * Vt(thisKb+1, j);
			currentValB += U(i, thisKb+2) * S(thisKb+2) * Vt(thisKb+2, j);
			currentValB += U(i, thisKb+3) * S(thisKb+3) * Vt(thisKb+3, j);
			currentValB += U(i, thisKb+4) * S(thisKb+4) * Vt(thisKb+4, j);
			currentValB += U(i, thisKb+5) * S(thisKb+5) * Vt(thisKb+5, j);
			currentValB += U(i, thisKb+6) * S(thisKb+6) * Vt(thisKb+6, j);
			currentValB += U(i, thisKb+7) * S(thisKb+7) * Vt(thisKb+7, j);
			currentValB += 127;

			for (int currK = 0; currK < k; ++currK){
				// average scene?
				//currentValMean += U(i, currK) * S(currK) * Vt(currK, j);

				currentValMean += V(currK, j) * S(currK) * S(currK) * Vt(currK, j);
				//currentValMean += U(currK, i) * S(currK) * S(currK) * Ut(currK, i);
				//currentValMean += ((1/(double)height) * U(currK, i) * S(currK) * S(currK) * Ut(currK, i));
				//currentValMean += Unt(currK, i) * S(currK) * S(currK) * Ut(currK, i);
			}

			fprintf(outFileMean, "%d ", currentValMean);
			fprintf(outFileKa, "%d ", currentValA);
			fprintf(outFileKb, "%d ", currentValB);
		}

		fprintf(outFileMean, "\n");
		fprintf(outFileKa, "\n");
		fprintf(outFileKb, "\n");
	}

	fclose(outFileMean);
	fclose(outFileKa);
	fclose(outFileKb);

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
