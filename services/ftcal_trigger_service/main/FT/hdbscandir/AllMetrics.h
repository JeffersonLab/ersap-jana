#pragma once
#include<vector>

//#include"FT/hdbscandir/IDistanceCalculator.h"


class CMetrics
{
public:
	virtual double computeDistance(std::vector<double> attributesOne, std::vector<double> attributesTwo)=0;
};


///  d = |x1-y1| + |x2-y2| + ... + |xn-yn|.
class ManhattanDist : CMetrics
{
public:
	double computeDistance(std::vector<double> vector1, std::vector<double> vector2);
};


// d = sqrt((x1-y1)^2 + (x2-y2)^2 + ... + (xn-yn)^2).

class EuclideanDist : CMetrics
{
public:
	double computeDistance(std::vector<double> vector1, std::vector<double> vector2);

};

///  d = ....
class CustomDist : CMetrics
{
public:
	double computeDistance(std::vector<double> vector1, std::vector<double> vector2);
};
