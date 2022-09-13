void FitAdcDistributions(){
        gStyle->SetOptStat(0);
        gStyle->SetPalette(kBlueGreenYellow);
        gStyle->SetNumberContours(999);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetPadTopMargin(0.13);
        gStyle->SetPadBottomMargin(0.25);
        gStyle->SetPadLeftMargin(0.17);
        //gStyle->SetPadRightMargin(0.25);
        gStyle->SetCanvasDefW(1500);
        gStyle->SetCanvasDefH(900);
        gStyle->SetTitleSize(0.06,"X");
        gStyle->SetTitleSize(0.06,"Y");
        gStyle->SetTitleSize(0.06,"Z");
        gStyle->SetTitleSize(0.06,"T"); // for title
        gStyle->SetLabelSize(0.04,"X");
        gStyle->SetLabelSize(0.04,"Y");
        gStyle->SetLabelSize(0.04,"Z");
        gStyle->SetTitleOffset(1.5,"X");
        gStyle->SetTitleOffset(1.1,"Y");
        gStyle->SetTitleOffset(1.3,"Z");
        gROOT->ForceStyle();
  TFile * infile = new TFile("C56290CCC19F8A9CCF6D3CB970F03B5B_sum.histograms.root","read");

  TH1D * m_h1d_AdcSum_NumHits_PP_TT_DEP[12][32];
  TH1D * m_h1d_AdcSum_NumHits_PP_TT_QT_West[12][32];
  TH1D * m_h1d_AdcSum_NumHits_PP_TT_QT_East[12][32];
  TProfile * tp1_QTWQTEDEP_WidOverMpv = new TProfile("tp1_QTWQTEDEP_WidOverMpv","",3,-0.5,2.5);


  TCanvas * can_Adc_NumHits_DEPandQT = new TCanvas("can_Adc_NumHits_DEPandQT","",3000,1000);
  can_Adc_NumHits_DEPandQT->Divide(3);
  can_Adc_NumHits_DEPandQT->Print("Adc_NumHits_DEPandQT.pdf[");

  int SupersectorOffset = 50;

  TGraphErrors * h1d_TTplus100PP_WidOverMpv_DEP = new TGraphErrors("h1d_TTplus100PP_WidOverMpv_DEP","DEP;TT+SupersectorOffset*PP;Width/MPV");
  TGraphErrors * tge_TTplus100PP_WidOverMpv_QT_West = new TGraphErrors("tge_TTplus100PP_WidOverMpv_QT_West","QT West;TT+SupersectorOffset*PP;Width/MPV");
  TGraphErrors * tge_TTplus100PP_WidOverMpv_QT_East = new TGraphErrors("tge_TTplus100PP_WidOverMpv_QT_East","QT East;TT+SupersectorOffset*PP;Width/MPV");
  TGraphErrors * h1d_TTplus100PP_WidOverMpv_DEPoverQT = new TGraphErrors("h1d_TTplus100PP_WidOverMpv_DEPoverQT","QT;TT+SupersectorOffset*PP;Width/MPV");
  for( int pp=1; pp<=12; pp++ ){
    for( int tt=0; tt<=31; tt++ ){
      int Rebin;
      double EstMax, EstMpv, EstWid;
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt] = (TH1D*)infile->Get(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_DEP",pp,tt));
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt] = (TH1D*)infile->Get(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_QT_West",pp,tt));
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt] = (TH1D*)infile->Get(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_QT_East",pp,tt));
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetXaxis()->SetTitle("ADC");
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->SetTitle(Form("%s West",m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetTitle()));
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetXaxis()->SetTitle("ADC");
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->SetTitle(Form("%s East",m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetTitle()));

      TF1 *MyLandau_DEP = new TF1("MyLandau_DEP","[0]*TMath::Landau(x,[1],[2])+[3]*TMath::Landau(x,[4],[5])",0,1e6);
      can_Adc_NumHits_DEPandQT->cd(1);
      Rebin = 16;
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->Rebin(Rebin);
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->GetXaxis()->SetRangeUser(400,900);
      EstMax = 5.*m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->GetMaximum();
      EstMpv = m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->GetBinCenter(m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->GetMaximumBin());
      EstWid = 100;
      MyLandau_DEP->SetParameter(0,EstMax);
      MyLandau_DEP->SetParameter(1,EstMpv);
      MyLandau_DEP->SetParameter(2,EstWid);
      MyLandau_DEP->SetParameter(3,.1*EstMax);
      MyLandau_DEP->SetParameter(4,2.*EstMpv);
      MyLandau_DEP->SetParLimits(4,1.6*EstMpv,2.5*EstMpv);
      MyLandau_DEP->SetParameter(5,1.*EstWid);
      MyLandau_DEP->SetParLimits(5,0.1*EstWid,3*EstWid);
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->GetXaxis()->UnZoom();
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->Fit("MyLandau_DEP","Q","",EstMpv-130,2000);
      gPad->SetLogy();
      TF1 * tf1_AdcSum_NumHits_PP_TT_DEP_FitResult = m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt]->GetFunction("MyLandau_DEP");
      if( tf1_AdcSum_NumHits_PP_TT_DEP_FitResult ){
        TF1 * tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(0));
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(1));
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(2));
        TF1 * tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(3));
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(4));
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(5));
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->SetLineColor(kGreen);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->SetFillColorAlpha(kGreen,0.5);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_FirstPeak->Draw("same");
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->SetLineColor(kBlue);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->SetFillColorAlpha(kBlue,0.5);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_DEP_FitResult_SecondPeak->Draw("same");
        tp1_QTWQTEDEP_WidOverMpv->Fill(2,tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(2)/tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(1));

      }

      TF1 *MyLandau_QT_West = new TF1("MyLandau_QT_West","[0]*TMath::Landau(x,[1],[2])+[3]*TMath::Landau(x,[4],[5])",0,1e6);
      can_Adc_NumHits_DEPandQT->cd(2);
      Rebin = 8;
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->Rebin(Rebin);
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetXaxis()->SetRangeUser(69,300);
      EstMax = 5.*m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetMaximum();
      EstMpv = m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetBinCenter(m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetMaximumBin());
      EstWid = 20;
      MyLandau_QT_West->SetParameter(0,EstMax);
      MyLandau_QT_West->SetParameter(1,EstMpv);
      MyLandau_QT_West->SetParameter(2,EstWid);
      MyLandau_QT_West->SetParameter(3,.05*EstMax);
      MyLandau_QT_West->SetParameter(4,2.*EstMpv);
      MyLandau_QT_West->SetParLimits(4,1.6*EstMpv,2.5*EstMpv);
      MyLandau_QT_West->SetParameter(5,1.*EstWid);
      MyLandau_QT_West->SetParLimits(5,0.1*EstWid,3*EstWid);
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetXaxis()->UnZoom();
      m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->Fit("MyLandau_QT_West","Q","",EstMpv-35,1000);
      gPad->SetLogy();
      TF1 * tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult = m_h1d_AdcSum_NumHits_PP_TT_QT_West[pp-1][tt]->GetFunction("MyLandau_QT_West");
      if( tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult ){
        TF1 * tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(0));
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(1));
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(2));
        TF1 * tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(3));
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(4));
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(5));
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->SetLineColor(kGreen);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->SetFillColorAlpha(kGreen,0.5);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_FirstPeak->Draw("same");
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->SetLineColor(kBlue);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->SetFillColorAlpha(kBlue,0.5);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult_SecondPeak->Draw("same");
        tp1_QTWQTEDEP_WidOverMpv->Fill(0.,tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(2)/tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(1));
      }

      TF1 *MyLandau_QT_East = new TF1("MyLandau_QT_East","[0]*TMath::Landau(x,[1],[2])+[3]*TMath::Landau(x,[4],[5])",0,1e6);
      can_Adc_NumHits_DEPandQT->cd(3);
      Rebin = 8;
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->Rebin(Rebin);
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetXaxis()->SetRangeUser(69,300);
      EstMax = 5.*m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetMaximum();
      EstMpv = m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetBinCenter(m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetMaximumBin());
      EstWid = 20;
      MyLandau_QT_East->SetParameter(0,EstMax);
      MyLandau_QT_East->SetParameter(1,EstMpv);
      MyLandau_QT_East->SetParameter(2,EstWid);
      MyLandau_QT_East->SetParameter(3,.05*EstMax);
      MyLandau_QT_East->SetParameter(4,2.*EstMpv);
      MyLandau_QT_East->SetParLimits(4,1.6*EstMpv,2.5*EstMpv);
      MyLandau_QT_East->SetParameter(5,1.*EstWid);
      MyLandau_QT_East->SetParLimits(5,0.1*EstWid,3*EstWid);
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetXaxis()->UnZoom();
      m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->Fit("MyLandau_QT_East","Q","",EstMpv-35,1000);
      gPad->SetLogy();
      TF1 * tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult = m_h1d_AdcSum_NumHits_PP_TT_QT_East[pp-1][tt]->GetFunction("MyLandau_QT_East");
      if( tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult ){
        TF1 * tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(0));
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(1));
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(2));
        TF1 * tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(3));
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(4));
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(5));
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->SetLineColor(kGreen);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->SetFillColorAlpha(kGreen,0.5);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_FirstPeak->Draw("same");
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->SetLineColor(kBlue);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->SetFillColorAlpha(kBlue,0.5);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult_SecondPeak->Draw("same");
        tp1_QTWQTEDEP_WidOverMpv->Fill(1,tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(2)/tf1_AdcSum_NumHits_PP_TT_QT_East_FitResult->GetParameter(1));
      }

      can_Adc_NumHits_DEPandQT->Print("Adc_NumHits_DEPandQT.pdf");


      double xshift = 0.1;
      double WidOverMpvDEP;
      double WidOverMpvErrDEP;
      double WidOverMpvQT;
      double WidOverMpvErrQT;
      if( tf1_AdcSum_NumHits_PP_TT_DEP_FitResult ){
        WidOverMpvDEP = tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(2)/tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParameter(1);
        WidOverMpvErrDEP = sqrt(pow(tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParError(2),2)+pow(tf1_AdcSum_NumHits_PP_TT_DEP_FitResult->GetParError(1),2));
        int iPoint = h1d_TTplus100PP_WidOverMpv_DEP->GetN();
        h1d_TTplus100PP_WidOverMpv_DEP->SetPoint(iPoint,tt+SupersectorOffset*pp-xshift,WidOverMpvDEP);
        //h1d_TTplus100PP_WidOverMpv_DEP->SetPointError(iPoint,0,WidOverMpvDEP);
        h1d_TTplus100PP_WidOverMpv_DEP->SetPointError(iPoint,0,0);
      }
      if( tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult ){
        WidOverMpvQT = tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(2)/tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParameter(1);
        WidOverMpvErrQT = sqrt(pow(tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParError(2),2)+pow(tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult->GetParError(1),2));
        int iPoint = tge_TTplus100PP_WidOverMpv_QT_West->GetN();
        tge_TTplus100PP_WidOverMpv_QT_West->SetPoint(iPoint,tt+SupersectorOffset*pp+xshift,WidOverMpvQT);
        //tge_TTplus100PP_WidOverMpv_QT_West->SetPointError(iPoint,0,WidOverMpvQT);
        tge_TTplus100PP_WidOverMpv_QT_West->SetPointError(iPoint,0,0);
      }
      if( tf1_AdcSum_NumHits_PP_TT_DEP_FitResult && tf1_AdcSum_NumHits_PP_TT_QT_West_FitResult ){
        int iPoint = h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetN();
        double err = sqrt(pow(WidOverMpvErrDEP,2)+pow(WidOverMpvErrQT,2));
        h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetPoint(iPoint,tt+SupersectorOffset*pp,WidOverMpvDEP/WidOverMpvQT);
        //h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetPointError(iPoint,0,err);
        h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetPointError(iPoint,0,0);
      }

    }


  }
  can_Adc_NumHits_DEPandQT->Print("Adc_NumHits_DEPandQT.pdf]");


  TF1 * pol1arr[12]; for( int i=0; i<12; i++ )pol1arr[i] = new TF1(Form("pol1arr_%i",i),"[0]+[1]*x",(i+1)*SupersectorOffset-1,(i+1)*SupersectorOffset+32+1);
  TF1 * pol1arr_result[12];
  TF1 * pol0arr[12]; for( int i=0; i<12; i++ )pol0arr[i] = new TF1(Form("pol0arr_%i",i),"[0]",(i+1)*SupersectorOffset-1,(i+1)*SupersectorOffset+32+1);
  TF1 * pol0arr_result[12];
  TCanvas * can_TTplus100PP_WidOverMpv = new TCanvas("can_TTplus100PP_WidOverMpv","",2500,1000);
  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf[");
  h1d_TTplus100PP_WidOverMpv_DEP->SetTitle("");
  h1d_TTplus100PP_WidOverMpv_DEP->GetXaxis()->SetTitle(Form("TT+%i*PP",SupersectorOffset));
  h1d_TTplus100PP_WidOverMpv_DEP->GetYaxis()->SetTitle("Width/MPV");
  h1d_TTplus100PP_WidOverMpv_DEP->SetMarkerSize(2);
  h1d_TTplus100PP_WidOverMpv_DEP->SetMarkerStyle(24);
  h1d_TTplus100PP_WidOverMpv_DEP->SetMarkerColor(kRed);
  h1d_TTplus100PP_WidOverMpv_DEP->SetLineColor(kRed);
  h1d_TTplus100PP_WidOverMpv_DEP->Draw("AP");
  tge_TTplus100PP_WidOverMpv_QT_West->GetXaxis()->SetTitle(Form("TT+%i*PP",SupersectorOffset));
  tge_TTplus100PP_WidOverMpv_QT_West->GetYaxis()->SetTitle("Width/MPV");
  tge_TTplus100PP_WidOverMpv_QT_West->SetMarkerSize(2);
  tge_TTplus100PP_WidOverMpv_QT_West->SetMarkerStyle(25);
  tge_TTplus100PP_WidOverMpv_QT_West->SetMarkerColor(kBlue);
  tge_TTplus100PP_WidOverMpv_QT_West->SetLineColor(kBlue);
  tge_TTplus100PP_WidOverMpv_QT_West->Draw("P,same");
  gPad->Update();
  double xmax = gPad->GetUxmax();
  TLine * tl_WidOverMpvQTWest = new TLine(0,tp1_QTWQTEDEP_WidOverMpv->GetBinContent(1),xmax,tp1_QTWQTEDEP_WidOverMpv->GetBinContent(1));
  tl_WidOverMpvQTWest->SetLineColorAlpha(kBlue,0.5);
  tl_WidOverMpvQTWest->SetLineWidth(2);
  tl_WidOverMpvQTWest->Draw("same");
  TLine * tl_WidOverMpvQTEast = new TLine(0,tp1_QTWQTEDEP_WidOverMpv->GetBinContent(2),xmax,tp1_QTWQTEDEP_WidOverMpv->GetBinContent(2));
  tl_WidOverMpvQTEast->SetLineColorAlpha(kBlue,0.5);
  tl_WidOverMpvQTEast->SetLineStyle(4);
  tl_WidOverMpvQTEast->SetLineWidth(2);
  tl_WidOverMpvQTEast->Draw("same");
  TLine * tl_WidOverMpvDEP = new TLine(0,tp1_QTWQTEDEP_WidOverMpv->GetBinContent(3),xmax,tp1_QTWQTEDEP_WidOverMpv->GetBinContent(3));
  tl_WidOverMpvDEP->SetLineColorAlpha(kRed,0.5);
  tl_WidOverMpvDEP->SetLineWidth(2);
  tl_WidOverMpvDEP->Draw("same");
  TLegend * leg = new TLegend(0.35,0.65,0.65,0.9);
  leg->SetNColumns(2);
  leg->AddEntry(tl_WidOverMpvDEP,"DEP","l");
  leg->AddEntry(h1d_TTplus100PP_WidOverMpv_DEP,"DEP","p");
  leg->AddEntry(tl_WidOverMpvQTWest,"QT West","l");
  leg->AddEntry(tge_TTplus100PP_WidOverMpv_QT_West,"QT West","p");
  leg->AddEntry(tl_WidOverMpvQTEast,"QT East","l");
  leg->Draw("same");
  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf");

  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetTitle("");
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetXaxis()->SetTitle(Form("TT+%i*PP",SupersectorOffset));
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetYaxis()->SetTitle("#frac{(Width/MPV)_{DEP}}{(Width/MPV)_{QT West}}");
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetMarkerStyle(28);
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetMarkerColor(kGreen);
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetLineColor(kGreen);
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->Draw("AP");
  for( int i=0; i<12; i++ ){
    h1d_TTplus100PP_WidOverMpv_DEPoverQT->Fit(Form("pol1arr_%i",i),(i==0)?"R":"R+");
    pol1arr_result[i] = h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetFunction(Form("pol1arr_%i",i));
    if( pol1arr_result[i] ){
      pol1arr_result[i]->SetLineColorAlpha(kMagenta,0.5);
      pol1arr_result[i]->Draw("same");
    }
    h1d_TTplus100PP_WidOverMpv_DEPoverQT->Fit(Form("pol0arr_%i",i),"R+");
    pol0arr_result[i] = h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetFunction(Form("pol0arr_%i",i));
    if( pol0arr_result[i] ){
      pol0arr_result[i]->SetLineColorAlpha(kBlack,0.5);
      pol0arr_result[i]->Draw("same");
    }

  }
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetYaxis()->SetRangeUser(0,2);

  TLine * ol = new TLine(0,1,xmax,1);
  ol->SetLineStyle(2);
  ol->Draw("same");

  TLegend * legRatio = new TLegend(0.2,0.65,0.5,0.85);
  legRatio->AddEntry(pol0arr_result[0],"Average","l");
  legRatio->AddEntry(pol1arr_result[0],"Linear fit","l");
  legRatio->Draw("same");


  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf");

  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf]");

  cout<<tp1_QTWQTEDEP_WidOverMpv->GetBinContent(1)<<", "<<tp1_QTWQTEDEP_WidOverMpv->GetBinError(1)<<endl;
  cout<<xmax<<endl;
} // main