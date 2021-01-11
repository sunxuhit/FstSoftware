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

double gaussian(double *var, double *par)
{
  double residual = var[0];

  double norm  = par[0];
  double mean  = par[1];
  double sigma = par[2];
  double binwidth = par[3];

  double sigmaSquare = sigma*sigma;
  double Norm = norm/(sqrt(2.0*TMath::Pi())*sigma);
  double Power = -0.5*(residual-mean)*(residual-mean)/sigmaSquare;

  double y = Norm*exp(Power)*binwidth;

  return y;
}

void plotResidual_WireBond(string mod = "Mod04", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetOptStat(21);
  //gStyle->SetOptStat(111211);
  gStyle->SetOptFit(11);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.20); gStyle->SetStatH(0.20);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  const int numOfHV = 2;
  string hv[numOfHV] = {"HV70V","HV140V"};
  
  TH1F *h_mScanClusterResR[2][numOfHV][3];
  TH1F *h_mScanClusterResPhi[2][numOfHV][3];
  
  for(int i_config = 0; i_config < 2; ++i_config)
  {
    for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
    {
      string config;
      if(i_config == 0) config = "configuration";
      if(i_config == 1) config = "configuration_wb";
      string inputfile = Form("../../output/%s/FstTracking_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",config.c_str(),mod.c_str(),hv[i_hv].c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

      TFile *File_InPut = TFile::Open(inputfile.c_str());
  
      TH1F *h_mScanClustersTrackFstResR_2Layer[FST::mFstNumSensorsPerModule];
      TH1F *h_mScanClustersTrackFstResPhi_2Layer[FST::mFstNumSensorsPerModule];
  
      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        string HistName;

        HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d",i_sensor);
        h_mScanClustersTrackFstResR_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
        HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d",i_sensor);
        h_mScanClustersTrackFstResPhi_2Layer[i_sensor]  = (TH1F*)File_InPut->Get(HistName.c_str());
      
        h_mScanClusterResR[i_config][i_hv][i_sensor] = h_mScanClustersTrackFstResR_2Layer[i_sensor];
        h_mScanClusterResPhi[i_config][i_hv][i_sensor] = h_mScanClustersTrackFstResPhi_2Layer[i_sensor];
    
      }  
    }
  }

  string outputname = Form("./figures/wirebond/Residual_%s_WireBondStudy.pdf",mod.c_str());
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,300*numOfHV,1200);
  c_play->Divide(numOfHV,4);
  for(int i_pad = 0; i_pad < numOfHV*4; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetRightMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = Form("%s[",outputname.c_str());
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    c_play->cd();
    string TitleName = Form("Sensor %d",i_sensor);
    TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
    canvas_title->SetBorderSize(1);
    canvas_title->Draw();
    for(int i_config = 0; i_config < 2; ++i_config)
    {
    string config;
    if(i_config == 0) config = "w/o Encapsulation";
    if(i_config == 1) config = "w/ Encapsulation";
      for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
      {
        c_play->cd(1+i_hv+numOfHV*i_config);
      
        string subtitle = Form("R-residual Scan Clusters %s %s",hv[i_hv].c_str(),config.c_str());
        h_mScanClusterResR[i_config][i_hv][i_sensor]->SetTitle(subtitle.c_str());
        h_mScanClusterResR[i_config][i_hv][i_sensor]->GetXaxis()->SetTitle("r-residual (mm)");
        h_mScanClusterResR[i_config][i_hv][i_sensor]->GetXaxis()->SetTitleSize(0.06);
        h_mScanClusterResR[i_config][i_hv][i_sensor]->GetYaxis()->SetTitle("No. Tracks");
        h_mScanClusterResR[i_config][i_hv][i_sensor]->GetYaxis()->SetTitleSize(0.06);
        h_mScanClusterResR[i_config][i_hv][i_sensor]->Draw();
        TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
        f_gausR->SetParameter(0,100.0);
        f_gausR->SetParameter(1,0.0);
        f_gausR->SetParameter(2,10.0);
        f_gausR->SetParName(0,"Norm");
        f_gausR->SetParName(1,"#mu");
        f_gausR->SetParName(2,"#sigma");
        f_gausR->FixParameter(3,h_mScanClusterResR[i_config][i_hv][i_sensor]->GetBinWidth(1));
        f_gausR->SetRange(-20,50);
        h_mScanClusterResR[i_config][i_hv][i_sensor]->Fit(f_gausR,"R");
        f_gausR->Draw("l same");

        c_play->cd(1+2*numOfHV+i_hv+numOfHV*i_config);
        subtitle = Form("Phi-residual Scan Clusters %s %s",hv[i_hv].c_str(),config.c_str());
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->SetTitle(subtitle.c_str());
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->GetXaxis()->SetTitle("#phi-residual (rad)");
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->GetXaxis()->SetTitleSize(0.06);
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->GetYaxis()->SetTitle("No. Tracks");
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->GetYaxis()->SetTitleSize(0.06);
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->Draw();
        TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
        f_gausPhi->SetParameter(0,100.0);
        f_gausPhi->SetParameter(1,0.0);
        f_gausPhi->SetParameter(2,10.0);
        f_gausPhi->SetParName(0,"Norm");
        f_gausPhi->SetParName(1,"#mu");
        f_gausPhi->SetParName(2,"#sigma");
        f_gausPhi->FixParameter(3,h_mScanClusterResPhi[i_config][i_hv][i_sensor]->GetBinWidth(1));
        f_gausPhi->SetRange(-0.05,0.05);
        h_mScanClusterResPhi[i_config][i_hv][i_sensor]->Fit(f_gausPhi,"R");
        f_gausPhi->Draw("l same");
      }
    }
    c_play->Update();
    c_play->Print(outputname.c_str());
  }
  string output_stop = Form("%s]",outputname.c_str());
  c_play->Print(output_stop.c_str()); // close pdf file
}
