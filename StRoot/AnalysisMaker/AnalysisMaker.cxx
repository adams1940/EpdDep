#include "AnalysisMaker.h"
#include "StRoot/StEvent/StEvent.h"
#include "StRoot/StEvent/StFcsCollection.h"
#include "StRoot/StEvent/StEpdCollection.h"
#include "StRoot/StEvent/StTpcHitCollection.h"
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
#include "StEpdHitMaker/StEpdHitMaker.h"

ClassImp(AnalysisMaker);

AnalysisMaker::AnalysisMaker(StMaker * Maker, TString FileNameBase) : StMaker("epdAna"){
  m_Maker = Maker;
  m_FileNameBase = FileNameBase;
  m_EventsStarted = 0;
  m_EventsFinished = 0;
} // PicoDst Constructor

AnalysisMaker::~AnalysisMaker(){
} // Destructor

int AnalysisMaker::Init(){
  m_HistogramFile = new TFile(Form("%s.histograms.root",m_FileNameBase.Data()),"recreate");
  m_h2d_TTplus100PP_TimeBin_Adc_DEP = new TH2D("h2d_TTplus100PP_TimeBin_Adc_DEP",";TT+100*PP;Time Bin; ADC",1301,-0.5,1300.5,96,-0.5,95.5);
  m_h1d_TTplus100PP_Adc_QT = new TH1D("h1d_TTplus100PP_Adc_QT",";TT+100*PP;ADC",1301,-0.5,1300.5);
  for( int pp=1; pp<=12; pp++ ){
    for( int tt=0; tt<=31; tt++ ){
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt] = new TH1D(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_DEP",pp,tt),Form("PP %i, TT %i, from DEP;#Sigma ADC;# hits",pp,tt),2048,-0.5,2047.5);
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt] = new TH1D(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_QT_West",pp,tt),Form("PP %i, TT %i, from QT;#Sigma ADC;# hits",pp,tt),1024,-0.5,1023.5);
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt] = new TH1D(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_QT_East",pp,tt),Form("PP %i, TT %i, from QT;#Sigma ADC;# hits",pp,tt),1024,-0.5,1023.5);
      m_h2d_AdcQT_AdcSumDep_NumHits_PP_TT[pp-1][tt] = new TH2D(Form("m_h2d_AdcQT_AdcSumDep_NumHits_PP%i_TT%i_DEP",pp,tt),Form("PP %i, TT %i;ADC from QT;#Sigma ADC from DEP;# hits",pp,tt),513,-0.5,512.5,513,-0.5,512.5);
    }
  }

  m_h2d_AdcQT_AdcSumDep_NumHits = new TH2D("m_h2d_AdcQT_AdcSumDep_NumHits",";ADC from QT;#Sigma ADC from DEP;# hits",1024,-0.5,1023.5,4096,-0.5,4095.5);
  m_h2d_AdcQTb_AdcSumDep_NumHits = new TH2D("m_h2d_AdcQTb_AdcSumDep_NumHits",";ADC from QTb;#Sigma ADC from DEP;# hits",1024,-0.5,1023.5,4096,-0.5,4095.5);
  m_h2d_AdcQTc_AdcSumDep_NumHits = new TH2D("m_h2d_AdcQTc_AdcSumDep_NumHits",";ADC from QTc;#Sigma ADC from DEP;# hits",1024,-0.5,1023.5,4096,-0.5,4095.5);

  m_h1d_TTplus100PP_DEPnoQT = new TH1D("m_h1d_TTplus100PP_DEPnoQT","DEP hit but no QT hit;TT+100*PP;# hits",1301,-0.5,1300.5);
  m_h1d_TTplus100PP_QTnoDEP = new TH1D("m_h1d_TTplus100PP_QTnoDEP","QT hit but no DEP hit;TT+100*PP;# hits",1301,-0.5,1300.5);
  m_h1d_TTplus100PP_DEPandQT = new TH1D("m_h1d_TTplus100PP_DEPandQT","DEP and QT hit;TT+100*PP;# hits",1301,-0.5,1300.5);
  m_h1d_TTplus100PP_noDEPnoQT = new TH1D("m_h1d_TTplus100PP_noDEPnoQT","no DEP or QT hit;TT+100*PP;# hits",1301,-0.5,1300.5);

  m_h2d_PMT_EW_ADC_VPD = new TH2D("m_h2d_PMT_EW_ADC_VPD","VPD;PMT;EW;ADC",25,-0.5,24.5,2,-0.5,1.5);

  m__NumEpdCollectionsFound = new TH1D("m__NumEpdCollectionsFound",";;# EpdCollections found",1,-1,1);
  LOG_INFO << "!!!AnalysisMaker::Init!!!" << endm;
  return kStOK;
} // Init

