#include <TSystem>

class StChain;
StChain * Chain;
void Analysis(TString InputFileList, TString OutputFileNameBase){
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();

  gSystem->Load("DstObjects");
  gSystem->Load("AnalysisMaker");
  gSystem->Load("St_base");
  gSystem->Load("StChain");
  gSystem->Load("libglobal_Tables");
  gSystem->Load("StUtilities");
  gSystem->Load("StIOMaker");
  gSystem->Load("StarClassLibrary");
  gSystem->Load("StEvent");

  Chain = new StChain("AChain");
  //StIOMaker *IOMk = new StIOMaker("IO","r",InputFileList.Data(),"bfcTree");
  StIOMaker *IOMk = new StIOMaker("IO","r","/star/data03/pwg/adams92/EpdDep/EventFiles/st_physics_22349014_raw_1500001.event.root","bfcTree");

  IOMk->SetDebug();
  IOMk->SetIOMode("r");
  IOMk->SetBranch("*",0,"0");
  IOMk->SetBranch("eventBranch",0,"r");

  AnalysisMaker * AnalysisCode = new AnalysisMaker(0,OutputFileNameBase.Data());

  Chain->Init();

  int istat=0;
  int ijk=1;

  TDataSet *deventBranch=0;
  TDataSet *dde=0;
 
  int nevents = 1e8;
    EventLoop: if (ijk <= nevents && istat!=2) {
       Chain->Clear();
       istat = Chain->Make(ijk);
        if (!istat) {
           deventBranch=Chain->GetDataSet("eventBranch");
           TDataSetIter eventBIter(deventBranch);
           if (deventBranch) {
             while (dde=eventBIter.Next()) {
               TString deName =  dde->GetName();
               if (deName == "StEvent") {
                 AnalysisCode->RunEventAnalysis(dde);
               }
             }
           }
        }
        elseif (istat==2)
          {cout << "Last  event processed. Status = " << istat << endl;}
 
        elseif (istat==3)
          {cout << "Error event processed. Status = " << istat << endl;}
       ijk++;
       goto EventLoop;
   }

  Chain->Finish();
} // Analysis