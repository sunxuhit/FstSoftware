#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile2D.h>

using namespace std;

void plotAdc_Hits()
{
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstQAStudy_HV140V_withPed_Th4o5Tb3.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mAdcIst_Hits[4];
  TH1F *h_mAdcFst_Hits[4];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    std::string HistName;
    HistName = Form("h_mAdcFst_Hits_RStrip%d",i_rstrip);
    h_mAdcFst_Hits[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName = Form("h_mAdcIst_Hits_Layer%d",i_rstrip);
    h_mAdcIst_Hits[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
  }

  TCanvas *c_AdcHits = new TCanvas("c_AdcHits","c_AdcHits",10,10,1600,800);
  c_AdcHits->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_AdcHits->cd(i_pad+1)->SetLeftMargin(0.15);
    c_AdcHits->cd(i_pad+1)->SetBottomMargin(0.15);
    c_AdcHits->cd(i_pad+1)->SetTicks(1,1);
    c_AdcHits->cd(i_pad+1)->SetGrid(0,0);
  }

  c_AdcHits->cd(1);
  TF1 *f_landau_IST[4];
  TLegend *leg_IST = new TLegend(0.4,0.6,0.85,0.8);
  leg_IST->SetBorderSize(0);
  leg_IST->SetFillColor(10);
  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    h_mAdcIst_Hits[i_layer]->SetStats(0);
    h_mAdcIst_Hits[i_layer]->SetTitle("FST+IST");
    h_mAdcIst_Hits[i_layer]->GetXaxis()->SetTitle("ADC");
    h_mAdcIst_Hits[i_layer]->GetXaxis()->SetTitleSize(0.06);
    h_mAdcIst_Hits[i_layer]->GetYaxis()->SetRangeUser(0.0,h_mAdcIst_Hits[1]->GetMaximum()*1.2);
    h_mAdcIst_Hits[i_layer]->SetLineColor(i_layer+1);

    string FuncName = Form("f_landau_IST_%d",i_layer);
    f_landau_IST[i_layer] = new TF1(FuncName.c_str(),"landau",0,2000);
    f_landau_IST[i_layer]->SetRange(300.0,1500.0);
    h_mAdcIst_Hits[i_layer]->Fit(f_landau_IST[i_layer],"NR");
    f_landau_IST[i_layer]->SetLineColor(i_layer+1);
    f_landau_IST[i_layer]->SetLineStyle(2);
    f_landau_IST[i_layer]->SetLineWidth(4);

    if(i_layer == 0) h_mAdcIst_Hits[i_layer]->Draw();
    else h_mAdcIst_Hits[i_layer]->Draw("same");
    f_landau_IST[i_layer]->Draw("l same");

    string LegName;
    if(i_layer == 0) LegName = Form("FST Layer %d: MPV %1.2f",i_layer,f_landau_IST[i_layer]->GetParameter(1));
    if(i_layer != 0) LegName = Form("IST Layer %d: MPV %1.2f",i_layer,f_landau_IST[i_layer]->GetParameter(1));
    leg_IST->AddEntry(h_mAdcIst_Hits[i_layer],LegName.c_str(),"L");
  }
  leg_IST->Draw("same");

  c_AdcHits->cd(2);
  TF1 *f_landau_FST[4];
  TLegend *leg_FST = new TLegend(0.4,0.6,0.85,0.8);
  leg_FST->SetBorderSize(0);
  leg_FST->SetFillColor(10);
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    h_mAdcFst_Hits[i_rstrip]->SetStats(0);
    h_mAdcFst_Hits[i_rstrip]->SetTitle("FST");
    h_mAdcFst_Hits[i_rstrip]->GetXaxis()->SetTitle("ADC");
    h_mAdcFst_Hits[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
    h_mAdcFst_Hits[i_rstrip]->SetLineColor(i_rstrip+1);

    string FuncName = Form("f_landau_FST_%d",i_rstrip);
    f_landau_FST[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
    f_landau_FST[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
    h_mAdcFst_Hits[i_rstrip]->Fit(f_landau_FST[i_rstrip],"NR");
    f_landau_FST[i_rstrip]->SetLineColor(i_rstrip+1);
    f_landau_FST[i_rstrip]->SetLineStyle(2);
    f_landau_FST[i_rstrip]->SetLineWidth(2);

    if(i_rstrip == 0) h_mAdcFst_Hits[i_rstrip]->Draw();
    else h_mAdcFst_Hits[i_rstrip]->Draw("same");
    f_landau_FST[i_rstrip]->Draw("l same");

    string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau_FST[i_rstrip]->GetParameter(1));
    leg_FST->AddEntry(h_mAdcFst_Hits[i_rstrip],LegName.c_str(),"L");

  }
  leg_FST->Draw("same");

  // c_AdcHits->SaveAs("./figures/AdcHits.eps");
}
