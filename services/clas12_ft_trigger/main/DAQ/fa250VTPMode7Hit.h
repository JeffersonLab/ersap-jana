// $Id$
//
//    File: fa250Mode1Hit.h
// Created: Mon Dec 28 11:04:04 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _fa250VTPMode7Hit_
#define _fa250VTPMode7Hit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "faHit.h"

#include "chronoTypeDef.h"

#include <vector>


class fa250VTPMode7Hit: public faHit {
public:


	fa250VTPMode7Hit(){};
	virtual ~fa250VTPMode7Hit(){};

	T4nsec m_time; //in ns
	float m_charge; //as reported from the waveboard

	virtual void Summarize(JObjectSummary& summary) const {
		summary.add(m_channel.crate,"CRATE","%f","The crate id of this board");
		summary.add(m_channel.slot,"SLOT","%f","The slot of this board");
		summary.add(m_channel.channel,"CHANNEL","%f","The channel");
};

//void toHisto(TH1D *h) const;

//virtual TCanvas* Draw(int id=0) const;

protected:
//mutable TH1D *hWave;
};

#endif // _fa250Mode1Hit_

