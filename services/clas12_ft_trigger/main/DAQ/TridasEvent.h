//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_TRIDASEVENT_H
#define JANA2_TRIDASEVENT_H

#include "DAQ/chronoTypeDef.h"
#include "JANA/JObject.h"

/*In order to minimize interference between Tridas and JANA,
 * we define here a minimal structure representing a single FADC hit.
 * This is supposed to not change during time.
 */

enum fadcHit_TYPE {
	WAVEBOARD = 0x00, FA250VTPMODE7 = 0x01
};

struct fadcHit {
	uint8_t crate, slot, channel;
	T4nsec time;
	float charge;  				//Charge corrected by ADC pedestals
	std::vector<uint16_t> data; //I assume the hits are sampled with 16bit digitizers at maximum

	fadcHit_TYPE type;
};

/// TridasEvent is an adapter between TRIDAS objects and JANA objects. The event source should
/// insert one TridasEvent into each JEvent. This is mostly for the online part.
struct TridasEvent: public JObject {

	std::vector<fadcHit> hits;

	int run_number;
	int event_number;
	int time_slice;

	mutable std::vector<uint32_t> triggerWords;

	mutable bool should_keep;  // TODO: This needs to be mutable because we will be updating a const JObject
							   //       This won't be a problem with the 'real' TRIDAS, whose should_keep lives behind the
							   //       tridas_data pointer, but we should start thinking about a more elegant way to handle
							   //       the general case.
};

#endif //JANA2_TRIDASEVENT_H
