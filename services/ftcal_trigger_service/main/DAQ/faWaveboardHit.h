// $Id$
//
//    File: fa250Mode1Hit.h
// Created: Mon Dec 28 11:04:04 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _fa250WaveboardHit_
#define _fa250WaveboardHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "faHit.h"
#include "chronoTypeDef.h"

#include <vector>

#include "TH1D.h"
#include "TCanvas.h"


class faWaveboardHit: public faHit {
public:


	faWaveboardHit():hWave(0){m_canvas=0;};
	virtual ~faWaveboardHit(){};
	std::vector<uint16_t> samples; //these are the raw samples
	float m_dT; //sampling time (ns)

	T4nsec m_time; //in ns

	float m_charge; //as reported from the waveboard


	virtual void Summarize(JObjectSummary& summary) const {
		summary.add(m_channel.crate,"CRATE","%i","The crate id of this board");
		summary.add(m_channel.slot,"SLOT","%i","The slot of this board");
		summary.add(m_channel.channel,"CHANNEL","%i","The channel");
		summary.add(m_charge,"CHARGE","%f","Charge");
		summary.add(4 * m_time.count(),"TIME","%i","time");

};

void toHisto(TH1D *h) const;

virtual TCanvas* Draw(int id=0) const;

protected:
mutable TH1D *hWave;
};

#endif // _fa250Mode1Hit_

