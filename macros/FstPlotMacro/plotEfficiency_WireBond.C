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
#include <TPaveStats.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotEfficiency_WireBond(string mod = "Mod04", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  //gStyle->SetOptStat(0);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.25); gStyle->SetStatH(0.20);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  const int numOfHV = 2;
  string hv[numOfHV] = {"HV70V","HV140V"};
  
  const int numOfSF = 3;
  int sf[numOfSF] {0,3,4};
  
  TH1F *h_mScanClustersTrackFstEffR_2Layer[2][numOfHV][FST::mFstNumSensorsPerModule][numOfSF];
  TH1F *h_mScanClustersTrackFstEffPhi_2Layer[2][numOfHV][FST::mFstNumSensorsPerModule][numOfSF];
  
  for(int i_config = 0; i_config < 2; ++i_config)
  {
    for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
    {
      string config;
      if(i_config == 0) config = "configuration";
      if(i_config == 1) config = "configuration_wb";
      string inputfile = Form("../../output/%s/FstTracking_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",config.c_str(),mod.c_str(),hv[i_hv].c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

      TFile *File_InPut = TFile::Open(inputfile.c_str());
      
      TH2F *h_mScanClustersTrackIstCounts_2Layer[FST::mFstNumSensorsPerModule][numOfSF];
      TH1F *h_mScanClustersTrackIstCountsR_2Layer[FST::mFstNumSensorsPerModule][numOfSF];
      TH1F *h_mScanClustersTrackIstCountsPhi_2Layer[FST::mFstNumSensorsPerModule][numOfSF];

      TH2F *h_mScanClustersTrackFstCounts_2Layer[FST::mFstNumSensorsPerModule][numOfSF];
      TH1F *h_mScanClustersTrackFstCountsR_2Layer[FST::mFstNumSensorsPerModule][numOfSF];
      TH1F *h_mScanClustersTrackFstCountsPhi_2Layer[FST::mFstNumSensorsPerModule][numOfSF];
      
      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        for(int i_match = 0; i_match < numOfSF; ++i_match)
        {
          string HistName;
          
          HistName = Form("h_mScanClustersTrackIstCounts_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
          h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->Sumw2();
          
          // IST R projection
          HistName = Form("h_mScanClustersTrackIstCountsR_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          int projIstBinX0 = h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->GetYaxis()->FindBin(FST::mFstPhiMin[i_sensor]+4*FST::pitchPhi);
          int projIstBinX1 = h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->GetYaxis()->FindBin(FST::mFstPhiMax[i_sensor]-4*FST::pitchPhi);
          h_mScanClustersTrackIstCountsR_2Layer[i_sensor][i_match] = (TH1F*)h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->ProjectionX(HistName.c_str(),projIstBinX0,projIstBinX1);

          // IST Phi projection
          HistName = Form("h_mScanClustersTrackIstCountsPhi_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          int projIstBinY0 = h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->GetXaxis()->FindBin(FST::mFstRMin[i_sensor]+0.4*FST::pitchR);
          int projIstBinY1 = h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->GetXaxis()->FindBin(FST::mFstRMax[i_sensor]-0.4*FST::pitchR);
          h_mScanClustersTrackIstCountsPhi_2Layer[i_sensor][i_match] = (TH1F*)h_mScanClustersTrackIstCounts_2Layer[i_sensor][i_match]->ProjectionY(HistName.c_str(),projIstBinY0,projIstBinY1);

          HistName = Form("h_mScanClustersTrackFstCounts_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
          h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->Sumw2();

          // FST R projection
          HistName = Form("h_mScanClustersTrackFstCountsR_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          int projFstBinX0 = h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->GetYaxis()->FindBin(FST::mFstPhiMin[i_sensor]+4*FST::pitchPhi);
          int projFstBinX1 = h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->GetYaxis()->FindBin(FST::mFstPhiMax[i_sensor]-4*FST::pitchPhi);
          h_mScanClustersTrackFstCountsR_2Layer[i_sensor][i_match] = (TH1F*)h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->ProjectionX(HistName.c_str(),projFstBinX0,projFstBinX1);

          // FST Phi projection
          HistName = Form("h_mScanClustersTrackFstCountsPhi_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          int projFstBinY0 = h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->GetXaxis()->FindBin(FST::mFstRMin[i_sensor]+0.4*FST::pitchR);
          int projFstBinY1 = h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->GetXaxis()->FindBin(FST::mFstRMax[i_sensor]-0.4*FST::pitchR);
          h_mScanClustersTrackFstCountsPhi_2Layer[i_sensor][i_match] = (TH1F*)h_mScanClustersTrackFstCounts_2Layer[i_sensor][i_match]->ProjectionY(HistName.c_str(),projFstBinY0,projFstBinY1);
        }
      }

      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        for(int i_match = 0; i_match < numOfSF; ++i_match)
        {
          string HistName;
          HistName = Form("h_mScanClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match] = (TH1F*)h_mScanClustersTrackIstCountsR_2Layer[i_sensor][i_match]->Clone(HistName.c_str());
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->SetTitle(HistName.c_str());
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->Reset();
          if(h_mScanClustersTrackIstCountsR_2Layer[i_sensor][i_match]->GetEntries() > 0)
          {
	    h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->Divide(h_mScanClustersTrackFstCountsR_2Layer[i_sensor][i_match],h_mScanClustersTrackIstCountsR_2Layer[i_sensor][i_match],1,1,"B");
          }

          HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_Sensor%d_SF%d",i_sensor,sf[i_match]);
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match] = (TH1F*)h_mScanClustersTrackIstCountsPhi_2Layer[i_sensor][i_match]->Clone(HistName.c_str());
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->SetTitle(HistName.c_str());
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->Reset();
          if(h_mScanClustersTrackIstCountsPhi_2Layer[i_sensor][i_match]->GetEntries() > 0)
          {
	    h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->Divide(h_mScanClustersTrackFstCountsPhi_2Layer[i_sensor][i_match],h_mScanClustersTrackIstCountsPhi_2Layer[i_sensor][i_match],1,1,"B");
          }
        }
      }  
    }
  }

  string outputname = Form("./figures/wirebond/Efficiency_%s_WireBondStudy.pdf",mod.c_str());
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,300*numOfHV,600);
  c_play->Divide(numOfHV,2);
  for(int i_pad = 0; i_pad < numOfHV*2; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetRightMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("%s[",outputname.c_str());
  c_play->Print(output_start.c_str()); // open pdf file
  
  for(int i_match = 0; i_match < numOfSF; ++i_match)
  {
    for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
    {
      c_play->cd();
      string TitleName = Form("Sensor %d SF%d",i_sensor, sf[i_match]);
      TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
      canvas_title->SetBorderSize(1);
      canvas_title->Draw();
      
      for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
      {
        for(int i_config = 0; i_config < 2; ++i_config)
        {
          c_play->cd(1+i_hv);
          string subtitle = Form("SF%d R Eff Scan Clusters %s",sf[i_match],hv[i_hv].c_str());
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->SetTitle(subtitle.c_str());
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->SetStats(0);
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
          h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->GetYaxis()->SetRangeUser(0.0,1.05);
          if(i_config == 0) 
          {
            h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->SetLineColor(1);
            h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->Draw("pE");
          }
          if(i_config == 1)
          {  
            h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->SetLineColor(2);
            h_mScanClustersTrackFstEffR_2Layer[i_config][i_hv][i_sensor][i_match]->Draw("pE same");
          }
          PlotLine(FST::mFstRMin[i_sensor], FST::mFstRMin[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::mFstRMax[i_sensor], FST::mFstRMax[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::rMin[i_sensor], FST::rMax[i_sensor], 0.90, 0.90, 4, 1, 2);
          PlotLine(FST::rMin[i_sensor], FST::rMax[i_sensor], 0.95, 0.95, 4, 1, 2);

          c_play->cd(1+numOfHV+i_hv);
          subtitle = Form("SF%d Phi Eff Scan Clusters %s",sf[i_match],hv[i_hv].c_str());
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->SetTitle(subtitle.c_str());
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->SetStats(0);
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->GetXaxis()->SetTitle("phi_{proj} (rad)");
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->GetYaxis()->SetRangeUser(0.0,1.05);
          h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->SetLineColor(1);
          if(i_config == 0) 
          {
            h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->SetLineColor(1);
            h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->Draw("pE");
          }
          if(i_config == 1)
          {  
            h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->SetLineColor(2);
            h_mScanClustersTrackFstEffPhi_2Layer[i_config][i_hv][i_sensor][i_match]->Draw("pE same");
          }
          PlotLine(FST::mFstPhiMin[i_sensor], FST::mFstPhiMin[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::mFstPhiMax[i_sensor], FST::mFstPhiMax[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::phiMin[i_sensor], FST::phiMax[i_sensor], 0.90, 0.90, 4, 1, 2);
          PlotLine(FST::phiMin[i_sensor], FST::phiMax[i_sensor], 0.95, 0.95, 4, 1, 2);
          
          if(i_match == 0 && i_sensor == 0 && i_hv == 0)
          {
	    TLegend *leg = new TLegend(0.3,0.4,0.7,0.6);
	    leg->SetBorderSize(0);
	    leg->SetFillColor(10);
	    leg->AddEntry(h_mScanClustersTrackFstEffPhi_2Layer[0][i_hv][i_sensor][i_match],"w/o Encapsulation","L");
	    leg->AddEntry(h_mScanClustersTrackFstEffPhi_2Layer[1][i_hv][i_sensor][i_match],"w/ Encapsulation","L");
	    leg->Draw("same");
          }
        }
      }
      c_play->Update();
      c_play->Print(outputname.c_str());
    }
  }
  
  string output_stop = Form("%s]",outputname.c_str());
  c_play->Print(output_stop.c_str()); // close pdf file
}
