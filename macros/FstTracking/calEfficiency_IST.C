#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

using namespace std;

void calEfficiency_IST()
{
  string inputfile = "./FstData_HV140.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mHits_IST13 = (TH2F*)File_InPut->Get("h_mHits_IST13");
  h_mHits_IST13->Sumw2();
  TH1F *h_mX_IST13 = (TH1F*)h_mHits_IST13->ProjectionX("h_mX_IST13");
  TH1F *h_mY_IST13 = (TH1F*)h_mHits_IST13->ProjectionY("h_mY_IST13");

  TH2F *h_mHits_IST2 = (TH2F*)File_InPut->Get("h_mHits_IST2");
  h_mHits_IST2->Sumw2();
  TH1F *h_mX_IST2 = (TH1F*)h_mHits_IST2->ProjectionX("h_mX_IST2");
  TH1F *h_mY_IST2 = (TH1F*)h_mHits_IST2->ProjectionY("h_mY_IST2");

  TH2F *h_mEfficiency = (TH2F*)h_mHits_IST13->Clone("h_mEfficiency");
  h_mEfficiency->SetTitle("h_mEfficiency");
  h_mEfficiency->Reset();
  h_mEfficiency->Divide(h_mHits_IST2,h_mHits_IST13,1,1,"B");

  TH1F *h_mEffX = (TH1F*)h_mX_IST13->Clone("h_mEffX");
  h_mEffX->SetTitle("h_mEffX");
  h_mEffX->Reset();
  h_mEffX->Divide(h_mX_IST2,h_mX_IST13,1,1,"B");

  TH1F *h_mEffY = (TH1F*)h_mY_IST13->Clone("h_mEffY");
  h_mEffY->SetTitle("h_mEffY");
  h_mEffY->Reset();
  h_mEffY->Divide(h_mY_IST2,h_mY_IST13,1,1,"B");

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,900,900);
  c_play->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  // projection position from IST13
  c_play->cd(1);
  h_mHits_IST13->SetStats(0);
  h_mHits_IST13->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mHits_IST13->GetYaxis()->SetTitle("y_{proj} (mm)");
  h_mHits_IST13->Draw("colz");

  c_play->cd(2);
  h_mX_IST13->SetStats(0);
  h_mX_IST13->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mX_IST13->Draw("HIST");

  c_play->cd(3);
  h_mY_IST13->SetStats(0);
  h_mY_IST13->GetXaxis()->SetTitle("y_{proj} (mm)");
  h_mY_IST13->Draw("HIST");

  // measured position from IST2
  c_play->cd(4);
  h_mHits_IST2->SetStats(0);
  h_mHits_IST2->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mHits_IST2->GetYaxis()->SetTitle("y_{proj} (mm)");
  h_mHits_IST2->Draw("colz");

  c_play->cd(5);
  h_mX_IST2->SetStats(0);
  h_mX_IST2->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mX_IST2->Draw("HIST");

  c_play->cd(6);
  h_mY_IST2->SetStats(0);
  h_mY_IST2->GetXaxis()->SetTitle("y_{proj} (mm)");
  h_mY_IST2->Draw("HIST");

  c_play->cd(7);
  h_mEfficiency->SetStats(0);
  h_mEfficiency->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mEfficiency->GetYaxis()->SetTitle("y_{proj} (mm)");
  h_mEfficiency->Draw("colz");

  c_play->cd(8);
  h_mEffX->SetStats(0);
  h_mEffX->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mEffX->Draw("pE");

  c_play->cd(9);
  h_mEffY->SetStats(0);
  h_mEffY->GetXaxis()->SetTitle("y_{proj} (mm)");
  h_mEffY->Draw("pE");

  c_play->SaveAs("Efficiency_HV140V_IST.eps");
}
