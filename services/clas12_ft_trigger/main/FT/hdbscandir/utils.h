#pragma once
#include<vector>
#include<cstdint>
#include"FT/hdbscandir/AllMetrics.h"
#include<iostream>

using namespace std;

//----------------------------------
class bitSet
{
private:
	std::vector<bool> _bits;
public:
	bool get(int pos);

	void set(int pos);

	void ensure(int pos);
};

//----------------------------------
class outlierScore
{
private:
	double coreDistance;
public:
	double score;
	int id;

	/// Creates a new OutlierScore for a given point.
	/// "score" The outlier score of the point</param>
	/// "coreDistance" The point's core distance</param>
	/// "id" The id (index) of the point</param>
	outlierScore(double score, double coreDistance, int id);
	outlierScore();

	/// Method Overridden to compare two objects.

	bool operator<(const outlierScore& other) const;


};

//----------------------------------
enum hdbscanConstraintType{mustLink, cannotLink};

/// A clustering constraint (either a must-link or cannot-link constraint between two points).

class hdbscanConstraint
{
private :
	hdbscanConstraintType _constraintType;
	int _pointA;
	int _pointB;

/// Creates a new constraint.

/// "pointA" The first point involved in the constraint</param>
/// "pointB" The second point involved in the constraint</param>
/// "type" The constraint type</param>
public:
	hdbscanConstraint(int pointA, int pointB, hdbscanConstraintType type);

	int getPointA();

	int getPointB();

	hdbscanConstraintType getConstraintType();

};

//------------------------------------
using namespace std;
class hdbscanParameters
{
public:

	/// Parameters to be Passed to the HDBSCAN Algorithm

	/// "distances"  The attributes of the first point</param>
	/// "dataset"  The attributes of the second point</param>
	/// "dataset"  The attributes of the second point</param>
	/// "distanceFunction"  Defines the type of distance measure to use : Euclidean, Manhattan ,..</param>
	/// "minPoints"  Min Points in the cluster</param>
	/// "minClusterSize"  The minimum number of points which a cluster needs to be a valid cluster</param>
	vector< vector <double> > distances;
	vector< vector <double> > dataset;
	string distanceFunction;
	uint32_t minPoints;
	uint32_t minClusterSize;
	vector<hdbscanConstraint> constraints;
};

//--------------------------------------
class hdbscanResult
{
public:
	vector <int> labels;
	vector <outlierScore> outliersScores;
	vector <double> membershipProbabilities;
	bool hasInfiniteStability;
	hdbscanResult();
	hdbscanResult(vector<int> pLables, vector<outlierScore> pOutlierScores, vector <double> pmembershipProbabilities, bool pHsInfiniteStability);
};
