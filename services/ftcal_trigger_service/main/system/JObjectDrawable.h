/*
 * JObjectDrawable.h
 *
 *  Created on: Apr 3, 2016
 *      Author: celentan
 */

#ifndef SRC_LIBRARIES_SYSTEM_JOBJECTDRAWABLE_H_
#define SRC_LIBRARIES_SYSTEM_JOBJECTDRAWABLE_H_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "TObject.h"

using namespace std;

class TCanvas;

class JObjectDrawable: public JObject {
public:
	JObjectDrawable();
	virtual ~JObjectDrawable();
	virtual TCanvas* Draw(int id = 0) const {
		return m_canvas;
	} //!

protected:
	mutable TCanvas *m_canvas; //!

ClassDef(JObjectDrawable,1);

};

#endif /* SRC_LIBRARIES_SYSTEM_JOBJECTDRAWABLE_H_ */
