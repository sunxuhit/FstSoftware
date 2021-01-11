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
#include "../FstPlotMacro/draw.h"
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

void plotResidualsThTbStudy(string mod = "Mod03", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.20); gStyle->SetStatH(0.20);

  //bool isSavePed             = true;
  //bool isApplyCMNCorr        = true;
  
  const int nFHC             = 4;
  float nFstHitsCut[nFHC]    = {3.0,3.5,4.0,4.5};
  
  const int nTB              = 3;
  int numOfUsedTimeBins[nTB] = {1,2,3};
  
  //const int nFHC             = 2;
  //float nFstHitsCut[nFHC]    = {3.5,4.0};
  
  //const int nTB              = 1;
  //int numOfUsedTimeBins[nTB] = {2};
  
  //float nFstThresholdCut2    = 2.5;
  //float nFstThresholdCut1    = 3.5;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  TFile *File_InPut[nFHC][nTB];

  TH1F *h_mScanClustersTrackFstResR_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule];
  TH1F *h_mScanClustersTrackFstResPhi_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule];
  TH1F *h_mScanClustersTrackFstResR_2Layer_Rstrips[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mScanClustersTrackFstResPhi_2Layer_Rstrips[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
/*
  TH1F *h_mSensorSimpleClustersTrackFstEffR_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorSimpleClustersTrackFstEffPhi_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffR_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffPhi_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
*/
  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
  {
    for(int i_TB = 0; i_TB < nTB; ++i_TB)
    {
      string inputfile = Form("../../output/configuration/FstTracking_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB],nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
      std::cout << "Opening File " << inputfile << std::endl;
      File_InPut[i_FHC][i_TB] = TFile::Open(inputfile.c_str());
      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        string HistName;
	HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d",i_sensor);
	h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
	HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d",i_sensor);
	h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());

	for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
	{
	  HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // R Residual
	  if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
	  h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
	  HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // Phi Residual
	  if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
	  h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
	}
      }
    }
  }

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  //________________________________PLOTTING R RESIDUALS________________________________//

  TH1F *h_play = new TH1F("h_play","h_play",400,-9.5,390.5);
  for(int i_bin = 0; i_bin < 400; ++i_bin)
  {
    h_play->SetBinContent(i_bin+1,-10.0);
    h_play->SetBinError(i_bin+1,1.0);
  }
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("R_meas - R_proj (mm)");
  h_play->GetXaxis()->SetTitleSize(0.06);
  h_play->GetXaxis()->SetNdivisions(505);
  h_play->GetXaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetTitle("Residuals");
  h_play->GetYaxis()->SetNdivisions(505);
  h_play->GetYaxis()->SetTitleSize(0.06);
  h_play->GetYaxis()->SetLabelSize(0.04);
  ///h_play->GetYaxis()->SetRangeUser(0.0,1.1);

  TCanvas *c_ResidualsR = new TCanvas("c_ResidualsR","c_ResidualsR",10,10,400*nFHC,400*nTB);
  c_ResidualsR->Divide(nFHC,nTB);
  for(int i_pad = 0; i_pad < nFHC*nTB; ++i_pad)
  {
    c_ResidualsR->cd(i_pad+1)->SetLeftMargin(0.15);
    c_ResidualsR->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ResidualsR->cd(i_pad+1)->SetTicks(1,1);
    c_ResidualsR->cd(i_pad+1)->SetGrid(0,0);
  }

  string outputname = Form("./figures/ResidualsRThTb%s_Ped%1.1fPed%1.1f.pdf",hv.c_str(),nFstThresholdCut2,nFstThresholdCut1);
  string output_start = Form("%s[",outputname.c_str());
  c_ResidualsR->Print(output_start.c_str());

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
    {
      for(int i_TB = 0; i_TB < nTB; ++i_TB)
      {
        c_ResidualsR->cd(i_TB*nFHC+i_FHC+1)->Clear();
        c_ResidualsR->cd(i_TB*nFHC+i_FHC+1);
        string title = Form("Sensor %d Th %1.1f Tb %d",i_sensor,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);
	h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->SetTitle(title.c_str());
        h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitle("R_{meas} - R_{proj} (mm)");
        h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->GetYaxis()->SetTitle("No. Tracks");
        h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->GetYaxis()->SetTitleSize(0.06);
        h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->Draw();
        TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
        f_gausR->SetParameter(0,100.0);
        f_gausR->SetParameter(1,0.0);
        f_gausR->SetParameter(2,10.0);
        f_gausR->FixParameter(3,h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->GetBinWidth(1));
        f_gausR->SetRange(-20,50);
        h_mScanClustersTrackFstResR_2Layer[i_FHC][i_TB][i_sensor]->Fit(f_gausR,"R");
        f_gausR->Draw("l same");
      }
    }
    c_ResidualsR->Update();
    c_ResidualsR->Print(outputname.c_str());
  }

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
      {
        for(int i_TB = 0; i_TB < nTB; ++i_TB)
        {
          c_ResidualsR->cd(i_TB*nFHC+i_FHC+1)->Clear();
          c_ResidualsR->cd(i_TB*nFHC+i_FHC+1);
          string title = Form("Sensor %d & Rstrip %d Th %1.1f Tb %d",i_sensor,i_rstrip,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);
          h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->SetTitle(title.c_str());
          h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetXaxis()->SetTitle("R_{meas} - R_{proj} (mm)");
          h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
          h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
          h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->Draw();
          TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
          f_gausR->SetParameter(0,100.0);
          f_gausR->SetParameter(1,0.0);
          f_gausR->SetParameter(2,10.0);
          f_gausR->FixParameter(3,h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetBinWidth(1));
          f_gausR->SetRange(-20,50);
          h_mScanClustersTrackFstResR_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->Fit(f_gausR,"R");
          f_gausR->Draw("l same");
        }
      }
      c_ResidualsR->Update();
      c_ResidualsR->Print(outputname.c_str());
    }
  }
  
  string output_stop = Form("%s]",outputname.c_str());
  c_ResidualsR->Print(output_stop.c_str()); // close pdf file

  //________________________________PLOTTING R EFFICIENCIES________________________________//
  
  //________________________________PLOTTING PHI EFFICIENCIES________________________________//

  TH1F *h_playPhi = new TH1F("h_playPhi","h_playPhi",100,-0.5,0.5);
  for(int i_bin = 0; i_bin < 100; ++i_bin)
  {
    h_playPhi->SetBinContent(i_bin+1,-10.0);
    h_playPhi->SetBinError(i_bin+1,1.0);
  }
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("#phi_meas - #phi_proj (mm)");
  h_play->GetXaxis()->SetTitleSize(0.06);
  h_play->GetXaxis()->SetNdivisions(505);
  h_play->GetXaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetTitle("Residuals");
  h_play->GetYaxis()->SetNdivisions(505);
  h_play->GetYaxis()->SetTitleSize(0.06);
  h_play->GetYaxis()->SetLabelSize(0.04);
  //h_playPhi->GetYaxis()->SetRangeUser(0.0,1.1);

  TCanvas *c_ResidualsPhi = new TCanvas("c_ResidualsPhi","c_ResidualsPhi",10,10,400*nFHC,400*nTB);
  c_ResidualsPhi->Divide(nFHC,nTB);
  for(int i_pad = 0; i_pad < nFHC*nTB; ++i_pad)
  {
    c_ResidualsPhi->cd(i_pad+1)->SetLeftMargin(0.15);
    c_ResidualsPhi->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ResidualsPhi->cd(i_pad+1)->SetTicks(1,1);
    c_ResidualsPhi->cd(i_pad+1)->SetGrid(0,0);
  }

  outputname = Form("./figures/ResidualsPhiThTb%s_Ped%1.1fPed%1.1f.pdf",hv.c_str(),nFstThresholdCut2,nFstThresholdCut1);
  output_start = Form("%s[",outputname.c_str());
  c_ResidualsPhi->Print(output_start.c_str());

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
    {
      for(int i_TB = 0; i_TB < nTB; ++i_TB)
      {
        c_ResidualsPhi->cd(i_TB*nFHC+i_FHC+1)->Clear();
        c_ResidualsPhi->cd(i_TB*nFHC+i_FHC+1);
        string title = Form("Sensor %d Th %1.1f Tb %d",i_sensor,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);
	h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->SetTitle(title.c_str());
        h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitle("#phi_{meas} - #phi_{proj} (mm)");
        h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->GetYaxis()->SetTitle("No. Tracks");
        h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->GetYaxis()->SetTitleSize(0.06);
        h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->Draw();
        TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
        f_gausPhi->SetParameter(0,100.0);
        f_gausPhi->SetParameter(1,0.0);
        f_gausPhi->SetParameter(2,10.0);
        f_gausPhi->FixParameter(3,h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->GetBinWidth(1));
        f_gausPhi->SetRange(-0.05,0.05);
        h_mScanClustersTrackFstResPhi_2Layer[i_FHC][i_TB][i_sensor]->Fit(f_gausPhi,"R");
        f_gausPhi->Draw("l same");
      }
    }
    c_ResidualsPhi->Update();
    c_ResidualsPhi->Print(outputname.c_str());
  }

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
      {
        for(int i_TB = 0; i_TB < nTB; ++i_TB)
        {
          c_ResidualsPhi->cd(i_TB*nFHC+i_FHC+1)->Clear();
          c_ResidualsPhi->cd(i_TB*nFHC+i_FHC+1);
          string title = Form("Sensor %d & Rstrip %d Th %1.1f Tb %d",i_sensor,i_rstrip,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);
          h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->SetTitle(title.c_str());
          h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetXaxis()->SetTitle("#phi_{meas} - #phi_{proj} (mm)");
          h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
          h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
          h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->Draw();
          TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
          f_gausPhi->SetParameter(0,100.0);
          f_gausPhi->SetParameter(1,0.0);
          f_gausPhi->SetParameter(2,10.0);
          f_gausPhi->FixParameter(3,h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->GetBinWidth(1));
          f_gausPhi->SetRange(-0.05,0.05);
          h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_FHC][i_TB][i_sensor][i_rstrip]->Fit(f_gausPhi,"R");
          f_gausPhi->Draw("l same");
        }
      }
      c_ResidualsPhi->Update();
      c_ResidualsPhi->Print(outputname.c_str());
    }
  }
  
  output_stop = Form("%s]",outputname.c_str());
  c_ResidualsPhi->Print(output_stop.c_str()); // close pdf file
  
  //________________________________PLOTTING PHI EFFICIENCIES________________________________//
 
}
