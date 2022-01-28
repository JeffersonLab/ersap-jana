#include "FTCalCluster_factory_hdbscan.h"
#include "JANA/JEvent.h"
#include "FT/FTCalHit.h"
#include "FT/FTCalCluster.h"
#include <vector>
#include <iostream>
#include <functional>
#include <FT/kmeans/kmeans.h>
#include <FT/hdbscandir/hdbscan_main.h>

#include "JANA/Services/JParameterManager.h"
#include "JANA/Calibrations/JCalibration.h"
#include "JANA/Calibrations/JCalibrationManager.h"

//Parametri clustering:
double time_min_hdbscan = -1;
double time_max_hdbscan = 30;

//this will set the hits in DESCENDING order wrt energy
bool FTCalCluster_factory_hdbscan::compareHits(const FTCalHit* a, const FTCalHit* b) {
	return (a->getHitEnergy() > b->getHitEnergy());
}

FTCalCluster_factory_hdbscan::FTCalCluster_factory_hdbscan() {
	// TODO Auto-generated constructor stub
  mTag = "Hdbscan";
}

FTCalCluster_factory_hdbscan::~FTCalCluster_factory_hdbscan() {
	// TODO Auto-generated destructor stub
}

void FTCalCluster_factory_hdbscan::CorrectClusterEnergy(FTCalCluster *clus) {
	const FTCalHit *seed = clus->getHit(0);
	auto component = seed->m_channel.component;

	auto cc0 = this->c0[component];
	auto cc1 = this->c1[component];
	auto cc2 = this->c2[component];
	auto cc3 = this->c3[component];
	auto cc4 = this->c4[component];

	auto rawEne = clus->getClusterEnergy(); //MeV
	rawEne /= 1000.;

	auto corr = (cc0 + cc1 * rawEne + cc2 * rawEne * rawEne + cc3 * rawEne * rawEne * rawEne + cc4 * rawEne * rawEne * rawEne * rawEne) / 1000.;
	rawEne += corr;

	clus->setClusterFullEnergy(rawEne * 1000); //MeV again

	return;

}

void FTCalCluster_factory_hdbscan::Init() {

}

void FTCalCluster_factory_hdbscan::ChangeRun(const std::shared_ptr<const JEvent> &aEvent) {
	static int hasLoaded = 0;
	if (hasLoaded == 0) {
		auto jcalib_manager = japp->GetService<JCalibrationManager>();
		static int runN = 11324;
		auto jcalib = jcalib_manager->GetJCalibration(11324);

		//Load FT-HODO
		std::vector<std::vector<double>> data;
		jcalib->Get("/calibration/ft/ftcal/energycorr", data);

		for (auto row : data) {
			//sector - layer - component - c0 - c1 - c2 - c3 - c4
			auto sector = int(row[0]);
			auto layer = int(row[1]);
			auto component = int(row[2]);
			c0[component] = row[3];
			c1[component] = row[4];
			c2[component] = row[5];
			c3[component] = row[6];
			c4[component] = row[7];
		}
	}
}

