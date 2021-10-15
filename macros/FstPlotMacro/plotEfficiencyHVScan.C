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

void plotEfficiencyHVScan(string mod = "Mod03", int sensorId = 0)
{
  string hv[3]        = {"70V","100V","140V"};
  int MarkerStyle[3]  = {24,25,29};
  int MarkerColor[3]  = {kGray+2,kBlue,kRed};
  float MarkerSize[3] = {1.4,1.4,2.8};

  TFile *File_InPut[3];
  TH1F *h_mEfficiencyR[3][FST::mFstNumMatching];
  TH1F *h_mEfficiencyPhi[3][FST::mFstNumMatching];

  for(int i_hv = 0; i_hv < 3; ++i_hv)
  {
    string inputfile = Form("/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/output/signal/%s/FstEfficiency_%s_HV%s_Th4.0Tb2Ped2.5Ped3.5_withPed_withCMNCorr.root",mod.c_str(),mod.c_str(),hv[i_hv].c_str());
    File_InPut[i_hv] = TFile::Open(inputfile.c_str());
    for(int i_match = 0; i_match < FST::mFstNumMatching; ++i_match)
    {
      string HistName = Form("h_mScanClustersTrackFstEffR_2Layer_Sensor%d_SF%d",sensorId,i_match);
      h_mEfficiencyR[i_hv][i_match] = (TH1F*)File_InPut[i_hv]->Get(HistName.c_str());
      HistName = Form("h_mScanClustersTrackFstEffPhi_2Layer_Sensor%d_SF%d",sensorId,i_match);
      h_mEfficiencyPhi[i_hv][i_match] = (TH1F*)File_InPut[i_hv]->Get(HistName.c_str());
    }
  }

  TCanvas *c_EfficiencyR = new TCanvas("c_EfficiencyR","c_EfficiencyR",10,10,800,800);
  c_EfficiencyR->cd()->SetLeftMargin(0.15);
  c_EfficiencyR->cd()->SetBottomMargin(0.15);
  c_EfficiencyR->cd()->SetTicks(1,1);
  c_EfficiencyR->cd()->SetGrid(0,0);

  TLegend *legR = new TLegend(0.30,0.25,0.50,0.45);
  legR->SetBorderSize(0);
  legR->SetFillColor(10);
  // legR->SetNColumns(3);

  for(int i_hv = 0; i_hv < 3; ++i_hv)
  {
    h_mEfficiencyR[i_hv][0]->SetTitle("Efficiency vs. R");
    h_mEfficiencyR[i_hv][0]->SetStats(0);
    h_mEfficiencyR[i_hv][0]->GetXaxis()->SetTitle("r (mm)");
    h_mEfficiencyR[i_hv][0]->GetXaxis()->SetTitleSize(0.06);
    h_mEfficiencyR[i_hv][0]->GetYaxis()->SetTitle("Efficiency");
    h_mEfficiencyR[i_hv][0]->GetYaxis()->SetTitleSize(0.06);
    h_mEfficiencyR[i_hv][0]->SetMarkerStyle(MarkerStyle[i_hv]);
    h_mEfficiencyR[i_hv][0]->SetMarkerColor(MarkerColor[i_hv]);
    h_mEfficiencyR[i_hv][0]->SetMarkerSize(MarkerSize[i_hv]);
    h_mEfficiencyR[i_hv][0]->SetLineColor(MarkerColor[i_hv]);
    if(i_hv == 0) 
    {
      h_mEfficiencyR[i_hv][0]->Draw("pEX0");
      PlotLine(FST::mFstRMin[sensorId], FST::mFstRMin[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::mFstRMax[sensorId], FST::mFstRMax[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::rMin[sensorId], FST::rMax[sensorId], 0.90, 0.90, 4, 1, 2);
      PlotLine(FST::rMin[sensorId], FST::rMax[sensorId], 0.95, 0.95, 4, 1, 2);
    }
    else  h_mEfficiencyR[i_hv][0]->Draw("pEX0 same");
    legR->AddEntry(h_mEfficiencyR[i_hv][0],hv[i_hv].c_str(),"p");
  }
  legR->Draw("same");

  string FigNameR = Form("./figures/%s/c_EfficiencyR_%s_Sensor%d.eps",mod.c_str(),mod.c_str(),sensorId);
  c_EfficiencyR->SaveAs(FigNameR.c_str());

  TCanvas *c_EfficiencyPhi = new TCanvas("c_EfficiencyPhi","c_EfficiencyPhi",10,10,800,800);
  c_EfficiencyPhi->cd()->SetLeftMargin(0.15);
  c_EfficiencyPhi->cd()->SetBottomMargin(0.15);
  c_EfficiencyPhi->cd()->SetTicks(1,1);
  c_EfficiencyPhi->cd()->SetGrid(0,0);

  TLegend *legPhi = new TLegend(0.30,0.25,0.50,0.45);
  legPhi->SetBorderSize(0);
  legPhi->SetFillColor(10);
  // legPhi->SetNColumns(3);

  for(int i_hv = 0; i_hv < 3; ++i_hv)
  {
    h_mEfficiencyPhi[i_hv][0]->SetTitle("Efficiency vs. #phi");
    h_mEfficiencyPhi[i_hv][0]->SetStats(0);
    h_mEfficiencyPhi[i_hv][0]->GetXaxis()->SetTitle("#phi (rad)");
    h_mEfficiencyPhi[i_hv][0]->GetXaxis()->SetTitleSize(0.06);
    h_mEfficiencyPhi[i_hv][0]->GetYaxis()->SetTitle("Efficiency");
    h_mEfficiencyPhi[i_hv][0]->GetYaxis()->SetTitleSize(0.06);
    h_mEfficiencyPhi[i_hv][0]->SetMarkerStyle(MarkerStyle[i_hv]);
    h_mEfficiencyPhi[i_hv][0]->SetMarkerColor(MarkerColor[i_hv]);
    h_mEfficiencyPhi[i_hv][0]->SetMarkerSize(MarkerSize[i_hv]);
    h_mEfficiencyPhi[i_hv][0]->SetLineColor(MarkerColor[i_hv]);
    if(i_hv == 0) 
    {
      h_mEfficiencyPhi[i_hv][0]->Draw("pE");
      PlotLine(FST::mFstPhiMin[sensorId], FST::mFstPhiMin[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::mFstPhiMax[sensorId], FST::mFstPhiMax[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::phiMin[sensorId], FST::phiMax[sensorId], 0.90, 0.90, 4, 1, 2);
      PlotLine(FST::phiMin[sensorId], FST::phiMax[sensorId], 0.95, 0.95, 4, 1, 2);
    }
    else  h_mEfficiencyPhi[i_hv][0]->Draw("pE same");
    legPhi->AddEntry(h_mEfficiencyPhi[i_hv][0],hv[i_hv].c_str(),"p");
  }
  legPhi->Draw("same");

  string FigNamePhi = Form("./figures/%s/c_EfficiencyPhi_%s_Sensor%d.eps",mod.c_str(),mod.c_str(),sensorId);
  c_EfficiencyPhi->SaveAs(FigNamePhi.c_str());

  TCanvas *c_Efficiency = new TCanvas("c_Efficiency","c_Efficiency",10,10,800,400);
  c_Efficiency->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_Efficiency->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Efficiency->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Efficiency->cd(i_pad+1)->SetTicks(1,1);
    c_Efficiency->cd(i_pad+1)->SetGrid(0,0);
  }

  TLegend *leg = new TLegend(0.30,0.25,0.50,0.45);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);

  c_Efficiency->cd(1);
  for(int i_hv = 0; i_hv < 3; ++i_hv)
  {
    h_mEfficiencyR[i_hv][0]->SetTitle("Efficiency vs. R");
    h_mEfficiencyR[i_hv][0]->SetStats(0);
    h_mEfficiencyR[i_hv][0]->GetXaxis()->SetTitle("r (mm)");
    h_mEfficiencyR[i_hv][0]->GetXaxis()->SetTitleSize(0.06);
    h_mEfficiencyR[i_hv][0]->GetYaxis()->SetTitle("Efficiency");
    h_mEfficiencyR[i_hv][0]->GetYaxis()->SetTitleSize(0.06);
    h_mEfficiencyR[i_hv][0]->SetMarkerStyle(MarkerStyle[i_hv]);
    h_mEfficiencyR[i_hv][0]->SetMarkerColor(MarkerColor[i_hv]);
    h_mEfficiencyR[i_hv][0]->SetMarkerSize(MarkerSize[i_hv]*0.5);
    h_mEfficiencyR[i_hv][0]->SetLineColor(MarkerColor[i_hv]);
    if(i_hv == 0) 
    {
      h_mEfficiencyR[i_hv][0]->Draw("pEX0");
      PlotLine(FST::mFstRMin[sensorId], FST::mFstRMin[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::mFstRMax[sensorId], FST::mFstRMax[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::rMin[sensorId], FST::rMax[sensorId], 0.90, 0.90, 4, 1, 2);
      PlotLine(FST::rMin[sensorId], FST::rMax[sensorId], 0.95, 0.95, 4, 1, 2);
    }
    else  h_mEfficiencyR[i_hv][0]->Draw("pEX0 same");
    leg->AddEntry(h_mEfficiencyR[i_hv][0],hv[i_hv].c_str(),"p");
  }
  leg->Draw("same");

  c_Efficiency->cd(2);
  for(int i_hv = 0; i_hv < 3; ++i_hv)
  {
    h_mEfficiencyPhi[i_hv][0]->SetTitle("Efficiency vs. #phi");
    h_mEfficiencyPhi[i_hv][0]->SetStats(0);
    h_mEfficiencyPhi[i_hv][0]->GetXaxis()->SetTitle("#phi (rad)");
    h_mEfficiencyPhi[i_hv][0]->GetXaxis()->SetTitleSize(0.06);
    h_mEfficiencyPhi[i_hv][0]->GetYaxis()->SetTitle("Efficiency");
    h_mEfficiencyPhi[i_hv][0]->GetYaxis()->SetTitleSize(0.06);
    h_mEfficiencyPhi[i_hv][0]->SetMarkerStyle(MarkerStyle[i_hv]);
    h_mEfficiencyPhi[i_hv][0]->SetMarkerColor(MarkerColor[i_hv]);
    h_mEfficiencyPhi[i_hv][0]->SetMarkerSize(MarkerSize[i_hv]*0.5);
    h_mEfficiencyPhi[i_hv][0]->SetLineColor(MarkerColor[i_hv]);
    if(i_hv == 0) 
    {
      h_mEfficiencyPhi[i_hv][0]->Draw("pE");
      PlotLine(FST::mFstPhiMin[sensorId], FST::mFstPhiMin[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::mFstPhiMax[sensorId], FST::mFstPhiMax[sensorId], 0.0, 1.05, 1, 2, 2);
      PlotLine(FST::phiMin[sensorId], FST::phiMax[sensorId], 0.90, 0.90, 4, 1, 2);
      PlotLine(FST::phiMin[sensorId], FST::phiMax[sensorId], 0.95, 0.95, 4, 1, 2);
    }
    else  h_mEfficiencyPhi[i_hv][0]->Draw("pE same");
  }
  leg->Draw("same");

  string FigName = Form("./figures/%s/c_Efficiency_%s_Sensor%d.eps",mod.c_str(),mod.c_str(),sensorId);
  c_Efficiency->SaveAs(FigName.c_str());
}
