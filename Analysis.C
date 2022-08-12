#include <TSystem>

void Analysis(TString InputFileList, TString OutputFileNameBase){
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  gSystem->Load("StPicoEvent");
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("DstObjects");
  gSystem->Load("AnalysisMaker");

  StChain * Chain = new StChain();
  //StPicoDstMaker * PicoDstMaker = new StPicoDstMaker(2,InputFileList.Data(),"picoDst");
  StMuDstMaker * MuDstMaker = new StMuDstMaker(0,0,"",InputFileList.Data(),"MuDst",1);
  AnalysisMaker * AnalysisCode = new AnalysisMaker(MuDstMaker,OutputFileNameBase.Data());

  Chain->Init();

  AnalysisCode->m_WriteRefMult = true;
  AnalysisCode->m_WritePrimaryVertex = true;

  Chain->EventLoop();
  Chain->Finish();
} // Analysis