#include "FT/hdbscandir/hdbscanRunner.h"
#include"FT/hdbscandir/AllMetrics.h"
#include"FT/hdbscandir/hdbscanAlgorithm.h"
#include"FT/hdbscandir/undirectedGraph.h"
#include"FT/hdbscandir/cluster.h"
#include"FT/hdbscandir/utils.h"

using namespace hdbscanStar;

hdbscanResult hdbscanRunner::run(hdbscanParameters parameters) {
	int numPoints = parameters.dataset.size() != 0 ? parameters.dataset.size() : parameters.distances.size();

	hdbscanAlgorithm algorithm;
	hdbscanResult result;
	if (parameters.distances.size() == 0) {
		std::vector<std::vector<double>> distances(numPoints);
		for (int i = 0; i < numPoints; i++) {
			distances[i].resize(numPoints);
			//distances[i]=std::vector<double>(numPoints);
			for (int j = 0; j < i; j++) {
				if (parameters.distanceFunction.length() == 0 || parameters.distanceFunction == "Euclidean") {
					//Default to Euclidean
					EuclideanDist EDistance;
					double distance;
					distance = EDistance.computeDistance(parameters.dataset[i], parameters.dataset[j]);
					distances[i][j] = distance;
					distances[j][i] = distance;

				}
				else if (parameters.distanceFunction == "Manhattan") {
					ManhattanDist MDistance;
					double distance;
					distance = MDistance.computeDistance(parameters.dataset[i], parameters.dataset[j]);
					distances[i][j] = distance;
					distances[j][i] = distance;
				}
				else if (parameters.distanceFunction == "Custom") {
					CustomDist MDistance;
					double distance;
					distance = MDistance.computeDistance(parameters.dataset[i], parameters.dataset[j]);
					distances[i][j] = distance;
					distances[j][i] = distance;
				}
			}
		}

		parameters.distances = distances;
	}

	std::vector <double> coreDistances = algorithm.calculateCoreDistances(
		parameters.distances,
		parameters.minPoints);

	undirectedGraph mst = algorithm.constructMst(
		parameters.distances,
		coreDistances,
		true);
	mst.quicksortByEdgeWeight();

	std::vector<double> pointNoiseLevels(numPoints);
	std::vector<int> pointLastClusters(numPoints);

	std::vector< std::vector <int> > hierarchy;

	std::vector<cluster*> clusters;
	algorithm.computeHierarchyAndClusterTree(
		&mst,
		parameters.minClusterSize,
		parameters.constraints,
		hierarchy,
		pointNoiseLevels,
		pointLastClusters,
		clusters);
	bool infiniteStability = algorithm.propagateTree(clusters);

	std::vector<int> prominentClusters = algorithm.findProminentClusters(clusters, hierarchy, numPoints);
	std::vector<double> membershipProbabilities = algorithm.findMembershipScore(prominentClusters, coreDistances);
	std::vector<outlierScore> scores = algorithm.calculateOutlierScores(
		clusters,
		pointNoiseLevels,
		pointLastClusters,
		coreDistances);

	return hdbscanResult(prominentClusters, scores, membershipProbabilities,  infiniteStability);
}
