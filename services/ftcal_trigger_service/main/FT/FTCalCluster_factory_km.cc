/*
 * faWaveboardClusterfactory.cc
 *
 *  Created on: Dec 23, 2019
 *      Author: cfanelli
 */

#include "FTCalCluster_factory_km.h"
#include "JANA/JEvent.h"
#include "FT/FTCalHit.h"
#include "FT/FTCalCluster.h"
#include <vector>
#include <iostream>
#include <functional>
#include <FT/kmeans/kmeans.h>

#include "JANA/Services/JParameterManager.h"
#include "JANA/Calibrations/JCalibration.h"
#include "JANA/Calibrations/JCalibrationManager.h"

//this will set the hits in DESCENDING order wrt energy
bool compareHits_km(const FTCalHit* a, const FTCalHit* b) { return (a->getHitEnergy() > b->getHitEnergy()); }

FTCalCluster_factory_km::FTCalCluster_factory_km() {
	// TODO Auto-generated constructor stub
	mTag = "Kmeans";
}

FTCalCluster_factory_km::~FTCalCluster_factory_km() {
	// TODO Auto-generated destructor stub
}

void FTCalCluster_factory_km::Init() {

}
void FTCalCluster_factory_km::ChangeRun(const std::shared_ptr<const JEvent> &aEvent) {
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



void FTCalCluster_factory_km::Process(const std::shared_ptr<const JEvent> &aEvent) {


	DataFrame tot_points;


	//std::cout<< test_fromkm <<std::endl;


	//Loop su gli hits di un evento
	std::vector<const FTCalHit*> hits = aEvent->Get<const FTCalHit>();

  //Sorting the hits
	std::sort(hits.begin(),hits.end(),compareHits_km); //here you can apply a cut on the energy


	//----------------------------------------------------------
	//--------------------   NEW ALGORITHM  --------------------
  //----------------------------------------------------------

	//--------------------- PRE-PROCESSING ---------------------

	DataFrame prepr_points;
	Point tmp_point;

  float tmp_x, tmp_y, tmp_ene;
  float ene_threshold = 0.000; //GeV

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
    //if(tmp_point.e > ene_threshold && tmp_point.t>0.)
		prepr_points.push_back(tmp_point); //background cut
  }
	//std::cout<<""<<std::endl;

	//&& (tmp_point.x2+tmp_point.y2)>80.*80. to be not in the beam hole

  /*
	double tDiff = std::fabs((hit->getHitTime() - hit_conf->getHitTime())); //ns
	auto xDiff = std::fabs(hit->getHitIX() - hit_conf->getHitIX());
	auto yDiff = std::fabs(hit->getHitIY() - hit_conf->getHitIY());
	if (tDiff < time_window_km && xDiff <= 1 && yDiff <= 1 && (xDiff + yDiff) > 0) flag = true;
	*/

  //---- calculate expected number of means

	//int res_k = count_means(prepr_points);
	PostProc post_res = compute_means(prepr_points);
  int res_k = post_res.kmeans;
  DataFrame post_points = post_res.res;

	//if(hits.size()>0)	std::cout<<"tot. #hits: "<< hits.size() <<", expected #means: "<< res_k << std::endl;

	/*
	for(int i=0;i<post_points.size();++i){
		std::cout<< "post_points.at(i).e: " << post_points.at(i).e<<std::endl;
		if(post_points.at(i).flag_bkgd==1) std::cout<<"post_points.at(i).flag_bkgd: "<<post_points.at(i).flag_bkgd<<std::endl;
	}
	std::cout<<""<<std::endl;
  */

	//------------- UNSUPERVISED LEARNING KMEANS ---------------
  int num_iter = 20; //number of iterations is hard-coded
	DataFrame res_clus_km;
	if(res_k>0) res_clus_km = k_means(post_points, res_k, num_iter);

	//res_clus_km = k_dummy(prepr_points);
	//res_clus_km = post_points; //? no events selected
	//res_clus_km = prepr_points;

	//std::cout<<"res_k: "<< res_k <<std::endl;

	std::vector<FTCalCluster*> clusters;

	//---------- clusters selection //c_
	int c_size;
	float c_energy, c_seed_energy;
	double c_time, c_X, c_Y, c_Z, c_X2, c_Y2, c_Dt;

	//FTCalCluster *cluster;

	for(int i=0; i <res_k; i++){
		c_X = res_clus_km.at(i).x;
		c_Y = res_clus_km.at(i).y;
		c_energy = res_clus_km.at(i).e;
		c_seed_energy = res_clus_km.at(i).seede;
    c_size = res_clus_km.at(i).nhits;
		c_time = res_clus_km.at(i).t;
		c_X2 = res_clus_km.at(i).x2;
		c_Y2 = res_clus_km.at(i).y2;
		c_Dt = res_clus_km.at(i).Dt;

		FTCalCluster *cluster = new FTCalCluster();//clusters.size()
		cluster->setCluster(c_size, c_energy, c_seed_energy, c_time, c_X, c_Y, c_X2, c_Y2, c_Dt);
		clusters.push_back(cluster);

	}

	int tmp_clu = (int ) clusters.size();

  //if(res_k>0) std::cout<<"clusters.size(): "<< tmp_clu << std::endl;

	//---------- clusters selection
	for(int i=0; i <(int ) clusters.size(); i++){

	if (clusters[i]->isGoodCluster()==true){
				mData.push_back(clusters[i]);
			 }
  //if (clusters[i]->getClusterSize()==0) mData.push_back(clusters[i]);

	}



}
