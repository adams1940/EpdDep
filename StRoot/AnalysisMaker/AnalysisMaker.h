#ifndef ANALYSISMAKER_HPP
#define ANALYSISMAKER_HPP

// ROOT
#include "TTree.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TH2D.h"
#include "TObjArray.h"
// StROOT
#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StarClassLibrary/StThreeVector.hh"
#include "StEventMaker/StEventMaker.h"

#include "StEpdUtil/StEpdGeom.h"
#include "StMuDSTMaker/COMMON/StMuEpdHit.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"

#include "StRoot/St_base/Stypes.h"
#include "StRoot/StEvent/StEnumerations.h"
class StFcsDb;
class StMuFcsCollection;
class StEpdCollection;
class StEpdHitMaker;

class AnalysisMaker : public StMaker{
private:

  TString m_FileNameBase;
  TFile * m_HistogramFile;
  StMaker * m_Maker;

  StEpdHitMaker * mEpdHitMaker;

  TH1D * m__NumEpdCollectionsFound;
  TH2D * m_h2d_TTplus100PP_TimeBin_Adc_DEP;
  TH1D * m_h1d_TTplus100PP_Adc_QT;
  TH1D * m_h1d_AdcSum_NumHits_PP_TT_DEP[12][32];
  TH2D * m_h2d_AdcQT_AdcSumDep_NumHits_PP_TT[12][32];
  TH2D * m_h2d_AdcQT_AdcSumDep_NumHits;

  int m_EventsStarted, m_EventsFinished;

public:

  AnalysisMaker(StMaker* Maker, TString FileNameBase);
  virtual ~AnalysisMaker();

  int Init();
  int Make();
  int Finish();

  void RunEventAnalysis(TDataSet * Event_DataSet);
  void getEPDfromId(int det, int id, int &pp, int &tt);

ClassDef(AnalysisMaker,1)

};
#endif