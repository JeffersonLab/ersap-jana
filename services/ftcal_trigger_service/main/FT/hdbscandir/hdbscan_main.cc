//*******************************************************************************************//
//-------------------- HDBScan References ------------------//
// Multiple references available online
// JANA implementation took inspiration from:
//*******************************************************************************************//
// Papers:
// (1) R.J.G.B. Campello, D. Moulavi, A. Zimek and J. Sander
// Hierarchical Density Estimates for Data Clustering, Visualization, and Outlier Detection,
// ACM Trans. on Knowledge Discovery from Data, Vol 10, 1 (July 2015), 1-51.
//*******************************************************************************************//
// Open source:
// (1) https://github.com/rohanmohapatra/hdbscan-cpp.git (C++)
// (2) https://github.com/ojmakhura/hdbscan (C/C++)
// (3) https://hdbscan.readthedocs.io/en/latest/index.html (Python)
//-------------------------------------------------------------------------------------------//
// Work has been done to improve the hdbscan implementation and
// make it more performing and suitable for this project.
// This work is still ongoing (as of Sep 18 2020):
//*******************************************************************************************//

#include "FT/hdbscandir/hdbscan_main.h"
#include "FT/FTCalCluster.h"
#include <iostream>
#include <cstdio>
#include <map>
#include <iterator>
#include <algorithm>
#include <vector>
#include "FT/hdbscandir/Hdb.h"

using namespace std;

using DataFrame = std::vector<Point>;


