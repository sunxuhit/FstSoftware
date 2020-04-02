#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

using namespace std;

void plotResidual()
{
  // gStyle->SetOptStat(1101);
  gStyle->SetOptStat(11111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.15); gStyle->SetStatH(0.25);

  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstTracking_HV140.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mXResidual_Before = (TH1F*)File_InPut->Get("h_mXResidual_Hits_Before");
  TH1F *h_mYResidual_Before = (TH1F*)File_InPut->Get("h_mYResidual_Hits_Before");
  TH1F *h_mXResidual = (TH1F*)File_InPut->Get("h_mXResidual_Hits");
  TH1F *h_mYResidual = (TH1F*)File_InPut->Get("h_mYResidual_Hits");
  TH1F *h_mRResidual = (TH1F*)File_InPut->Get("h_mRResidual_Hits");
  TH1F *h_mPhiResidual = (TH1F*)File_InPut->Get("h_mPhiResidual_Hits");

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,1200);
  c_play->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  c_play->cd(1);
  h_mXResidual_Before->SetTitle("x-residual before correction");
  h_mXResidual_Before->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual_Before->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual_Before->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual_Before->Draw();
  h_mXResidual_Before->Fit("gaus");

  c_play->cd(2);
  h_mYResidual_Before->SetTitle("y-residual before correction");
  h_mYResidual_Before->GetXaxis()->SetTitle("y-residual (mm)");
  h_mYResidual_Before->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual_Before->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual_Before->Draw();
  h_mYResidual_Before->Fit("gaus");

  c_play->cd(3);
  h_mXResidual->SetTitle("x-residual after correction");
  h_mXResidual->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual->Draw();
  h_mXResidual->Fit("gaus");

  c_play->cd(4);
  h_mYResidual->SetTitle("y-residual after correction");
  h_mYResidual->GetXaxis()->SetTitle("y-residual (mm)");
  h_mYResidual->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual->Draw();
  h_mYResidual->Fit("gaus");

  c_play->cd(5);
  h_mRResidual->SetTitle("r-residual after correction");
  h_mRResidual->GetXaxis()->SetTitle("r-residual (mm)");
  h_mRResidual->GetXaxis()->SetTitleSize(0.06);
  h_mRResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mRResidual->GetYaxis()->SetTitleSize(0.06);
  h_mRResidual->Draw();
  h_mRResidual->Fit("gaus");

  c_play->cd(6);
  h_mPhiResidual->SetTitle("#phi-residual after correction");
  h_mPhiResidual->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mPhiResidual->GetXaxis()->SetTitleSize(0.06);
  h_mPhiResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mPhiResidual->GetYaxis()->SetTitleSize(0.06);
  h_mPhiResidual->Draw();
  h_mPhiResidual->Fit("gaus");

  // c_play->SaveAs("TrackRes_HV140.eps");
}