int AnalysisMaker::Make(){
  GetParentMaker()->SetDebug(0);
  return kStOK;
} // Make

void AnalysisMaker::RunEventAnalysis(TDataSet * Event_DataSet){
  StEvent * Event = (StEvent *)Event_DataSet;
  StFcsCollection * FcsCollection = Event->fcsCollection();

  float AdcDep[12][32], AdcQT[12][32];
  for( int pp=1; pp<=12; pp++ ){
    for( int tt=0; tt<=31; tt++ ){
      AdcDep[pp-1][tt] = -1;
      AdcQT[pp-1][tt] = -1;
    }
  }

  for(int det=kFcsPresNorthDetId; det<kFcsNDet; det++){
    int nhit=FcsCollection->numberOfHits(det);
    if(nhit<=0) continue;
    StSPtrVecFcsHit& FcsHits = FcsCollection->hits(det);
    for (int i=0; i<nhit; i++){
      StFcsHit * Hit = FcsHits[i];
      int id  = Hit->id();
      int ns  = Hit->ns();
      int dep = Hit->dep();
      int ch  = Hit->channel();
      int pp,tt;
      getEPDfromId(Hit->detectorId(),Hit->id(),pp,tt);
      int adcsum = 0;
      for(unsigned int i=0; i<Hit->nTimeBin(); i++){	
        unsigned int tb=Hit->timebin(i);
        unsigned int adc=Hit->adc(i);
        m_h2d_TTplus100PP_TimeBin_Adc_DEP->Fill(tt+100*pp,tb,adc);
        if(47<=tb && tb<=54) adcsum += adc; //Triggered xing is TB=47~54
        //cout<<adc<<endl;
      }
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->Fill(adcsum);
      AdcDep[pp-1][tt] = adcsum;
    }
  }



  for( int ew=0; ew<2; ew++ ){
    for(int pmt=0; pmt<=24; pmt++ ){
      m_h2d_PMT_EW_ADC_VPD->Fill(pmt,ew,Event->triggerData()->vpdADC(StBeamDirection(ew),pmt,0));
    }
  }

  StEpdCollection * EpdCollection = Event->epdCollection();
  if(EpdCollection) m__NumEpdCollectionsFound->Fill(0.);
  int NumEpdHits = EpdCollection->epdHits().size();
  for( int iHit=0; iHit<NumEpdHits; iHit++ ){
    StEpdHit * Hit = (StEpdHit*)(EpdCollection->epdHits()[iHit]);
    int pp = Hit->position(), tt = Hit->tile();
    float adc = Hit->adc();
    AdcQT[pp-1][tt] = adc;
    if( Hit->side()<0 ) m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->Fill(adc);
    if( Hit->side()>0 ) m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->Fill(adc);

    //cout<<Hit->adc()<<endl;
  }

  for( int pp=1; pp<=12; pp++ ){
    for( int tt=0; tt<=31; tt++ ){
      if( AdcDep[pp-1][tt]>-1 && AdcQT[pp-1][tt]>-1 ){
        m_h2d_AdcQT_AdcSumDep_NumHits->Fill(AdcQT[pp-1][tt],AdcDep[pp-1][tt]);
        if( tt>=10) m_h2d_AdcQTb_AdcSumDep_NumHits->Fill(AdcQT[pp-1][tt],AdcDep[pp-1][tt]);
        if( tt<10) m_h2d_AdcQTc_AdcSumDep_NumHits->Fill(AdcQT[pp-1][tt],AdcDep[pp-1][tt]);
        m_h2d_AdcQT_AdcSumDep_NumHits_PP_TT[pp-1][tt]->Fill(AdcQT[pp-1][tt],AdcDep[pp-1][tt]);
      }
      if( AdcDep[pp-1][tt]>-1 && AdcQT[pp-1][tt]==-1 ) m_h1d_TTplus100PP_DEPnoQT->Fill(tt+100*pp);
      if( AdcDep[pp-1][tt]==-1 && AdcQT[pp-1][tt]>-1 ) m_h1d_TTplus100PP_QTnoDEP->Fill(tt+100*pp);
      if( AdcDep[pp-1][tt]>-1 && AdcQT[pp-1][tt]>-1 ) m_h1d_TTplus100PP_DEPandQT->Fill(tt+100*pp);
      if( AdcDep[pp-1][tt]==-1 && AdcQT[pp-1][tt]==-1 ) m_h1d_TTplus100PP_noDEPnoQT->Fill(tt+100*pp);
    }
  }

}

int AnalysisMaker::Finish(){
  m_HistogramFile->Write();
  m_HistogramFile->Close();

  cout<<m_EventsStarted<<" events started and "<<m_EventsFinished<<" events finished."<<endl; // Keep this line as is; OrganizeOutputFiles.sh will be looking for it
  return kStOK;
} // Finish

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