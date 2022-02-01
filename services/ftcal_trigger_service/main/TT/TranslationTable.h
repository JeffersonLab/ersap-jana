// $Id$
//
//    File: TranslationTable.h
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TranslationTable_
#define _TranslationTable_

#include <map>
#include <iostream>
#include "JANA/JObject.h"

class JApplication;
class JCalibrationManager;
class TranslationTable: public JObject {

public:
	TranslationTable(JApplication *app, int runN);
	~TranslationTable();

	// Each detector system has its own native indexing scheme.
	// Here, we define a class for each of them that has those
	// indexes. These are then used below in the DChannelInfo
	// class to relate them to the DAQ indexing scheme of
	// crate, slot, channel.
	struct csc_t {
		uint8_t crate;
		uint8_t slot;
		uint8_t channel;

		inline bool operator==(const struct csc_t &rhs) const {
			return (crate == rhs.crate) && (slot == rhs.slot) && (channel == rhs.channel);
		}

		csc_t() {
		}

		virtual ~csc_t() {
		}

		void setCrateSlotChannel(uint8_t cr, uint8_t sl, uint8_t ch) {
			crate = cr;
			slot = sl;
			channel = ch;
		}
		//...................................
		// Less than operator for csc_t data types. This is used by
		// the map<csc_t, XX> to order the entires by key
		bool operator<(const TranslationTable::csc_t &b) const {
			if (crate < b.crate)
				return true;
			if (crate > b.crate)
				return false;
			if (slot < b.slot)
				return true;
			if (slot > b.slot)
				return false;
			if (channel < b.channel)
				return true;
			if (channel > b.channel)
				return false;
			return false;
		}
		//std::string print()const{return Form("DAQ crate: %i slot: %i channel: %i",rocid,slot,channel);}

	};

	enum Detector_t {
		UNKNOWN_DETECTOR, FTCAL, FTHODO, HallDCAL, NUM_DETECTOR_TYPES
	};

	std::string DetectorName(Detector_t type) const {
		switch (type) {
		case FTCAL:
			return "FTCAL";
			break;
		case FTHODO:
			return "FTHODO";
			break;
		case HallDCAL:
			return "HallDCAL";
			break;
		case UNKNOWN_DETECTOR:
		default:
			return "UNKNOWN";
		}
	}

	/*Here follows the classes to handle the IDs and intrinsic indexes of the different detectors
	 *
	 * There are some conventions on few indexes:
	 *
	 * - sector is an index regarding different, identical, detectors (in BDX we think about independent, but equal, detectors)
	 * - readout is an index to handle the case of different sensors reading out the same active volume
	 *
	 * Since we use these indexes both for the readout sensor and for the active volume, the convention is to use
	 * readout = 0 when dealing with the PHYSICAL volume
	 * readout = 1 ... Nreadouts when dealing with the sensors
	 * */

	class FTHODO_Index_t: public JObject {
	public:
		int sector;
		int layer;
		int component;
		int& ID(int n) {
			switch (n) {
			case 0:
				return sector;
				break;
			case 1:
				return layer;
				break;
			case 2:
				return component;
				break;
			default:
				std::cerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 3;
		}
		inline bool isSameActive(const FTHODO_Index_t &rhs) const {
			return (sector == rhs.sector) && (layer == rhs.layer) && (component == rhs.component);
		}
		inline bool operator==(const FTHODO_Index_t &rhs) const {
			return isSameActive(rhs);
		}
		inline bool operator<(const FTHODO_Index_t &rhs) const { //A.C. for the maps
			if (sector > rhs.sector)
				return true;
			if (sector < rhs.sector)
				return false;
			if (layer > rhs.layer)
				return true;
			if (layer < rhs.layer)
				return false;
			if (component > rhs.component)
				return true;
			if (component < rhs.component)
				return false;
			return false;
		}
		std::string name() const {
			return "FTHODO";
		}
		std::string print() const {
			char buf[50];
			sprintf(buf, "FTHODO sector: %i layer: %i component: %i", sector, layer, component);
			return std::string(buf);
		}
	};

