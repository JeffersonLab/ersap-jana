#ifndef SRC_LIBRARIES_KMEANS_H_
#define SRC_LIBRARIES_KMEANS_H_

#include <stdio.h>
#include <stddef.h>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <random>
#include <vector>
#include <string>
#include <TH2I.h>
#include <TH2F.h>
#include <TFile.h>
#include <TRandom.h>
#include <TMarker.h>
#include <TCanvas.h>
#include <TPolyMarker3D.h>

//Point
//if point is a hit seede = e, nhits =1
//if point is an equivalent cluster, seede = max_e, nhits>=1
struct Point {
  float e{0.},seede{0.};
  double x{0.},y{0.},t{0.},x2{0.},y2{0.},Dt{0.}; //average time radius
  int nhits{0},idx{0},idy{0};
  int flag_bkgd{0};//all false by default
};


double square(double value);

double time_difference(Point , Point );

double squared_l2_distance(Point , Point , int);

double squared_e2_distance(Point , Point );

double squared_3D_distance(Point , Point , int );

double squared_4D_distance(Point , Point , int );

using DataFrame = std::vector<Point>;

DataFrame k_means(const DataFrame& ,size_t , size_t );

DataFrame k_dummy(const DataFrame& );

struct PostProc {
  int kmeans{0};
  DataFrame res;
};

PostProc compute_means(DataFrame );

//DataFrame postpr_points;

//DataFrame getPrepr_points();


#endif
