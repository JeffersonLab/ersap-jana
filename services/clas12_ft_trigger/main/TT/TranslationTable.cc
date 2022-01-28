#include "TranslationTable.h"

#include <expat.h>
#include <sstream>
#include "JANA/JApplication.h"
#include "JANA/JException.h"
#include "JANA/Services/JParameterManager.h"
#include "JANA/Calibrations/JCalibration.h"
#include "JANA/Calibrations/JCalibrationManager.h"
using namespace std;

// Use one translation table for all threads
pthread_mutex_t& TranslationTable::Get_TT_Mutex(void) const {
	static pthread_mutex_t tt_mutex = PTHREAD_MUTEX_INITIALIZER;
	return tt_mutex;
}

bool& TranslationTable::Get_TT_Initialized(void) const {
	static bool tt_initialized = false;
	return tt_initialized;
}

map<TranslationTable::csc_t, TranslationTable::ChannelInfo>& TranslationTable::Get_TT(void) const {
	static map<TranslationTable::csc_t, TranslationTable::ChannelInfo> TT;
	return TT;
}

TranslationTable::TranslationTable(JApplication *app, int runN) :
		m_japp(app), m_runN(runN), m_verbose(0) {
	/*In preparation for multiple runs, in different configurations, we foresee diffeent ways to read in the translation table.
	 * We use the parameter RUNTYPE to do this (RUNTYPE may be used also somewhere else)
	 *
	 * Hall-B FT tests:
	 *
	 * 	RUNTYPE=HALLB
	 * 	The translation table is in CCDB (mysql://clas12reader@clasdb.jlab.org/clas12), default variation
	 *
	 * 	FT-CAL: table /daq/tt/ftcal
	 * 	FT-HODO: table /daq/tt/fthodo
	 * 	CRATE		 SLOT		  CHANNEL	   SECTOR		LAYER	     COMPONENT	  ORDER
	 * 	71         | 8          | 2          | 1          | 0          | 176        | 0          |
	 *
	 *
	 */
	string tttype;
//#define WORK_AROUND

	 if (m_japp->GetJParameterManager()->Exists("RUNTYPE") == false) {
	 throw JException("RUNTYPE PARAMETER NOT SPECIFIED");
	 }


	 m_japp->GetJParameterManager()->GetParameter("RUNTYPE",tttype);
	 std::cout << "TranslationTable ttype is: -->" << tttype << " <--- "<< std::endl;fflush(stdout);


#ifdef WORK_AROUND
	tttype = "HALLB";
#endif

	if (tttype == "HALLB") {
		ReadTranslationTableHALLB();
	} else if (tttype == "HALLD") {
		ReadTranslationTableHALLD();
	} else {
		ostringstream strm;
		strm << "RUNTYPE PARAMETER NOT SUPPORTED BY TranslationTable: " << tttype;
		throw JException(strm.str());
	}

}
void TranslationTable::ReadTranslationTableHALLD() {

	//TODO
	for (int ii = 0; ii < 9; ii++) {
		TranslationTable::csc_t csc;
		csc.crate = 0;
		csc.slot = 1;
		csc.channel = ii;

		TranslationTable::ChannelInfo ch;
		ch.det_sys = TranslationTable::HallDCAL;
		ch.HallDCAL = new TranslationTable::HallDCAL_Index_t;
		ch.HallDCAL->iX=ii/3; //TODO
		ch.HallDCAL->iY=ii%3; //TODO


		//insert into TT data - [] operator creates a new entry in map
		Get_TT()[csc] = ch;
	}
}
void TranslationTable::ReadTranslationTableHALLB() {
	//std::cout << "ReadTranslationTableHALLB() start" << std::endl;

	// It seems expat is not thread safe so we lock a mutex here and
	// read in the translation table just once
	pthread_mutex_lock(&Get_TT_Mutex());
	if (Get_TT_Initialized()) {
		pthread_mutex_unlock(&Get_TT_Mutex());
		return;
	}

	//Try to read from CCDB

	auto jcalib_manager = m_japp->GetService<JCalibrationManager>();
	auto jcalib = jcalib_manager->GetJCalibration(m_runN);

	//Load FT-CAL
	vector<map<string, int> > ttdata;
	jcalib->Get("/daq/tt/ftcal", ttdata);

	std::cout << "ReadTranslationTableHALLB(): ftcal size: " << ttdata.size() << std::endl;

	for (auto line : ttdata) {
		TranslationTable::csc_t csc;
		csc.crate = line["crate"];
		csc.slot = line["slot"];
		csc.channel = line["chan"];

		TranslationTable::ChannelInfo ch;
		ch.det_sys = TranslationTable::FTCAL;
		ch.FTCAL = new TranslationTable::FTCAL_Index_t;
		ch.FTCAL->sector = line["sector"];
		ch.FTCAL->layer = line["layer"];
		ch.FTCAL->component = line["component"];

		//insert into TT data - [] operator creates a new entry in map
		Get_TT()[csc] = ch;
	}

	//Load FT-HODO;
	ttdata.clear();
	jcalib->Get("/daq/tt/fthodo", ttdata);
	std::cout << "ReadTranslationTableHALLB(): fthodo size: " << ttdata.size() << std::endl;

	for (auto line : ttdata) {
		TranslationTable::csc_t csc;
		csc.crate = line["crate"];
		csc.slot = line["slot"];
		csc.channel = line["chan"];

		TranslationTable::ChannelInfo ch;
		ch.det_sys = TranslationTable::FTHODO;
		ch.FTHODO = new TranslationTable::FTHODO_Index_t;
		ch.FTHODO->sector = line["sector"];
		ch.FTHODO->layer = line["layer"];
		ch.FTHODO->component = line["component"];

		//insert into TT data - [] operator creates a new entry in map
		Get_TT()[csc] = ch;
	}

	Get_TT_Initialized() = true;
	pthread_mutex_unlock(&Get_TT_Mutex());

}

