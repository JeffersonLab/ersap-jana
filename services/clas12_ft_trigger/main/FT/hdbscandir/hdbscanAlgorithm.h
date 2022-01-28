#include <limits>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "FT/hdbscandir/utils.h"
#include <list>
#include "FT/hdbscandir/undirectedGraph.h"
#include"FT/hdbscandir/cluster.h"

namespace hdbscanStar
{
	class hdbscanAlgorithm
	{
	public:

		/// Calculates the core distances for each point in the data set, given some value for k.

		/// "distances" A vector of vectors where index [i][j] indicates the jth attribute of data point i</param>
		/// "k" Each point's core distance will be it's distance to the kth nearest neighbor</param>
		/// Returns an array of core distances</returns>
		static std::vector<double> calculateCoreDistances(std::vector<std::vector<double>> distances, int k);

		static undirectedGraph constructMst(std::vector<std::vector<double>> distances, std::vector<double> coreDistances, bool selfEdges);


		/// Propagates constraint satisfaction, stability, and lowest child death level from each child
		/// cluster to each parent cluster in the tree.  This method must be called before calling
		/// findProminentClusters() or calculateOutlierScores().
		///
		/// "clusters" A list of Clusters forming a cluster tree</param>
		/// Returns true if there are any clusters with infinite stability, false otherwise</returns>


		static void computeHierarchyAndClusterTree(undirectedGraph *mst, int minClusterSize, std::vector<hdbscanConstraint> constraints, std::vector<std::vector<int>> &hierarchy, std::vector<double> &pointNoiseLevels, std::vector<int> &pointLastClusters, std::vector<cluster*> &clusters);

		static std::vector<int> findProminentClusters(std::vector<cluster*> &clusters, std::vector<std::vector<int>> &hierarchy, int numPoints);

		static std::vector<double> findMembershipScore(std::vector<int> clusterids, std::vector<double> coreDistances);

		static bool propagateTree(std::vector<cluster*> &sclusters);


		/// Produces the outlier score for each point in the data set, and returns a sorted list of outlier
		/// scores.  propagateTree() must be called before calling this method.

		/// "clusters" A list of Clusters forming a cluster tree which has already been propagated</param>
		/// "pointNoiseLevels" A double[] with the levels at which each point became noise</param>
		/// "pointLastClusters" An int[] with the last label each point had before becoming noise</param>
		/// "coreDistances" An array of core distances for each data point</param>

		/// Returns a List of OutlierScores, sorted in descending order</returns>
		static std::vector<outlierScore> calculateOutlierScores(
			std::vector<cluster*> &clusters,
			std::vector<double> &pointNoiseLevels,
			std::vector<int> &pointLastClusters,
			std::vector<double> coreDistances);


		/// Removes the set of points from their parent Cluster, and creates a new Cluster, provided the
		/// clusterId is not 0 (noise).

		/// "points" The set of points to be in the new Cluster</param>
		/// "clusterLabels" An array of cluster labels, which will be modified</param>
		/// "parentCluster" The parent Cluster of the new Cluster being created</param>
		/// "clusterLabel" The label of the new Cluster </param>
		/// "edgeWeight" The edge weight at which to remove the points from their previous Cluster</param>
		/// Returns the new Cluster, or null if the clusterId was 0</returns>
		static cluster* createNewCluster(
			std::set<int>& points,
			std::vector<int> &clusterLabels,
			cluster *parentCluster,
			int clusterLabel,
			double edgeWeight);


		/// Calculates the number of constraints satisfied by the new clusters and virtual children of the
		/// parents of the new clusters.

		/// "newClusterLabels" Labels of new clusters</param>
		/// "clusters" A List of clusters</param>
		/// "constraints" A List of constraints</param>
		/// "clusterLabels" An array of current cluster labels for points</param>
		static void calculateNumConstraintsSatisfied(
			std::set<int>& newClusterLabels,
			std::vector<cluster*>& clusters,
			std::vector<hdbscanConstraint>& constraints,
			std::vector<int>& clusterLabels);

	};

}
