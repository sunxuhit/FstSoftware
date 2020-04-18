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

void plotResidual_FSTClusterTracks_2Layer(string hv = "HV140V", string config = "Th4o5Tb3")
{
  gStyle->SetOptStat(11111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.15); gStyle->SetStatH(0.25);

  string inputfile = Form("/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstTracking_%s_withPed_%s.root",hv.c_str(),config.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mTrackXResIST_2Layer = (TH1F*)File_InPut->Get("h_mTrackXResIST_2Layer");
  TH1F *h_mTrackYResIST_2Layer = (TH1F*)File_InPut->Get("h_mTrackYResIST_2Layer");
  TH1F *h_mTrackXRes_Clusters_2Layer = (TH1F*)File_InPut->Get("h_mTrackXRes_Clusters_2Layer");
  TH1F *h_mTrackYRes_Clusters_2Layer = (TH1F*)File_InPut->Get("h_mTrackYRes_Clusters_2Layer");
  TH1F *h_mTrackRRes_Clusters_2Layer = (TH1F*)File_InPut->Get("h_mTrackRRes_Clusters_2Layer");
  TH1F *h_mTrackPhiRes_Clusters_2Layer = (TH1F*)File_InPut->Get("h_mTrackPhiRes_Clusters_2Layer");

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,1200);
  c_play->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetRightMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  c_play->cd(1);
  h_mTrackXResIST_2Layer->SetTitle("Corrected X-residual IST2");
  h_mTrackXResIST_2Layer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXResIST_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackXResIST_2Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackXResIST_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackXResIST_2Layer->Draw();
  h_mTrackXResIST_2Layer->Fit("gaus");

  c_play->cd(2);
  h_mTrackYResIST_2Layer->SetTitle("Corrected Y-residual IST2");
  h_mTrackYResIST_2Layer->GetXaxis()->SetTitle("y-residual (mm)");
  h_mTrackYResIST_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackYResIST_2Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackYResIST_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackYResIST_2Layer->Draw();
  h_mTrackYResIST_2Layer->Fit("gaus");

  c_play->cd(3);
  h_mTrackXRes_Clusters_2Layer->SetTitle("Corrected X-residual FST");
  h_mTrackXRes_Clusters_2Layer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXRes_Clusters_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters_2Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackXRes_Clusters_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters_2Layer->Draw();
  h_mTrackXRes_Clusters_2Layer->Fit("gaus");

  c_play->cd(4);
  h_mTrackYRes_Clusters_2Layer->SetTitle("Corrected Y-residual FST");
  h_mTrackYRes_Clusters_2Layer->GetXaxis()->SetTitle("y-residual (mm)");
  h_mTrackYRes_Clusters_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters_2Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackYRes_Clusters_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters_2Layer->Draw();
  h_mTrackYRes_Clusters_2Layer->Fit("gaus");

  c_play->cd(5);
  h_mTrackRRes_Clusters_2Layer->SetTitle("Corrected R-residual FST");
  h_mTrackRRes_Clusters_2Layer->GetXaxis()->SetTitle("r-residual (mm)");
  h_mTrackRRes_Clusters_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters_2Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackRRes_Clusters_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters_2Layer->Draw();
  h_mTrackRRes_Clusters_2Layer->Fit("gaus");

  c_play->cd(6);
  h_mTrackPhiRes_Clusters_2Layer->SetTitle("Corrected #phi-residual FST");
  h_mTrackPhiRes_Clusters_2Layer->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mTrackPhiRes_Clusters_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters_2Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackPhiRes_Clusters_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters_2Layer->Draw();
  h_mTrackPhiRes_Clusters_2Layer->Fit("gaus");

  c_play->SaveAs("./figures/Residual_FSTClusterTracks_2Layer.pdf");
}
