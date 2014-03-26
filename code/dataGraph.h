/* dataGraph.h
	class definition for a graph to store a multivariate table of data
	and then perform analytics on that data
	Created by Michael Crouse, 3/25/14
*/
#ifndef DATAGRAPH_H
#define DATAGRAPH_H

#include <string>
#include <Eigen/Dense>
#include <Eigen/SVD>

//in case we define any additional input filetypes for the constructor...
#define CSVFile 0

using namespace std;
using namespace Eigen;

/* class dataGraph
	this stores a table of multivariate data in a format helpful to run matrix operations on it
	format is 
*/
class dataGraph {
	public:
		//constructor from a .csv file 
		dataGraph(const char * filename, int fileType);
		/*PCA analysis of the graph
			k value determines how many eigenvectors we want to consider
			sigCutoff is the minimum level of correlation in which we are interested
		*/
		void runPCA(const char * filename, int k, double sigCutoff);
	private:
		MatrixXd dataTable;
		string * variableNames;
		int width;
		int height;
};
#endif