DataFrame hdbscan_main(const DataFrame& data, int dim, int par1, int par2) {


    Hdb*hdbscan = new Hdb();

    hdbscan->loadDataFrame(data,dim);//data, //x,y,t,E if dim==4

    hdbscan->dataset;
    //minPoints, minClusterSize
    hdbscan->execute(par1, par2, "Manhattan");


    //cout<<""<<endl;
    //cout<<"--------------------------"<<endl;
    //hdbscan->displayResult();


    //cout << "You can access other fields like cluster labels, membership probabilities and outlier scores."<<endl;

    hdbscan->labels_;
    hdbscan->membershipProbabilities_;
    hdbscan->outlierScores_;


    //MultiMap of char and int
    //MultiMap of char and int
  	std::multimap<int, int> mmapOfPos;

  	for(int i=0;i<hdbscan->labels_.size();i++) mmapOfPos.insert(pair<int,int>(hdbscan->labels_.at(i),i) );


  	// It returns a pair representing the range of elements with key equal to 0
  	typedef std::multimap<int, int>::iterator MMAPIterator;
  	std::pair<MMAPIterator, MMAPIterator> result;// = mmapOfPos.equal_range(0);


    vector <int> unique_keys;
    int count_noise_key = 0;

  	for(  multimap<int,int>::iterator it = mmapOfPos.begin(), end = mmapOfPos.end(); it != end; it = mmapOfPos.upper_bound(it->first))
  	  {
          //cout<<"---unique---"<<endl;
  	      //cout << it->first << ' ' << it->second << endl;
  	      unique_keys.push_back(it->first);
          if(it->first==0) count_noise_key = 1; //there are noise hits

  	  }

    //cout<<""<<endl;
  	//cout<<"-------- for each unique key do clustering -----------"<<endl;
   	//---------------- clustering:

    int num_clusters = unique_keys.size()-count_noise_key;
    bool have_clu = true;
    if(num_clusters==0) have_clu = false;

    /*
    cout<<" "<<endl;
    cout<<"hdbscan->labels_.size(): "<< hdbscan->labels_.size() <<endl;
    cout<<"hdbscan->outlierScores_.size(): "<< hdbscan->outlierScores_.size() <<endl;
    cout<<"hdbscan->membershipProbabilities_.size(): "<< hdbscan->membershipProbabilities_.size() <<endl;
    cout<<"num_clusters: "<< num_clusters <<endl;
    */

    DataFrame new_means(num_clusters);
    DataFrame fin_means(num_clusters);

    //cout<<"unique_keys.size(): "<<unique_keys.size()<<endl;

    if(have_clu){

      std::vector<double> w_tot(num_clusters, 0);
      std::vector<int> count_hits(num_clusters, 0);

      int new_cluster_index = 0;
      int tmp_real_idx = -9;
      Point tmp_hit;
      double tmpw1;
      double tmp_memb_score, tmp_out_score;
      int tmp_out_idx;

      for(int i=0;i<unique_keys.size();i++){
        result = mmapOfPos.equal_range(unique_keys.at(i));
        if(unique_keys.at(i)==0) continue;
        // Iterate over the subrange
        for (MMAPIterator it = result.first; it != result.second; it++){
          //if(it->first == 0) continue; //is noise
          //std::cout << it->first << ", "<< it->second << std::endl;
          //cout<<""<<endl;
          tmp_real_idx = it->second;
          tmp_memb_score = hdbscan->membershipProbabilities_.at(tmp_real_idx);
          tmp_out_score = hdbscan->outlierScores_.at(tmp_real_idx).score ;
          tmp_out_idx =  hdbscan->outlierScores_.at(tmp_real_idx).id ;

          //------------------------------------------------------------------------------------------------//
          //------------- Here one can apply a selection based on Membership and Outlier Scores ------------//
          //------------------------------------------------------------------------------------------------//

          //nothing (1)
          //if(tmp_memb_score<=0.20 && tmp_out_score>=0.80) continue; //loose (2)
          //if(tmp_memb_score<=0.50 || tmp_out_score>=0.50) continue;  //tight (3)
          //if(tmp_out_score>=0.10) continue;
          //if(tmp_memb_score<=0.10 || tmp_out_score>=0.70) continue;
          //if(tmp_memb_score<=0.0) continue;
          //cout<<"idx: "<< tmp_real_idx << ", label: "<< hdbscan->labels_.at(tmp_real_idx)
          // <<  ", memb score: "<<tmp_memb_score << ", outl score: " << tmp_out_score <<
          // ", out idx: "<< tmp_out_idx <<endl;

          //------------------------------------------------------------------------------------------------//


          tmp_hit = data.at(tmp_real_idx);

          //cout<<"filtered tmp_hit.x: "<< tmp_hit.x <<endl;
          //cout<<"filtered tmp_hit.t: "<< tmp_hit.t <<endl;
          //cout<<"tmp_hit.e: "<< tmp_hit.e <<endl;

          tmpw1 = tmp_hit.e;
          if(tmpw1<0.) tmpw1 = 0.;
          w_tot[new_cluster_index]+=tmpw1;
          count_hits[new_cluster_index]+=1;

          //double w1 = std::max(0., (3.45 + std::log(hit->getHitEnergy() / _clusEnergy)));
          //cout<<"new_cluster_index: "<<new_cluster_index<<endl;

          new_means[new_cluster_index].x += tmpw1 * tmp_hit.x;
          new_means[new_cluster_index].y += tmpw1 * tmp_hit.y;
          new_means[new_cluster_index].x2 += tmpw1 * tmp_hit.x2;
          new_means[new_cluster_index].y2 += tmpw1 * tmp_hit.y2;
          new_means[new_cluster_index].e += tmpw1;
          if(tmpw1>new_means[new_cluster_index].seede) new_means[new_cluster_index].seede = tmpw1;
          new_means[new_cluster_index].t += tmpw1 * tmp_hit.t;
          new_means[new_cluster_index].Dt += tmpw1 * tmp_hit.t * tmp_hit.t;

        } // loop on cluster elements
        //cout<<""<<endl;
        //cout<<"tot # hits: "<< count_hits[new_cluster_index]<<endl;
        new_cluster_index+=1; //initiate new cluster
      }//loop over unique keys

      double den=1.;

      //------ normalize clusters -------//
      //cout<<"normalizing..."<<endl;
      for(int i=0;i<new_means.size();i++){
        if(w_tot[i]>0.) den = w_tot[i];
        else den = 1.;
        fin_means[i].x  = new_means[i].x/den;
        fin_means[i].y  = new_means[i].y/den;
        fin_means[i].x2 = new_means[i].x2/den;
        fin_means[i].y2 = new_means[i].y2/den;
        fin_means[i].e  = new_means[i].e;
        fin_means[i].seede  = new_means[i].seede;
        fin_means[i].t  = new_means[i].t/den;
        fin_means[i].Dt = sqrt(new_means[i].Dt/den - fin_means[i].t*fin_means[i].t);
        fin_means[i].nhits  = count_hits[i];

      }

    }// end of have_clu==true






  return fin_means;
}
