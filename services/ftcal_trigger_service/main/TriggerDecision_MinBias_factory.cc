/*
 * TriggerDecision_MinBias_factory.cc
 *
 *  Created on: Jan 13, 2020
 *      Author: celentan
 *
 *  Modified on Aug. 15, 2020
 *              David Lawrence
 */

#include "TriggerDecision_MinBias_factory.h"

#include "JANA/JEvent.h"

static std::atomic<uint64_t> Nevents(0);

//-----------------------------------------------
// TriggerDecision_MinBias_factory (constructor)
//-----------------------------------------------
TriggerDecision_MinBias_factory::TriggerDecision_MinBias_factory(){
  mTag="MinBias";//A.C. is this the right way to set the tag? (D.L. for the moment yes. SetTag() available in next release)
}

//-----------------------------------------------
// Init
//-----------------------------------------------
void TriggerDecision_MinBias_factory::Init(){

	ENABLED = true;
	PRESCALE_FACTOR     = 50; // trigger on one event out of this many
	
	mApp->SetDefaultParameter("TRIGGER:MinBias:ENABLED", ENABLED, "Set to 0 to disable the MinBias trigger completely (no TriggerDecision objects will be produced).");
	mApp->SetDefaultParameter("TRIGGER:MinBias:PRESCALE_FACTOR", PRESCALE_FACTOR, "Prescale factor for min bias events. (n.b. 0 and 1 will both trigger on every event.");
	
	if( PRESCALE_FACTOR == 0 ) PRESCALE_FACTOR = 1; // protect from division by zero
}

//-----------------------------------------------
// Process
//-----------------------------------------------
void TriggerDecision_MinBias_factory::Process(const std::shared_ptr<const JEvent> &aEvent){

	if( !ENABLED ) return; // allow user to disable this via JANA config. param.
	
	// Create TriggerDecision object to publish the decision
	// Argument is trigger description. It will end up in metadata file so keep it simple.
	// I think a good convention here is to just give it the tag of the factory.
	auto mTriggerDecision = new TriggerDecision( mTag );
	bool decision = ((Nevents++)%PRESCALE_FACTOR) == 0;
	mTriggerDecision->SetDecision( decision );
	mTriggerDecision->SetID(0x02); // this will show up in 16 high order bit in TriggeredEvent::plugin_nseeds[] (lower 16 will be 0 or 1 depending on whether trigger fired)
	mData.push_back(mTriggerDecision);
}


