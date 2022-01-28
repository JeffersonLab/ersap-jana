//*******************************************************************************************//
//-------------------- k-means References ------------------//
// Multiple references available online
// JANA implementation took inspiration from:
//*******************************************************************************************//
// Papers:
// (1) Kanungo, Tapas, et al.
// "An efficient k-means clustering algorithm: Analysis and implementation."
//  IEEE transactions on pattern analysis and machine intelligence 24.7 (2002): 881-892.
//*******************************************************************************************//
//-------------------------------------------------------------------------------------------//
// Work has been done to improve the k-means implementation and
// make it more performing and suitable for this project.
//*******************************************************************************************//


#include "FT/kmeans/kmeans.h"
#include "FT/FTCalCluster.h"

double square(double value) {
  return value * value;
}

double time_difference(Point first, Point second) {
  return abs(first.t - second.t);
}


double squared_l2_distance(Point first, Point second, int iteration) {
  //if(iteration==99) std::cout<<"first.x: "<<first.x <<", second.x: "<< second.x << std::endl;
  //if(iteration==99) std::cout<<"first.y: "<<first.y <<", second.y: "<< second.y << std::endl;

  return square(first.x - second.x) + square(first.y - second.y);
}

double squared_3D_distance(Point first, Point second, int iteration) {

  double termx = square(first.x - second.x)/15./15.;///15./15.;
  double termy = square(first.y - second.y)/15./15.;///15./15.;
  double termt = square(first.t - second.t)/12./12.;///5./5.;
  //double termx = square(first.x - second.x);///15./15.;
  //double termy = square(first.y - second.y);///15./15.;
  //double termt = square(first.t - second.t);///5./5.;
  double terme = square(first.e - second.e)/(1000*1000);

  /*
  if(iteration==0 || iteration==1)
  {
   std::cout<<"iteration: "<<iteration ;
   std::cout<<", termx: "<< termx << ",  termy: "<< termy << ", termt: "<< termt << ", terme: "<< terme << std::endl;
   std::cout<<"first.t: "<<first.t <<", second.t: "<< second.t << std::endl;
   std::cout<<"first.x: "<<first.x <<", second.x: "<< second.x << std::endl;
   std::cout<<"first.y: "<<first.y <<", second.y: "<< second.y << std::endl;
   std::cout<<"first.e: "<<first.e <<", second.e: "<< second.e << std::endl;
   std::cout<<" "<< std::endl;

  }
  */


  return termx + termy + termt ; //+ terme
}

double squared_4D_distance(Point first, Point second, int iteration) {

  double termx = square(first.x - second.x)/15./15.;///15./15.;
  double termy = square(first.y - second.y)/15./15.;///15./15.;
  double termt = square(first.t - second.t)/50./50.;///5./5.;
  double terme = square(first.e - second.e);

  //if(iteration==99) std::cout<<"termx: "<< termx << ",  termy: "<< termy << ", termt: "<< termt << ", terme: "<< terme << std::endl;
  /*
  if(iteration==99) std::cout<<"first.t: "<<first.t <<", second.t: "<< second.t << std::endl;
  if(iteration==99) std::cout<<"first.x: "<<first.x <<", second.x: "<< second.x << std::endl;
  if(iteration==99) std::cout<<"first.y: "<<first.y <<", second.y: "<< second.y << std::endl;
  if(iteration==99) std::cout<<"first.e: "<<first.e <<", second.e: "<< second.e << std::endl;
  if(iteration==99) std::cout<<" "<< std::endl;
  */
  return termx + termy + termt + terme;
}

double squared_e2_distance(Point first, Point second) {
  return square(first.e - second.e) + square(first.e - second.e);
}

using DataFrame = std::vector<Point>;


DataFrame k_dummy(const DataFrame& data ){

  return data;
}

