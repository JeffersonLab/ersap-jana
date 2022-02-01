/*
 * JObjectDrawable.cc
 *
 *  Created on: Apr 3, 2016
 *      Author: celentan
 */

#include "JObjectDrawable.h"

#include "TCanvas.h"

JObjectDrawable::JObjectDrawable():m_canvas(0){
}

JObjectDrawable::~JObjectDrawable() {
	if (m_canvas) delete m_canvas;
	m_canvas=0;
}


