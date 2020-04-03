#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

using namespace std;

void calTrackClusterEfficiency()
{
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstTracking_HV140.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mTrackClusters_IST = (TH2F*)File_InPut->Get("h_mTrackClusters_IST");
  h_mTrackClusters_IST->Sumw2();
  TH1F *h_mR_IST = (TH1F*)h_mTrackClusters_IST->ProjectionX("h_mR_IST");
  TH1F *h_mPhi_IST = (TH1F*)h_mTrackClusters_IST->ProjectionY("h_mPhi_IST");

  TH2F *h_mTrackClusters_FST = (TH2F*)File_InPut->Get("h_mTrackClusters_FST");
  h_mTrackClusters_FST->Sumw2();
  TH1F *h_mR_FST = (TH1F*)h_mTrackClusters_FST->ProjectionX("h_mR_FST");
  TH1F *h_mPhi_FST = (TH1F*)h_mTrackClusters_FST->ProjectionY("h_mPhi_FST");

  TH2F *h_mEfficiency = (TH2F*)h_mTrackClusters_IST->Clone("h_mEfficiency");
  h_mEfficiency->SetTitle("h_mEfficiency");
  h_mEfficiency->Reset();
  h_mEfficiency->Divide(h_mTrackClusters_FST,h_mTrackClusters_IST,1,1,"B");

  TH1F *h_mEffR = (TH1F*)h_mR_IST->Clone("h_mEffR");
  h_mEffR->SetTitle("h_mEffR");
  h_mEffR->Reset();
  h_mEffR->Divide(h_mR_FST,h_mR_IST,1,1,"B");

  TH1F *h_mEffPhi = (TH1F*)h_mPhi_IST->Clone("h_mEffPhi");
  h_mEffPhi->SetTitle("h_mEffPhi");
  h_mEffPhi->Reset();
  h_mEffPhi->Divide(h_mPhi_FST,h_mPhi_IST,1,1,"B");

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
  h_mTrackClusters_IST->GetXaxis()->SetTitle("r_{proj} (mm)");
  h_mTrackClusters_IST->GetYaxis()->SetTitle("phi_{proj} (rad)");
  h_mTrackClusters_IST->Draw("colz");

  c_play->cd(2);
  h_mR_IST->GetXaxis()->SetTitle("r_{proj} (mm)");
  h_mR_IST->Draw("HIST");

  c_play->cd(3);
  h_mPhi_IST->GetXaxis()->SetTitle("phi_{proj} (rad)");
  h_mPhi_IST->Draw("HIST");

  // measured position from FST
  c_play->cd(4);
  h_mTrackClusters_FST->GetXaxis()->SetTitle("r_{proj} (mm)");
  h_mTrackClusters_FST->GetYaxis()->SetTitle("phi_{proj} (rad)");
  h_mTrackClusters_FST->Draw("colz");

  c_play->cd(5);
  h_mR_FST->GetXaxis()->SetTitle("r_{proj} (mm)");
  h_mR_FST->Draw("HIST");

  c_play->cd(6);
  h_mPhi_FST->GetXaxis()->SetTitle("phi_{proj} (rad)");
  h_mPhi_FST->Draw("HIST");

  c_play->cd(7);
  h_mEfficiency->GetXaxis()->SetTitle("r_{proj} (mm)");
  h_mEfficiency->GetYaxis()->SetTitle("phi_{proj} (rad)");
  h_mEfficiency->Draw("colz");

  c_play->cd(8);
  h_mEffR->GetXaxis()->SetTitle("r_{proj} (mm)");
  h_mEffR->Draw("pE");

  c_play->cd(9);
  h_mEffPhi->GetXaxis()->SetTitle("phi_{proj} (rad)");
  h_mEffPhi->Draw("pE");

  c_play->SaveAs("Efficiency_TrackCluster_HV140V.eps");
}
