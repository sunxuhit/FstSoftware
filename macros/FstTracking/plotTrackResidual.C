#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

using namespace std;

void plotTrackResidual()
{
  // gStyle->SetOptStat(1101);
  gStyle->SetOptStat(11111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.15); gStyle->SetStatH(0.25);

  string inputfile = "../../output/FstTracking_HV140V_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mFstHitsTrackXRes = (TH1F*)File_InPut->Get("h_mFstHitsTrackXRes");
  TH1F *h_mFstHitsTrackYRes = (TH1F*)File_InPut->Get("h_mFstHitsTrackYRes");
  TH1F *h_mTrackRRes_Hits = (TH1F*)File_InPut->Get("h_mTrackRRes_Hits");
  TH1F *h_mTrackPhiRes_Hits = (TH1F*)File_InPut->Get("h_mTrackPhiRes_Hits");

  TH1F *h_mTrackXRes_Clusters = (TH1F*)File_InPut->Get("h_mTrackXRes_Clusters");
  TH1F *h_mTrackYRes_Clusters = (TH1F*)File_InPut->Get("h_mTrackYRes_Clusters");
  TH1F *h_mTrackRRes_Clusters = (TH1F*)File_InPut->Get("h_mTrackRRes_Clusters");
  TH1F *h_mTrackPhiRes_Clusters = (TH1F*)File_InPut->Get("h_mTrackPhiRes_Clusters");

  TH1F *h_mTrackXRes_Clusters_triLayer = (TH1F*)File_InPut->Get("h_mTrackXRes_Clusters_triLayer");
  TH1F *h_mTrackYRes_Clusters_triLayer = (TH1F*)File_InPut->Get("h_mTrackYRes_Clusters_triLayer");
  TH1F *h_mTrackRRes_Clusters_triLayer = (TH1F*)File_InPut->Get("h_mTrackRRes_Clusters_triLayer");
  TH1F *h_mTrackPhiRes_Clusters_triLayer = (TH1F*)File_InPut->Get("h_mTrackPhiRes_Clusters_triLayer");

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1600,1200);
  c_play->Divide(4,3);
  for(int i_pad = 0; i_pad < 12; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  // track resoluiton with Hits
  c_play->cd(1);
  h_mFstHitsTrackXRes->SetTitle("Hits: x-residual");
  h_mFstHitsTrackXRes->GetXaxis()->SetTitle("x-residual (mm)");
  h_mFstHitsTrackXRes->GetXaxis()->SetTitleSize(0.06);
  h_mFstHitsTrackXRes->GetYaxis()->SetTitle("No. Tracks");
  h_mFstHitsTrackXRes->GetYaxis()->SetTitleSize(0.06);
  h_mFstHitsTrackXRes->Draw();
  h_mFstHitsTrackXRes->Fit("gaus");

  c_play->cd(2);
  h_mFstHitsTrackYRes->SetTitle("Hits: y-residual");
  h_mFstHitsTrackYRes->GetXaxis()->SetTitle("y-residual (mm)");
  h_mFstHitsTrackYRes->GetXaxis()->SetTitleSize(0.06);
  h_mFstHitsTrackYRes->GetYaxis()->SetTitle("No. Tracks");
  h_mFstHitsTrackYRes->GetYaxis()->SetTitleSize(0.06);
  h_mFstHitsTrackYRes->Draw();
  h_mFstHitsTrackYRes->Fit("gaus");

  c_play->cd(3);
  h_mTrackRRes_Hits->SetTitle("Hits: r-residual");
  h_mTrackRRes_Hits->GetXaxis()->SetTitle("r-residual (mm)");
  h_mTrackRRes_Hits->GetXaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Hits->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackRRes_Hits->GetYaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Hits->Draw();
  h_mTrackRRes_Hits->Fit("gaus");

  c_play->cd(4);
  h_mTrackPhiRes_Hits->SetTitle("Hits: #phi-residual");
  h_mTrackPhiRes_Hits->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mTrackPhiRes_Hits->GetXaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Hits->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackPhiRes_Hits->GetYaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Hits->Draw();
  h_mTrackPhiRes_Hits->Fit("gaus");

  // track resoluiton with Clusters
  c_play->cd(5);
  h_mTrackXRes_Clusters->SetTitle("Clusters: x-residual");
  h_mTrackXRes_Clusters->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXRes_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackXRes_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters->Draw();
  h_mTrackXRes_Clusters->Fit("gaus");

  c_play->cd(6);
  h_mTrackYRes_Clusters->SetTitle("Clusters: y-residual");
  h_mTrackYRes_Clusters->GetXaxis()->SetTitle("y-residual (mm)");
  h_mTrackYRes_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackYRes_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters->Draw();
  h_mTrackYRes_Clusters->Fit("gaus");

  c_play->cd(7);
  h_mTrackRRes_Clusters->SetTitle("Clusters: r-residual");
  h_mTrackRRes_Clusters->GetXaxis()->SetTitle("r-residual (mm)");
  h_mTrackRRes_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackRRes_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters->Draw();
  h_mTrackRRes_Clusters->Fit("gaus");

  c_play->cd(8);
  h_mTrackPhiRes_Clusters->SetTitle("Clusters: #phi-residual");
  h_mTrackPhiRes_Clusters->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mTrackPhiRes_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackPhiRes_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters->Draw();
  h_mTrackPhiRes_Clusters->Fit("gaus");

  // track resoluiton with Clusters triLayer
  c_play->cd(9);
  h_mTrackXRes_Clusters_triLayer->SetTitle("Clusters with 3 Layer: x-residual");
  h_mTrackXRes_Clusters_triLayer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXRes_Clusters_triLayer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters_triLayer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackXRes_Clusters_triLayer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters_triLayer->Draw();
  h_mTrackXRes_Clusters_triLayer->Fit("gaus");

  c_play->cd(10);
  h_mTrackYRes_Clusters_triLayer->SetTitle("Clusters with 3 Layer: y-residual");
  h_mTrackYRes_Clusters_triLayer->GetXaxis()->SetTitle("y-residual (mm)");
  h_mTrackYRes_Clusters_triLayer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters_triLayer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackYRes_Clusters_triLayer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters_triLayer->Draw();
  h_mTrackYRes_Clusters_triLayer->Fit("gaus");

  c_play->cd(11);
  h_mTrackRRes_Clusters_triLayer->SetTitle("Clusters with 3 Layer: r-residual");
  h_mTrackRRes_Clusters_triLayer->GetXaxis()->SetTitle("r-residual (mm)");
  h_mTrackRRes_Clusters_triLayer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters_triLayer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackRRes_Clusters_triLayer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters_triLayer->Draw();
  h_mTrackRRes_Clusters_triLayer->Fit("gaus");

  c_play->cd(12);
  h_mTrackPhiRes_Clusters_triLayer->SetTitle("Clusters with 3 Layer: #phi-residual");
  h_mTrackPhiRes_Clusters_triLayer->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mTrackPhiRes_Clusters_triLayer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters_triLayer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackPhiRes_Clusters_triLayer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters_triLayer->Draw();
  h_mTrackPhiRes_Clusters_triLayer->Fit("gaus");


  c_play->SaveAs("./figures/TrackResolution_HV140.eps");
}