/*TranslationTable::TranslationTable(JEventLoop *loop){


 // Default is to just read translation table from CCDB. If this fails,
 // then an attempt will be made to read from a file on the local disk.
 // The filename can be specified to be anything, but if the user specifies
 // this, then we assume that they want to use it and skip using the CCDB.
 // They may also specify that they want to skip checking the CCDB via
 // the "TT:NO_CCDB" parameter. This would only be useful if they want to
 // force the use of a local file named "tt.xml".
 NO_CCDB = false;
 XML_FILENAME = "tt.xml";
 VERBOSE = 0;
 SYSTEMS_TO_PARSE = "";
 gPARMS->GetParameter("TT:NO_CCDB", NO_CCDB);
 JParameter *p = gPARMS->GetParameter("TT:XML_FILENAME", XML_FILENAME);
 if (p->GetDefault() != p->GetValue()){
 NO_CCDB = true;
 }
 gPARMS->GetParameter("TT:VERBOSE");
 gPARMS->GetParameter("TT:ROCID_MAP_FILENAME", ROCID_MAP_FILENAME);

 gPARMS->GetParameter("TT:SYSTEMS_TO_PARSE", SYSTEMS_TO_PARSE);

 // Initialize dedicated JStreamLog used for debugging messages
 ttout.SetTag("--- TT ---: ");
 ttout.SetTimestampFlag();
 ttout.SetThreadstampFlag();

 // Look for and read in an optional rocid <-> rocid translation table
 //ReadOptionalROCidTranslation();

 // Read in Translation table. This will create DChannelInfo objects
 // and store them in the "TT" map, indexed by csc_t objects
 ReadTranslationTable(loop->GetJCalibration());


 }*/

