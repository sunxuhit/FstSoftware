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

void plotSignalQA(string hv = "HV140V", string config = "Th4o5Tb3")
{
  string inputfile = Form("/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstQAStudy_%s_withPed_%s.root",hv.c_str(),config.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TProfile2D *p_mPedMap_FST = (TProfile2D*)File_InPut->Get("p_mPedMap_FST");;
  TProfile2D *p_mSigMap_FST = (TProfile2D*)File_InPut->Get("p_mSigMap_FST");
  TH1F *h_mSignalHits_FST = (TH1F*)File_InPut->Get("h_mSignalHits_FST");
  TH1F *h_mNoiseHits_FST = (TH1F*)File_InPut->Get("h_mNoiseHits_FST");
  TH1F *h_mSNRatioHits_FST = (TH1F*)File_InPut->Get("h_mSNRatioHits_FST");
  TH1F *h_mSignalClusters_FST = (TH1F*)File_InPut->Get("h_mSignalClusters_FST");

  TH1F *h_mSignalHits_Rstrip[4];
  TH1F *h_mNoiseHits_Rstrip[4];
  TH1F *h_mSNRatioHits_Rstrip[4];
  TH1F *h_mSignalClusters_Rstrip[4];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    string HistName;
    HistName = Form("h_mSignalHits_Rstrip%d",i_rstrip);
    h_mSignalHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mNoiseHits_Rstrip%d",i_rstrip);
    h_mNoiseHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSNRatioHits_Rstrip%d",i_rstrip);
    h_mSNRatioHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mSignalClusters_Rstrip%d",i_rstrip);
    h_mSignalClusters_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
  }

  string outputname = "./figures/SignalQA.pdf";

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,800,1200);
  c_Signal->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_Signal->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Signal->cd(i_pad+1)->SetRightMargin(0.15);
    c_Signal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Signal->cd(i_pad+1)->SetTicks(1,1);
    c_Signal->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = "./figures/SignalQA.pdf[";
  c_Signal->Print(output_start.c_str()); // open pdf file

  c_Signal->cd(1);
  p_mPedMap_FST->SetTitle("Pedestal");
  p_mPedMap_FST->SetStats(0);
  p_mPedMap_FST->GetXaxis()->SetTitle("R");
  p_mPedMap_FST->GetYaxis()->SetTitle("#phi");
  p_mPedMap_FST->GetXaxis()->SetTitleSize(0.06);
  p_mPedMap_FST->GetYaxis()->SetTitleSize(0.06);
  p_mPedMap_FST->Draw("colz");

  c_Signal->cd(2);
  p_mSigMap_FST->SetTitle("Signal");
  p_mSigMap_FST->SetStats(0);
  p_mSigMap_FST->GetXaxis()->SetTitle("R");
  p_mSigMap_FST->GetYaxis()->SetTitle("#phi");
  p_mSigMap_FST->GetXaxis()->SetTitleSize(0.06);
  p_mSigMap_FST->GetYaxis()->SetTitleSize(0.06);
  p_mSigMap_FST->Draw("colz");

  c_Signal->cd(3);
  h_mSignalHits_FST->SetTitle("Signal of Hits");
  h_mSignalHits_FST->GetXaxis()->SetTitle("ADC");
  h_mSignalHits_FST->GetXaxis()->SetTitleSize(0.06);
  h_mSignalHits_FST->Draw("HIST");

  c_Signal->cd(4);
  h_mSignalClusters_FST->SetTitle("Signal of Clusters");
  h_mSignalClusters_FST->GetXaxis()->SetTitle("ADC");
  h_mSignalClusters_FST->GetXaxis()->SetTitleSize(0.06);
  h_mSignalClusters_FST->Draw("HIST");

  c_Signal->cd(5);
  h_mNoiseHits_FST->SetTitle("Noise");
  h_mNoiseHits_FST->GetXaxis()->SetTitle("ADC");
  h_mNoiseHits_FST->GetXaxis()->SetTitleSize(0.06);
  h_mNoiseHits_FST->Draw("HIST");

  c_Signal->cd(6);
  h_mSNRatioHits_FST->SetTitle("Signal/Noise of Hits");
  h_mSNRatioHits_FST->GetXaxis()->SetTitle("Signal/Noise");
  h_mSNRatioHits_FST->GetXaxis()->SetTitleSize(0.06);
  h_mSNRatioHits_FST->Draw("HIST");

  c_Signal->Update();
  c_Signal->Print(outputname.c_str());

  TCanvas *c_Adc = new TCanvas("c_Adc","c_Adc",10,10,800,800);
  c_Adc->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_Adc->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Adc->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Adc->cd(i_pad+1)->SetTicks(1,1);
    c_Adc->cd(i_pad+1)->SetGrid(0,0);
  }

  c_Adc->cd(1);
  {
    TF1 *f_landau_FST[4];
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mSignalHits_Rstrip[i_rstrip]->SetStats(0);
      h_mSignalHits_Rstrip[i_rstrip]->SetTitle("FST Hits");
      h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mSignalHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      string FuncName = Form("f_landau_FST_%d",i_rstrip);
      f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
      f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
      h_mSignalHits_Rstrip[i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
      f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
      f_landau_FST[i_rstrip]->SetLineStyle(2);
      f_landau_FST[i_rstrip]->SetLineWidth(2);

      if(i_rstrip == 0) h_mSignalHits_Rstrip[i_rstrip]->Draw();
      else h_mSignalHits_Rstrip[i_rstrip]->Draw("same");
      // f_landau_FST[i_rstrip]->Draw("l same");

      // string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mSignalHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mSignalHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Adc->cd(2);
  {
    TF1 *f_landau_FST[4];
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mSignalClusters_Rstrip[i_rstrip]->SetStats(0);
      h_mSignalClusters_Rstrip[i_rstrip]->SetTitle("FST Clusters");
      h_mSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mSignalClusters_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mSignalClusters_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      string FuncName = Form("f_landau_FST_%d",i_rstrip);
      f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
      f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
      h_mSignalClusters_Rstrip[i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
      f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
      f_landau_FST[i_rstrip]->SetLineStyle(2);
      f_landau_FST[i_rstrip]->SetLineWidth(2);

      if(i_rstrip == 0) h_mSignalClusters_Rstrip[i_rstrip]->Draw();
      else h_mSignalClusters_Rstrip[i_rstrip]->Draw("same");
      // f_landau_FST[i_rstrip]->Draw("l same");

      // string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mSignalClusters_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mSignalClusters_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Adc->cd(3);
  {
    TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mNoiseHits_Rstrip[i_rstrip]->SetStats(0);
      h_mNoiseHits_Rstrip[i_rstrip]->SetTitle("FST Noise");
      h_mNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
      h_mNoiseHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mNoiseHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mNoiseHits_Rstrip[i_rstrip]->Draw();
      else h_mNoiseHits_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mNoiseHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mNoiseHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Adc->cd(4);
  {
    TLegend *leg_FST = new TLegend(0.50,0.6,0.90,0.8);
    leg_FST->SetBorderSize(0);
    leg_FST->SetFillColor(10);
    for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
    {
      h_mSNRatioHits_Rstrip[i_rstrip]->SetStats(0);
      h_mSNRatioHits_Rstrip[i_rstrip]->SetTitle("FST Signal/Noise");
      h_mSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("Signal/Noise");
      h_mSNRatioHits_Rstrip[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
      h_mSNRatioHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);

      if(i_rstrip == 0) h_mSNRatioHits_Rstrip[i_rstrip]->Draw();
      else h_mSNRatioHits_Rstrip[i_rstrip]->Draw("same");

      string LegName = Form("R_strip %d: Mean %1.2f",i_rstrip,h_mSNRatioHits_Rstrip[i_rstrip]->GetMean());
      leg_FST->AddEntry(h_mSNRatioHits_Rstrip[i_rstrip],LegName.c_str(),"L");
    }
    leg_FST->Draw("same");
  }

  c_Adc->Update();
  c_Adc->Print(outputname.c_str());

  // c_Signal->SaveAs("./figures/SignalQA.pdf");
  string output_stop = "./figures/SignalQA.pdf]";
  c_Signal->Print(output_stop.c_str()); // open pdf file
}

