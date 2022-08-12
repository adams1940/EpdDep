#include "AnalysisMaker.h"
#include "StRoot/StEvent/StEvent.h"
#include "StRoot/StEvent/StFcsCollection.h"
#include "StRoot/StEvent/StEpdCollection.h"
#include "StRoot/StEvent/StEpdHit.h"
#include "StRoot/St_base/StMessMgr.h"
#include "StRoot/StEvent/StTriggerData.h"
#include "StRoot/StMuDSTMaker/COMMON/StMuFcsCollection.h"
#include "StRoot/StMuDSTMaker/COMMON/StMuFcsHit.h"
#include "StRoot/StEvent/StEpdCollection.h"
#include "StRoot/StEvent/StFcsHit.h"
#include "StRoot/StEvent/StEpdHit.h"
#include "StRoot/StFcsDbMaker/StFcsDb.h"
#include "StRoot/StSpinPool/StFcsRawDaqReader/StFcsRawDaqReader.h"

ClassImp(AnalysisMaker);

AnalysisMaker::AnalysisMaker(StMaker * Maker, TString FileNameBase):StMaker("AnalysisMaker"){
  m_iHist = 0;
  m_FileNameBase = FileNameBase;
  m_EventsStarted = 0;
  m_EventsFinished = 0;
} // PicoDst Constructor

AnalysisMaker::~AnalysisMaker(){
} // Destructor

int AnalysisMaker::Init(){
  m_HistogramFile = new TFile(Form("%s.histograms.root",m_FileNameBase.Data()),"recreate");
  for( int i=0; i<100; i++ ){
    m_h1d_TimeBin_Adc_Hit[i] = new TH1D(Form("h1d_TimeBin_Adc_Hit%i",i),Form("Hit %i;Time Bin; ADC",i),101,-0.5,100.5);
  }
  m_h2d_TTplus100PP_TimeBin_Adc_DEP = new TH2D("h2d_TTplus100PP_TimeBin_Adc_DEP",";TT+100*PP;Time Bin; ADC",1301,-0.5,1300.5,96,-0.5,95.5);
  m_h1d_TTplus100PP_Adc_QT = new TH1D("h1d_TTplus100PP_Adc_QT",";TT+100*PP;ADC",1301,-0.5,1300.5);
  for( int pp=1; pp<=12; pp++ ){
    for( int tt=0; tt<=31; tt++ ){
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt] = new TH1D(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_DEP",pp,tt),Form("PP %i, TT %i, from DEP;#Sigma ADC;# hits",pp,tt),2048,-0.5,2047.5);
    }
  }

  m__NumEpdCollectionsFound = new TH1D("m__NumEpdCollectionsFound",";;# EpdCollections found",1,-1,1);

  return kStOK;
} // Init

int AnalysisMaker::Make(){
  return kStOK;
} // Make

void AnalysisMaker::RunEventAnalysis(TDataSet * Event_DataSet){
  StEvent * Event = (StEvent *)Event_DataSet;
  StFcsCollection * FcsCollection = Event->fcsCollection();


  for(int det=kFcsPresNorthDetId; det<kFcsNDet; det++){
    int nhit=FcsCollection->numberOfHits(det);
    if(nhit<=0) continue;
    StSPtrVecFcsHit& FcsHits = FcsCollection->hits(det);
    int adcsum = 0;
    for (int i=0; i<nhit; i++){
      StFcsHit * Hit = FcsHits[i];
      int id  = Hit->id();
      int ns  = Hit->ns();
      int dep = Hit->dep();
      int ch  = Hit->channel();
      int pp,tt;
      getEPDfromId(Hit->detectorId(),Hit->id(),pp,tt);
      for(unsigned int i=0; i<Hit->nTimeBin(); i++){	
        unsigned int tb=Hit->timebin(i);
        unsigned int adc=Hit->adc(i);
        m_h2d_TTplus100PP_TimeBin_Adc_DEP->Fill(tt+100*pp,tb,adc);
        if(m_iHist<100){
          m_h1d_TimeBin_Adc_Hit[m_iHist]->Fill(tb,adc);
        }
        if(47<=tb && tb<=54) adcsum += adc; //Triggered xing is TB=47~54
      }
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->Fill(adcsum);
    }
  }

  StEpdCollection * EpdCollection = Event->epdCollection();
  cout<<EpdCollection<<"<------------------------"<<endl;
  if(Event->epdCollection()) m__NumEpdCollectionsFound->Fill(0.);

}

int AnalysisMaker::Finish(){
  m_HistogramFile->Write();
  m_HistogramFile->Close();


  cout<<m_EventsStarted<<" events started and "<<m_EventsFinished<<" events finished."<<endl; // Keep this line as is; OrganizeOutputFiles.sh will be looking for it
  return kStOK;
} // Finish

TVector3 AnalysisMaker::StTVtoTV(StThreeVectorF StTV){
  return TVector3(StTV.x(),StTV.y(),StTV.z());
} // StTVtoTV

void AnalysisMaker::getEPDfromId(int det, int id, int &pp, int &tt){
    pp=-1; tt=-1;
     int row=id/16 + 1;
     int col=id%16 + 1;
     if(det==4){
         pp = (row-1)/2 + 1;
         tt = (col-1)*2 - (row-1)%2 + 1;
     }else if(det==5){
         pp = 12 - (row-1)/2;
         tt = (col-1)*2 + (row-1)%2;
     }
}