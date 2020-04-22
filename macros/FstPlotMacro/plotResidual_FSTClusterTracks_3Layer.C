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

double gaussian(double *var, double *par)
{
  double residual = var[0];

  double norm  = par[0];
  double mean  = par[1];
  double sigma = par[2];

  double sigmaSquare = sigma*sigma;
  double Norm = norm/sqrt(2.0*sigmaSquare*TMath::Pi());
  double Power = -0.5*(residual-mean)*(residual-mean)/sigmaSquare;

  double y = Norm*exp(Power);

  return y;
}

void plotResidual_FSTClusterTracks_3Layer(string hv = "HV140V", string config = "Th4o5Tb3")
{
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.15); gStyle->SetStatH(0.25);

  string inputfile = Form("/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstTracking_%s_withPed_%s.root",hv.c_str(),config.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mTrackXResIST_3Layer = (TH1F*)File_InPut->Get("h_mTrackXResIST_3Layer");
  TH1F *h_mTrackYResIST_3Layer = (TH1F*)File_InPut->Get("h_mTrackYResIST_3Layer");
  TH2F *h_mTrackXYResIST_3Layer = (TH2F*)File_InPut->Get("h_mTrackXYResIST_3Layer");;

  TH1F *h_mTrackXRes_Clusters_3Layer = (TH1F*)File_InPut->Get("h_mTrackXRes_Clusters_3Layer");
  TH1F *h_mTrackYRes_Clusters_3Layer = (TH1F*)File_InPut->Get("h_mTrackYRes_Clusters_3Layer");
  TH2F *h_mTrackXYRes_Clusters_3Layer = (TH2F*)File_InPut->Get("h_mTrackXYRes_Clusters_3Layer");

  TH1F *h_mTrackRRes_Clusters_3Layer = (TH1F*)File_InPut->Get("h_mTrackRRes_Clusters_3Layer");
  TH1F *h_mTrackPhiRes_Clusters_3Layer = (TH1F*)File_InPut->Get("h_mTrackPhiRes_Clusters_3Layer");
  TH2F *h_mTrackRPhiRes_Clusters_3Layer = (TH2F*)File_InPut->Get("h_mTrackRPhiRes_Clusters_3Layer");;

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1200,1200);
  c_play->Divide(3,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetRightMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  c_play->cd(1);
  h_mTrackXYResIST_3Layer->SetTitle("Corrected X & Y Residual IST2");
  h_mTrackXYResIST_3Layer->SetStats(0);
  h_mTrackXYResIST_3Layer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXYResIST_3Layer->GetYaxis()->SetTitle("y-residual (mm)");
  h_mTrackXYResIST_3Layer->Draw("colz");

  c_play->cd(2);
  h_mTrackXResIST_3Layer->SetTitle("Corrected X-residual IST2");
  h_mTrackXResIST_3Layer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXResIST_3Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackXResIST_3Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackXResIST_3Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackXResIST_3Layer->Draw();
  // h_mTrackXResIST_3Layer->Fit("gaus");
  TF1 *f_gausX_IST = new TF1("f_gausX_IST",gaussian,-80.0,80.0,3);
  f_gausX_IST->SetParameter(0,100.0);
  f_gausX_IST->SetParameter(1,0.0);
  f_gausX_IST->SetParameter(2,3.0);
  f_gausX_IST->SetRange(-10,10);
  h_mTrackXResIST_3Layer->Fit(f_gausX_IST,"R");
  f_gausX_IST->Draw("l same");

  c_play->cd(3);
  h_mTrackYResIST_3Layer->SetTitle("Corrected Y-residual IST2");
  h_mTrackYResIST_3Layer->GetXaxis()->SetTitle("y-residual (mm)");
  h_mTrackYResIST_3Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackYResIST_3Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackYResIST_3Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackYResIST_3Layer->Draw();
  // h_mTrackYResIST_3Layer->Fit("gaus");
  TF1 *f_gausY_IST = new TF1("f_gausY_IST",gaussian,-80.0,80.0,3);
  f_gausY_IST->SetParameter(0,100.0);
  f_gausY_IST->SetParameter(1,0.0);
  f_gausY_IST->SetParameter(2,3.0);
  f_gausY_IST->SetRange(-2.0,2.0);
  h_mTrackYResIST_3Layer->Fit(f_gausY_IST,"R");
  f_gausY_IST->Draw("l same");

  c_play->cd(4);
  h_mTrackXYRes_Clusters_3Layer->SetTitle("Corrected X & Y Residual FST");
  h_mTrackXYRes_Clusters_3Layer->SetStats(0);
  h_mTrackXYRes_Clusters_3Layer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXYRes_Clusters_3Layer->GetYaxis()->SetTitle("y-residual (mm)");
  h_mTrackXYRes_Clusters_3Layer->Draw("colz");

  c_play->cd(5);
  h_mTrackXRes_Clusters_3Layer->SetTitle("Corrected X-residual FST");
  h_mTrackXRes_Clusters_3Layer->GetXaxis()->SetTitle("x-residual (mm)");
  h_mTrackXRes_Clusters_3Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters_3Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackXRes_Clusters_3Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackXRes_Clusters_3Layer->Draw();
  // h_mTrackXRes_Clusters_3Layer->Fit("gaus");
  TF1 *f_gausX = new TF1("f_gausX",gaussian,-150.0,150.0,3);
  f_gausX->SetParameter(0,100.0);
  f_gausX->SetParameter(1,0.0);
  f_gausX->SetParameter(2,10.0);
  f_gausX->SetRange(-20,50);
  h_mTrackXRes_Clusters_3Layer->Fit(f_gausX,"R");
  f_gausX->Draw("l same");

  c_play->cd(6);
  h_mTrackYRes_Clusters_3Layer->SetTitle("Corrected Y-residual FST");
  h_mTrackYRes_Clusters_3Layer->GetXaxis()->SetTitle("y-residual (mm)");
  h_mTrackYRes_Clusters_3Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters_3Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackYRes_Clusters_3Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackYRes_Clusters_3Layer->Draw();
  // h_mTrackYRes_Clusters_3Layer->Fit("gaus");
  TF1 *f_gausY = new TF1("f_gausY",gaussian,-150.0,150.0,3);
  f_gausY->SetParameter(0,100.0);
  f_gausY->SetParameter(1,0.0);
  f_gausY->SetParameter(2,10.0);
  f_gausY->SetRange(-1.5,5);
  h_mTrackYRes_Clusters_3Layer->Fit(f_gausY,"R");
  f_gausY->Draw("l same");

  c_play->cd(7);
  h_mTrackRPhiRes_Clusters_3Layer->SetTitle("Corrected R & #phi Residual FST");
  h_mTrackRPhiRes_Clusters_3Layer->SetStats(0);
  h_mTrackRPhiRes_Clusters_3Layer->GetXaxis()->SetTitle("r-residual (mm)");
  h_mTrackRPhiRes_Clusters_3Layer->GetYaxis()->SetTitle("#phi-residual (rad)");
  h_mTrackRPhiRes_Clusters_3Layer->Draw("colz");

  c_play->cd(8);
  h_mTrackRRes_Clusters_3Layer->SetTitle("Corrected R-residual FST");
  h_mTrackRRes_Clusters_3Layer->GetXaxis()->SetTitle("r-residual (mm)");
  h_mTrackRRes_Clusters_3Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters_3Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackRRes_Clusters_3Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackRRes_Clusters_3Layer->Draw();
  // h_mTrackRRes_Clusters_3Layer->Fit("gaus");
  TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,3);
  f_gausR->SetParameter(0,100.0);
  f_gausR->SetParameter(1,0.0);
  f_gausR->SetParameter(2,10.0);
  f_gausR->SetRange(-20,50);
  h_mTrackRRes_Clusters_3Layer->Fit(f_gausR,"R");
  f_gausR->Draw("l same");

  c_play->cd(9);
  h_mTrackPhiRes_Clusters_3Layer->SetTitle("Corrected #phi-residual FST");
  h_mTrackPhiRes_Clusters_3Layer->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mTrackPhiRes_Clusters_3Layer->GetXaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters_3Layer->GetYaxis()->SetTitle("No. Tracks");
  h_mTrackPhiRes_Clusters_3Layer->GetYaxis()->SetTitleSize(0.06);
  h_mTrackPhiRes_Clusters_3Layer->Draw();
  // h_mTrackPhiRes_Clusters_3Layer->Fit("gaus");
  TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-150.0,150.0,3);
  f_gausPhi->SetParameter(0,100.0);
  f_gausPhi->SetParameter(1,0.0);
  f_gausPhi->SetParameter(2,10.0);
  f_gausPhi->SetRange(-0.015,0.015);
  h_mTrackPhiRes_Clusters_3Layer->Fit(f_gausPhi,"R");
  f_gausPhi->Draw("l same");

  c_play->SaveAs("./figures/Residual_FSTClusterTracks_3Layer.pdf");
}
