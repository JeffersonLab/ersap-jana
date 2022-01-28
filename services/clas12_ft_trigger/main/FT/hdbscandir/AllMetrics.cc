#pragma once
#include"FT/hdbscandir/AllMetrics.h"
#include<vector>
#include<cmath>
#include<cstdint>



double EuclideanDist::computeDistance(std::vector<double> vector1, std::vector<double> vector2) {
	double distance = 0;
	for (uint32_t i = 0; i < vector1.size() && i < vector2.size(); i++) {
		distance += ((vector1[i] - vector2[i]) * (vector1[i] - vector2[i]));
	}

	return sqrt(distance);
}


double ManhattanDist::computeDistance(std::vector<double> vector1, std::vector<double> vector2) {
	double distance = 0;
	for (uint32_t i = 0; i < vector1.size() && i < vector2.size(); i++) {
		distance += fabs(vector1[i] - vector2[i]);
	}

	return distance;
}


double CustomDist::computeDistance(std::vector<double> vector1, std::vector<double> vector2) {
	double distance = 0;
  //------------- whatever metric you want to define ------------//
	//for (uint32_t i = 0; i < vector1.size() && i < vector2.size(); i++) {
	//	distance += fabs(vector1[i] - vector2[i]);
	//}

	return distance;
}