//---------------------------------
// ~DTranslationTable    (Destructor)
//---------------------------------
TranslationTable::~TranslationTable() {

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//  The following routines access the translation table
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static TranslationTable::Detector_t DetectorStr2DetID(string &type);
static void StartElement(void *userData, const char *xmlname, const char **atts);
static void EndElement(void *userData, const char *xmlname);

//---------------------------------
// ReadTranslationTable
//---------------------------------
/*void TranslationTable::ReadTranslationTable(JCalibration *jcalib)
 {
 // It seems expat is not thread safe so we lock a mutex here and
 // read in the translation table just once
 pthread_mutex_lock(&Get_TT_Mutex());
 if (Get_TT_Initialized()) {
 pthread_mutex_unlock(&Get_TT_Mutex());
 return;
 }

 // String to hold entire XML translation table
 string tt_xml;

 // Try getting it from CCDB first
 if (jcalib && !NO_CCDB) {
 map<string,string> tt;
 string namepath = "Translation/DAQ2detector";
 jout << "Reading translation table from calib DB: " << namepath << " ..." << std::endl;
 jcalib->GetCalib(namepath, tt);
 if (tt.size() != 1) {
 jerr << " Error: Unexpected translation table format!" << std::endl;
 jerr << "        tt.size()=" << tt.size() << " (expected 1)" << std::endl;
 }else{
 // Copy table into tt string
 map<string,string>::iterator iter = tt.begin();
 tt_xml = iter->second;
 }
 }

 // If getting from CCDB fails, try just reading in local file
 if (tt_xml.size() == 0) {
 if (!NO_CCDB) jout << "Unable to get translation table from CCDB." << std::endl;
 jout << "Will try reading TT from local file: " << XML_FILENAME << std::endl;

 // Open file
 ifstream ifs(XML_FILENAME.c_str());
 if (! ifs.is_open()) {
 jerr << " Error: Cannot open file! Translation table unavailable." << std::endl;
 pthread_mutex_unlock(&Get_TT_Mutex());
 return;
 }

 // read lines into stringstream object
 stringstream ss;
 while (ifs.good()) {
 char line[4096];
 ifs.getline(line, 4096);
 ss << line;
 }

 // Close file
 ifs.close();

 // Copy from stringstream to tt
 tt_xml = ss.str();
 }

 // create parser and specify element handlers
 XML_Parser xmlParser = XML_ParserCreate(NULL);
 if (xmlParser == NULL) {
 jerr << "readTranslationTable...unable to create parser" << std::endl;
 exit(EXIT_FAILURE);
 }
 XML_SetElementHandler(xmlParser,StartElement,EndElement);
 XML_SetUserData(xmlParser, &Get_TT());

 // Parse XML string
 int status=XML_Parse(xmlParser, tt_xml.c_str(), tt_xml.size(), 1); // "1" indicates this is the final piece of XML
 if (status == 0) {
 jerr << "  ?readTranslationTable...parseXMLFile parse error for " << XML_FILENAME << std::endl;
 jerr << XML_ErrorString(XML_GetErrorCode(xmlParser)) << std::endl;
 }

 jout << Get_TT().size() << " channels defined in translation table" << std::endl;
 XML_ParserFree(xmlParser);

 pthread_mutex_unlock(&Get_TT_Mutex());
 Get_TT_Initialized() = true;
 if (VERBOSE > 6){
 std::map<TranslationTable::csc_t, TranslationTable::ChannelInfo>::const_iterator it;
 for (it=Get_TT().begin();it!=Get_TT().end();it++){
 jout<<"Crate: "<<(it)->first.rocid<<" Slot: "<<(it)->first.slot<<" "<<" Channel: "<<(it)->first.channel<<" Detector: "<<DetectorName((it)->second.det_sys)<<endl;
 }
 }

 }
 */

TranslationTable::ChannelInfo TranslationTable::getChannelInfo(const csc_t &csc) const {
	TranslationTable::ChannelInfo m_channel;
	/*Get the map, search for csc, return an iterator*/
	map<csc_t, ChannelInfo>::const_iterator iter = Get_TT().find(csc);

	if (iter == Get_TT().end()) {
		/*	if (m_verbose > 6) {
		 std::cout << 1.*csc.crate << " " << 1.*csc.slot << " " << 1.*csc.channel << " ";
		 std::cout << "    - Didn't find it " << m_verbose<<" "<<std::endl;
		 }*/
		m_channel.det_sys = UNKNOWN_DETECTOR;
		return m_channel;
	}
	const ChannelInfo &chaninfo = iter->second;
	/*if (m_verbose > 6) { //TODO!!
	 std::cout << 1.*csc.crate << " " << 1.*csc.slot << " " << 1.*csc.channel << " ";
	 std::cout <<"     - Found entry for: " << DetectorName(chaninfo.det_sys) <<" "<<this<<" "<<m_verbose<<std::endl;
	 }*/
	m_channel = chaninfo;
	return m_channel;
}

TranslationTable::ChannelInfo TranslationTable::getChannelInfo(int crate, int slot, int channel) const {
	TranslationTable::csc_t csc;
	csc.setCrateSlotChannel(crate, slot, channel);
	return this->getChannelInfo(csc);
}
//---------------------------------
// DetectorStr2DetID
//---------------------------------
TranslationTable::Detector_t DetectorStr2DetID(string &type) {
	if (type == "FTCAL") {
		return TranslationTable::FTCAL;
	} else if (type == "FTHODO") {
		return TranslationTable::FTHODO;
	} else
		return TranslationTable::UNKNOWN_DETECTOR;

}

/*

 //---------------------------------
 // StartElement
 //---------------------------------
 void StartElement(void *userData, const char *xmlname, const char **atts) {
 static int crate = 0, slot = 0;

 static string type, Type;
 //	int mc2codaType= 0;
 int channel = 0;
 string Detector, locSystem;
 //	int end=0;
 int row = 0, column = 0, module = 0, sector = 0, layer = 0, component = 0, readout = 0;
 int id = 0;
 //	int side=0;

 // This complicated line just recasts the userData pointer into
 // a reference to the "TT" member of the TranslationTable object
 // that called us.
 map<TranslationTable::csc_t, TranslationTable::ChannelInfo> &TT = *((map<TranslationTable::csc_t, TranslationTable::ChannelInfo>*) userData);

 // store crate summary info, fill both maps
 if (strcasecmp(xmlname, "bdx_translation_table") == 0) {
 // do nothing

 } else if (strcasecmp(xmlname, "crate") == 0) {
 for (int i = 0; atts[i]; i += 2) {
 if (strcasecmp(atts[i], "number") == 0) {
 crate = atoi(atts[i + 1]);
 break;
 }
 }

 } else if (strcasecmp(xmlname, "slot") == 0) {
 for (int i = 0; atts[i]; i += 2) {
 if (strcasecmp(atts[i], "number") == 0) {
 slot = atoi(atts[i + 1]);
 } else if (strcasecmp(atts[i], "type") == 0) {
 Type = string(atts[i + 1]);
 type = string(atts[i + 1]);
 std::transform(type.begin(), type.end(), type.begin(), (int (*)(int)) tolower);}
 }

 // The detID value set here shows up in the header of the Data Block Bank
 // of the output file. It should be set to one if this crate has JLab
 // made modules that output in the standard format (see document:
 // "VME Data Format Standards for JLAB Modules"). These would include
 // f250ADC, f125ADC, F1TDC, .... Slots containing other types of modules
 // (e.g. CAEN1290) should have their own unique detID. We use detID of
 // zero for non-digitizing modules like CPUs nd TIDs even though potentially,
 // one could read data from these.
 //      mc2codaType = ModuleStr2ModID(type);

 } else if (strcasecmp(xmlname,"channel") == 0) {

 for (int i=0; atts[i]; i+=2) {
 string tag(atts[i+0]);
 string sval(atts[i+1]);

 int ival = atoi(atts[i+1]);

 if (tag == "number")
 channel = ival;
 else if (tag == "detector")
 Detector = sval;
 else if (tag == "sector")
 sector = ival;
 else if (tag == "readout")
 readout = ival;
 else if ((tag == "row")||(tag == "iy"))
 row = ival;
 else if ((tag == "col")|| (tag == "column")||(tag == "ix"))
 column = ival;
 else if (tag == "module")
 module = ival;
 else if (tag == "sector")
 sector = ival;
 else if (tag == "layer")
 layer = ival;
 else if (tag == "component")
 component = ival;
 else if (tag == "id")
 id = ival;
 }

 TranslationTable::csc_t csc;
 csc.setCrateSlotChannel((uint32_t)crate,(uint32_t)slot,(uint32_t)channel);
 string detector = Detector;
 std::transform(detector.begin(), detector.end(), detector.begin(), (int(*)(int)) tolower);

 //string s="unknown::";

 // Common indexes
 TranslationTable::ChannelInfo &ci = TT[csc];
 ci.CSC = csc;
 //      ci.module_type = (DModuleType::type_id_t)mc2codaType;
 ci.det_sys = DetectorStr2DetID(detector);
 //     TranslationTable::Get_ROCID_By_System()[ci.det_sys].insert(crate);

 // detector-specific indexes
 //This is the part that - probably - has to be modified
 switch (ci.det_sys) {
 case TranslationTable::EXT_VETO:
 ci.ext_veto = new TranslationTable::EXT_VETO_Index_t;
 ci.ext_veto->sector = sector;
 ci.ext_veto->layer = layer;
 ci.ext_veto->component = component;
 ci.ext_veto->readout = readout;
 break;
 case TranslationTable::INT_VETO:
 ci.int_veto = new TranslationTable::INT_VETO_Index_t;
 ci.int_veto->sector = sector;
 ci.int_veto->layer = layer;
 ci.int_veto->component = component;
 ci.int_veto->readout = readout;
 break;
 case TranslationTable::CALORIMETER:
 ci.calorimeter = new TranslationTable::CALO_Index_t;
 ci.calorimeter->sector = sector;
 ci.calorimeter->x = column;  //A.C. fine, x is a column and y is a row
 ci.calorimeter->y = row;
 ci.calorimeter->readout = readout;
 break;
 case TranslationTable::PADDLES:
 ci.paddles = new TranslationTable::PADDLES_Index_t;
 ci.paddles->id = id;
 break;
 case TranslationTable::UNKNOWN_DETECTOR:
 default:
 break;
 }

 } else {
 jerr << std::endl << std::endl
 << "?startElement...unknown xml tag " << xmlname
 << std::endl << std::endl;
 }

 }
 */
//--------------------------------------------------------------------------
void EndElement(void *userData, const char *xmlname) {
	// nothing to do yet...
}

//--------------------------------------------------------------------------

