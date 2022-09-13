void FitAdcDistributions(){
  TFile * infile = new TFile("FB6DB01BB27B8A2154C37BAE33E00002_sum.histograms.root","read");

  TH1D * m_h1d_AdcSum_NumHits_PP_TT_DEP[12][32];
  TH1D * m_h1d_AdcSum_NumHits_PP_TT_QT[12][32];


  TCanvas * can_Adc_NumHits_DEPandQT = new TCanvas("can_Adc_NumHits_DEPandQT","",2000,1000);
  can_Adc_NumHits_DEPandQT->Divide(2);
  can_Adc_NumHits_DEPandQT->Print("Adc_NumHits_DEPandQT.pdf[");

  TGraphErrors * h1d_TTplus100PP_WidOverMpv_DEP = new TGraphErrors("h1d_TTplus100PP_WidOverMpv_DEP","DEP;TT+100*PP;Width/MPV");
  TGraphErrors * h1d_TTplus100PP_WidOverMpv_QT = new TGraphErrors("h1d_TTplus100PP_WidOverMpv_QT","QT;TT+100*PP;Width/MPV");
  TGraphErrors * h1d_TTplus100PP_WidOverMpv_DEPoverQT = new TGraphErrors("h1d_TTplus100PP_WidOverMpv_DEPoverQT","QT;TT+100*PP;Width/MPV");
  for( int pp=1; pp<=12; pp++ ){
    for( int tt=0; tt<=31; tt++ ){
      int Rebin;
      double EstMax, EstMpv, EstWid;
      m_h1d_AdcSum_NumHits_PP_TT_DEP[pp-1][tt] = (TH1D*)infile->Get(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_DEP",pp,tt));
      m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt] = (TH1D*)infile->Get(Form("m_h1d_AdcSum_NumHits_PP%i_TT%i_QT",pp,tt));

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

      }

      TF1 *MyLandau_QT = new TF1("MyLandau_QT","[0]*TMath::Landau(x,[1],[2])+[3]*TMath::Landau(x,[4],[5])",0,1e6);
      can_Adc_NumHits_DEPandQT->cd(2);
      Rebin = 8;
      m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->Rebin(Rebin);
      m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->GetXaxis()->SetRangeUser(69,300);
      EstMax = 5.*m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->GetMaximum();
      EstMpv = m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->GetBinCenter(m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->GetMaximumBin());
      EstWid = 20;
      MyLandau_QT->SetParameter(0,EstMax);
      MyLandau_QT->SetParameter(1,EstMpv);
      MyLandau_QT->SetParameter(2,EstWid);
      MyLandau_QT->SetParameter(3,.05*EstMax);
      MyLandau_QT->SetParameter(4,2.*EstMpv);
      MyLandau_QT->SetParLimits(4,1.6*EstMpv,2.5*EstMpv);
      MyLandau_QT->SetParameter(5,1.*EstWid);
      MyLandau_QT->SetParLimits(5,0.1*EstWid,3*EstWid);
      m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->GetXaxis()->UnZoom();
      m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->Fit("MyLandau_QT","Q","",EstMpv-35,1000);
      gPad->SetLogy();
      TF1 * tf1_AdcSum_NumHits_PP_TT_QT_FitResult = m_h1d_AdcSum_NumHits_PP_TT_QT[pp-1][tt]->GetFunction("MyLandau_QT");
      if( tf1_AdcSum_NumHits_PP_TT_QT_FitResult ){
        TF1 * tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(0));
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(1));
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(2));
        TF1 * tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak = new TF1("tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak","[0]*TMath::Landau(x,[1],[2])",0,1e6);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->SetParameter(0,tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(3));
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->SetParameter(1,tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(4));
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->SetParameter(2,tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(5));
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->SetLineColor(kGreen);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->SetFillColorAlpha(kGreen,0.5);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_FirstPeak->Draw("same");
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->SetLineColor(kBlue);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->SetFillColorAlpha(kBlue,0.5);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->SetFillStyle(1001);
        tf1_AdcSum_NumHits_PP_TT_QT_FitResult_SecondPeak->Draw("same");
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
        h1d_TTplus100PP_WidOverMpv_DEP->SetPoint(iPoint,tt+100*pp-xshift,WidOverMpvDEP);
        //h1d_TTplus100PP_WidOverMpv_DEP->SetPointError(iPoint,0,WidOverMpvDEP);
        h1d_TTplus100PP_WidOverMpv_DEP->SetPointError(iPoint,0,0);
      }
      if( tf1_AdcSum_NumHits_PP_TT_QT_FitResult ){
        WidOverMpvQT = tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(2)/tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParameter(1);
        WidOverMpvErrQT = sqrt(pow(tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParError(2),2)+pow(tf1_AdcSum_NumHits_PP_TT_QT_FitResult->GetParError(1),2));
        int iPoint = h1d_TTplus100PP_WidOverMpv_QT->GetN();
        h1d_TTplus100PP_WidOverMpv_QT->SetPoint(iPoint,tt+100*pp+xshift,WidOverMpvQT);
        //h1d_TTplus100PP_WidOverMpv_QT->SetPointError(iPoint,0,WidOverMpvQT);
        h1d_TTplus100PP_WidOverMpv_QT->SetPointError(iPoint,0,0);
      }
      if( tf1_AdcSum_NumHits_PP_TT_DEP_FitResult && tf1_AdcSum_NumHits_PP_TT_QT_FitResult ){
        int iPoint = h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetN();
        double err = sqrt(pow(WidOverMpvErrDEP,2)+pow(WidOverMpvErrQT,2));
        h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetPoint(iPoint,tt+100*pp,WidOverMpvDEP/WidOverMpvQT);
        //h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetPointError(iPoint,0,err);
        h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetPointError(iPoint,0,0);
      }

    }


  }
  can_Adc_NumHits_DEPandQT->Print("Adc_NumHits_DEPandQT.pdf]");

  TCanvas * can_TTplus100PP_WidOverMpv = new TCanvas();
  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf[");
  h1d_TTplus100PP_WidOverMpv_DEP->SetMarkerStyle(24);
  h1d_TTplus100PP_WidOverMpv_DEP->SetMarkerColor(kRed);
  h1d_TTplus100PP_WidOverMpv_DEP->SetLineColor(kRed);
  h1d_TTplus100PP_WidOverMpv_DEP->Draw("AP");
  h1d_TTplus100PP_WidOverMpv_QT->SetMarkerStyle(25);
  h1d_TTplus100PP_WidOverMpv_QT->SetMarkerColor(kBlue);
  h1d_TTplus100PP_WidOverMpv_QT->SetLineColor(kBlue);
  h1d_TTplus100PP_WidOverMpv_QT->Draw("P,same");
  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf");
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetMarkerStyle(28);
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetMarkerColor(kGreen);
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->SetLineColor(kGreen);
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->Draw("AP");
  h1d_TTplus100PP_WidOverMpv_DEPoverQT->GetYaxis()->SetRangeUser(0,2);
  TLine * ol = new TLine(0,1,1300,1);
  ol->SetLineStyle(9);
  ol->Draw("same");
  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf");

  can_TTplus100PP_WidOverMpv->Print("TTplus100PP_WidOverMpv.pdf]");

} // main