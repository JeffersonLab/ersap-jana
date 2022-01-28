//
//    File: FTCalHit.h
// Created: Wed Jan  8 12:49:28 CET 2020
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

#ifndef _FTCalHit_h_
#define _FTCalHit_h_

#include "TT/TranslationTable.h"
#include "DAQ/chronoTypeDef.h"
#include <JANA/JObject.h>
#include <JANA/JFactory.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief class description.
///
/// Detailed class description.
//////////////////////////////////////////////////////////////////////////////////////////////////
class FTCalHit : public JObject {
public:
	FTCalHit();
	virtual ~FTCalHit();

	float getHitEnergy() const {
		return energy;
	}

	void setHitEnergy(float charge) {
		this->energy = charge;
	}

	float getHitTime() const {
		return time;
	}

	void setHitTime(float time) {
		this->time = time;
	}

	double getHitX() const{
		return  _X;
	}

	double getHitY() const{
		return _Y;
	}


	int getHitIX() const{
		return m_channel.iX;
	}

	int getHitIY() const{
		return m_channel.iY;
	}

	double getHitZ() const{
		return _Z;
	}

	double getTimeWalkCorrection() const {
		return twCorrection;
	}

	void setTimeWalkCorrection(double twcorr) {
		this->twCorrection = twcorr;
	}


	void setHitX(double x){_X=x;}
	void setHitY(double y){_Y=y;}
	void setHitZ(double z){_Z=z;}

	virtual void Summarize(JObjectSummary& summary) const {
			summary.add(_X, "X", "%f", "Y");
			summary.add(_Y, "Y", "%f", "X");
			summary.add(energy, "ENE", "%f", "ENERGY - MeV");
			summary.add(time, "TIME", "%f", "TIME - ns");
		}


	TranslationTable::FTCAL_Index_t m_channel;

protected:

private:
	float time;
	float energy;
	double twCorrection;

	double _X,_Y,_Z;




};

#endif // _FTCalHit_h_
