/*
 * FTHodoHit_factory
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_FT_FTHODOHIT_FACTORY_H_
#define SRC_LIBRARIES_FT_FTHODOHIT_FACTORY_H_

#include "JANA/JFactoryT.h"
#include "FTHodoHit.h"
#include "TT/TranslationTable.h"
class TranslationTable;

class FTHodoHit_factory: public JFactoryT<FTHodoHit> {
public:


	FTHodoHit_factory();
	virtual ~FTHodoHit_factory();

	// Overloaded by user Factories
	virtual void Init();
	virtual void ChangeRun(const std::shared_ptr<const JEvent> &aEvent);
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);

private:


	TranslationTable const *m_tt;
	double daq_gain[3][22][16];

	double mips_charge_hodo[8][2][20];
	double mips_energy_hodo[8][2][20];
};

#endif /* SRC_LIBRARIES_FT_FTHODOHIT_FACTORY_H_ */
