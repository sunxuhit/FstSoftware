#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile2D.h>
#include <TGraph.h>
#include <TStyle.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotMatchedScanClusterSignalSensor(string mod = "Mod01", string hv = "HV200V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.25); gStyle->SetStatH(0.55);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstTracking_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  
  TH1F *h_mMatchedScanSignal[FST::mFstNumMatching];
  TH1F *h_mMatchedAngleCorrectedScanSignal[FST::mFstNumMatching];
  TH1F *h_mMatchedScanSignal_Rstrips[FST::mFstNumRstripPerSensor][FST::mFstNumMatching];
  TH1F *h_mMatchedAngleCorrectedScanSignal_Rstrips[FST::mFstNumRstripPerSensor][FST::mFstNumMatching];

  double maxCountsSignal[FST::mFstNumMatching] = {0};
  double maxCountsSignalCorr[FST::mFstNumMatching] = {0};

    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      std::string HistName;
      HistName = Form("h_mMatchedScanSignal_SF%d",i_match);
      h_mMatchedScanSignal[i_match] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mMatchedAngleCorrectedScanSignal_SF%d",i_match);
      h_mMatchedAngleCorrectedScanSignal[i_match] = (TH1F*)File_InPut->Get(HistName.c_str());

      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip){
        HistName = Form("h_mMatchedScanSignal_Rstrip%d_SF%d",i_rstrip,i_match);
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match] = (TH1F*)File_InPut->Get(HistName.c_str());
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->Rebin(4);
        double max = h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->GetMaximum();
        if(max > maxCountsSignal[i_match]) maxCountsSignal[i_match] = max;

        HistName = Form("h_mMatchedAngleCorrectedScanSignal_Rstrip%d_SF%d",i_rstrip,i_match);
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match] = (TH1F*)File_InPut->Get(HistName.c_str());
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->Rebin(4);
        max = h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->GetMaximum();
        if(max > maxCountsSignalCorr[i_match]) maxCountsSignalCorr[i_match] = max;
      }
    }

  string outputname = Form("./figures/MatchedScanClusterSignalModule_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.pdf",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,800,400);
  c_Signal->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_Signal->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Signal->cd(i_pad+1)->SetRightMargin(0.15);
    c_Signal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Signal->cd(i_pad+1)->SetTicks(1,1);
    c_Signal->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("%s[",outputname.c_str());
  c_Signal->Print(output_start.c_str()); // open pdf file
  
  for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
  {
      c_Signal->cd(1)->Clear();
      c_Signal->cd(1);
      
      TLegend *leg = new TLegend(0.35,0.6,0.80,0.85);
      leg->SetBorderSize(0);
      leg->SetFillColor(10);
  
      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip){
        string HistTitle = Form("Matched Scan Cluster Signal SF%d",i_match);
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->SetTitle(HistTitle.c_str());
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->GetXaxis()->SetTitle("ADC");
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->GetYaxis()->SetTitle("Counts");
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->SetLineColor(1+i_rstrip);
        h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->SetMaximum(maxCountsSignal[i_match]*1.05);
        //TH1F *scanSignal = (TH1F*)h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->Rebin(4,"scanSignal");
        TH1F *scanSignal = (TH1F*)h_mMatchedScanSignal_Rstrips[i_rstrip][i_match]->Clone("scanSignal");
        scanSignal->SetStats(0);
        if(i_rstrip != 0) scanSignal->Draw("same");
        else scanSignal->Draw(); 
        std::string LegName = Form("Rstrip%d, #mu = %.1f#pm%.1f, #sigma = %.1f#pm%.1f",i_rstrip+4,scanSignal->GetMean(),scanSignal->GetMeanError(),scanSignal->GetRMS(),scanSignal->GetRMSError());
        leg->AddEntry(h_mMatchedScanSignal_Rstrips[i_rstrip][i_match],LegName.c_str(),"L");
        if(i_rstrip == 3) leg->Draw("same");
        //h_mMatchedScanSignal[i_match]->Draw();
      }

      c_Signal->cd(2)->Clear();
      c_Signal->cd(2);
      
      TLegend *leg2 = new TLegend(0.35,0.6,0.80,0.85);
      leg2->SetBorderSize(0);
      leg2->SetFillColor(10);

      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip){
        string HistTitle = Form("Angle Corrected Matched Scan Cluster Signal SF%d",i_match);
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->SetTitle(HistTitle.c_str());
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->GetXaxis()->SetTitle("ADC*cos(#theta_{incident})");
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->GetYaxis()->SetTitle("Counts");
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->SetLineColor(1+i_rstrip);
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->SetMaximum(maxCountsSignalCorr[i_match]*1.05);
        //TH1F *scanSignalCorr = (TH1F*)h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->Rebin(4,"scanSignalCorr");
        TH1F *scanSignalCorr = (TH1F*)h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match]->Clone("scanSignalCorr");
        scanSignalCorr->SetStats(0);
        if(i_rstrip != 0) scanSignalCorr->Draw("same");
        else scanSignalCorr->Draw();
        std::string LegName = Form("Rstrip%d, #mu = %.1f#pm%.1f, #sigma = %.1f#pm%.1f",i_rstrip+4,scanSignalCorr->GetMean(),scanSignalCorr->GetMeanError(),scanSignalCorr->GetRMS(),scanSignalCorr->GetRMSError());
        leg2->AddEntry(h_mMatchedAngleCorrectedScanSignal_Rstrips[i_rstrip][i_match],LegName.c_str(),"L");
        if(i_rstrip == 3) leg2->Draw("same");

        //h_mMatchedAngleCorrectedScanSignal[i_match]->Draw();
      }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str());
  }

  string output_stop = Form("%s]",outputname.c_str());
  c_Signal->Print(output_stop.c_str()); // open pdf file
}

