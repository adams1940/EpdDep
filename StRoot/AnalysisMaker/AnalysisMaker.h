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
// DstObjects
#include "DstObjects/Event.h"
#include "DstObjects/Particle.h"
#include "DstObjects/Parent.h"
#include "DstObjects/StableDaughter.h"

#include "StEpdUtil/StEpdGeom.h"
#include "StMuDSTMaker/COMMON/StMuEpdHit.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"

#include "StRoot/St_base/Stypes.h"
#include "StRoot/StEvent/StEnumerations.h"
class StFcsDb;
class StMuFcsCollection;
class StEpdCollection;

class AnalysisMaker : public StMaker{
private:
  bool m_IsPicoAnalysis;
  bool FirstRun;

  TString m_FileNameBase;
  TFile * m_FemtoDstFile;
  TFile * m_HistogramFile;

  TH1D * m_h1d_TimeBin_Adc_Hit[100];
  TH2D * m_h2d_TTplus100PP_TimeBin_Adc_DEP;
  TH1D * m_h1d_TTplus100PP_Adc_QT;
  TH1D * m_h1d_AdcSum_NumHits_PP_TT_DEP[12][32];
  TH1D * m__NumEpdCollectionsFound;

  StFcsDb *mFcsDb;
  int m_iHist;

	StPicoDstMaker * m_PicoDstMaker;
	StMuDstMaker * m_MuDstMaker;
  StEventMaker * m_EventMaker;
  StPicoDst * m_PicoDst;

  int m_EventsStarted, m_EventsFinished;

public:
  bool m_WriteRefMult, m_WritePrimaryVertex, m_WriteBField;

  AnalysisMaker(StMaker* Maker, TString FileNameBase);
  virtual ~AnalysisMaker();

  int Init();
  int Make();
  int Finish();

  TVector3 StTVtoTV(StThreeVectorF StTV);
  void ReconstructLambdas();
  void getEPDfromId(int det, int id, int &pp, int &tt);

  void RunEventAnalysis(TDataSet * Event_DataSet);

ClassDef(AnalysisMaker,1)

};
#endif