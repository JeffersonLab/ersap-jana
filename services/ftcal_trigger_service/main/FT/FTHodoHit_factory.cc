/*
 * faWaveboardHitfactory.cc
 *
 *  Created on: Dec 23, 2019
 *      Author: celentan
 */

#include "FTHodoHit_factory.h"
#include "JANA/JEvent.h"

#include "DAQ/faWaveboardHit.h"
#include "DAQ/fa250VTPMode7Hit.h"
#include "TT/TranslationTable.h"

#include "JANA/Services/JParameterManager.h"
#include "JANA/Calibrations/JCalibration.h"
#include "JANA/Calibrations/JCalibrationManager.h"
#include<vector>

//I am hard-coding the indexes here, since it will always be 8 sectors, 2 layers, 20 components.
double px[8][2][20];
double py[8][2][20];
double pz[8][2][20];

static auto fadc_to_charge = 4*0.4884/50.;


FTHodoHit_factory::FTHodoHit_factory() {
	// TODO Auto-generated constructor stub
	m_tt = 0;
}

FTHodoHit_factory::~FTHodoHit_factory() {
	// TODO Auto-generated destructor stub
}

void FTHodoHit_factory::Init() {

}
void FTHodoHit_factory::ChangeRun(const std::shared_ptr<const JEvent> &aEvent) {

	//TODO: get the TT
	std::cout << "FTHodoHit_factory::ChangeRun run number: " << aEvent->GetRunNumber() << " " << aEvent->GetEventNumber() <<" "<< this<<std::endl;

	static int hasLoaded=0;
	if (m_tt==0) m_tt = aEvent->GetSingle<TranslationTable>();

	if (hasLoaded == 0) {
		auto jcalib_manager = japp->GetService<JCalibrationManager>();
	//	auto jcalib = jcalib_manager->GetJCalibration(aEvent->GetEventNumber());
		auto jcalib = jcalib_manager->GetJCalibration(13000);
		//Load FT-HODO
		std::vector<std::vector<double>> data;
		jcalib->Get("/geometry/ft/fthodo", data);

		for (auto row : data) {
			//sector - layer - component - x - y - z - width - thickness
			auto sector = int(row[0]);
			auto layer = int(row[1]);
			auto component = int(row[2]);
			px[sector - 1][layer - 1][component - 1] = row[3];
			py[sector - 1][layer - 1][component - 1] = row[4];
			pz[sector - 1][layer - 1][component - 1] = row[5];
		}




		//Load FT-HODO
		std::vector<std::vector<double>> data_calib;
		auto jcalib_calib = jcalib_manager->GetJCalibration(13000);
		jcalib_calib->Get("/calibration/ft/fthodo/charge_to_energy", data_calib);

		for (auto row : data_calib) {
			//sector - layer - component - mips_charge - mips_energy - fadc_to_charge - preamp_gain - apd_gain
			auto sector = int(row[0]);
			auto layer = int(row[1]);
			auto component = int(row[2]);
			mips_charge_hodo[sector - 1][layer - 1][component - 1] = row[3];
			mips_energy_hodo[sector - 1][layer - 1][component - 1] = row[4];
		//	cout << sector<< " "<<layer<<" "<<" "<<layer<<" "<<mips_charge[component]<<endl;
		}

		daq_gain[2][3][0] = 0.020601;
		daq_gain[2][3][1] = 0.021433;
		daq_gain[2][3][2] = 0.020831;
		daq_gain[2][3][3] = 0.021973;
		daq_gain[2][3][4] = 0.021393;
		daq_gain[2][3][5] = 0.021927;
		daq_gain[2][3][6] = 0.021269;
		daq_gain[2][3][7] = 0.021230;
		daq_gain[2][3][8] = 0.021478;
		daq_gain[2][3][9] = 0.019070;
		daq_gain[2][3][10] = 0.019228;
		daq_gain[2][3][11] = 0.019411;
		daq_gain[2][3][12] = 0.020821;
		daq_gain[2][3][13] = 0.018868;
		daq_gain[2][3][14] = 0.020336;
		daq_gain[2][3][15] = 0.019917;

		daq_gain[2][4][0] = 0.022190;
		daq_gain[2][4][1] = 0.020896;
		daq_gain[2][4][2] = 0.020819;
		daq_gain[2][4][3] = 0.020936;
		daq_gain[2][4][4] = 0.020890;
		daq_gain[2][4][5] = 0.021249;
		daq_gain[2][4][6] = 0.020788;
		daq_gain[2][4][7] = 0.021339;
		daq_gain[2][4][8] = 0.020333;
		daq_gain[2][4][9] = 0.019671;
		daq_gain[2][4][10] = 0.019029;
		daq_gain[2][4][11] = 0.018571;
		daq_gain[2][4][12] = 0.018944;
		daq_gain[2][4][13] = 0.018853;
		daq_gain[2][4][14] = 0.018011;
		daq_gain[2][4][15] = 0.021368;

		daq_gain[2][5][0] = 0.020767;
		daq_gain[2][5][1] = 0.020350;
		daq_gain[2][5][2] = 0.020470;
		daq_gain[2][5][3] = 0.020307;
		daq_gain[2][5][4] = 0.019516;
		daq_gain[2][5][5] = 0.019823;
		daq_gain[2][5][6] = 0.019682;
		daq_gain[2][5][7] = 1.000000;
		daq_gain[2][5][8] = 0.021573;
		daq_gain[2][5][9] = 0.020951;
		daq_gain[2][5][10] = 0.020685;
		daq_gain[2][5][11] = 0.022037;
		daq_gain[2][5][12] = 0.020514;
		daq_gain[2][5][13] = 0.021058;
		daq_gain[2][5][14] = 0.020723;
		daq_gain[2][5][15] = 0.020710;

		daq_gain[2][6][0] = 0.021675;
		daq_gain[2][6][1] = 0.021106;
		daq_gain[2][6][2] = 0.021436;
		daq_gain[2][6][3] = 0.020985;
		daq_gain[2][6][4] = 0.021042;
		daq_gain[2][6][5] = 0.020920;
		daq_gain[2][6][6] = 0.021033;
		daq_gain[2][6][7] = 0.020926;
		daq_gain[2][6][8] = 0.018119;
		daq_gain[2][6][9] = 0.017262;
		daq_gain[2][6][10] = 0.021625;
		daq_gain[2][6][11] = 0.022225;
		daq_gain[2][6][12] = 0.017906;
		daq_gain[2][6][13] = 0.022037;
		daq_gain[2][6][14] = 0.021827;
		daq_gain[2][6][15] = 0.021419;

		daq_gain[2][7][0] = 0.020629;
		daq_gain[2][7][1] = 0.009768;
		daq_gain[2][7][2] = 0.020872;
		daq_gain[2][7][3] = 0.020629;
		daq_gain[2][7][4] = 0.021087;
		daq_gain[2][7][5] = 0.020579;
		daq_gain[2][7][6] = 0.009768;
		daq_gain[2][7][7] = 0.021168;
		daq_gain[2][7][8] = 0.022556;
		daq_gain[2][7][9] = 0.009768;
		daq_gain[2][7][10] = 0.016259;
		daq_gain[2][7][11] = 0.021763;
		daq_gain[2][7][12] = 0.016493;
		daq_gain[2][7][13] = 0.009768;
		daq_gain[2][7][14] = 0.022269;
		daq_gain[2][7][15] = 0.020946;


		daq_gain[2][8][0] = 0.020784;
		daq_gain[2][8][1] = 0.021133;
		daq_gain[2][8][2] = 0.021470;
		daq_gain[2][8][3] = 0.021206;
		daq_gain[2][8][4] = 0.021807;
		daq_gain[2][8][5] =  0.021790;
		daq_gain[2][8][6] =  0.021864;
		daq_gain[2][8][7] = 0.021133;
		daq_gain[2][8][8] = 0.021104;
		daq_gain[2][8][9] = 0.019687;
		daq_gain[2][8][10] = 0.021141;
		daq_gain[2][8][11] =  0.021471;
		daq_gain[2][8][12] = 0.018883;
		daq_gain[2][8][13] = 0.019443;
		daq_gain[2][8][14] = 0.019646;
		daq_gain[2][8][15] = 0.018956;

		daq_gain[2][9][0] = 0.021623 ;
		daq_gain[2][9][1] = 0.021407;
		daq_gain[2][9][2] = 0.021861;
		daq_gain[2][9][3] = 0.021311;
		daq_gain[2][9][4] = 0.020795;
		daq_gain[2][9][5] = 0.044481;
		daq_gain[2][9][6] = 0.021133;
		daq_gain[2][9][7] = 0.021485;
		daq_gain[2][9][8] = 0.009768;
		daq_gain[2][9][9] = 0.076278;
		daq_gain[2][9][10] = 0.018371;
		daq_gain[2][9][11] = 0.022906;
		daq_gain[2][9][12] = 0.021076;
		daq_gain[2][9][13] = 0.021229;
		daq_gain[2][9][14] = 0.018810;
		daq_gain[2][9][15] = 0.022460;

		daq_gain[2][10][0] = 0.021293;
		daq_gain[2][10][1] = 0.021872;
		daq_gain[2][10][2] = 0.021443;
		daq_gain[2][10][3] = 0.021392;
		daq_gain[2][10][4] = 0.009768;
		daq_gain[2][10][5] = 0.009768;
		daq_gain[2][10][6] = 0.024220;
		daq_gain[2][10][7] = 0.020965;
		daq_gain[2][10][8] = 0.022321;
		daq_gain[2][10][9] = 0.020780;
		daq_gain[2][10][10] = 0.020554;
		daq_gain[2][10][11] = 0.021448;
		daq_gain[2][10][12] = 0.009768;
		daq_gain[2][10][13] = 0.014867;
		daq_gain[2][10][14] = 0.021551;
		daq_gain[2][10][15] = 0.021892;

		daq_gain[2][11][0] = 1.;
		daq_gain[2][11][1] = 1.;
		daq_gain[2][11][2] = 1.;
		daq_gain[2][11][3] = 1.;
		daq_gain[2][11][4] = 1.;
		daq_gain[2][11][5] = 1.;
		daq_gain[2][11][6] = 1.;
		daq_gain[2][11][7] = 1.;
		daq_gain[2][11][8] = 1.;
		daq_gain[2][11][9] = 1.;
		daq_gain[2][11][10] = 1.;
		daq_gain[2][11][11] = 1.;
		daq_gain[2][11][12] = 1.;
		daq_gain[2][11][13] = 1.;
		daq_gain[2][11][14] = 1.;
		daq_gain[2][11][15] = 1.;

		daq_gain[2][12][0] = 1.;
		daq_gain[2][12][1] = 1.;
		daq_gain[2][12][2] = 1.;
		daq_gain[2][12][3] = 1.;
		daq_gain[2][12][4] = 1.;
		daq_gain[2][12][5] = 1.;
		daq_gain[2][12][6] = 1.;
		daq_gain[2][12][7] = 1.;
		daq_gain[2][12][8] = 1.;
		daq_gain[2][12][9] = 1.;
		daq_gain[2][12][10] =1.;
		daq_gain[2][12][11] = 1.;
		daq_gain[2][12][12] = 1.;
		daq_gain[2][12][13] = 1.;
		daq_gain[2][12][14] = 1.;
		daq_gain[2][12][15] = 1.;

		daq_gain[2][13][0] = 0.021978;
		daq_gain[2][13][1] = 0.020897;
		daq_gain[2][13][2] = 0.022082;
		daq_gain[2][13][3] = 0.022142;
		daq_gain[2][13][4] = 0.022040;
		daq_gain[2][13][5] = 0.021688;
		daq_gain[2][13][6] = 0.009768;
		daq_gain[2][13][7] = 0.022248;
		daq_gain[2][13][8] = 0.022448;
		daq_gain[2][13][9] = 0.020895;
		daq_gain[2][13][10] = 0.009768;
		daq_gain[2][13][11] = 0.020245;
		daq_gain[2][13][12] = 0.016737;
		daq_gain[2][13][13] = 0.018649;
		daq_gain[2][13][14] = 0.021772;
		daq_gain[2][13][15] = 0.021918;

		daq_gain[2][14][0] = 0.021285;
		daq_gain[2][14][1] = 0.021573;
		daq_gain[2][14][2] = 0.021804;
		daq_gain[2][14][3] = 0.009768;
		daq_gain[2][14][4] = 0.021562;
		daq_gain[2][14][5] = 0.021711;
		daq_gain[2][14][6] = 0.020419;
		daq_gain[2][14][7] = 0.020535;
		daq_gain[2][14][8] = 0.009768;
		daq_gain[2][14][9] = 0.009768;
		daq_gain[2][14][10] = 0.019652;
		daq_gain[2][14][11] = 0.022003;
		daq_gain[2][14][12] = 0.022265;
		daq_gain[2][14][13] = 0.021385;
		daq_gain[2][14][14] = 0.020919;
		daq_gain[2][14][15] = 0.021383;

		daq_gain[2][15][0] = 0.021684;
		daq_gain[2][15][1] = 0.020251;
		daq_gain[2][15][2] = 0.021126;
		daq_gain[2][15][3] = 0.019273;
		daq_gain[2][15][4] = 0.020612;
		daq_gain[2][15][5] = 0.021637;
		daq_gain[2][15][6] = 0.020695;
		daq_gain[2][15][7] = 0.021108;
		daq_gain[2][15][8] = 0.020355;
		daq_gain[2][15][9] = 0.020743;
		daq_gain[2][15][10] = 0.020782;
		daq_gain[2][15][11] = 0.021093;
		daq_gain[2][15][12] = 0.017792;
		daq_gain[2][15][13] = 0.018598;
		daq_gain[2][15][14] = 0.021715;
		daq_gain[2][15][15] = 0.020847;

		daq_gain[2][16][0] = 0.020886;
		daq_gain[2][16][1] = 0.020830;
		daq_gain[2][16][2] = 0.021258;
		daq_gain[2][16][3] = 0.021490;
		daq_gain[2][16][4] = 0.021235;
		daq_gain[2][16][5] = 0.020690;
		daq_gain[2][16][6] = 0.021460;
		daq_gain[2][16][7] = 0.021573;
		daq_gain[2][16][8] = 0.020983;
		daq_gain[2][16][9] = 0.020689;
		daq_gain[2][16][10] = 0.021674;
		daq_gain[2][16][11] = 0.021660;
		daq_gain[2][16][12] = 0.019970;
		daq_gain[2][16][13] = 0.020580;
		daq_gain[2][16][14] = 0.020817;
		daq_gain[2][16][15] = 0.021310;


		daq_gain[2][17][0] = 0.021894;
		daq_gain[2][17][1] = 0.021445;
		daq_gain[2][17][2] = 0.021112;
		daq_gain[2][17][3] = 0.022185;
		daq_gain[2][17][4] = 0.021464;
		daq_gain[2][17][5] = 0.019741;
		daq_gain[2][17][6] = 0.020868;
		daq_gain[2][17][7] = 0.021001;
		daq_gain[2][17][8] = 0.019038;
		daq_gain[2][17][9] = 0.018902;
		daq_gain[2][17][10] = 0.021820;
		daq_gain[2][17][11] = 0.020646;
		daq_gain[2][17][12] = 0.017644;
		daq_gain[2][17][13] = 0.020805;
		daq_gain[2][17][14] = 0.020947;
		daq_gain[2][17][15] = 0.022021;


		daq_gain[2][18][0] = 0.021880;
		daq_gain[2][18][1] = 0.021475;
		daq_gain[2][18][2] = 0.021725;
		daq_gain[2][18][3] = 0.020583;
		daq_gain[2][18][4] = 0.020532;
		daq_gain[2][18][5] = 0.021553;
		daq_gain[2][18][6] = 0.021772;
		daq_gain[2][18][7] = 0.021195;
		daq_gain[2][18][8] = 0.021082;
		daq_gain[2][18][9] = 0.019296;
		daq_gain[2][18][10] = 0.017336;
		daq_gain[2][18][11] = 0.015617;
		daq_gain[2][18][12] = 0.020660;
		daq_gain[2][18][13] = 0.017760;
		daq_gain[2][18][14] = 0.020898;
		daq_gain[2][18][15] = 0.021512;


		daq_gain[2][19][0] = 1.000000;
		daq_gain[2][19][1] = 0.021060;
		daq_gain[2][19][2] = 1.000000;
		daq_gain[2][19][3] = 0.020823;
		daq_gain[2][19][4] = 0.021046;
		daq_gain[2][19][5] = 1.;
		daq_gain[2][19][6] = 0.021038;
		daq_gain[2][19][7] = 1.;
		daq_gain[2][19][8] = 1.;
		daq_gain[2][19][9] = 0.019539;
		daq_gain[2][19][10] = 1.;
		daq_gain[2][19][11] = 0.019827;
		daq_gain[2][19][12] = 0.016967;
		daq_gain[2][19][13] = 1.;
		daq_gain[2][19][14] = 0.017722;
		daq_gain[2][19][15] = 1.;





		hasLoaded = 1;
	}

}
void FTHodoHit_factory::Process(const std::shared_ptr<const JEvent> &aEvent) {

	TranslationTable::ChannelInfo m_channel;
	//Get the hits from FADC. Support bot the waveboard hit and the fa250VTPMode7 hit
	auto faHits_waveboard = aEvent->Get<faWaveboardHit>();
	auto faHits_fa250VTPMode7 = aEvent->Get<fa250VTPMode7Hit>();

	for (auto faHit : faHits_waveboard) {

		m_channel = m_tt->getChannelInfo(faHit->m_channel);

		if ((m_channel.det_sys == TranslationTable::FTHODO)) {
			//Convert the waveboard hit. Probably will never be used, unless we will perform FT tests with waveboard.
			//It is used to test the system with data from a waveboard

			//Create a new FTHodo Hit Object object
			auto ftHodoHit = new FTHodoHit();

			//Assign the channel
			//Since this comes from the TT, it is still sector-layer-component)
			ftHodoHit->m_channel = *(m_channel.FTHODO);

			//Assign the time
			ftHodoHit->setHitTime(4*faHit->m_time.count());

			//Assign the energy
			//TODO: eventually apply another correction, here I just take the energy as provided by VTP
			ftHodoHit->setHitEnergy(faHit->m_charge);

			//Set the hit position
			ftHodoHit->setDx(px[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1]);
			ftHodoHit->setDy(py[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1]);
			ftHodoHit->setDz(pz[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1]);

			mData.push_back(ftHodoHit);
		}
	}

	//Here is the important part - converting the faHit from VTPMode7 to ftHodoHit
	for (auto faHit : faHits_fa250VTPMode7) {
		m_channel = m_tt->getChannelInfo(faHit->m_channel);
		if ((m_channel.det_sys == TranslationTable::FTHODO)) {

			//Create a new FTHodo Hit Object object
			auto ftHodoHit = new FTHodoHit();

			//Assign the channel
			//Since this comes from the TT, it is still sector-layer-component)
			ftHodoHit->m_channel = *(m_channel.FTHODO);

			//Assign the time
			ftHodoHit->setHitTime(4*faHit->m_time.count());

			//Assign the energy
			//TODO: eventually apply another correction, here I just take the energy as provided by VTP

			auto charge = faHit->m_charge / daq_gain[2][faHit->m_channel.slot][faHit->m_channel.channel];

			charge = charge *fadc_to_charge;

			auto ene = charge * mips_energy_hodo[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1];
			 ene = ene/mips_charge_hodo[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1];
			 ftHodoHit->setHitEnergy(ene);

		//	ftHodoHit->setHitEnergy(faHit->m_charge);

			//Set the hit position
			ftHodoHit->setDx(px[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1]);
			ftHodoHit->setDy(py[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1]);
			ftHodoHit->setDz(pz[ftHodoHit->m_channel.sector - 1][ftHodoHit->m_channel.layer - 1][ftHodoHit->m_channel.component - 1]);

			mData.push_back(ftHodoHit);
		}
	}

}
