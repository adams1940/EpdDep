#include "AnalysisMaker.h"
#include "StRoot/StEvent/StEvent.h"
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
  m_WriteRefMult = false;
  m_WritePrimaryVertex = false;
  m_WriteBField = false;

  if( TString(Maker->IsA()->GetName()).EqualTo("StMuDstMaker") ){
    m_MuDstMaker = (StMuDstMaker *)Maker;
    m_IsPicoAnalysis = false;
  }
  if( TString(Maker->IsA()->GetName()).EqualTo("StPicoDstMaker") ){
    m_PicoDstMaker = (StPicoDstMaker *)Maker;
    m_IsPicoAnalysis = true;
  }
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

  m_Event = new Event;
  m_FemtoDstFile = new TFile(Form("%s.FemtoDst.root",m_FileNameBase.Data()),"recreate");
  m_EventTree = new TTree("m_EventTree","Dst Events");
    m_EventTree->Branch("m_Event","Event",&m_Event);   
    FirstRun = false;

  return kStOK;
} // Init

int AnalysisMaker::Make(){
  m_EventsStarted++;
  //if( !FirstRun ) return kStOK;
  //FirstRun = false;

  int RefMult;
  TVector3 PrimaryVertex;
  StMuDst * MuDst;
  StMuEvent MuEvent;
  if( m_IsPicoAnalysis ){
    StPicoEvent PicoEvent = *(m_PicoDstMaker->picoDst()->event());

    RefMult = PicoEvent.refMult();
    //PrimaryVertex = PicoEvent.primaryVertex();
  } // m_IsPicoAnalysis
  else{
    MuDst = m_MuDstMaker->muDst();
    MuEvent = *(m_MuDstMaker->muDst()->event());

    RefMult = MuEvent.refMult();
    //PrimaryVertex = StTVtoTV(MuEvent.primaryVertexPosition());
  } // !m_IsPicoAnalysis

  if( m_WriteRefMult ) m_Event->RefMult(RefMult);
  if( m_WritePrimaryVertex ) m_Event->PrimaryVertex(PrimaryVertex);

//  mFcsCollection=0;
//  StTriggerData* trg=0;

  //Getting StFcsRawDaqReader and TriggerData
//  StFcsRawDaqReader* fcsraw=(StFcsRawDaqReader*)GetMaker("daqReader");
//  StEvent* event= (StEvent*)GetInputDS("StEvent");
//  if(fcsraw){
//      //Getting trigger data (if daq file)
//      trg = fcsraw->trgdata();
//      if(!trg){
//          LOG_INFO << "Canot find Trigger Data from StFcsRawDaqReader" << endm;
//      }
//  }else if(event){
//      trg=event->triggerData();
//      if(!trg){
//          LOG_INFO << "Canot find Trigger Data from StEvent" << endm;
//      }
//  }

  StFcsDb* mFcsDb = static_cast<StFcsDb*>(GetDataSet("fcsDb"));
 
  StMuFcsCollection * FcsCollection = MuDst->muFcsCollection();
  TClonesArray * Hits = FcsCollection->getHitArray();
  for( int iHit=0; iHit<Hits->GetEntriesFast(); iHit++ ){
    StMuFcsHit * Hit = (StMuFcsHit *)(Hits->At(iHit));
    if(Hit->detectorId() == 4 || Hit->detectorId()==5){
//      cout<<"----------------"<<endl;
//      cout<<"Det="<< Hit->detectorId()<<endl;
//      cout<<"Id ="<<Hit->id()<<endl;
//      cout<<"NTB="<<Hit->nTimeBin()<<endl;
      unsigned int adcsum = 0;
      int pp,tt;
      getEPDfromId(Hit->detectorId(),Hit->id(),pp,tt);
      for(unsigned int i=0; i<Hit->nTimeBin(); i++){	
	unsigned int tb=Hit->timebin(i);
	unsigned int adc=Hit->adc(i);
	//cout<<Form(" i=%3d tb=%4d adc=%4d\n",i,tb,adc);
  if(m_iHist<100){
    m_h1d_TimeBin_Adc_Hit[m_iHist]->Fill(tb,adc);
    //cout<<m_iHist<<", "<<i<<": "<<adc<<endl;
  }
	if(47<=tb && tb<=54) adcsum += adc; //Triggered xing is TB=47~54

      m_h2d_TTplus100PP_TimeBin_Adc_DEP->Fill(tt+100*pp,tb,adc);
      
      }
    m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->Fill(adcsum);
      //cout<<pp<<", "<<tt<<endl;//", "<<tt+10*pp<<", "<<adcsum<<endl;
    m_iHist++;
      //cout<<"SUM="<<adcsum<<endl;

//      cout << Form("PP=%02d %4s TT=%02d AdcSum=%6d",pp,tt%2==0?"Even":"Odd ",tt,adcsum)<<endl;
//
//      cout<<Hit->adcSum()<<endl;
//      cout<<Hit->fitPeak()<<endl;
//      cout<<Hit->fitSigma()<<endl;
//      cout<<Hit->fitChi2()<<endl;
//      cout<<Hit->nPeak()<<endl;
//      cout<<Hit->energy()<<endl;
    }
  }



//  for( int iTrack=0; iTrack<MuDst->numberOfPrimaryTracks(); iTrack++ ){
//    StMuTrack * Track = MuDst->primaryTracks(iTrack);
//  }


if(MuDst->epdHits()) m__NumEpdCollectionsFound->Fill(0.);

  int NumEpdHits = MuDst->numberOfEpdHit();
  //cout<<NumEpdHits<<endl;
  for (int iEpdHit=0; iEpdHit<NumEpdHits; iEpdHit++){
    StMuEpdHit * EpdHit = MuDst->epdHit(iEpdHit);
    m_h1d_TTplus100PP_Adc_QT->Fill(EpdHit->tile()+100*EpdHit->position());
//    TVector3 RandomPointOnTile = EpdGeom.RandomPointOnTile(EpdHit->id());
//    TVector3 lineToRandomPoint = RandomPointOnTile - PrimaryVertex;
//    double Eta = lineToRandomPoint.Eta();
//    double Phi = EpdGeom.TileCenter(EpdHit->id()).Phi();
//    double NumMip = (EpdHit->nMIP()>2.)?2.:EpdHit->nMIP();
//    int Ring = EpdHit->row();
//    double NumTilesInRing = (Ring==1)?12:24;
//    int Side = EpdHit->side();
//    int TileId = EpdHit->id();
//    double SignEta = Eta/fabs(Eta);
//    double GainMatchedNumMip = NumMip*(h1d_RingTimesSide_SumNumMip_ReadIn->GetBinContent(h1d_RingTimesSide_SumNumMip_ReadIn->FindBin(Ring*Side))/NumTilesInRing)
//                                   /h1d_TileId_SumNumMip_ReadIn->GetBinContent(h1d_TileId_SumNumMip_ReadIn->FindBin(TileId));
//    double GainMatchedAndV1WeightedNumMip = GainMatchedNumMip*tf2_Centrality_AbsEta_V1->Eval(Centrality,fabs(Eta));
//
//    h1d_TileId_SumNumMip_->Fill(TileId,NumMip);
//    h1d_RingTimesSide_SumNumMip_->Fill(Ring*Side,NumMip);
//    h2d_X_Y_SumNumMip_->Fill(RandomPointOnTile.X(),RandomPointOnTile.Y(),NumMip);
//    h2d_X_Y_SumGainMatchedNumMip_->Fill(RandomPointOnTile.X(),RandomPointOnTile.Y(),GainMatchedNumMip);
//
//    TVector2 HitQRaw(SignEta*NumMip*cos(Phi),SignEta*NumMip*sin(Phi));
//    TVector2 HitQGainMatched(SignEta*GainMatchedNumMip*cos(Phi),SignEta*GainMatchedNumMip*sin(Phi));
//    TVector2 HitQGainMatchedAndV1Weighted(SignEta*GainMatchedAndV1WeightedNumMip*cos(Phi),SignEta*GainMatchedAndV1WeightedNumMip*sin(Phi));
//    if( Side==-1 ){
//      QEpdRawEast += HitQRaw;
//      QEpdGainMatchedEast += HitQGainMatched;
//      QEpdGainMatchedAndV1WeightedEast += HitQGainMatchedAndV1Weighted;
//      if( Ring<=5 ) QEpdGainMatchedInnerEast += HitQGainMatched;
//    } // if East
//    else{
//      QEpdRawWest += HitQRaw;
//      QEpdGainMatchedWest += HitQGainMatched;
//      QEpdGainMatchedAndV1WeightedWest += HitQGainMatchedAndV1Weighted;
//      if( Ring<= 5 ) QEpdGainMatchedInnerWest += HitQGainMatched;
//    } // if West
  } // iEpdHit



   //mFcsCollection=event->fcsCollection();
   //cout<<m_Event<<", "<<event<<", "<<fcsraw<<endl;
//   for(int det=kFcsPresNorthDetId; det<kFcsNDet; det++){
//     int nhit=mFcsCollection->numberOfHits(det);
//     StSPtrVecFcsHit& hits = mFcsCollection->hits(det);
//     for (int i=0; i<nhit; i++){
//       int id  = hits[i]->id();
//       int ns  = hits[i]->ns();
//       int dep = hits[i]->dep();
//       int ch  = hits[i]->channel();
//       int pp,tt;
//       mFcsDb->getEPDfromId(det,id,pp,tt);
//       int QTcQRb = tt<=9?0:1;
//     }
//   }

  m_EventTree->Fill();
  m_EventsFinished++;
  return kStOK;
} // Make

int AnalysisMaker::Finish(){
  m_FemtoDstFile->cd();
  m_EventTree->Write();
  m_FemtoDstFile->Close();
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