#include "FT/hdbscandir/utils.h"
#include <tuple>

//----------------------------------------------

bool bitSet::get(int pos) {
		return pos < _bits.size() && _bits[pos];
}

void bitSet::set(int pos) {
		ensure(pos);
		_bits[pos] = true;
}

void bitSet::ensure(int pos) {
	if (pos >= _bits.size())
	{
		_bits.resize(pos + 64);
	}
}

//-----------------------------------------------

outlierScore::outlierScore() {
	;
}

outlierScore::outlierScore(double score, double coreDistance, int id) {
	outlierScore::score = score;
	outlierScore::coreDistance = coreDistance;
	outlierScore::id = id;
}

bool outlierScore::operator<(const outlierScore& other) const {
	/*
	if (score < other.score)
		return score < other.score;
	else if (coreDistance < other.coreDistance)
		return coreDistance < other.coreDistance;
	else if (id < other.id)
		return id < other.id;
	else
		return false;*/
	return std::tie(score, coreDistance, id) < std::tie(other.score, other.coreDistance, other.id);
}

//----------------------------------------------
/// A clustering constraint (either a must-link or cannot-link constraint between two points).
hdbscanConstraint::hdbscanConstraint(int pointA, int pointB, hdbscanConstraintType type) {
	_pointA = pointA; // The first point involved in the constraint
	_pointB = pointB; // The second point involved in the constraint
	_constraintType = type; // The constraint type
}

int hdbscanConstraint::getPointA() {
	return _pointA;
}

int hdbscanConstraint::getPointB() {
	return _pointB;
}

hdbscanConstraintType hdbscanConstraint::getConstraintType() {
	return _constraintType;
}

//--------------------------------------------
hdbscanResult::hdbscanResult() {
	;
}
hdbscanResult::hdbscanResult(vector<int> pLables, vector<outlierScore> pOutlierScores, vector<double> pmembershipProbabilities, bool pHsInfiniteStability) {
	labels = pLables;
	outliersScores = pOutlierScores;
	membershipProbabilities = pmembershipProbabilities;
	hasInfiniteStability = pHsInfiniteStability;
}
