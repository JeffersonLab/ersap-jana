#include "DAQ/faWaveboardHit.h"
#include "TH1D.h"
#include "TCanvas.h"


TCanvas* faWaveboardHit::Draw(int id) const{


	if (m_canvas==0){
		if (id<0){
			m_canvas=new TCanvas();
		}
		else{
			m_canvas=new TCanvas(Form("ch%i_%i_%i",m_channel.crate,m_channel.slot,m_channel.channel),100,100,id);
		}
	}
	m_canvas->cd();

	if (hWave!=0) delete hWave;
	hWave=new TH1D(Form("hh%i_%i_%i",m_channel.slot,m_channel.slot,m_channel.channel),Form("h%i_%i_%i",m_channel.crate,m_channel.slot,m_channel.channel),this->samples.size(),-0.5*m_dT,(this->samples.size()-0.5)*m_dT);
	this->toHisto(hWave);
	hWave->Draw();
	return m_canvas;
}


void faWaveboardHit::toHisto(TH1D *h)const{
	if (h==0){
		cerr<<"fa250Mode1CalibPedSubHit::toHisto, h pointer is null. Do nothing"<<std::endl;
	}
	int N=this->samples.size();
	h->Reset();
	for (int ii=0;ii<N;ii++){
		h->Fill(ii*this->m_dT,this->samples[ii]);
	}
	for (int ii=0;ii<=h->GetNbinsX();ii++){
		h->SetBinError(ii,0.);
	}
	return;
}
