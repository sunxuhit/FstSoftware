#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

using namespace std;

void calEfficiency()
{
  string inputfile = "./FstData_HV140.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mHits_IST = (TH2F*)File_InPut->Get("h_mHits_IST");
  h_mHits_IST->Sumw2();
  TH1F *h_mX_IST = (TH1F*)h_mHits_IST->ProjectionX("h_mX_IST");
  TH1F *h_mY_IST = (TH1F*)h_mHits_IST->ProjectionY("h_mY_IST");

  TH2F *h_mHits_FST = (TH2F*)File_InPut->Get("h_mHits_FST");
  h_mHits_FST->Sumw2();
  TH1F *h_mX_FST = (TH1F*)h_mHits_FST->ProjectionX("h_mX_FST");
  TH1F *h_mY_FST = (TH1F*)h_mHits_FST->ProjectionY("h_mY_FST");

  TH2F *h_mEfficiency = (TH2F*)h_mHits_IST->Clone("h_mEfficiency");
  h_mEfficiency->SetTitle("h_mEfficiency");
  h_mEfficiency->Reset();
  h_mEfficiency->Divide(h_mHits_FST,h_mHits_IST,1,1,"B");

  TH1F *h_mEffX = (TH1F*)h_mX_IST->Clone("h_mEffX");
  h_mEffX->SetTitle("h_mEffX");
  h_mEffX->Reset();
  h_mEffX->Divide(h_mX_FST,h_mX_IST,1,1,"B");

  TH1F *h_mEffY = (TH1F*)h_mY_IST->Clone("h_mEffY");
  h_mEffY->SetTitle("h_mEffY");
  h_mEffY->Reset();
  h_mEffY->Divide(h_mY_FST,h_mY_IST,1,1,"B");

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,900,900);
  c_play->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  // projection position from IST
  c_play->cd(1);
  h_mHits_IST->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mHits_IST->GetYaxis()->SetTitle("y_{proj} (mm)");
  h_mHits_IST->Draw("colz");

  c_play->cd(2);
  h_mX_IST->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mX_IST->Draw("HIST");

  c_play->cd(3);
  h_mY_IST->GetXaxis()->SetTitle("y_{proj} (mm)");
  h_mY_IST->Draw("HIST");

  // measured position from FST
  c_play->cd(4);
  h_mHits_FST->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mHits_FST->GetYaxis()->SetTitle("y_{proj} (mm)");
  h_mHits_FST->Draw("colz");

  c_play->cd(5);
  h_mX_FST->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mX_FST->Draw("HIST");

  c_play->cd(6);
  h_mY_FST->GetXaxis()->SetTitle("y_{proj} (mm)");
  h_mY_FST->Draw("HIST");

  c_play->cd(7);
  h_mEfficiency->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mEfficiency->GetYaxis()->SetTitle("y_{proj} (mm)");
  h_mEfficiency->Draw("colz");

  c_play->cd(8);
  h_mEffX->GetXaxis()->SetTitle("x_{proj} (mm)");
  h_mEffX->Draw("pE");

  c_play->cd(9);
  h_mEffY->GetXaxis()->SetTitle("y_{proj} (mm)");
  h_mEffY->Draw("pE");

  c_play->SaveAs("Efficiency_HV140V.eps");
}
