//
//    File: FTCalCluster.h
// Created: Wed Jan  8 12:49:31 CET 2020
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Jefferson Science Associates LLC Copyright Notice:
// Copyright 251 2014 Jefferson Science Associates LLC All Rights Reserved. Redistribution
// and use in source and binary forms, with or without modification, are permitted as a
// licensed user provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//    list of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products derived
//    from this software without specific prior written permission.
// This material resulted from work developed under a United States Government Contract.
// The Government retains a paid-up, nonexclusive, irrevocable worldwide license in such
// copyrighted data to reproduce, distribute copies to the public, prepare derivative works,
// perform publicly and display publicly and to permit others to do so.
// THIS SOFTWARE IS PROVIDED BY JEFFERSON SCIENCE ASSOCIATES LLC "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
// JEFFERSON SCIENCE ASSOCIATES, LLC OR THE U.S. GOVERNMENT BE LIABLE TO LICENSEE OR ANY
// THIRD PARTES FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef _FTCalCluster_h_
#define _FTCalCluster_h_

#include <JANA/JObject.h>
#include "system/JObjectDrawable.h"
#include <vector>
#include "FTCalHit.h"
#include "TVector3.h"
#include "TH2D.h"
#include "TCanvas.h"
//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class FTCalCluster: public JObjectDrawable {
public:

	FTCalCluster();

	FTCalCluster(int clusid);

	virtual ~FTCalCluster();

	void setClusterID(int clusid);

	void setCluster(int , float , \
		float , double , double , double , \
		double , double , double);

	void computeCluster(int, double, double);

	int getClusterId() const;

	//Return number of crystal in a cluster.
	int getClusterSize() const;

	//Return energy of a cluster.
	float getClusterEnergy() const;

	//Return energy of a cluster with correction.
	float getClusterFullEnergy() const;

	void setClusterFullEnergy(float ene);

	float getClusterSeedEnergy() const;

	double getClusterTime() const;

	//Return center of cluster.
	TVector3 getCentroid() const;

	double getX() const;

	double getY() const;

	double getZ() const;

	double getXX() const;

	double getYY() const;

	double getWidthX() const;

	double getWidthY() const;

	double getRadius() const;

	double getTheta() const;

	double getPhi() const;

	const FTCalHit* getHit(int i) const;

	bool isGoodCluster() const;

	//Controlla se l'hit analizzato e' contenuto nel cluster.
	bool containsHit(const FTCalHit *hit, double time_min, double time_max) const;

	void push_hit(const FTCalHit *hit);

	void toHisto(TH2D *h) const;
	virtual TCanvas* Draw(int id = 0) const;

	virtual void Summarize(JObjectSummary& summary) const {
		summary.add(_clusCenter.X(), "X", "%f", "Y");
		summary.add(_clusCenter.Y(), "Y", "%f", "X");
		summary.add(_clusEnergy, "ENE", "%f", "ENERGY - MeV");
		summary.add(_clusTime, "TIME", "%f", "TIME - ns");
	}

protected:
	mutable TH2D *hCLUS;

private:

	int _clusID;
	int _clusSize;
	float _clusEnergy;
	float _clusRecEnergy;
	double _clusTime, _clusDt;
	double _clusXX, _clusYY;
	double _clusSigmaX, _clusSigmaY;
	double _clusRadius;
	float _clusSeedEnergy;
	double _clusTheta, _clusPhi;
	bool _goodCluster;
	TVector3 _clusCenter;
	std::vector<const FTCalHit*> hits;

	double CRYS_ZPOS, depth_z;

};

#endif // _FTCalCluster_h_
