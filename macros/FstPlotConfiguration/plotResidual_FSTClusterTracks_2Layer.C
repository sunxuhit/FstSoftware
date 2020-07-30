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

void plotResidual_FSTClusterTracks_2Layer(string mod = "Mod03", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.20); gStyle->SetStatH(0.20);

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstTracking_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());

  // FST simple clusters
  TH1F *h_mSimpleClustersTrackFstResX_2Layer[FST::mFstNumSensorsPerModule]; // 0 for inner | 1 & 2 for outer
  TH1F *h_mSimpleClustersTrackFstResY_2Layer[FST::mFstNumSensorsPerModule];
  TH2F *h_mSimpleClustersTrackFstResXY_2Layer[FST::mFstNumSensorsPerModule]; // 2D distribution

  TH1F *h_mSimpleClustersTrackFstResR_2Layer[FST::mFstNumSensorsPerModule];
  TH1F *h_mSimpleClustersTrackFstResPhi_2Layer[FST::mFstNumSensorsPerModule];
  TH2F *h_mSimpleClustersTrackFstResRPhi_2Layer[FST::mFstNumSensorsPerModule]; // 2D distribution

  TH1F *h_mSimpleClustersTrackFstResX_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mSimpleClustersTrackFstResY_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH2F *h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor]; // 2D distribution

  TH1F *h_mSimpleClustersTrackFstResR_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH2F *h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor]; // 2D distribution

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackFstResX_2Layer_Sensor%d",i_sensor);
    h_mSimpleClustersTrackFstResX_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstResY_2Layer_Sensor%d",i_sensor);
    h_mSimpleClustersTrackFstResY_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstResXY_2Layer_Sensor%d",i_sensor);
    h_mSimpleClustersTrackFstResXY_2Layer[i_sensor]   = (TH2F*)File_InPut->Get(HistName.c_str());

    HistName = Form("h_mSimpleClustersTrackFstResR_2Layer_Sensor%d",i_sensor);
    h_mSimpleClustersTrackFstResR_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstResPhi_2Layer_Sensor%d",i_sensor);
    h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]  = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstResRPhi_2Layer_Sensor%d",i_sensor);
    h_mSimpleClustersTrackFstResRPhi_2Layer[i_sensor] = (TH2F*)File_InPut->Get(HistName.c_str());

    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      HistName = Form("h_mSimpleClustersTrackFstResX_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // X Residual
      if(i_sensor > 0) HistName = Form("h_mSimpleClustersTrackFstResX_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]  = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mSimpleClustersTrackFstResY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // Y Residual
      if(i_sensor > 0) HistName = Form("h_mSimpleClustersTrackFstResY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]  = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mSimpleClustersTrackFstResXY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // XY Residual
      if(i_sensor > 0) HistName = Form("h_mSimpleClustersTrackFstResXY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip] = (TH2F*)File_InPut->Get(HistName.c_str());

      HistName = Form("h_mSimpleClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // R Residual
      if(i_sensor > 0) HistName = Form("h_mSimpleClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]    = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mSimpleClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // Phi Residual
      if(i_sensor > 0) HistName = Form("h_mSimpleClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]  = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mSimpleClustersTrackFstResRPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // RPhi Residual
      if(i_sensor > 0) HistName = Form("h_mSimpleClustersTrackFstResRPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip] = (TH2F*)File_InPut->Get(HistName.c_str());
    }
  }

  // FST scan clusters
  TH1F *h_mScanClustersTrackFstResX_2Layer[FST::mFstNumSensorsPerModule]; // 0 for inner | 1 & 2 for outer
  TH1F *h_mScanClustersTrackFstResY_2Layer[FST::mFstNumSensorsPerModule];
  TH2F *h_mScanClustersTrackFstResXY_2Layer[FST::mFstNumSensorsPerModule]; // 2D distribution

  TH1F *h_mScanClustersTrackFstResR_2Layer[FST::mFstNumSensorsPerModule];
  TH1F *h_mScanClustersTrackFstResPhi_2Layer[FST::mFstNumSensorsPerModule];
  TH2F *h_mScanClustersTrackFstResRPhi_2Layer[FST::mFstNumSensorsPerModule]; // 2D distribution

  TH1F *h_mScanClustersTrackFstResX_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mScanClustersTrackFstResY_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH2F *h_mScanClustersTrackFstResXY_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor]; // 2D distribution

  TH1F *h_mScanClustersTrackFstResR_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH1F *h_mScanClustersTrackFstResPhi_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
  TH2F *h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor]; // 2D distribution

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    string HistName;
    HistName = Form("h_mScanClustersTrackFstResX_2Layer_Sensor%d",i_sensor);
    h_mScanClustersTrackFstResX_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstResY_2Layer_Sensor%d",i_sensor);
    h_mScanClustersTrackFstResY_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstResXY_2Layer_Sensor%d",i_sensor);
    h_mScanClustersTrackFstResXY_2Layer[i_sensor]   = (TH2F*)File_InPut->Get(HistName.c_str());

    HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d",i_sensor);
    h_mScanClustersTrackFstResR_2Layer[i_sensor]    = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d",i_sensor);
    h_mScanClustersTrackFstResPhi_2Layer[i_sensor]  = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstResRPhi_2Layer_Sensor%d",i_sensor);
    h_mScanClustersTrackFstResRPhi_2Layer[i_sensor] = (TH2F*)File_InPut->Get(HistName.c_str());

    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    {
      HistName = Form("h_mScanClustersTrackFstResX_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // X Residual
      if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResX_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]  = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mScanClustersTrackFstResY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // Y Residual
      if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]  = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mScanClustersTrackFstResXY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // XY Residual
      if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResXY_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mScanClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip] = (TH2F*)File_InPut->Get(HistName.c_str());

      HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // R Residual
      if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]    = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // Phi Residual
      if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]  = (TH1F*)File_InPut->Get(HistName.c_str());
      HistName = Form("h_mScanClustersTrackFstResRPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip); // RPhi Residual
      if(i_sensor > 0) HistName = Form("h_mScanClustersTrackFstResRPhi_2Layer_Sensor%d_Rstrip%d",i_sensor,i_rstrip+4);
      h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip] = (TH2F*)File_InPut->Get(HistName.c_str());
    }
  }

  string outputname = "./figures/Residual_FSTClusterTracks_2Layer.pdf";
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,900,1200);
  c_play->Divide(3,4);
  for(int i_pad = 0; i_pad < 12; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetRightMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = "./figures/Residual_FSTClusterTracks_2Layer.pdf[";
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    c_play->cd();
    string TitleName = Form("Sensor %d",i_sensor);
    TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.055,1.0,TitleName.c_str());
    canvas_title->SetBorderSize(1);
    canvas_title->Draw();
    {
      c_play->cd(1);
      h_mSimpleClustersTrackFstResXY_2Layer[i_sensor]->SetTitle("Corrected X & Y Residual FST Simple Clusters");
      h_mSimpleClustersTrackFstResXY_2Layer[i_sensor]->SetStats(0);
      h_mSimpleClustersTrackFstResXY_2Layer[i_sensor]->GetXaxis()->SetTitle("x-residual (mm)");
      h_mSimpleClustersTrackFstResXY_2Layer[i_sensor]->GetYaxis()->SetTitle("y-residual (mm)");
      h_mSimpleClustersTrackFstResXY_2Layer[i_sensor]->Draw("colz");

      c_play->cd(2);
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->SetTitle("Corrected X-residual FST Simple Clusters");
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->GetXaxis()->SetTitle("x-residual (mm)");
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->Draw();
      // h_mSimpleClustersTrackFstResX_2Layer->Fit("gaus");
      TF1 *f_gausX = new TF1("f_gausX",gaussian,-150.0,150.0,4);
      f_gausX->SetParameter(0,100.0);
      f_gausX->SetParameter(1,0.0);
      f_gausX->SetParameter(2,10.0);
      f_gausX->FixParameter(3,h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->GetBinWidth(1));
      f_gausX->SetRange(-20,50);
      h_mSimpleClustersTrackFstResX_2Layer[i_sensor]->Fit(f_gausX,"R");
      f_gausX->Draw("l same");

      c_play->cd(3);
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->SetTitle("Corrected Y-residual FST Simple Clusters");
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->GetXaxis()->SetTitle("y-residual (mm)");
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->Draw();
      // h_mSimpleClustersTrackFstResY_2Layer->Fit("gaus");
      TF1 *f_gausY = new TF1("f_gausY",gaussian,-150.0,150.0,4);
      f_gausY->SetParameter(0,100.0);
      f_gausY->SetParameter(1,0.0);
      f_gausY->SetParameter(2,10.0);
      f_gausY->FixParameter(3,h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->GetBinWidth(1));
      f_gausY->SetRange(-1.5,5);
      h_mSimpleClustersTrackFstResY_2Layer[i_sensor]->Fit(f_gausY,"R");
      f_gausY->Draw("l same");

      c_play->cd(4);
      h_mSimpleClustersTrackFstResRPhi_2Layer[i_sensor]->SetTitle("Corrected R & #phi Residual FST Simple Clusters");
      h_mSimpleClustersTrackFstResRPhi_2Layer[i_sensor]->SetStats(0);
      h_mSimpleClustersTrackFstResRPhi_2Layer[i_sensor]->GetXaxis()->SetTitle("r-residual (mm)");
      h_mSimpleClustersTrackFstResRPhi_2Layer[i_sensor]->GetYaxis()->SetTitle("#phi-residual (rad)");
      h_mSimpleClustersTrackFstResRPhi_2Layer[i_sensor]->Draw("colz");

      c_play->cd(5);
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->SetTitle("Corrected R-residual FST Simple Clusters");
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->GetXaxis()->SetTitle("r-residual (mm)");
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->Draw();
      // h_mSimpleClustersTrackFstResR_2Layer->Fit("gaus");
      TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
      f_gausR->SetParameter(0,100.0);
      f_gausR->SetParameter(1,0.0);
      f_gausR->SetParameter(2,10.0);
      f_gausR->FixParameter(3,h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->GetBinWidth(1));
      f_gausR->SetRange(-20,50);
      h_mSimpleClustersTrackFstResR_2Layer[i_sensor]->Fit(f_gausR,"R");
      f_gausR->Draw("l same");
      // gPad->Update();
      // TPaveStats *st1 = (TPaveStats*)h_mSimpleClustersTrackFstResR_2Layer->FindObject("stats");
      // st1->SetOptStat(111111);
      // st1->SetX1NDC(0.6);
      // st1->SetX2NDC(0.9);

      c_play->cd(6);
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->SetTitle("Corrected #phi-residual FST Simple Clusters");
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->GetXaxis()->SetTitle("#phi-residual (rad)");
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->Draw();
      // h_mSimpleClustersTrackFstResPhi_2Layer->Fit("gaus");
      TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
      f_gausPhi->SetParameter(0,100.0);
      f_gausPhi->SetParameter(1,0.0);
      f_gausPhi->SetParameter(2,10.0);
      // f_gausPhi->SetParName(0,"No. Tracks");
      // f_gausPhi->SetParName(1,"Mean");
      // f_gausPhi->SetParName(2,"#sigma");
      f_gausPhi->FixParameter(3,h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->GetBinWidth(1));
      f_gausPhi->SetRange(-0.05,0.05);
      h_mSimpleClustersTrackFstResPhi_2Layer[i_sensor]->Fit(f_gausPhi,"R");
      f_gausPhi->Draw("l same");
      // gPad->Update();
      // TPaveStats *st2 = (TPaveStats*)h_mSimpleClustersTrackFstResPhi_2Layer->FindObject("stats");
      // st2->SetOptStat(110011);
    }

    {
      c_play->cd(7);
      h_mScanClustersTrackFstResXY_2Layer[i_sensor]->SetTitle("Corrected X & Y Residual FST Scan Clusters");
      h_mScanClustersTrackFstResXY_2Layer[i_sensor]->SetStats(0);
      h_mScanClustersTrackFstResXY_2Layer[i_sensor]->GetXaxis()->SetTitle("x-residual (mm)");
      h_mScanClustersTrackFstResXY_2Layer[i_sensor]->GetYaxis()->SetTitle("y-residual (mm)");
      h_mScanClustersTrackFstResXY_2Layer[i_sensor]->Draw("colz");

      c_play->cd(8);
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->SetTitle("Corrected X-residual FST Scan Clusters");
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->GetXaxis()->SetTitle("x-residual (mm)");
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->Draw();
      // h_mScanClustersTrackFstResX_2Layer->Fit("gaus");
      TF1 *f_gausX = new TF1("f_gausX",gaussian,-150.0,150.0,4);
      f_gausX->SetParameter(0,100.0);
      f_gausX->SetParameter(1,0.0);
      f_gausX->SetParameter(2,10.0);
      f_gausX->FixParameter(3,h_mScanClustersTrackFstResX_2Layer[i_sensor]->GetBinWidth(1));
      f_gausX->SetRange(-20,50);
      h_mScanClustersTrackFstResX_2Layer[i_sensor]->Fit(f_gausX,"R");
      f_gausX->Draw("l same");

      c_play->cd(9);
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->SetTitle("Corrected Y-residual FST Scan Clusters");
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->GetXaxis()->SetTitle("y-residual (mm)");
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->Draw();
      // h_mScanClustersTrackFstResY_2Layer->Fit("gaus");
      TF1 *f_gausY = new TF1("f_gausY",gaussian,-150.0,150.0,4);
      f_gausY->SetParameter(0,100.0);
      f_gausY->SetParameter(1,0.0);
      f_gausY->SetParameter(2,10.0);
      f_gausY->FixParameter(3,h_mScanClustersTrackFstResY_2Layer[i_sensor]->GetBinWidth(1));
      f_gausY->SetRange(-1.5,5);
      h_mScanClustersTrackFstResY_2Layer[i_sensor]->Fit(f_gausY,"R");
      f_gausY->Draw("l same");

      c_play->cd(10);
      h_mScanClustersTrackFstResRPhi_2Layer[i_sensor]->SetTitle("Corrected R & #phi Residual FST Scan Clusters");
      h_mScanClustersTrackFstResRPhi_2Layer[i_sensor]->SetStats(0);
      h_mScanClustersTrackFstResRPhi_2Layer[i_sensor]->GetXaxis()->SetTitle("r-residual (mm)");
      h_mScanClustersTrackFstResRPhi_2Layer[i_sensor]->GetYaxis()->SetTitle("#phi-residual (rad)");
      h_mScanClustersTrackFstResRPhi_2Layer[i_sensor]->Draw("colz");

      c_play->cd(11);
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->SetTitle("Corrected R-residual FST Scan Clusters");
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->GetXaxis()->SetTitle("r-residual (mm)");
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->Draw();
      TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
      f_gausR->SetParameter(0,100.0);
      f_gausR->SetParameter(1,0.0);
      f_gausR->SetParameter(2,10.0);
      f_gausR->FixParameter(3,h_mScanClustersTrackFstResR_2Layer[i_sensor]->GetBinWidth(1));
      f_gausR->SetRange(-20,50);
      h_mScanClustersTrackFstResR_2Layer[i_sensor]->Fit(f_gausR,"R");
      f_gausR->Draw("l same");

      c_play->cd(12);
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->SetTitle("Corrected #phi-residual FST Scan Clusters");
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->GetXaxis()->SetTitle("#phi-residual (rad)");
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->GetXaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->GetYaxis()->SetTitle("No. Tracks");
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->GetYaxis()->SetTitleSize(0.06);
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->Draw();
      TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
      f_gausPhi->SetParameter(0,100.0);
      f_gausPhi->SetParameter(1,0.0);
      f_gausPhi->SetParameter(2,10.0);
      f_gausPhi->FixParameter(3,h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->GetBinWidth(1));
      f_gausPhi->SetRange(-0.05,0.05);
      h_mScanClustersTrackFstResPhi_2Layer[i_sensor]->Fit(f_gausPhi,"R");
      f_gausPhi->Draw("l same");
    }
    c_play->Update();
    c_play->Print(outputname.c_str());
  }

  TCanvas *c_RStrip = new TCanvas("c_RStrip","c_RStrip",10,10,900,1200);
  c_RStrip->Divide(3,4);
  for(int i_pad = 0; i_pad < 12; ++i_pad)
  {
    c_RStrip->cd(i_pad+1)->SetLeftMargin(0.15);
    c_RStrip->cd(i_pad+1)->SetRightMargin(0.15);
    c_RStrip->cd(i_pad+1)->SetBottomMargin(0.15);
    c_RStrip->cd(i_pad+1)->SetTicks(1,1);
    c_RStrip->cd(i_pad+1)->SetGrid(0,0);
  }

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
    { 
      c_RStrip->cd();
      string TitleName = Form("Sensor %d & R Strip %d",i_sensor, i_rstrip);
      TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.085,1.0,TitleName.c_str());
      canvas_title->SetBorderSize(1);
      canvas_title->Draw();
      string HistName;
      {
	c_RStrip->cd(1)->Clear();
	c_RStrip->cd(1);
	HistName = Form("Corrected X & Y Residual FST Simple Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->SetStats(0);
	h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("x-residual (mm)");
	h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("y-residual (mm)");
	h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->Draw("colz");

	c_RStrip->cd(2)->Clear();
	c_RStrip->cd(2);
	HistName = Form("Corrected X-residual FST Simple Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("x-residual (mm)");
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	// h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_rstrip]->Fit("gaus");
	TF1 *f_gausX = new TF1("f_gausX",gaussian,-150.0,150.0,4);
	f_gausX->SetParameter(0,100.0);
	f_gausX->SetParameter(1,0.0);
	f_gausX->SetParameter(2,10.0);
	f_gausX->FixParameter(3,h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausX->SetRange(-20,50);
	h_mSimpleClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausX,"R");
	f_gausX->Draw("l same");

	c_RStrip->cd(3)->Clear();
	c_RStrip->cd(3);
	HistName = Form("Corrected Y-residual FST Simple Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("y-residual (mm)");
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	// h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_rstrip]->Fit("gaus");
	TF1 *f_gausY = new TF1("f_gausY",gaussian,-150.0,150.0,4);
	f_gausY->SetParameter(0,100.0);
	f_gausY->SetParameter(1,0.0);
	f_gausY->SetParameter(2,10.0);
	f_gausY->FixParameter(3,h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausY->SetRange(-1.5,5);
	h_mSimpleClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausY,"R");
	f_gausY->Draw("l same");

	c_RStrip->cd(4)->Clear();
	c_RStrip->cd(4);
	HistName = Form("Corrected R & #phi Residual FST Simple Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->SetStats(0);
	h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("r-residual (mm)");
	h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("#phi-residual (rad)");
	h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->Draw("colz");

	c_RStrip->cd(5)->Clear();
	c_RStrip->cd(5);
	HistName = Form("Corrected R-residual FST Simple Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("r-residual (mm)");
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	// h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_rstrip]->Fit("gaus");
	TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
	f_gausR->SetParameter(0,100.0);
	f_gausR->SetParameter(1,0.0);
	f_gausR->SetParameter(2,10.0);
	f_gausR->FixParameter(3,h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausR->SetRange(-20,50);
	h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausR,"R");
	f_gausR->Draw("l same");
	// gPad->Update();
	// TPaveStats *st1 = (TPaveStats*)h_mSimpleClustersTrackFstResR_2Layer_Rstrips[i_rstrip]->FindObject("stats");
	// st1->SetOptStat(111111);
	// st1->SetX1NDC(0.6);
	// st1->SetX2NDC(0.9);

	c_RStrip->cd(6)->Clear();
	c_RStrip->cd(6);
	HistName = Form("Corrected #phi-residual FST Simple Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("#phi-residual (rad)");
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	// h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_rstrip]->Fit("gaus");
	TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
	f_gausPhi->SetParameter(0,100.0);
	f_gausPhi->SetParameter(1,0.0);
	f_gausPhi->SetParameter(2,10.0);
	// f_gausPhi->SetParName(0,"No. Tracks");
	// f_gausPhi->SetParName(1,"Mean");
	// f_gausPhi->SetParName(2,"#sigma");
	f_gausPhi->FixParameter(3,h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausPhi->SetRange(-0.05,0.05);
	h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausPhi,"R");
	f_gausPhi->Draw("l same");
	// gPad->Update();
	// TPaveStats *st2 = (TPaveStats*)h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[i_rstrip]->FindObject("stats");
	// st2->SetOptStat(110011);
      }

      {
	c_RStrip->cd(7)->Clear();
	c_RStrip->cd(7);
	HistName = Form("Corrected X & Y Residual FST Scan Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mScanClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mScanClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->SetStats(0);
	h_mScanClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("x-residual (mm)");
	h_mScanClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("y-residual (mm)");
	h_mScanClustersTrackFstResXY_2Layer_Rstrips[i_sensor][i_rstrip]->Draw("colz");

	c_RStrip->cd(8)->Clear();
	c_RStrip->cd(8);
	HistName = Form("Corrected X-residual FST Scan Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("x-residual (mm)");
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	// h_mScanClustersTrackFstResX_2Layer_Rstrips[i_rstrip]->Fit("gaus");
	TF1 *f_gausX = new TF1("f_gausX",gaussian,-150.0,150.0,4);
	f_gausX->SetParameter(0,100.0);
	f_gausX->SetParameter(1,0.0);
	f_gausX->SetParameter(2,10.0);
	f_gausX->FixParameter(3,h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausX->SetRange(-20,50);
	h_mScanClustersTrackFstResX_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausX,"R");
	f_gausX->Draw("l same");

	c_RStrip->cd(9)->Clear();
	c_RStrip->cd(9);
	HistName = Form("Corrected Y-residual FST Scan Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("y-residual (mm)");
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	// h_mScanClustersTrackFstResY_2Layer_Rstrips[i_rstrip]->Fit("gaus");
	TF1 *f_gausY = new TF1("f_gausY",gaussian,-150.0,150.0,4);
	f_gausY->SetParameter(0,100.0);
	f_gausY->SetParameter(1,0.0);
	f_gausY->SetParameter(2,10.0);
	f_gausY->FixParameter(3,h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausY->SetRange(-1.5,5);
	h_mScanClustersTrackFstResY_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausY,"R");
	f_gausY->Draw("l same");

	c_RStrip->cd(10)->Clear();
	c_RStrip->cd(10);
	HistName = Form("Corrected R & #phi Residual FST Scan Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->SetStats(0);
	h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("r-residual (mm)");
	h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("#phi-residual (rad)");
	h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[i_sensor][i_rstrip]->Draw("colz");

	c_RStrip->cd(11)->Clear();
	c_RStrip->cd(11);
	HistName = Form("Corrected R-residual FST Scan Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("r-residual (mm)");
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
	f_gausR->SetParameter(0,100.0);
	f_gausR->SetParameter(1,0.0);
	f_gausR->SetParameter(2,10.0);
	f_gausR->FixParameter(3,h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausR->SetRange(-20,50);
	h_mScanClustersTrackFstResR_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausR,"R");
	f_gausR->Draw("l same");

	c_RStrip->cd(12)->Clear();
	c_RStrip->cd(12);
	HistName = Form("Corrected #phi-residual FST Scan Clusters Sensor %d Rstrip%d",i_sensor, i_rstrip);
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->SetTitle(HistName.c_str());
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitle("#phi-residual (rad)");
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetXaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitle("No. Tracks");
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetYaxis()->SetTitleSize(0.06);
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->Draw();
	TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
	f_gausPhi->SetParameter(0,100.0);
	f_gausPhi->SetParameter(1,0.0);
	f_gausPhi->SetParameter(2,10.0);
	f_gausPhi->FixParameter(3,h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->GetBinWidth(1));
	f_gausPhi->SetRange(-0.05,0.05);
	h_mScanClustersTrackFstResPhi_2Layer_Rstrips[i_sensor][i_rstrip]->Fit(f_gausPhi,"R");
	f_gausPhi->Draw("l same");
      }
      c_RStrip->Update();
      c_RStrip->Print(outputname.c_str());
    }
  }

  string output_stop = "./figures/Residual_FSTClusterTracks_2Layer.pdf]"; 
  c_play->Print(output_stop.c_str()); // close pdf file
}
