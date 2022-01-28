#ifndef _JFACTORYGENERATOR_TT_H_
#define _JFACTORYGENERATOR_TT_H_

#include <JANA/JFactoryGenerator.h>
#include <JANA/JFactoryT.h>
//#include "MyCluster.h"
//#include "MyHit.h"

#include "TT/TranslationTable_factory.h"


class JFactoryGenerator_TT:public JFactoryGenerator{
	public:

	void GenerateFactories(JFactorySet *factory_set){
		factory_set->Add(new TranslationTable_factory());
	}
};

#endif   // _JFACTORYGENERATOR_EXAMPLE2_H_
