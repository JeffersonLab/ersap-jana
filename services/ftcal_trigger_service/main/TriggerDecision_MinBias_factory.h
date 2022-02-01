/*
 * TriggerDecision_MinBias_factory.h
 *
 *  Created on: Jan 13, 2020
 *      Author: celentan
 *
 *  Modified on Aug. 15, 2020
 *              David Lawrence
 */

#ifndef _TRIGGERDECISION_MINBIAS_FACTORY_H_
#define _TRIGGERDECISION_MINBIAS_FACTORY_H_

#include <atomic>

#include "Trigger/TriggerDecision.h"
#include "JANA/JFactoryT.h"

class TriggerDecision_MinBias_factory: public JFactoryT<TriggerDecision> {
public:
	TriggerDecision_MinBias_factory();
	virtual ~TriggerDecision_MinBias_factory() = default;

	// Overloaded by user Factories
	virtual void Init();
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);

	bool ENABLED;
	uint64_t PRESCALE_FACTOR;
};

#endif /* _TRIGGERDECISION_MINBIAS_FACTORY_H_ */
