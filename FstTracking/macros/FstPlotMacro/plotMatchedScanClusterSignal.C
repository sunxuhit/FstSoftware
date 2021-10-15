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

void plotMatchedScanClusterSignal(string mod = "Mod04", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.25); gStyle->SetStatH(0.55);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstTracking_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  
  TH1F *h_mMatchedScanSignal[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mMatchedAngleCorrectedScanSignal[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mMatchedScanSignal_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::mFstNumMatching];
  TH1F *h_mMatchedAngleCorrectedScanSignal_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::mFstNumMatching];

  double maxCountsSignal[FST::mFstNumSensorsPerModule][FST::mFstNumMatching] = {{0}};
  double maxCountsSignalCorr[FST::mFstNumSensorsPerModule][FST::mFstNumMatching] = {{0}};

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      std::string HistName;
      HistName = Form("h_mMatchedScanSignal_Sensor%d_SF%d",i_sensor,i_match);
      h_mMatchedScanSignal[i_sensor][i_match] = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mMatchedAngleCorrectedScanSignal_Sensor%d_SF%d",i_sensor,i_match);
      h_mMatchedAngleCorrectedScanSignal[i_sensor][i_match] = (TH1F*)File_InPut->Get(HistName.c_str());

      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip){
        HistName = Form("h_mMatchedScanSignal_Sensor%d_Rstrip%d_SF%d",i_sensor,i_rstrip,i_match);
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match] = (TH1F*)File_InPut->Get(HistName.c_str());
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->Rebin(4);
        double max = h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->GetMaximum();
        if(max > maxCountsSignal[i_sensor][i_match]) maxCountsSignal[i_sensor][i_match] = max;

        HistName = Form("h_mMatchedAngleCorrectedScanSignal_Sensor%d_Rstrip%d_SF%d",i_sensor,i_rstrip,i_match);
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match] = (TH1F*)File_InPut->Get(HistName.c_str());
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->Rebin(4);
        max = h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->GetMaximum();
        if(max > maxCountsSignalCorr[i_sensor][i_match]) maxCountsSignalCorr[i_sensor][i_match] = max;
      }
    }
  }

  string outputname = Form("./figures/MatchedScanClusterSignal_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.pdf",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,1200,800);
  c_Signal->Divide(3,2);
  for(int i_pad = 0; i_pad < 24; ++i_pad)
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
    for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
    { 
      c_Signal->cd(i_sensor+1)->Clear();
      c_Signal->cd(i_sensor+1);
      
      TLegend *leg = new TLegend(0.35,0.6,0.80,0.85);
      leg->SetBorderSize(0);
      leg->SetFillColor(10);
  
      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip){
        string HistTitle = Form("Matched Scan Cluster Signal Sensor %d SF%d",i_sensor,i_match);
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->SetTitle(HistTitle.c_str());
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->GetXaxis()->SetTitle("ADC");
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->GetYaxis()->SetTitle("Counts");
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->SetLineColor(1+i_rstrip);
        h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->SetMaximum(maxCountsSignal[i_sensor][i_match]*1.05);
        //TH1F *scanSignal = (TH1F*)h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->Rebin(4,"scanSignal");
        TH1F *scanSignal = (TH1F*)h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->Clone("scanSignal");
        scanSignal->SetStats(0);
        if(i_rstrip != 0) scanSignal->Draw("same");
        else scanSignal->Draw(); 
        std::string LegName = Form("Rstrip%d, #mu = %.1f#pm%.1f, #sigma = %.1f#pm%.1f",i_rstrip,scanSignal->GetMean(),scanSignal->GetMeanError(),scanSignal->GetRMS(),scanSignal->GetRMSError());
        leg->AddEntry(h_mMatchedScanSignal_Rstrips[i_sensor][i_rstrip][i_match],LegName.c_str(),"L");
        if(i_rstrip == 3) leg->Draw("same");
        //h_mMatchedScanSignal[i_sensor][i_match]->Draw();
      }

      c_Signal->cd(3+i_sensor+1)->Clear();
      c_Signal->cd(3+i_sensor+1);
      
      TLegend *leg2 = new TLegend(0.35,0.6,0.80,0.85);
      leg2->SetBorderSize(0);
      leg2->SetFillColor(10);

      for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip){
        string HistTitle = Form("Angle Corrected Matched Scan Cluster Signal Sensor %d SF%d",i_sensor,i_match);
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->SetTitle(HistTitle.c_str());
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->GetXaxis()->SetTitle("ADC*cos(#theta_{incident})");
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->GetYaxis()->SetTitle("Counts");
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->SetLineColor(1+i_rstrip);
        h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->SetMaximum(maxCountsSignalCorr[i_sensor][i_match]*1.05);
        //TH1F *scanSignalCorr = (TH1F*)h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->Rebin(4,"scanSignalCorr");
        TH1F *scanSignalCorr = (TH1F*)h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match]->Clone("scanSignalCorr");
        scanSignalCorr->SetStats(0);
        if(i_rstrip != 0) scanSignalCorr->Draw("same");
        else scanSignalCorr->Draw();
        std::string LegName = Form("Rstrip%d, #mu = %.1f#pm%.1f, #sigma = %.1f#pm%.1f",i_rstrip,scanSignalCorr->GetMean(),scanSignalCorr->GetMeanError(),scanSignalCorr->GetRMS(),scanSignalCorr->GetRMSError());
        leg2->AddEntry(h_mMatchedAngleCorrectedScanSignal_Rstrips[i_sensor][i_rstrip][i_match],LegName.c_str(),"L");
        if(i_rstrip == 3) leg2->Draw("same");

        //h_mMatchedAngleCorrectedScanSignal[i_sensor][i_match]->Draw();
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str());
  }

  string output_stop = Form("%s]",outputname.c_str());
  c_Signal->Print(output_stop.c_str()); // open pdf file
}

