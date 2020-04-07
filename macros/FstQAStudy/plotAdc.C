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

void plotAdc()
{
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mAdcFst_Hits[4];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    string HistName = Form("h_mAdcFst_Hits_RStrip%d",i_rstrip);
    h_mAdcFst_Hits[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
  }

  TCanvas *c_AdcRstrip = new TCanvas("c_AdcRstrip","c_AdcRstrip",10,10,800,800);
  c_AdcRstrip->cd()->SetLeftMargin(0.15);
  c_AdcRstrip->cd()->SetBottomMargin(0.15);
  c_AdcRstrip->cd()->SetTicks(1,1);
  c_AdcRstrip->cd()->SetGrid(0,0);

  TF1 *f_landau[4];
  TLegend *leg = new TLegend(0.5,0.6,0.85,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    h_mAdcFst_Hits[i_rstrip]->SetStats(0);
    h_mAdcFst_Hits[i_rstrip]->GetXaxis()->SetTitle("ADC");
    h_mAdcFst_Hits[i_rstrip]->GetXaxis()->SetTitleSize(0.06);
    h_mAdcFst_Hits[i_rstrip]->SetLineColor(i_rstrip+1);

    string FuncName = Form("f_landau_%d",i_rstrip);
    f_landau[i_rstrip] = new TF1(FuncName.c_str(),"landau",0,2000);
    f_landau[i_rstrip]->SetRange(300.0+i_rstrip*20,1500.0);
    h_mAdcFst_Hits[i_rstrip]->Fit(f_landau[i_rstrip],"NR");
    f_landau[i_rstrip]->SetLineColor(i_rstrip+1);
    f_landau[i_rstrip]->SetLineStyle(2);
    f_landau[i_rstrip]->SetLineWidth(2);

    if(i_rstrip == 0) h_mAdcFst_Hits[i_rstrip]->Draw();
    else h_mAdcFst_Hits[i_rstrip]->Draw("same");
    f_landau[i_rstrip]->Draw("l same");

    string LegName = Form("R_strip %d: MPV %1.2f",i_rstrip,f_landau[i_rstrip]->GetParameter(1));
    leg->AddEntry(h_mAdcFst_Hits[i_rstrip],LegName.c_str(),"L");

  }
  leg->Draw("same");

  c_AdcRstrip->SaveAs("./figures/AdcRstrip.eps");

  TProfile2D *p_mAdcFst_Column = (TProfile2D*)File_InPut->Get("p_mAdcFst_Column");
  TProfile2D *p_mAdcFst_Row = (TProfile2D*)File_InPut->Get("p_mAdcFst_Row");
  TCanvas *c_AdcFst = new TCanvas("c_AdcFst","c_AdcFst",10,10,800,400);
  c_AdcFst->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_AdcFst->cd(i_pad+1)->SetLeftMargin(0.15);
    c_AdcFst->cd(i_pad+1)->SetBottomMargin(0.15);
    c_AdcFst->cd(i_pad+1)->SetTicks(1,1);
    c_AdcFst->cd(i_pad+1)->SetGrid(0,0);
  }

  c_AdcFst->cd(1);
  p_mAdcFst_Column->SetStats(0);
  p_mAdcFst_Column->GetXaxis()->SetRangeUser(4,8);
  p_mAdcFst_Column->GetXaxis()->SetTitle("<R> from Cluster");
  p_mAdcFst_Column->GetXaxis()->SetTitleSize(0.06);
  p_mAdcFst_Column->GetYaxis()->SetRangeUser(4,8);
  p_mAdcFst_Column->GetYaxis()->SetTitle("R from Hits");
  p_mAdcFst_Column->GetYaxis()->SetTitleSize(0.06);
  p_mAdcFst_Column->Draw("colz");

  c_AdcFst->cd(2);
  p_mAdcFst_Row->SetStats(0);
  p_mAdcFst_Row->GetXaxis()->SetRangeUser(0,64);
  p_mAdcFst_Row->GetXaxis()->SetTitle("<#phi> from Cluster");
  p_mAdcFst_Row->GetXaxis()->SetTitleSize(0.06);
  p_mAdcFst_Row->GetYaxis()->SetRangeUser(0,64);
  p_mAdcFst_Row->GetYaxis()->SetTitle("#phi from Hits");
  p_mAdcFst_Row->GetYaxis()->SetTitleSize(0.06);
  p_mAdcFst_Row->Draw("colz");
  c_AdcFst->SaveAs("./figures/AdcFst.eps");

  TProfile2D *p_mMaxTbFst_Column = (TProfile2D*)File_InPut->Get("p_mMaxTbFst_Column");
  TProfile2D *p_mMaxTbFst_Row = (TProfile2D*)File_InPut->Get("p_mMaxTbFst_Row");
  TH1F *h_mTbDiff_Clusters = (TH1F*)File_InPut->Get("h_mTbDiff_Clusters");
  TCanvas *c_MaxTbFst = new TCanvas("c_MaxTbFst","c_MaxTbFst",10,10,1200,400);
  c_MaxTbFst->Divide(3,1);
  for(int i_pad = 0; i_pad < 3; ++i_pad)
  {
    c_MaxTbFst->cd(i_pad+1)->SetLeftMargin(0.15);
    c_MaxTbFst->cd(i_pad+1)->SetBottomMargin(0.15);
    c_MaxTbFst->cd(i_pad+1)->SetTicks(1,1);
    c_MaxTbFst->cd(i_pad+1)->SetGrid(0,0);
  }

  c_MaxTbFst->cd(1);
  p_mMaxTbFst_Column->SetStats(0);
  p_mMaxTbFst_Column->GetXaxis()->SetRangeUser(4,8);
  p_mMaxTbFst_Column->GetXaxis()->SetTitle("<R> from Cluster");
  p_mMaxTbFst_Column->GetXaxis()->SetTitleSize(0.06);
  p_mMaxTbFst_Column->GetYaxis()->SetRangeUser(4,8);
  p_mMaxTbFst_Column->GetYaxis()->SetTitle("R from Hits");
  p_mMaxTbFst_Column->GetYaxis()->SetTitleSize(0.06);
  p_mMaxTbFst_Column->Draw("colz");

  c_MaxTbFst->cd(2);
  p_mMaxTbFst_Row->SetStats(0);
  p_mMaxTbFst_Row->GetXaxis()->SetRangeUser(0,64);
  p_mMaxTbFst_Row->GetXaxis()->SetTitle("<#phi> from Cluster");
  p_mMaxTbFst_Row->GetXaxis()->SetTitleSize(0.06);
  p_mMaxTbFst_Row->GetYaxis()->SetRangeUser(0,64);
  p_mMaxTbFst_Row->GetYaxis()->SetTitle("#phi from Hits");
  p_mMaxTbFst_Row->GetYaxis()->SetTitleSize(0.06);
  p_mMaxTbFst_Row->Draw("colz");

  c_MaxTbFst->cd(3);
  h_mTbDiff_Clusters->SetStats(0);
  h_mTbDiff_Clusters->GetXaxis()->SetTitle("Tb(maxAdc)-Tb(other hits)");
  h_mTbDiff_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mTbDiff_Clusters->Draw("h");
  c_MaxTbFst->SaveAs("./figures/MaxTbFst.eps");
}
