/*dataGraph.cpp
	implementation details for the dataGraph class
	created 3/25/14 by Michael Crouse
*/

#include "dataGraph.h"
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace Eigen;

dataGraph::dataGraph(const char * filename, int  fileType) {
	if (fileType == CSVFile) {
		MatrixXd tempMatrix;
		string inLine, substring;
		int position = 0, newPosition;
		//TODO: guard against unopenable file
		ifstream inFile(filename);
		//start by getting the line containing our variable names
		getline(inFile, inLine, '\n');
		//how many variables do we have in this file?  One won't have a comma at the end...
		width = count(inLine.begin(), inLine.end(), ',') + 1;
		height = 0;
		variableNames = new string[width];
		for (int i = 0; i < width; i++) {
			newPosition = inLine.find(',', position+1);
			variableNames[i] = inLine.substr(position, newPosition-position);
			position = newPosition+1;
		}
		while(getline(inFile, inLine, '\n')) {
			//todo: come up with a more efficient way than recreating the matrix to add lines from the file
			//THIS IS A HUGE HACK
			tempMatrix.resize(height+1, width);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					tempMatrix(i, j) = dataTable(i, j);
				}
			}
			position = 0;
			for (int i = 0; i < width; i++) {
				newPosition = inLine.find(',', position+1);
				tempMatrix(height, i) = atof(inLine.substr(position, newPosition-position).c_str());
				position = newPosition+1;
			}
			dataTable = tempMatrix;
			height++;
		}
	}
}

void dataGraph::runPCA(const char* filename, int k, double sigCutoff) {
	//first, we need to standardize and centralize the dataset 
	MatrixXd centered(height, width), standardized(height, width);
	MatrixXd S, V;
	double currentTotal, mean, deviation;
	int variables;
	for (int j = 0; j < width; j++) { //column j so we can use (i, j) as accustomed
		currentTotal = 0;
		for (int i = 0; i < height; i++) { //row i
			currentTotal += dataTable(i, j); 
		}
		mean = currentTotal/height;
		currentTotal=0;
		//now work out the standard deviation
		for (int i = 0; i < height; i++) {
			deviation = (dataTable(i,j) - mean);
			currentTotal += deviation * deviation;
		}
		deviation = sqrt(currentTotal/height);
		//finally, standardize the data by subtracting the mean, then dividing by deviation
		//or just centralize it by subtracting the mean 
		for (int i = 0; i < height; i++) {
			centered(i, j) = dataTable(i,j) - mean;
			standardized(i, j) = (dataTable(i, j) - mean) / deviation;
		}
	}
	cout << "Standardized eigenvectors and eigenvalues:\n";
	JacobiSVD<Eigen::MatrixXd> svd(standardized, Eigen::ComputeFullU | Eigen::ComputeFullV);
	cout << svd.matrixV() << endl << svd.singularValues() << endl;
	cout << "Analysis for first " << k << " components with cutoff " << sigCutoff << endl;
	currentTotal = 0;
	V = svd.matrixV();
	S = svd.singularValues();
	for (int i = 0; i < S.size(); i++) { //get the mean of the eigenvalues
		currentTotal += S(i) * S(i);
	}
	for (int j = 0; j < k; j++) {
		variables = 0;
		cout << "For eigenvalue " << k << " which stores " << ((S(j) * S(j))/currentTotal) * 100 << "% of the variance: ";
		for (int i = 0; i < width; i++) {
			if (V(i,j) >= sigCutoff) {
				cout << "high " << variableNames[i] << ' ';
				variables++;
			}
			if (V(i,j) <= (sigCutoff * -1)) {
				cout << "low " << variableNames[i] << ' ';
				variables++;
			}
		}
		if (variables > 0) {
			cout << "correspond most strongly to the variance.\n";
		}
		else {
			cout << "There appears to be little pattern among the data.\n";
		}
	}
		cout << "Centered eigenvectors and eigenvalues:\n";
	JacobiSVD<Eigen::MatrixXd> svd2(centered, Eigen::ComputeFullU | Eigen::ComputeFullV);
	cout << svd.matrixV() << endl << svd.singularValues() << endl;
	cout << "Analysis for first " << k << " components with cutoff " << sigCutoff << endl;
	currentTotal = 0;
	V = svd2.matrixV();
	S = svd2.singularValues();
	for (int i = 0; i < S.size(); i++) { //get the mean of the eigenvalues
		currentTotal += S(i) * S(i);
	}
	for (int j = 0; j < k; j++) {
		variables = 0;
		cout << "For eigenvalue " << k << " which stores " << ((S(j) * S(j))/currentTotal) * 100 << "% of the variance: ";
		for (int i = 0; i < width; i++) {
			if (V(i,j) >= sigCutoff) {
				cout << "high " << variableNames[i] << ' ';
				variables++;
			}
			if (V(i,j) <= (sigCutoff * -1)) {
				cout << "low " << variableNames[i] << ' ';
				variables++;
			}
		}
		if (variables > 0) {
			cout << "correspond most strongly to the variance.\n";
		}
		else {
			cout << "There appears to be little pattern among the data.\n";
		}
	}
}
