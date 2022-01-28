/*
 * TranslationTable_factory.h
 *
 *  Created on: Jan 11, 2020
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_TT_TRANSLATIONTABLE_FACTORY_H_
#define SRC_LIBRARIES_TT_TRANSLATIONTABLE_FACTORY_H_

#include "JANA/JFactoryT.h"
#include "TranslationTable.h"


class TranslationTable_factory: public JFactoryT<TranslationTable> {
public:
	TranslationTable_factory();
	virtual ~TranslationTable_factory();

	// Overloaded by user Factories
	virtual void Init();
	virtual void ChangeRun(const std::shared_ptr<const JEvent> &aEvent);
	virtual void Process(const std::shared_ptr<const JEvent> &aEvent);

private:

};

#endif /* SRC_LIBRARIES_TT_TRANSLATIONTABLE_FACTORY_H_ */
