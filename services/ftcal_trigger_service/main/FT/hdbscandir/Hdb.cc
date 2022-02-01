#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
#include<map>
#include<cstdint>
#include <math.h>
#include "FT/hdbscandir/Hdb.h"
#include <algorithm>

using namespace std;


//Hdb::Hdb(){}


///	Loads the dataset of each event


void Hdb::loadDataFrame(const DataFrame& data, int dim) { //

  vector<vector<double> > dataset;

  //cout<<".....loading dataframe: "<<endl;

  vector<double> row;


  for(int i=0; i<data.size(); i++){

    row.clear();
    //cout<<"x: "<<data.at(i).x<<", y: "<< data.at(i).y <<endl;
	  row.push_back(data.at(i).x);
	  row.push_back(data.at(i).y);

	 if(dim>=3){
      row.push_back(data.at(i).t);
      //cout<<"x: "<<data.at(i).x<<", y: "<< data.at(i).y <<endl;
    }
	 if(dim==4){
     double log_ene;
     if(data.at(i).e<=0.) log_ene = log(0.0001);
     else log_ene = log(data.at(i).e);
     //cout<<"log_ene: "<< log_ene<<endl;
     //log_ene = data.at(i).e;

     row.push_back(log_ene);
   }

   dataset.push_back(row);
   //cout<<"x: "<< data.at(i).x << ", y: "<< data.at(i).y <<", t: "<< data.at(i).t <<", E: "<< data.at(i).e <<endl;
   //cout<< data.at(i).x <<", " << data.at(i).y <<", " << data.at(i).t << ", "<< data.at(i).e <<endl;

   //cout<<"dim of row: "<<row.size()<<endl;

	}


	this->dataset = dataset;

}


void Hdb::execute(int minPoints, int minClusterSize, string distanceMetric) {
	//Call The Runner Class here
	hdbscanRunner runner;
	hdbscanParameters parameters;
	uint32_t noisyPoints = 0;
	set<int> numClustersSet;
	map<int, int> clustersMap;
	vector<int> normalizedLabels;

	parameters.dataset = this->dataset;
	parameters.minPoints = minPoints;
	parameters.minClusterSize = minClusterSize;
	parameters.distanceFunction = distanceMetric;
    	this->result = runner.run(parameters);
	this->labels_ = result.labels;
	this->outlierScores_ = result.outliersScores;
	for (uint32_t i = 0; i < result.labels.size(); i++) {
		if (result.labels[i] == 0) {
			noisyPoints++;
		}
		else {
			numClustersSet.insert(result.labels[i]);
		}
	}
	this->numClusters_ = numClustersSet.size();
	this->noisyPoints_ = noisyPoints;
	int iNdex = 1;
	for (auto it = numClustersSet.begin(); it != numClustersSet.end(); it++) {
		clustersMap[*it] = iNdex++;
	}
	for (int i = 0; i < labels_.size(); i++) {
		if (labels_[i] != 0)
			normalizedLabels.push_back(clustersMap[labels_[i]]);
		else if (labels_[i] == 0) {
			normalizedLabels.push_back(-1);
		}

	}
	this->normalizedLabels_ = normalizedLabels;
	this->membershipProbabilities_ = result.membershipProbabilities;
}


void Hdb::displayResult() {

	hdbscanResult result = this->result;
	uint32_t numClusters = 0;

   vector<int> tmpvect(result.labels.size(),-1);

	cout << "HDBSCAN clustering for " << this->dataset.size() << " objects." << endl;

	for (uint32_t i = 0; i < result.labels.size(); i++) {
  		cout << result.labels[i] << " ";
	  	//tmpvect.at(i)=result.labels[i];
	}

	cout << endl << endl;

	cout << "The Clustering contains " << this->numClusters_ << " clusters with " << this->noisyPoints_ << " noise Points." << endl;
  cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;

}
