/*
 * FTCalCluster_factory
 *
 *  Created on: Dec 23, 2019
 *      Author: cfanelli
 */

#ifndef SRC_LIBRARIES_FT_FTCALCluster_FACTORY_KM_H_
#define SRC_LIBRARIES_FT_FTCALCluster_FACTORY_KM_H_

#include "JANA/JFactoryT.h"
#include "FTCalCluster.h"

class FTCalCluster_factory_km: public JFactoryT<FTCalCluster> {
public:
	FTCalCluster_factory_km();
	virtual ~FTCalCluster_factory_km();

	// Overloaded by user Factories
	virtual void Init();
	virtual void ChangeRun(const std::shared_ptr<const JEvent> &aEvent);
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);


private:
	//I am hard-coding the indexes here, since it will always be 1 sector, 1 layer, 332 components.
	std::map<int, double> c0, c1, c2, c3, c4; //not really true, since the components are 332, but they are not continuous from 1 to 332!
	void CorrectClusterEnergy(FTCalCluster *cluster);
};


//------------- implementing kmeans ----------------//



#endif /* SRC_LIBRARIES_FT_FTCALCluster_FACTORY_KM_H_ */
