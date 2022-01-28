/*
 * TriggerDecision_FTCalCluster_factory.h
 *
 *  Created on: Jan 13, 2020
 *      Author: celentan
 *
 *  Modified on Aug. 15, 2020
 *              David Lawrence
 */

#ifndef _TRIGGERDECISION_FTCALCLUSTER_FACTORY_H_
#define _TRIGGERDECISION_FTCALCLUSTER_FACTORY_H_

#include <functional>

#include "Trigger/TriggerDecision.h"
#include "JANA/JFactoryT.h"
#include "FT/FTCalCluster.h"


class TriggerDecision_FTCalCluster_factory: public JFactoryT<TriggerDecision> {
public:
	TriggerDecision_FTCalCluster_factory();
	// virtual ~TriggerDecision_FTCalCluster_factory() = default;
	virtual ~TriggerDecision_FTCalCluster_factory();

	// Overloaded by user Factories
	virtual void Init();
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);

	enum {
		MODE_Ncluster = 0,
		MODE_Energy_Cut =1
	};

	bool   ENABLED;
	int    TRIGGER_MODE;
	std::map<std::string, int> FACTORY_TAGS;
	int    MIN_CLUSTERS;
	int    MIN_CLUSTER_SIZE;
	double MIN_SEED_ENERGY;
	double MIN_CLUSTER_ENERGY;

	// This is used to hold a lambda function that decides
	// whether a cluster is included in the count for the
	// the trigger. See comments in Init().
	std::function<bool(const FTCalCluster*)> lambda;
};

#endif // _TRIGGERDECISION_FTCALCLUSTER_FACTORY_H_