DataFrame k_means(const DataFrame& data,
                  size_t k,
                  size_t number_of_iterations) {
  static std::random_device seed;
  static std::mt19937 random_number_generator(seed());
  std::uniform_int_distribution<size_t> indices(0, data.size() - 1);

  //std::cout<< "KM size : "<< k <<std::endl;

  //double time_window = 50.;

  // Pick centroids as random points from the dataset.
  // volendo uno potrebbe inizializzare in modo deterministico i centroidi

  DataFrame container = data;

  //std::iota(container.begin(), container.end(), container.at(0));
  // Create random engine generator
  //std::mt19937 eng{ std::random_device{}() };
  // shuffle it!!
  //std::shuffle(container.begin(), container.end(), eng);
  std::random_shuffle ( container.begin(), container.end() );
  //is generating always the same ordered random sequence; need seed CORRECT

  DataFrame means(k);
  int tmpidx=0;
  for (auto& cluster : means) {
    //ogni tanto becca lo stesso punto
    //cluster = data[indices(random_number_generator)];
    //cosi' invece...
    cluster = container.at(tmpidx);
    tmpidx+=1;
    //std::cout<<"cluster.x: "<< cluster.x << std::endl;
  }

  //for(int i=0;i<k;i++){
    //std::cout<<"i: " << i << ", means[i].x: "<< means[i].x << std::endl;
  //}

  //std::cout<<""<<std::endl;


  //di sotto dentro le iterazioni si aggiornano le means dei clusters...

  std::vector<size_t> assignments(data.size());

  double w_tot = 0;

  for (size_t iteration = 0; iteration < number_of_iterations; ++iteration) {

    //FWD TAGGER calo
    //408 PbWO4 crystals
    //15x15x200 mm size
    //coverage 1.8 to 5.2 deg

    /*
    if(iteration==0)
    {

      for(int i=0;i<k;i++){
        std::cout<< "number of peaks: "<< k<<std::endl;
        std::cout<<"iteration: "<< iteration <<", original cluster: " << i <<
        ", means[cluster].x: "<< means[i].x <<
        ", means[cluster].y: "<< means[i].y <<
        ", means[cluster].e: "<< means[i].e << std::endl;
        //std::cout<<""<<std::endl;
      }
    }
    */

    //std::cout<<""<<std::endl;

    // Find assignments.
    for (size_t point = 0; point < data.size(); ++point) {
      double best_distance = std::numeric_limits<double>::max();
      double bad_distance = (15.*20.)*(15.*20.);
      double best_time_diff = std::numeric_limits<double>::max();
      double bad_time_diff = 100; //ns
      double best_space_diff = std::numeric_limits<double>::max();
      double bad_space_diff = 15.*1.41*15.; //more than 10 blocks (diag) away
      double min_space_diff = 15.*1.41*2.;  //
      size_t best_cluster = 0;

      //if(data[point].flag_bkgd==false) std::cout<<"flag_bkgd: "<< data[point].flag_bkgd <<std::endl;
      //if(data[point].flag_bkgd==true) std::cout<<"flag_bkgd: "<< data[point].flag_bkgd <<std::endl;

      //sta facendo una strage?

      if(data[point].flag_bkgd==1){// && data[point].e<0.005 // considered as bkgd
        best_cluster = -1;
        assignments[point] = best_cluster;
        continue;
      }


      /*
        std::cout<<"BEFORE 3D DISTANCE"<<std::endl;
        std::cout<<"iteration: "<< iteration<< ", point: "<< point<< ", data[point].x: " << data[point].x\
        <<", data[point].y: " << data[point].y\
        <<", data[point].t: " << data[point].t\
        <<", data[point].e: " << data[point].e << std::endl;
      */


      //it seems that most of the hits have no neighbors, still
      //you have to do some clustering... the above lines kill all stats
      //w/o inclding some threshold in energy

      for (size_t cluster = 0; cluster < k; ++cluster) {
        const double distance =
        //squared_4D_distance(data[point], means[cluster], iteration);
        squared_3D_distance(data[point], means[cluster], iteration);
        //squared_l2_distance(data[point], means[cluster], iteration);

        /*
        std::cout<<"AFTER 3D DISTANCE"<<std::endl;
        std::cout<<"iteration: "<< iteration <<", point: "<< point<< ", cluster: " << cluster <<
        ", means[cluster].x: "<< means[cluster].x <<
        ", means[cluster].y: "<< means[cluster].y <<
        ", means[cluster].t: "<< means[cluster].t <<
        ", means[cluster].e: "<< means[cluster].e << std::endl;
        //std::cout<<"data[point].x: "<< data[point].x << std::endl;
        */

        if (distance < best_distance) {
          best_distance = distance;
          //if(distance==0.) std::cout<< "!!!!!!!!!!!!!SAME POINTS, cluster: "<< cluster<<std::endl;
          best_cluster = cluster; //updated clustering in space and time
        }
        const double time_diff = time_difference(data[point], means[cluster]);
        if (time_diff < best_time_diff) {
          best_time_diff = time_diff;
          //best_cluster = cluster;
        }
        const double space_diff = sqrt(squared_l2_distance(data[point], means[cluster],iteration));
        if (space_diff < best_space_diff) {
          best_space_diff = space_diff;
          //best_cluster = cluster; //if you do this clustering is crappy
        }


      }//end loop on cluster
      //if(iteration==99) std::cout<<"best_distance: "<< best_distance <<std::endl;
      //if(iteration==99) std::cout<<"best_time_diff: "<< best_time_diff <<std::endl;

      //if(best_distance > bad_distance || best_time_diff > bad_time_diff) best_cluster = -1; //mask
      //if(best_time_diff > bad_time_diff) best_cluster = -1; //mask (the hit does not belong to any cluster)
      //if(best_space_diff > bad_space_diff) best_cluster = -1; //mask (the hit does not belong to any cluster)

      assignments[point] = best_cluster;
    }//end loop on points

    //if(iteration==0) std::cout<< " "<<std::endl;


    //----- di seguito le posizioni posson essere pesate con l'energia
    // Sum up and count points for each cluster.
    DataFrame new_means(k);

    double tot_ene(0.);

    std::vector<double> w_tot(k, 0);
    for (size_t point = 0; point < data.size(); ++point) {
      const auto cluster = assignments[point];

      //if(cluster==-1) std::cout<<"@@@@@@@@FILTERING OUT EVENTS cluster=-1@@@@@@@@"<<std::endl;
      if(cluster==-1) continue; //filtering bad hit

      w_tot[cluster]+=data[point].e;

    }


    std::vector<size_t> counts(k, 0);
    std::vector<double> w1(k, 0);
    double tmpw1 = 1.;
    for (size_t point = 0; point < data.size(); ++point) {
      const auto cluster = assignments[point];

      if(cluster==-1) continue; //filtering bad hit
      //if(cluster==-1) std::cout<<"@@@@@@@@FILTERING OUT EVENTS cluster=-1@@@@@@@@"<<std::endl;

      /*
      std::cout << "iteration: "<<iteration<<std::endl;
      std::cout << "data[point].e: "<< data[point].e << std::endl;
      std::cout << "data[point].x: "<< data[point].x << std::endl;
      std::cout << "data[point].y: "<< data[point].y << std::endl;
      std::cout << "data[point].t: "<< data[point].t << std::endl;
      std::cout << "data[point].seede: "<< data[point].seede << std::endl;
      std::cout << "data[point].nhits: "<< data[point].nhits << std::endl;
      std::cout << "cluster: " << cluster << std::endl;
      std::cout<<""<<std::endl;
      */


      //double w1 = std::max(0., (3.45 + std::log(data[point].e/tot_ene)));
      //w1[cluster] = data[point].e;
      //tmpw1 = w1[cluster];
      tmpw1 = data[point].e;

      new_means[cluster].x += tmpw1 * data[point].x;
      new_means[cluster].y += tmpw1 * data[point].y;
      new_means[cluster].x2 += tmpw1 * data[point].x2;
      new_means[cluster].y2 += tmpw1 * data[point].y2;
      new_means[cluster].e += tmpw1;
      new_means[cluster].t += tmpw1 * data[point].t;
      //here seede is defined as the most energetic hit in the cluster
      //if(iteration==0) std::cout<<"new_means[cluster].seede: "<< new_means[cluster].seede <<std::endl;
      if(new_means[cluster].seede < data[point].e) new_means[cluster].seede = data[point].e;
      new_means[cluster].Dt += tmpw1 * data[point].t * data[point].t;

      counts[cluster] += 1;
    }


    // Divide sums by counts to get new centroids.
    for (size_t cluster = 0; cluster < k; ++cluster) {
      // Turn 0/0 into 0/1 to avoid zero division.
      const auto count = std::max<size_t>(1, counts[cluster]);
      float den = 1.;
      /*
      if(new_means[cluster].e>0.){
        den = new_means[cluster].e;
      }
      */
      if(w_tot[cluster]>0.){
        den = w_tot[cluster];
      }
      //if(w_tot[cluster]<0.) std::cout<<"PROBLEM DETECTED: sum of energies is less than 0"<<std::endl;

      means[cluster].x = new_means[cluster].x / den;
      means[cluster].y = new_means[cluster].y / den;
      means[cluster].x2 = new_means[cluster].x2 / den;
      means[cluster].y2 = new_means[cluster].y2 / den;
      means[cluster].e = new_means[cluster].e ;
      means[cluster].seede = new_means[cluster].seede;
      means[cluster].t = new_means[cluster].t /den;
      //if(iteration==99) std::cout<<">>>>>>>>>>>>>>>>>> number of hits: "<< count << std::endl;
      means[cluster].Dt = sqrt(new_means[cluster].Dt/den - means[cluster].t*means[cluster].t);
      means[cluster].nhits = count;//+1?

      /*
      std::cout<<"iteration: "<< iteration <<", updated cluster: " << cluster <<
      ", means[cluster].x: "<< means[cluster].x <<
      ", means[cluster].y: "<< means[cluster].y <<
      ", means[cluster].e: "<< means[cluster].e << std::endl;
      std::cout<<""<<std::endl;
      */
    }

  }//end of iterations

  return means;
}


