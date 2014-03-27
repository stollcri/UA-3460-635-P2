#include "dataGraph.h"

int main(int argc, char** argv) {
	dataGraph dg(argv[1], CSVFile);
	dg.runPCA("output.txt", 2, 0.5);
}