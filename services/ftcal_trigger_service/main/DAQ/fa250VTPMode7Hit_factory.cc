/*
 * faWaveboardHitfactory.cc
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#include "TridasEvent.h"
#include "DAQ/chronoTypeDef.h"
#include "JANA/JEvent.h"
#include "fa250VTPMode7Hit_factory.h"
#include "JANA/JException.h"


using namespace std;


fa250VTPMode7Hit_factory::fa250VTPMode7Hit_factory() {
	// TODO Auto-generated constructor stub

}

fa250VTPMode7Hit_factory::~fa250VTPMode7Hit_factory() {
	// TODO Auto-generated destructor stub
}

void fa250VTPMode7Hit_factory::Init() {

}
void fa250VTPMode7Hit_factory::ChangeRun(const std::shared_ptr<const JEvent> &aEvent) {

}
void fa250VTPMode7Hit_factory::Process(const std::shared_ptr<const JEvent> &aEvent) {

	std::vector<const TridasEvent*> tridas_events;
	try {
		tridas_events = aEvent->Get<TridasEvent>();
	} catch (JException &e) {
		return;
	}
	if (tridas_events.size() != 1) {
		return;
	}
	auto tridas_event = tridas_events[0];
//loop over the hits in the tridas event
	/*	At the moment (2020), TriDAS can be feed by three hardware sources:

	 - Waveboard (V1)
	 - Waveboard (V2)
	 - fa250 stream trough VTP

	 In the future, there may be more than one hit type per event, if the readout architecture is mixed.
	 Ideally, one would recognize the hit type from the hit itself, and have one JANA2 factory per hit - hits may be later processed differently.
	 One can judge this from the crate/slot/channel combination, but this is setup-dependent.

	 For the moment, this is not supported. Hence, for the moment I differentiate between waveboard and fa250 by considering that the fa250 has no samples, only time and charge.

	 */

	/*The Hit Time is, probably, an absolute time, and we don't want this, we just care about the relative time in the event.
	 *Hence, I take the earliest hit time and remove it from the all hits.
	 *Note that I loop on all hits in tridas_event, hence even if I mix fa250VTPMode7Hit and waveveboardHit, that is ok.
	 */
	T4nsec firstTime = tridas_event->hits[0].time;
	for (auto hit : tridas_event->hits) {
		if (hit.time < firstTime) firstTime = hit.time;
	}

	for (auto hit : tridas_event->hits) {
		if (hit.type == fadcHit_TYPE::FA250VTPMODE7) {
			// Create a faWaveboardHit object;
			auto faHit = new fa250VTPMode7Hit;

			faHit->m_charge = hit.charge;
			faHit->m_time = hit.time - firstTime;

			faHit->m_channel.crate = hit.crate;
			faHit->m_channel.slot = hit.slot;
			faHit->m_channel.channel = hit.channel;

#define WORK_AROUND
#ifdef WORK_AROUND
			if (hit.crate == 0) faHit->m_channel.crate = 70;
			if (hit.crate == 1) faHit->m_channel.crate = 71;
			if (hit.crate == 2) faHit->m_channel.crate = 72;
#endif


			//Add to the framework
			mData.push_back(faHit);
		}
	}
}