	class FTCAL_Index_t: public JObject {
	public:
		int sector;
		int layer;
		int component;
		int8_t iX, iY; //these two are not used to in the == and < operators, but are here for convenience.
		int& ID(int n) {
			switch (n) {
			case 0:
				return sector;
				break;
			case 1:
				return layer;
				break;
			case 2:
				return component;
				break;
			default:
				std::cerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 3;
		}

		inline bool isSameActive(const FTCAL_Index_t &rhs) const {
			return (sector == rhs.sector) && (layer == rhs.layer) && (component == rhs.component);
		}
		inline bool operator==(const FTCAL_Index_t &rhs) const {
			return isSameActive(rhs);
		}
		inline bool operator<(const FTCAL_Index_t &rhs) const { //A.C. for the maps
			if (sector > rhs.sector)
				return true;
			if (sector < rhs.sector)
				return false;
			if (layer > rhs.layer)
				return true;
			if (layer < rhs.layer)
				return false;
			if (component > rhs.component)
				return true;
			if (component < rhs.component)
				return false;
			return false;
		}
		std::string name() const {
			return "FTCAL";
		}
		std::string print() const {
			char buf[50];
			sprintf(buf, "FTCAL sector: %i x: %i y: %i", sector, iX, iY);
			return std::string(buf);
		}
	};

	class HallDCAL_Index_t: public JObject {
	public:

		int iX, iY; //these two are not used to in the == and < operators, but are here for convenience.
		int& ID(int n) {
			switch (n) {
			case 0:
				return iX;
				break;
			case 1:
				return iY;
				break;
			default:
				std::cerr << "Wrong id" << std::endl;
				break;
			}
		}
		static const int nIDs() {
			return 2;
		}

		inline bool isSameActive(const HallDCAL_Index_t &rhs) const {
			return (iX == rhs.iX) && (iY == rhs.iY);
		}
		inline bool operator==(const HallDCAL_Index_t &rhs) const {
			return isSameActive(rhs);
		}
		inline bool operator<(const HallDCAL_Index_t &rhs) const { //A.C. for the maps
			if (iX > rhs.iX)
				return true;
			if (iX < rhs.iX)
				return false;
			if (iY > rhs.iY)
				return true;
			if (iY < rhs.iY)
				return false;

			return false;
		}
		std::string name() const {
			return "HallDCAL";
		}
		std::string print() const {
			char buf[50];
			sprintf(buf, "HallDCAL x: %i y: %i", iX, iY);
			return std::string(buf);
		}
	};

	/*A single class that handles ALL the possible indexes trough a C++ union*/
	class ChannelInfo: public JObject {
	public:
		csc_t CSC;   //This is crate - slot - channel
		//DModuleType::type_id_t module_type;
		Detector_t det_sys;
		union {
			FTHODO_Index_t *FTHODO;
			FTCAL_Index_t *FTCAL;
			HallDCAL_Index_t *HallDCAL;
		};
	};

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	virtual void Summarize(JObjectSummary& summary) const {

	}

	void ReadTranslationTableHALLB();
	void ReadTranslationTableHALLD();
	/*Here goes the methods to return the channel info (detector name / detector-specific id) given the csc*/
	TranslationTable::ChannelInfo getChannelInfo(const csc_t &csc) const;
	TranslationTable::ChannelInfo getChannelInfo(int crate, int slot, int channel) const;

	int GetVerbose() const {
		return m_verbose;
	}
	;

private:
	int m_verbose;
	int m_runN;
	JApplication* m_japp;

	/****************************************** STATIC-VARIABLE-ACCESSING PRIVATE MEMBER FUNCTIONS ******************************************/

	//Some variables needs to be shared amongst threads (e.g. the memory used for the branch variables)
	//However, you cannot make them global/extern/static/static-member variables in the header file:
	//The header file is included in the TTAB library AND in each plugin that uses it
	//When a header file is included in a src file, it's contents are essentially copied directly into it
	//Thus there are two instances of each static variable: one in each translation unit (library)
	//Supposedly(?) they are linked together during runtime when loading, so there is (supposedly) no undefined behavior.
	//However, this causes a double free (double-deletion) when these libraries are closed at the end of the program, crashing it.
	//Thus the variables must be in a single source file that is compiled into a single library
	//However, you (somehow?) cannot make them global/extern variables in the cpp function
	//This also (somehow?) causes the double-free problem above for (at least) stl containers
	//It works for pointers-to-stl-containers and fundamental types, but I dunno why.
	//It's not good encapsulation anyway though.
	//THE SOLUTION:
	//Define the variables as static, in the source file, WITHIN A PRIVATE MEMBER FUNCTION.
	//Thus the static variables themselves only have function scope.
	//Access is only available via the private member functions, thus access is fully controlled.
	//They are shared amongst threads, so locks are necessary, but since they are private this class can handle it internally
	pthread_mutex_t& Get_TT_Mutex(void) const;
	bool& Get_TT_Initialized(void) const;
	std::map<TranslationTable::csc_t, TranslationTable::ChannelInfo>& Get_TT(void) const;

};

#endif // _TranslationTable_

