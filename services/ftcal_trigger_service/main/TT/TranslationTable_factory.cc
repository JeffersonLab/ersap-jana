/*
 * TranslationTable_factory.cc
 *
 *  Created on: Jan 11, 2020
 *      Author: celentan
 */
#include "JANA/JEvent.h"
#include "TranslationTable_factory.h"

TranslationTable_factory::TranslationTable_factory() {
	// TODO Auto-generated constructor stub

}

TranslationTable_factory::~TranslationTable_factory() {
	// TODO Auto-generated destructor stub

}

// Overloaded by user Factories
void TranslationTable_factory::Init() {
	//std::cout << "TranslationTable_factory::Init() " << this << " "<<m_tt<< std::endl;
	/*This is very, very important: the Reset Method is not called at every event, so we do not clear data, and we
	 can get the TranslationTable also in evnt method of other factories / event processors*/
	//SetFactoryFlag (PERSISTENT);
}
void TranslationTable_factory::ChangeRun(const std::shared_ptr<const JEvent> &aEvent) {
	//std::cout << "TranslationTable_factory::ChangeRun() " << this << " insert new TT into event" << std::endl;

/*	if (m_tt == 0) {
		m_tt = new TranslationTable(aEvent->GetJApplication(), aEvent->GetRunNumber());
		if (m_tt == 0) {
			throw JException("TranslationTable_factory::ChangeRun() m_tt is zero?");
		}
	}

	aEvent->Insert(m_tt);*/

}
void TranslationTable_factory::Process(const std::shared_ptr<const JEvent> &aEvent) {
	aEvent->Insert(new TranslationTable(aEvent->GetJApplication(),aEvent->GetRunNumber()));
}
