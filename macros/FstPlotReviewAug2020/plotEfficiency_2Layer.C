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
#include "../FstPlotConfiguration/draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotEfficiency_2Layer(string hv = "HV100V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputModule_left = Form("../../output/review/FstModuleEff_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_left.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Module_left = TFile::Open(inputModule_left.c_str());
  TH2F *h_mSimpleClustersTrackFstEff_2Layer_left[FST::mFstNumMatching];
  TH1F *h_mSimpleClustersTrackFstEffR_2Layer_left[FST::mFstNumMatching];
  TH1F *h_mSimpleClustersTrackFstEffPhi_2Layer_left[FST::mFstNumMatching];
  TH2F *h_mScanClustersTrackFstEff_2Layer_left[FST::mFstNumMatching];
  TH1F *h_mScanClustersTrackFstEffR_2Layer_left[FST::mFstNumMatching];
  TH1F *h_mScanClustersTrackFstEffPhi_2Layer_left[FST::mFstNumMatching];
  for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackFstEff_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEff_2Layer_left[i_match] = (TH2F*)File_Module_left->Get(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstEffR_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffR_2Layer_left[i_match] = (TH1F*)File_Module_left->Get(HistName.c_str());
    HistName = Form("h_mSimpleClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffPhi_2Layer_left[i_match] = (TH1F*)File_Module_left->Get(HistName.c_str());

    HistName = Form("h_mScanClustersTrackFstEff_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEff_2Layer_left[i_match] = (TH2F*)File_Module_left->Get(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstEffR_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffR_2Layer_left[i_match] = (TH1F*)File_Module_left->Get(HistName.c_str());
    HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffPhi_2Layer_left[i_match] = (TH1F*)File_Module_left->Get(HistName.c_str());
  }


  string inputSensor_left = Form("../../output/review/FstEfficiency_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_left.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Sensor_left = TFile::Open(inputSensor_left.c_str());
  TH1F *h_mSensorSimpleClustersTrackFstEffR_2Layer_left[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffR_2Layer_left[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      string HistName;
      HistName = Form("h_mSimpleClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,i_match);
      h_mSensorSimpleClustersTrackFstEffR_2Layer_left[i_sensor][i_match] = (TH1F*)File_Sensor_left->Get(HistName.c_str());

      HistName = Form("h_mScanClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,i_match);
      h_mSensorScanClustersTrackFstEffR_2Layer_left[i_sensor][i_match] = (TH1F*)File_Sensor_left->Get(HistName.c_str());
    }
  }

  string inputModule_right = Form("../../output/review/FstModuleEff_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_right.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Module_right = TFile::Open(inputModule_right.c_str());
  TH1F *h_mSimpleClustersTrackFstEffR_2Layer_right[FST::mFstNumMatching];
  TH1F *h_mScanClustersTrackFstEffR_2Layer_right[FST::mFstNumMatching];
  for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackFstEffR_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffR_2Layer_right[i_match] = (TH1F*)File_Module_right->Get(HistName.c_str());

    HistName = Form("h_mScanClustersTrackFstEffR_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffR_2Layer_right[i_match] = (TH1F*)File_Module_right->Get(HistName.c_str());
  }

  string inputSensor_right = Form("../../output/review/FstEfficiency_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_right.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Sensor_right = TFile::Open(inputSensor_right.c_str());
  TH1F *h_mSensorSimpleClustersTrackFstEffR_2Layer_right[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffR_2Layer_right[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      string HistName;
      HistName = Form("h_mSimpleClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,i_match);
      h_mSensorSimpleClustersTrackFstEffR_2Layer_right[i_sensor][i_match] = (TH1F*)File_Sensor_right->Get(HistName.c_str());

      HistName = Form("h_mScanClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,i_match);
      h_mSensorScanClustersTrackFstEffR_2Layer_right[i_sensor][i_match] = (TH1F*)File_Sensor_right->Get(HistName.c_str());
    }
  }

  string inputModule_inner = Form("../../output/review/FstModuleEff_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_inner.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Module_inner = TFile::Open(inputModule_inner.c_str());
  TH1F *h_mSimpleClustersTrackFstEffPhi_2Layer_inner[FST::mFstNumMatching];
  TH1F *h_mScanClustersTrackFstEffPhi_2Layer_inner[FST::mFstNumMatching];
  for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffPhi_2Layer_inner[i_match] = (TH1F*)File_Module_inner->Get(HistName.c_str());

    HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match] = (TH1F*)File_Module_inner->Get(HistName.c_str());
  }

  string inputModule_outer = Form("../../output/review/FstModuleEff_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s_outer.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Module_outer = TFile::Open(inputModule_outer.c_str());
  TH1F *h_mSimpleClustersTrackFstEffPhi_2Layer_outer[FST::mFstNumMatching];
  TH1F *h_mScanClustersTrackFstEffPhi_2Layer_outer[FST::mFstNumMatching];
  for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
  {
    string HistName;
    HistName = Form("h_mSimpleClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mSimpleClustersTrackFstEffPhi_2Layer_outer[i_match] = (TH1F*)File_Module_outer->Get(HistName.c_str());

    HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_SF%d",i_match);
    h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match] = (TH1F*)File_Module_outer->Get(HistName.c_str());
  }

  string inputSensor = Form("../../output/review/FstEfficiency_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
  TFile *File_Sensor = TFile::Open(inputSensor.c_str());
  TH1F *h_mSensorSimpleClustersTrackFstEffPhi_2Layer[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffPhi_2Layer[FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      string HistName;
      HistName = Form("h_mSimpleClustersTrackFstEffPhi_2Layer_Sensor%d_SF%d",i_sensor,i_match);
      h_mSensorSimpleClustersTrackFstEffPhi_2Layer[i_sensor][i_match] = (TH1F*)File_Sensor->Get(HistName.c_str());

      HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_Sensor%d_SF%d",i_sensor,i_match);
      h_mSensorScanClustersTrackFstEffPhi_2Layer[i_sensor][i_match] = (TH1F*)File_Sensor->Get(HistName.c_str());
    }
  }

  string outputname = Form("./figures/EffComparison_%s_TrackCluster_2Layer.pdf",hv.c_str());
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,400);
  c_play->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }
  string output_start = Form("./figures/EffComparison_%s_TrackCluster_2Layer.pdf[",hv.c_str());
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
  {
    c_play->cd(1);
    string titleLeftR = Form("R Efficiency for SF%d",i_match);
    // left Module
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->SetTitle(titleLeftR.c_str());
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->GetXaxis()->SetTitle("r (mm)");
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->GetYaxis()->SetTitle("Efficiency");
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->SetMarkerStyle(24);
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->SetMarkerSize(0.8);
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->SetMarkerColor(2);
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->SetLineColor(2);
    h_mScanClustersTrackFstEffR_2Layer_left[i_match]->Draw("pE");

    // left Sensor
    h_mSensorScanClustersTrackFstEffR_2Layer_left[0][i_match]->SetLineColor(2);
    h_mSensorScanClustersTrackFstEffR_2Layer_left[0][i_match]->SetLineStyle(1);
    h_mSensorScanClustersTrackFstEffR_2Layer_left[0][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffR_2Layer_left[0][i_match]->Draw("HIST same");
    h_mSensorScanClustersTrackFstEffR_2Layer_left[1][i_match]->SetLineColor(2);
    h_mSensorScanClustersTrackFstEffR_2Layer_left[1][i_match]->SetLineStyle(2);
    h_mSensorScanClustersTrackFstEffR_2Layer_left[1][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffR_2Layer_left[1][i_match]->Draw("HIST same");

    // right Module
    h_mScanClustersTrackFstEffR_2Layer_right[i_match]->SetMarkerStyle(24);
    h_mScanClustersTrackFstEffR_2Layer_right[i_match]->SetMarkerSize(0.8);
    h_mScanClustersTrackFstEffR_2Layer_right[i_match]->SetMarkerColor(4);
    h_mScanClustersTrackFstEffR_2Layer_right[i_match]->SetLineColor(4);
    h_mScanClustersTrackFstEffR_2Layer_right[i_match]->Draw("pE same");

    // right Sensor
    h_mSensorScanClustersTrackFstEffR_2Layer_right[0][i_match]->SetLineColor(4);
    h_mSensorScanClustersTrackFstEffR_2Layer_right[0][i_match]->SetLineStyle(1);
    h_mSensorScanClustersTrackFstEffR_2Layer_right[0][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffR_2Layer_right[0][i_match]->Draw("HIST same");
    h_mSensorScanClustersTrackFstEffR_2Layer_right[2][i_match]->SetLineColor(4);
    h_mSensorScanClustersTrackFstEffR_2Layer_right[2][i_match]->SetLineStyle(2);
    h_mSensorScanClustersTrackFstEffR_2Layer_right[2][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffR_2Layer_right[2][i_match]->Draw("HIST same");
    for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule-1; ++i_sensor)
    {
      PlotLine(FST::mFstRMin[i_sensor], FST::mFstRMin[i_sensor], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::mFstRMax[i_sensor], FST::mFstRMax[i_sensor], 0.0, 1.05, 1, 2, 2);
    }
    PlotLine(FST::rMin[0], FST::rMax[1], 0.90, 0.90, 1, 1, 2);
    PlotLine(FST::rMin[0], FST::rMax[1], 0.95, 0.95, 1, 1, 2);
    TLegend *legR = new TLegend(0.25,0.2,0.80,0.4);
    legR->SetBorderSize(0);
    legR->SetFillColor(10);
    legR->SetNColumns(2);
    legR->AddEntry(h_mScanClustersTrackFstEffR_2Layer_left[i_match],"Module03 Left","p");
    legR->AddEntry(h_mScanClustersTrackFstEffR_2Layer_right[i_match],"Module03 Right","p");
    legR->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer_left[0][i_match],"Sensor0 Left","L");
    legR->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer_right[0][i_match],"Sensor0 Right","L");
    legR->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer_left[1][i_match],"Sensor1 Left","L");
    legR->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer_right[2][i_match],"Sensor2 Right","L");
    legR->Draw("same");

    c_play->cd(2);
    string titleLeftPhi = Form("#phi Efficiency for SF%d",i_match);
    // inner Module
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->SetTitle(titleLeftPhi.c_str());
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->GetXaxis()->SetTitle("#phi (rad)");
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->GetYaxis()->SetTitle("Efficiency");
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->SetMarkerStyle(24);
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->SetMarkerSize(0.8);
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->SetMarkerColor(2);
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->SetLineColor(2);
    h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match]->Draw("pE");

    // inner Sensor
    h_mSensorScanClustersTrackFstEffPhi_2Layer[0][i_match]->SetLineColor(2);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[0][i_match]->SetLineStyle(1);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[0][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[0][i_match]->Draw("HIST same");

    // outer Module
    h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match]->SetMarkerStyle(24);
    h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match]->SetMarkerSize(0.8);
    h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match]->SetMarkerColor(4);
    h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match]->SetLineColor(4);
    h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match]->Draw("pE same");

    // right Sensor
    h_mSensorScanClustersTrackFstEffPhi_2Layer[1][i_match]->SetLineColor(4);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[1][i_match]->SetLineStyle(1);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[1][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[1][i_match]->Draw("HIST same");
    h_mSensorScanClustersTrackFstEffPhi_2Layer[2][i_match]->SetLineColor(4);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[2][i_match]->SetLineStyle(2);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[2][i_match]->SetLineWidth(1);
    h_mSensorScanClustersTrackFstEffPhi_2Layer[2][i_match]->Draw("HIST same");
    for(int i_sensor = 1; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
    {
      PlotLine(FST::mFstPhiMin[i_sensor], FST::mFstPhiMin[i_sensor], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::mFstPhiMax[i_sensor], FST::mFstPhiMax[i_sensor], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::phiMin[i_sensor], FST::phiMax[i_sensor], 0.90, 0.90, 1, 1, 2);
      PlotLine(FST::phiMin[i_sensor], FST::phiMax[i_sensor], 0.95, 0.95, 1, 1, 2);
    }
    TLegend *legPhi = new TLegend(0.20,0.2,0.50,0.5);
    legPhi->SetBorderSize(0);
    legPhi->SetFillColor(10);
    // legPhi->SetNColumns(2);
    legPhi->AddEntry(h_mScanClustersTrackFstEffPhi_2Layer_inner[i_match],"Module03 Inner","p");
    legPhi->AddEntry(h_mScanClustersTrackFstEffPhi_2Layer_outer[i_match],"Module03 Outer","p");
    legPhi->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[0][i_match],"Sensor0","L");
    legPhi->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[1][i_match],"Sensor1","L");
    legPhi->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[2][i_match],"Sensor2","L");
    legPhi->Draw("same");

    c_play->Update();
    c_play->Print(outputname.c_str());
  }

  string output_stop = Form("./figures/EffComparison_%s_TrackCluster_2Layer.pdf]",hv.c_str());
  c_play->Print(output_stop.c_str()); // open pdf file
}
