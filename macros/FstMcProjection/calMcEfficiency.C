#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include "../FstPlotConfiguration/draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void calMcEfficiency(bool isRot = true, int rAligned = 0)
{
  const double rMaxFst = FST::rOuter + 4.0*FST::pitchR;
  const double rMinFst = FST::rOuter;
  const double phiMaxFst = 64.0*FST::pitchPhi;
  const double phiMinFst = 0.0;
  double phiMaxProj = 64.0*FST::pitchPhi-0.5*TMath::Pi()/180.0;
  double phiMinProj = 0.0+0.5*TMath::Pi()/180.0;
  if(isRot)
  {
    phiMaxProj = 64.0*FST::pitchPhi-2.5*TMath::Pi()/180.0;
    phiMinProj = 0.0+2.5*TMath::Pi()/180.0;
  }
  // const double phiMinFst = -64.0*FST::pitchPhi;

  const int nMatch = 8;

  string inputname = "../../output/simulation/FstMcProjection_woRot.root";
  if(isRot) 
  {
    inputname = Form("../../output/simulation/FstMcProjection_Rot_AlignedRstrip%d.root",rAligned);
  }
  TFile *File_InPut = TFile::Open(inputname.c_str());
  TH2F *h_mIstCounts_2Layer[nMatch];
  TH1F *h_mIstCountsR_2Layer[nMatch];
  TH1F *h_mIstCountsPhi_2Layer[nMatch];

  TH2F *h_mFstCounts_2Layer[nMatch];
  TH1F *h_mFstCountsR_2Layer[nMatch];
  TH1F *h_mFstCountsPhi_2Layer[nMatch];
  for(int i_match = 0; i_match < 8; ++i_match)
  {
    string HistName;
    // simple clusters
    HistName = Form("h_mIstCounts_2Layer_SF%d",i_match);
    h_mIstCounts_2Layer[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    h_mIstCounts_2Layer[i_match]->Sumw2();
    int projIstBinX0 = h_mIstCounts_2Layer[i_match]->GetYaxis()->FindBin(phiMinProj);
    int projIstBinX1 = h_mIstCounts_2Layer[i_match]->GetYaxis()->FindBin(phiMaxProj);
    HistName = Form("h_mIstCountsR_2Layer_SF%d",i_match);
    h_mIstCountsR_2Layer[i_match] = (TH1F*)h_mIstCounts_2Layer[i_match]->ProjectionX(HistName.c_str(),projIstBinX0,projIstBinX1);

    int projIstBinY0 = h_mIstCounts_2Layer[i_match]->GetXaxis()->FindBin(rMinFst+0.4*FST::pitchR);
    int projIstBinY1 = h_mIstCounts_2Layer[i_match]->GetXaxis()->FindBin(rMaxFst-0.4*FST::pitchR);
    HistName = Form("h_mIstCountsPhi_2Layer_SF%d",i_match);
    h_mIstCountsPhi_2Layer[i_match] = (TH1F*)h_mIstCounts_2Layer[i_match]->ProjectionY(HistName.c_str(),projIstBinY0,projIstBinY1);

    HistName = Form("h_mFstCounts_2Layer_SF%d",i_match);
    h_mFstCounts_2Layer[i_match] = (TH2F*)File_InPut->Get(HistName.c_str());
    h_mFstCounts_2Layer[i_match]->Sumw2();
    int projFstBinX0 = h_mFstCounts_2Layer[i_match]->GetYaxis()->FindBin(phiMinProj);
    int projFstBinX1 = h_mFstCounts_2Layer[i_match]->GetYaxis()->FindBin(phiMaxProj);
    HistName = Form("h_mFstCountsR_2Layer_SF%d",i_match);
    h_mFstCountsR_2Layer[i_match] = (TH1F*)h_mFstCounts_2Layer[i_match]->ProjectionX(HistName.c_str(),projFstBinX0,projFstBinX1);

    int projFstBinY0 = h_mFstCounts_2Layer[i_match]->GetXaxis()->FindBin(rMinFst+0.4*FST::pitchR);
    int projFstBinY1 = h_mFstCounts_2Layer[i_match]->GetXaxis()->FindBin(rMaxFst-0.4*FST::pitchR);
    HistName = Form("h_mFstCountsPhi_2Layer_SF%d",i_match);
    h_mFstCountsPhi_2Layer[i_match] = (TH1F*)h_mFstCounts_2Layer[i_match]->ProjectionY(HistName.c_str(),projFstBinY0,projFstBinY1);
  }

  TH2F *h_mFstEff_2Layer[nMatch];
  TH1F *h_mFstEffR_2Layer[nMatch];
  TH1F *h_mFstEffPhi_2Layer[nMatch];
  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    string HistName;
    HistName = Form("h_mFstEff_2Layer_SF%d",i_match);
    h_mFstEff_2Layer[i_match] = (TH2F*)h_mIstCounts_2Layer[i_match]->Clone(HistName.c_str());
    h_mFstEff_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mFstEff_2Layer[i_match]->Reset();
    h_mFstEff_2Layer[i_match]->Divide(h_mFstCounts_2Layer[i_match],h_mIstCounts_2Layer[i_match],1,1,"B");

    HistName = Form("h_mFstEffR_2Layer_SF%d",i_match);
    h_mFstEffR_2Layer[i_match] = (TH1F*)h_mIstCountsR_2Layer[i_match]->Clone(HistName.c_str());
    h_mFstEffR_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mFstEffR_2Layer[i_match]->Reset();
    h_mFstEffR_2Layer[i_match]->Divide(h_mFstCountsR_2Layer[i_match],h_mIstCountsR_2Layer[i_match],1,1,"B");

    HistName = Form("h_mFstEffPhi_2Layer_SF%d",i_match);
    h_mFstEffPhi_2Layer[i_match] = (TH1F*)h_mIstCountsPhi_2Layer[i_match]->Clone(HistName.c_str());
    h_mFstEffPhi_2Layer[i_match]->SetTitle(HistName.c_str());
    h_mFstEffPhi_2Layer[i_match]->Reset();
    h_mFstEffPhi_2Layer[i_match]->Divide(h_mFstCountsPhi_2Layer[i_match],h_mIstCountsPhi_2Layer[i_match],1,1,"B");
  }


  string outputname = "./figures/McEfficiency_woRot.pdf";
  if(isRot) outputname = Form("./figures/McEfficiency_Rot_AlignedRstrip%d.pdf",rAligned);
  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,900,900);
  c_play->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start= "./figures/McEfficiency_woRot.pdf[";
  if(isRot) output_start = Form("./figures/McEfficiency_Rot_AlignedRstrip%d.pdf[",rAligned);
  c_play->Print(output_start.c_str()); // open pdf file

  for(int i_match = 0; i_match < nMatch; ++i_match)
  {
    // projection position from IST
    c_play->cd(1);
    h_mIstCounts_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mIstCounts_2Layer[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mIstCounts_2Layer[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(2);
    h_mIstCountsR_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mIstCountsR_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.1*h_mIstCountsR_2Layer[i_match]->GetMaximum());
    h_mIstCountsR_2Layer[i_match]->Draw("HIST");
    PlotLine(rMinFst, rMinFst, 0.5, h_mIstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.5, h_mIstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(3);
    h_mIstCountsPhi_2Layer[i_match]->GetXaxis()->SetTitle("phi_{proj} (mm)");
    h_mIstCountsPhi_2Layer[i_match]->GetXaxis()->SetRangeUser(-1.2*phiMaxFst,1.2*phiMaxFst);
    h_mIstCountsPhi_2Layer[i_match]->Draw("HIST");
    PlotLine(phiMinFst, phiMinFst, 0.5, h_mIstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.5, h_mIstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    // measured position from FST
    c_play->cd(4);
    h_mFstCounts_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mFstCounts_2Layer[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mFstCounts_2Layer[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(5);
    h_mFstCountsR_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mFstCountsR_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.1*h_mFstCountsR_2Layer[i_match]->GetMaximum());
    h_mFstCountsR_2Layer[i_match]->Draw("HIST");
    PlotLine(rMinFst, rMinFst, 0.5, h_mFstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.5, h_mFstCountsR_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(6);
    h_mFstCountsPhi_2Layer[i_match]->GetXaxis()->SetTitle("phi_{proj} (mm)");
    h_mFstCountsPhi_2Layer[i_match]->GetXaxis()->SetRangeUser(-1.2*phiMaxFst,1.2*phiMaxFst);
    h_mFstCountsPhi_2Layer[i_match]->Draw("HIST");
    PlotLine(phiMinFst, phiMinFst, 0.5, h_mFstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.5, h_mFstCountsPhi_2Layer[i_match]->GetMaximum(), 1, 2, 2);

    c_play->cd(7);
    h_mFstEff_2Layer[i_match]->SetStats(0);
    h_mFstEff_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mFstEff_2Layer[i_match]->GetYaxis()->SetTitle("phi_{proj} (rad)");
    h_mFstEff_2Layer[i_match]->Draw("colz");
    PlotLine(rMinFst, rMaxFst, phiMinFst, phiMinFst, 1, 2, 2);
    PlotLine(rMinFst, rMaxFst, phiMaxFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMinFst, rMinFst, phiMinFst, phiMaxFst, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, phiMinFst, phiMaxFst, 1, 2, 2);

    c_play->cd(8);
    h_mFstEffR_2Layer[i_match]->SetStats(0);
    h_mFstEffR_2Layer[i_match]->GetXaxis()->SetTitle("r_{proj} (mm)");
    h_mFstEffR_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.05);
    h_mFstEffR_2Layer[i_match]->SetLineColor(1);
    h_mFstEffR_2Layer[i_match]->Draw("pE");
    PlotLine(rMinFst, rMinFst, 0.0, 1.05, 1, 2, 2);
    PlotLine(rMaxFst, rMaxFst, 0.0, 1.05, 1, 2, 2);
    PlotLine(rMinFst-FST::pitchR, rMaxFst+FST::pitchR, 0.90, 0.90, 4, 1, 2);
    PlotLine(rMinFst-FST::pitchR, rMaxFst+FST::pitchR, 0.95, 0.95, 4, 1, 2);

    c_play->cd(9);
    h_mFstEffPhi_2Layer[i_match]->SetStats(0);
    h_mFstEffPhi_2Layer[i_match]->GetXaxis()->SetTitle("phi_{proj} (rad)");
    h_mFstEffPhi_2Layer[i_match]->GetXaxis()->SetRangeUser(-1.2*phiMaxFst,1.2*phiMaxFst);
    h_mFstEffPhi_2Layer[i_match]->GetYaxis()->SetRangeUser(0.0,1.05);
    h_mFstEffPhi_2Layer[i_match]->SetLineColor(1);
    h_mFstEffPhi_2Layer[i_match]->Draw("pE");
    PlotLine(phiMinFst, phiMinFst, 0.0, 1.05, 1, 2, 2);
    PlotLine(phiMaxFst, phiMaxFst, 0.0, 1.05, 1, 2, 2);

    c_play->Update();
    c_play->Print(outputname.c_str());
  }

  string output_stop= "./figures/McEfficiency_woRot.pdf]";
  if(isRot) output_stop = Form("./figures/McEfficiency_Rot_AlignedRstrip%d.pdf]",rAligned);
  c_play->Print(output_stop.c_str()); // open pdf file
}
