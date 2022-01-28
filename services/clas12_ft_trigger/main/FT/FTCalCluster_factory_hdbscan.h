/*
 * FTCalCluster_factory
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_FT_FTCALCluster_FACTORY_HDBSCAN_H_
#define SRC_LIBRARIES_FT_FTCALCluster_FACTORY_HDBSCAN_H_

#include "JANA/JFactoryT.h"
#include "FTCalCluster.h"

class FTCalCluster_factory_hdbscan: public JFactoryT<FTCalCluster> {
public:
	FTCalCluster_factory_hdbscan();
	virtual ~FTCalCluster_factory_hdbscan();

	// Overloaded by user Factories
	virtual void Init();
	virtual void ChangeRun(const std::shared_ptr<const JEvent> &aEvent);
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);
	static bool compareHits(const FTCalHit* a, const FTCalHit* b);

private:
	//I am hard-coding the indexes here, since it will always be 1 sector, 1 layer, 332 components.
	std::map<int, double> c0, c1, c2, c3, c4; //not really true, since the components are 332, but they are not continuous from 1 to 332!
	void CorrectClusterEnergy(FTCalCluster *cluster);
};

#endif /* SRC_LIBRARIES_FT_FTCALCluster_FACTORY_HDBSCAN_H_ */
