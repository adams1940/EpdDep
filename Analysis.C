#include <TSystem>
#include <iostream>
#include <fstream>

class StChain;
StChain * Chain;

void Analysis(TString InputFileList="EventFilesPwg.list", TString OutputFileNameBase="test"){
  ifstream infile(InputFileList.Data());
  string file;
  getline(infile,file);
  TString FileName(file);
  gROOT->LoadMacro("bfc.C");    // Load big "full" chain                                              
  bfc(-1,"in epdHit",FileName.Data()); // Setup but do not init          

  St_db_Maker *dbMk= (St_db_Maker*) chain->GetMaker("db");
  if(dbMk){
    dbMk->SetAttr("blacklist", "tpc"); dbMk->SetAttr("blacklist", "svt"); dbMk->SetAttr("blacklist", "ssd");
    dbMk->SetAttr("blacklist", "ist"); dbMk->SetAttr("blacklist", "pxl"); dbMk->SetAttr("blacklist", "pp2pp");
    dbMk->SetAttr("blacklist", "ftpc"); dbMk->SetAttr("blacklist", "emc"); dbMk->SetAttr("blacklist", "eemc");
    dbMk->SetAttr("blacklist", "mtd"); dbMk->SetAttr("blacklist", "pmd"); dbMk->SetAttr("blacklist", "tof");
    dbMk->SetAttr("blacklist", "etof");dbMk->SetAttr("blacklist", "rhicf");
  }

  gSystem->Load("AnalysisMaker");
  AnalysisMaker * AnalysisCode = new AnalysisMaker(0,OutputFileNameBase.Data());

  chain->Init();

  int istat=0;
  int ijk=1;

  TDataSet *deventBranch=0;
  TDataSet *dde=0;
 
  int nevents = 1e9;
    EventLoop: if (ijk <= nevents && istat!=2) {
       chain->Clear();
       istat = chain->Make(ijk);
        if (!istat) {
           deventBranch=chain->GetDataSet("eventBranch");
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

  chain->Finish();
} // Analysis
