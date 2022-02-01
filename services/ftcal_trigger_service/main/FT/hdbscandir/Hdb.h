#ifndef HDBSCAN_H_
#define HDBSCAN_H_

//#pragma once
#include<string>
#include<vector>

#include"FT/hdbscandir/hdbscanRunner.h"

#include "FT/kmeans/kmeans.h"
#include"FT/hdbscandir/utils.h"

//using namespace std;


class Hdb
{

private:

	//string fileName;

	hdbscanResult result;

public:

	std::vector < std::vector <double > > dataset;

	std::vector<int> labels_;

	std::vector<int> normalizedLabels_;

	std::vector<outlierScore>outlierScores_;

	std::vector <double> membershipProbabilities_;

	uint32_t noisyPoints_;

	uint32_t numClusters_;



	void loadDataFrame(const DataFrame& data, int dim); //const DataFrame& data,

	void execute(int minPoints, int minClusterSize, string distanceMetric);

	void displayResult();


};

#endif // _Hdbscan_h_