void FTCalCluster_factory_hdbscan::Process(const std::shared_ptr<const JEvent> &aEvent) {

	//Loop su gli hits di un evento
	std::vector<const FTCalHit*> hits = aEvent->Get<const FTCalHit>();

	std::sort(hits.begin(), hits.end(), FTCalCluster_factory_hdbscan::compareHits);


	//----------------------------------------------------------
	//--------------------  NEW ALGORITHM   --------------------
  //----------------------------------------------------------

	//----------------- hdbscan hyperparameters --------------//

	int dim  = 4;
	int par1 = 2; // minPoints
	int par2 = 2; // minClusterSize

	//--------------------- PRE-PROCESSING -------------------//

	DataFrame prepr_points;
	Point tmp_point;

  float tmp_x, tmp_y, tmp_ene;
  float ene_threshold = 0.000; //GeV

	//-------------- add real hits ----------------//
	for(auto hit : hits){
    tmp_point.x = hit->getHitX();
    tmp_point.y = hit->getHitY();
		tmp_point.idx = hit->getHitIX();
		tmp_point.idy = hit->getHitIY();
		tmp_point.x2 = tmp_point.x*tmp_point.x;
    tmp_point.y2 = tmp_point.y*tmp_point.y;
    tmp_point.e = hit->getHitEnergy();
		//std::cout<< "tmp_point.e: "<< tmp_point.e << std::endl; //decreasing order
		tmp_point.t = hit->getHitTime();
		//std::cout<<"tmp_point.t: "<<tmp_point.t << ", tmp_point.x: "<<tmp_point.x << ", tmp_point.y: "<<tmp_point.y <<std::endl;
		tmp_point.seede = tmp_point.e;
		tmp_point.nhits = 1;
    //if(tmp_point.e > ene_threshold && tmp_point.t>0.) //sometimes times are negative!!!!
		prepr_points.push_back(tmp_point); //background cut
  }

	//-------------- add pivotal clusters here ----------------//
	//N.b., this is temporary patch to address hierarchy of 1-cluster-events

	if(hits.size()>0){
	//cout<<"hits.size(): "<< hits.size()<<", add pivotal!"<<endl;
		for(int i=0;i<dim;i++){
	    tmp_point.x =-200.;
	    tmp_point.y =-200.;
			tmp_point.idx = 1000+i;
			tmp_point.idy = 1000+i;
			tmp_point.x2 =-200.;
	    tmp_point.y2 =-200.;
	    tmp_point.e = 1.;
			tmp_point.t =-200.;
			tmp_point.seede = tmp_point.e;
			tmp_point.nhits = 1;
			prepr_points.push_back(tmp_point); //background cut
	  }


		prepr_points.push_back(tmp_point); //background cut


	}

	//------------ UNSUPERVISED LEARNING HDBSCAN ---------------

	DataFrame res_clus_hdbscan;
	//cout<<"::::: hdbscan with FTCalCluster_factory"<<endl;
  if(prepr_points.size()>0) res_clus_hdbscan = hdbscan_main(prepr_points, dim, par1, par2);

	std::vector<FTCalCluster*> clusters;

	//---------- clusters selection //c_
	int c_size;
	float c_energy, c_seed_energy;
	double c_time, c_X, c_Y, c_Z, c_X2, c_Y2, c_Dt;

	//FTCalCluster *cluster;

  //std::cout<<"res_clus_hdbscan.size(): "<< res_clus_hdbscan.size() <<std::endl;
	for(int i=0; i <res_clus_hdbscan.size(); i++){
		c_X = res_clus_hdbscan.at(i).x;
		c_Y = res_clus_hdbscan.at(i).y;
		c_energy = res_clus_hdbscan.at(i).e;
		c_seed_energy = res_clus_hdbscan.at(i).seede;
    c_size = res_clus_hdbscan.at(i).nhits;
		c_time = res_clus_hdbscan.at(i).t;
		c_X2 = res_clus_hdbscan.at(i).x2;
		c_Y2 = res_clus_hdbscan.at(i).y2;
		c_Dt = res_clus_hdbscan.at(i).Dt;

		FTCalCluster *cluster = new FTCalCluster();//clusters.size()
		cluster->setCluster(c_size, c_energy, c_seed_energy, c_time, c_X, c_Y, c_X2, c_Y2, c_Dt);
		//this->CorrectClusterEnergy(cluster);
		clusters.push_back(cluster);

	}

	//---------- clusters selection
	for(int i=0; i <clusters.size(); i++){

	if (clusters[i]->isGoodCluster()==true){
				//std::cout<<"found a good cluster!!!!"<<std::endl;
				//std::cout<<"from cluster factory, mass: "<< damass << std::endl;
				mData.push_back(clusters[i]);
			}
  //if (clusters[i]->getClusterSize()==0) mData.push_back(clusters[i]);

	  //mData.push_back(clusters[i]);

	}




}