//----------------

bool compareByEnergy(const  Point&a, const Point&b)
{
    return a.e > b.e;
}

bool compareByX(const  Point&a, const Point&b)
{
    return a.x > b.x;
}



//------------ preprocessing
PostProc compute_means(DataFrame prepr_points){

  PostProc fin;

  float dcr = 1.1*sqrt(2.)*15.;//mm  diagonal crystal, pre-computed hardcoded (CHECK)
  float tmp_dist;
  bool good_peak,flag_multicluster;
  int count_peaks =0;
  int count_cells = 0;

  double time_window_km = 50.;
  double min_ene_km = 100; //keep high energetic single hits?
  double t_diff, x_diff, y_diff;

  bool neighbor_cell;
  bool atleast_oneclose;
  int count_close;

   for(int i=0;i<prepr_points.size();++i){
     good_peak = true;
     flag_multicluster=false;
     atleast_oneclose = false;
     count_cells = 0;
     count_close = 0;

     //never zeros...
     //std::cout<<"x,y,t: "<< prepr_points.at(i).x <<", "<<prepr_points.at(i).y <<", "<< prepr_points.at(i).t << std::endl;

     for(int j=0;j<prepr_points.size();++j){//0 beacuse you want to exclude...
       if(i==j) continue;
       x_diff = std::fabs(prepr_points.at(i).idx - prepr_points.at(j).idx);
       y_diff = std::fabs(prepr_points.at(i).idy - prepr_points.at(j).idy);
       t_diff = std::fabs(prepr_points.at(i).t - prepr_points.at(j).t);
       //if(j==4) break;
       //tmp_dist = sqrt(squared_l2_distance(prepr_points.at(i),prepr_points.at(j)));
       //if(tmp_dist<=dcr){

       //is this working properly?
       neighbor_cell = t_diff<time_window_km && x_diff <= 1 && y_diff <= 1 &&\
          (x_diff + y_diff) > 0; //is this working properly?



       if(neighbor_cell==true){

         atleast_oneclose = true;
         count_close+=1;

         if(prepr_points.at(i).e<prepr_points.at(j).e){
           good_peak = false;
           //cout<<"break loop because of energy (i, j, ei, ej): "<< i<<" "<<j<<" "<<" "<<prepr_points.at(i).e<<" "<<prepr_points.at(j).e<<endl;
           break;
         }
         if(prepr_points.at(i).e>prepr_points.at(j).e){
         //    good_peak *= true;
           count_cells += 1;
         }
         if(prepr_points.at(i).e==prepr_points.at(j).e && i>j) flag_multicluster=true;

       }

       //if(tmp_dist>dcr) continue;
       else if(neighbor_cell==false){
          continue; //skip j
        }
       //cout<<"(i,j): "<<i<<", "<<j<<", distance: "<< tmp_dist <<endl;

       //if(good_cluster==false) continue;

     }//end j loop

     if(good_peak==true && flag_multicluster==false && count_cells>=1) count_peaks+=1;
     //std::cout<<"count_cells: "<< count_cells << std::endl;

     /*
     //INCLUDE SINGLE HITS IF ENOUGH ENERGETIC
     if(count_cells==0 && prepr_points.at(i).e<min_ene_km){
       //std::cout<<"found 0 neighbors cells, flagging this hit"<<std::endl;
       prepr_points.at(i).flag_bkgd = 1; //if true you will remove it
      }
     else if(count_cells==0 && prepr_points.at(i).e>min_ene_km){
       count_peaks+=1; //adding single energetic hits to cluster algorithm
     }*/

     //DO NOT INCLUDE ISOLATED HITs
     //if(atleast_oneclose==false) prepr_points.at(i).flag_bkgd = 1;
     //std::cout<<"count_close: " << count_close << std::endl;
     //if(count_close<1 && prepr_points.at(i).e<min_ene_km) prepr_points.at(i).flag_bkgd = 1;
     //NEED TO WORK HERE

     //a more stringent cut is labeling as bkgd ALL isolated hits
     if(count_close<1) prepr_points.at(i).flag_bkgd = 1;

     if(prepr_points.at(i).x==0. && prepr_points.at(i).y==0. && \
     prepr_points.at(i).t==0.){
       //std::cout<< "excluding point with all zeros by default"<<std::endl;
       prepr_points.at(i).flag_bkgd = 1;
     }

     //ERASE BKGDs ELEMENT
     if(prepr_points.at(i).flag_bkgd == 1) prepr_points.erase(prepr_points.begin()+i);


     //if(count_cells<1){
       //std::cout<<"flag before: " << prepr_points.at(i).flag_bkgd << std::endl;
       //prepr_points.at(i).flag_bkgd = 1;
       //std::cout<<"flag after: " << prepr_points.at(i).flag_bkgd << std::endl;
       //std::cout<<""<<std::endl;
     //}
   }

   //std::cout<<">>>>>>> number of peaks: "<< count_peaks << std::endl;

   //postpr_points=prepr_points;

   fin.kmeans = count_peaks;
   fin.res = prepr_points;
   //return count_peaks;
   return fin;
}
