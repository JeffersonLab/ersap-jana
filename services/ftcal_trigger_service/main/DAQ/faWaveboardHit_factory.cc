/*
 * faWaveboardHitfactory.cc
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#include "faWaveboardHit_factory.h"
#include "TridasEvent.h"

#include "JANA/JEvent.h"

using namespace std;

faWaveboardHit_factory::faWaveboardHit_factory() {
	// TODO Auto-generated constructor stub

}

faWaveboardHit_factory::~faWaveboardHit_factory() {
	// TODO Auto-generated destructor stub
}

void faWaveboardHit_factory::Init() {

}
void faWaveboardHit_factory::ChangeRun(const std::shared_ptr<const JEvent> &aEvent) {

}
void faWaveboardHit_factory::Process(const std::shared_ptr<const JEvent> &aEvent) {

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
	/*The Hit Time is, probably, an absolute time, and we don't want this, we just care about the relative time in the event.
	 *Hence, I take the earliest hit time and remove it from the all hits.
	 *Note that I loop on all hits in tridas_event, hence even if I mix fa250VTPMode7Hit and waveveboardHit, that is ok, since the faWaveboardHit factory has the same code.
	 */
	T4nsec firstTime = tridas_event->hits[0].time;
	for (auto hit : tridas_event->hits) {
		if (hit.time < firstTime) firstTime = hit.time;
	}

	for (auto hit : tridas_event->hits) {
		if (hit.type == fadcHit_TYPE::WAVEBOARD) {
			// Create a faWaveboardHit object;
			auto faHit = new faWaveboardHit;

			faHit->m_charge = hit.charge;
			faHit->m_time = hit.time - firstTime;

			faHit->m_channel.crate = hit.crate;
			faHit->m_channel.slot = hit.slot;
			faHit->m_channel.channel = hit.channel;

			//Assign the samples
			faHit->samples = hit.data;

			//Assign the dT
			faHit->m_dT = 4; //Todo: read from dB

			//Add to the framework
			mData.push_back(faHit);
		}
	}
}

