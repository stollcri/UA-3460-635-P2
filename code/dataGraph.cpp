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
			cout << "height " << height << ' ';
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
				cout << inLine.substr(position, newPosition-position) << ' ';
				tempMatrix(height, i) = atof(inLine.substr(position, newPosition-position).c_str());
				position = newPosition+1;
			}
			cout << endl;
			dataTable = tempMatrix;
			height++;
		}
		cout << dataTable << endl;
	}
}

void dataGraph::runPCA(const char* filename, int k, double sigCutoff) {
	
}
