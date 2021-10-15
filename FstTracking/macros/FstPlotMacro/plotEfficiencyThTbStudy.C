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

void plotEfficiencyThTbStudy(string mod = "Mod03", string hv = "HV70V", bool isSavePed = true, bool isApplyCMNCorr = true, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
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
  TH1F *h_mSensorSimpleClustersTrackFstEffR_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorSimpleClustersTrackFstEffPhi_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffR_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];
  TH1F *h_mSensorScanClustersTrackFstEffPhi_2Layer[nFHC][nTB][FST::mFstNumSensorsPerModule][FST::mFstNumMatching];

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
  {
    for(int i_TB = 0; i_TB < nTB; ++i_TB)
    {
      string inputfile = Form("../../output/configuration/FstEfficiency_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB],nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
      std::cout << "Opening File " << inputfile << std::endl;
      File_InPut[i_FHC][i_TB] = TFile::Open(inputfile.c_str());
      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
        {
	      string HistName;
	      HistName = Form("h_mSimpleClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,i_match);
	      h_mSensorSimpleClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
          if(h_mSensorSimpleClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match] == NULL){
            std::cout << "Simple Cluster R is NULL" << std::endl;
          }
	      HistName = Form("h_mSimpleClustersTrackFstEffPhi_2Layer_Sensor%d_SF%d",i_sensor,i_match);
	      h_mSensorSimpleClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
	      if(h_mSensorSimpleClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match] == NULL){
            std::cout << "Simple Cluster Phi is NULL" << std::endl;
          }
	      HistName = Form("h_mScanClustersTrackFstEffR_2Layer_Sensor%d_SF%d",i_sensor,i_match);
	      h_mSensorScanClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
          if(h_mSensorScanClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match] == NULL){
            std::cout << "Scan Cluster R is NULL" << std::endl;
          }
	      HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_Sensor%d_SF%d",i_sensor,i_match);
	      h_mSensorScanClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
	      if(h_mSensorScanClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match] == NULL){
            std::cout << "Scan Cluster Phi is NULL" << std::endl;
          }
	    }
      }
    }
  }

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  //________________________________PLOTTING R EFFICIENCIES________________________________//

  TH1F *h_play = new TH1F("h_play","h_play",400,-9.5,390.5);
  for(int i_bin = 0; i_bin < 400; ++i_bin)
  {
    h_play->SetBinContent(i_bin+1,-10.0);
    h_play->SetBinError(i_bin+1,1.0);
  }
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("R (mm)");
  h_play->GetXaxis()->SetTitleSize(0.06);
  h_play->GetXaxis()->SetNdivisions(505);
  h_play->GetXaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetTitle("Efficiency");
  h_play->GetYaxis()->SetNdivisions(505);
  h_play->GetYaxis()->SetTitleSize(0.06);
  h_play->GetYaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetRangeUser(0.0,1.1);

  const int markerColor[4] = {2, 4, 6,kGray+2};
  const int markerStyle[4] = {24, 24, 24, 20};
  TCanvas *c_EfficiencyR = new TCanvas("c_EfficiencyR","c_EfficiencyR",10,10,400*nFHC,400*nTB);
  c_EfficiencyR->Divide(nFHC,nTB);
  for(int i_pad = 0; i_pad < nFHC*nTB; ++i_pad)
  {
    c_EfficiencyR->cd(i_pad+1)->SetLeftMargin(0.15);
    c_EfficiencyR->cd(i_pad+1)->SetBottomMargin(0.15);
    c_EfficiencyR->cd(i_pad+1)->SetTicks(1,1);
    c_EfficiencyR->cd(i_pad+1)->SetGrid(0,0);
  }

  string outputname = Form("./figures/EfficiencyRThTb%s_Ped%1.1fPed%1.1f.pdf",hv.c_str(),nFstThresholdCut2,nFstThresholdCut1);
  string output_start = Form("%s[",outputname.c_str());
  c_EfficiencyR->Print(output_start.c_str());

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
      {
        for(int i_TB = 0; i_TB < nTB; ++i_TB)
        {
          c_EfficiencyR->cd(i_TB*nFHC+i_FHC+1)->Clear();
          c_EfficiencyR->cd(i_TB*nFHC+i_FHC+1);
          string title = Form("Sensor %d & Match %d Th %1.1f Tb %d",i_sensor,i_match,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);
          h_play->SetTitle(title.c_str());
          h_play->GetXaxis()->SetRangeUser(FST::rMin[i_sensor],FST::rMax[i_sensor]);
          h_play->DrawCopy("hE");
          PlotLine(FST::mFstRMin[i_sensor], FST::mFstRMin[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::mFstRMax[i_sensor], FST::mFstRMax[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::rMin[i_sensor], FST::rMax[i_sensor], 0.90, 0.90, 4, 1, 2);
          PlotLine(FST::rMin[i_sensor], FST::rMax[i_sensor], 0.95, 0.95, 4, 1, 2);
          //for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
          //{
          //  h_mSensorScanClustersTrackFstEffR_2Layer[i_hv][i_sensor][0]->SetMarkerStyle(markerStyle[i_hv]);
          //  h_mSensorScanClustersTrackFstEffR_2Layer[i_hv][i_sensor][0]->SetMarkerColor(markerColor[i_hv]);
          //  h_mSensorScanClustersTrackFstEffR_2Layer[i_hv][i_sensor][0]->SetMarkerSize(0.8);
          //  h_mSensorScanClustersTrackFstEffR_2Layer[i_hv][i_sensor][0]->SetLineColor(markerColor[i_hv]);
          //  h_mSensorScanClustersTrackFstEffR_2Layer[i_hv][i_sensor][0]->Draw("PE same");
          //}
          h_mSensorScanClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match]->SetMarkerColor(2);
          h_mSensorScanClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match]->Draw("PE same");
          h_mSensorSimpleClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match]->SetMarkerColor(1);
          h_mSensorSimpleClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match]->Draw("PE same");
          if(i_match == 0 && i_TB == 0 && i_FHC == 0)
          {
            TLegend *leg = new TLegend(0.3,0.2,0.7,0.4);
	        leg->SetBorderSize(0);
	        leg->SetFillColor(10);
	        leg->AddEntry(h_mSensorSimpleClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match],"Simple Cluster","L");
	        leg->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer[i_FHC][i_TB][i_sensor][i_match],"Scan Cluster","L");
	        leg->Draw("same");
          }
          //TLegend *leg = new TLegend(0.45,0.25,0.75,0.55);
          //leg->SetBorderSize(0);
          //leg->SetFillColor(0);
          //leg->SetMargin(0.1);
          //leg->SetNColumns(2); commented out originally
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer[0][i_sensor][0],hv[0].c_str(),"P");
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer[1][i_sensor][0],hv[1].c_str(),"P");
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer[2][i_sensor][0],hv[2].c_str(),"P");
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffR_2Layer[3][i_sensor][0],hv[3].c_str(),"P");
          //leg->Draw("same");
        }
      }
      c_EfficiencyR->Update();
      c_EfficiencyR->Print(outputname.c_str());
    }
  }
  //c_EfficiencyR->SaveAs("./figures/c_EfficiencyRThTbScan.eps");
  
  string output_stop = Form("%s]",outputname.c_str());
  c_EfficiencyR->Print(output_stop.c_str()); // close pdf file

  //________________________________PLOTTING R EFFICIENCIES________________________________//
  
  //________________________________PLOTTING PHI EFFICIENCIES________________________________//

  TH1F *h_playPhi = new TH1F("h_playPhi","h_playPhi",100,-0.5,0.5);
  for(int i_bin = 0; i_bin < 100; ++i_bin)
  {
    h_playPhi->SetBinContent(i_bin+1,-10.0);
    h_playPhi->SetBinError(i_bin+1,1.0);
  }
  h_playPhi->SetStats(0);
  h_playPhi->GetXaxis()->SetTitle("#phi (rad)");
  h_playPhi->GetXaxis()->SetTitleSize(0.06);
  h_playPhi->GetXaxis()->SetNdivisions(505);
  h_playPhi->GetXaxis()->SetLabelSize(0.04);
  h_playPhi->GetYaxis()->SetTitle("Efficiency");
  h_playPhi->GetYaxis()->SetNdivisions(505);
  h_playPhi->GetYaxis()->SetTitleSize(0.06);
  h_playPhi->GetYaxis()->SetLabelSize(0.04);
  h_playPhi->GetYaxis()->SetRangeUser(0.0,1.1);

  TCanvas *c_EfficiencyPhi = new TCanvas("c_EfficiencyPhi","c_EfficiencyPhi",10,10,400*nFHC,400*nTB);
  c_EfficiencyPhi->Divide(nFHC,nTB);
  for(int i_pad = 0; i_pad < nFHC*nTB; ++i_pad)
  {
    c_EfficiencyPhi->cd(i_pad+1)->SetLeftMargin(0.15);
    c_EfficiencyPhi->cd(i_pad+1)->SetBottomMargin(0.15);
    c_EfficiencyPhi->cd(i_pad+1)->SetTicks(1,1);
    c_EfficiencyPhi->cd(i_pad+1)->SetGrid(0,0);
  }

  outputname = Form("./figures/EfficiencyPhiThTb%s_Ped%1.1fPed%1.1f.pdf",hv.c_str(),nFstThresholdCut2,nFstThresholdCut1);
  output_start = Form("%s[",outputname.c_str());
  c_EfficiencyPhi->Print(output_start.c_str());

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
      {
        for(int i_TB = 0; i_TB < nTB; ++i_TB)
        {
          c_EfficiencyPhi->cd(i_TB*nFHC+i_FHC+1)->Clear();
          c_EfficiencyPhi->cd(i_TB*nFHC+i_FHC+1);
          string title = Form("Sensor %d & Match %d Th %1.1f Tb %d",i_sensor,i_match,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);
          h_playPhi->SetTitle(title.c_str());
          h_playPhi->GetXaxis()->SetRangeUser(FST::phiMin[i_sensor],FST::phiMax[i_sensor]);
          h_playPhi->DrawCopy("hE");
          PlotLine(FST::mFstPhiMin[i_sensor], FST::mFstPhiMin[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::mFstPhiMax[i_sensor], FST::mFstPhiMax[i_sensor], 0.0, 1.05, 1, 2, 2);
          PlotLine(FST::phiMin[i_sensor], FST::phiMax[i_sensor], 0.90, 0.90, 4, 1, 2);
          PlotLine(FST::phiMin[i_sensor], FST::phiMax[i_sensor], 0.95, 0.95, 4, 1, 2);
          //for(int i_hv = 0; i_hv < numOfHV; ++i_hv)
          //{
          //  h_mSensorScanClustersTrackFstEffPhi_2Layer[i_hv][i_sensor][0]->SetMarkerStyle(markerStyle[i_hv]);
          //  h_mSensorScanClustersTrackFstEffPhi_2Layer[i_hv][i_sensor][0]->SetMarkerColor(markerColor[i_hv]);
          //  h_mSensorScanClustersTrackFstEffPhi_2Layer[i_hv][i_sensor][0]->SetMarkerSize(0.8);
          //  h_mSensorScanClustersTrackFstEffPhi_2Layer[i_hv][i_sensor][0]->SetLineColor(markerColor[i_hv]);
          //  h_mSensorScanClustersTrackFstEffPhi_2Layer[i_hv][i_sensor][0]->Draw("PE same");
          //}
          h_mSensorScanClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match]->SetMarkerColor(2);
          h_mSensorScanClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match]->Draw("PE same");
          h_mSensorSimpleClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match]->SetMarkerColor(1);
          h_mSensorSimpleClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match]->Draw("PE same");
          if(i_match == 0 && i_TB == 0 && i_FHC == 0)
          {
            TLegend *leg = new TLegend(0.3,0.2,0.7,0.4);
	        leg->SetBorderSize(0);
	        leg->SetFillColor(10);
	        leg->AddEntry(h_mSensorSimpleClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match],"Simple Cluster","L");
	        leg->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[i_FHC][i_TB][i_sensor][i_match],"Scan Cluster","L");
	        leg->Draw("same");
          }
          //TLegend *leg = new TLegend(0.45,0.25,0.75,0.55);
          //leg->SetBorderSize(0);
          //leg->SetFillColor(0);
          //leg->SetMargin(0.1);
          //leg->SetNColumns(2);
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[0][i_sensor][0],hv[0].c_str(),"P");
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[1][i_sensor][0],hv[1].c_str(),"P");
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[2][i_sensor][0],hv[2].c_str(),"P");
          //leg->AddEntry(h_mSensorScanClustersTrackFstEffPhi_2Layer[3][i_sensor][0],hv[3].c_str(),"P");
          //leg->Draw("same");
        }
      }
      c_EfficiencyPhi->Update();
      c_EfficiencyPhi->Print(outputname.c_str());
    }
  }
  //c_EfficiencyPhi->SaveAs("./figures/c_EfficiencyPhiThTbScan.eps");
  
  output_stop = Form("%s]",outputname.c_str());
  c_EfficiencyR->Print(output_stop.c_str()); // close pdf file
  
  //________________________________PLOTTING PHI EFFICIENCIES________________________________//
 
}
