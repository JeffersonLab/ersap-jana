#ifndef _HDBSCAN_MAIN_H_
#define _HDBSCAN_MAIN_H_

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

#include "FT/kmeans/kmeans.h"




//Point
//if point is a hit seede = e, nhits =1
//if point is an equivalent cluster, seede = max_e, nhits>=1


//DataFrame hdbscan_main(const DataFrame& );

DataFrame hdbscan_main(const DataFrame& , int dim, int par1, int par2);

#endif
