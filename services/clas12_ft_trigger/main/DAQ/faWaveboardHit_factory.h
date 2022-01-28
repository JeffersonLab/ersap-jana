/*
 * faWaveboardHit_factory.h
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_DAQ_FAWAVEBOARDHIT_FACTORY_H_
#define SRC_LIBRARIES_DAQ_FAWAVEBOARDHIT_FACTORY_H_

#include "JANA/JFactoryT.h"
#include "faWaveboardHit.h"

class faWaveboardHit_factory: public JFactoryT<faWaveboardHit> {
public:
	faWaveboardHit_factory();
	virtual ~faWaveboardHit_factory();

	// Overloaded by user Factories
	virtual void Init();
	virtual void ChangeRun(const std::shared_ptr<const JEvent> &aEvent);
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);
};

#endif /* SRC_LIBRARIES_DAQ_FAWAVEBOARDHIT_FACTORY_H_ */
