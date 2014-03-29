#include "dataGraph.h"

int main(int argc, char** argv) {
	dataGraph dg("pca-data/example.csv", CSVFile);
	dg.runPCA("output.txt", 2, 0.5);
}