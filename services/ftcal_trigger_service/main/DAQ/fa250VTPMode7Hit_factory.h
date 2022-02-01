/*
 * faWaveboardHit_factory.h
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_DAQ_FA250VTPMODE7HIT_FACTORY_H_
#define SRC_LIBRARIES_DAQ_FA250VTPMODE7HIT_FACTORY_H_

#include "JANA/JFactoryT.h"
#include "fa250VTPMode7Hit.h"

class fa250VTPMode7Hit_factory: public JFactoryT<fa250VTPMode7Hit> {
public:
	fa250VTPMode7Hit_factory();
	virtual ~fa250VTPMode7Hit_factory();

	// Overloaded by user Factories
	virtual void Init();
	virtual void ChangeRun(const std::shared_ptr<const JEvent> &aEvent);
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);
};

#endif /* SRC_LIBRARIES_DAQ_FA250VTPMODE7HIT_FACTORY_H_*/
