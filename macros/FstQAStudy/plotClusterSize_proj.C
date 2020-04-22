#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>
#include "./draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotClusterSize_proj()
{
  string inputfile = "../../output/FstQAStudy_HV140_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TProfile *p_mNHitsR_rP[4]; // 0: no matching | 1-3 matching within (1-3)*pitchR in r & (1-3)*pitchPhi in phi
  TProfile *p_mNHitsR_phiP[4];
  TProfile *p_mNHitsPhi_rP[4];
  TProfile *p_mNHitsPhi_phiP[4];
  for(int i_match = 0; i_match < 4; ++i_match)
  {
    string ProName;
    ProName = Form("p_mNHitsR_rP_SF%d",i_match*10);
    p_mNHitsR_rP[i_match] = (TProfile*)File_InPut->Get(ProName.c_str());
    ProName = Form("p_mNHitsR_phiP_SF%d",i_match*10);
    p_mNHitsR_phiP[i_match] = (TProfile*)File_InPut->Get(ProName.c_str());
    ProName = Form("p_mNHitsPhi_rP_SF%d",i_match*10);
    p_mNHitsPhi_rP[i_match] = (TProfile*)File_InPut->Get(ProName.c_str());
    ProName = Form("p_mNHitsPhi_phiP_SF%d",i_match*10);
    p_mNHitsPhi_phiP[i_match] = (TProfile*)File_InPut->Get(ProName.c_str());
  }

  const double rMax = FST::rOuter + 4.0*FST::pitchR;
  const double rMin = FST::rOuter;
  const double phiMax = 64.0*FST::pitchPhi;
  const double phiMin = 0.0;

  string outputname = "./figures/ClusterSize_QA.pdf";
  TCanvas *c_ClusterSize = new TCanvas("c_ClusterSize","c_ClusterSize",10,10,800,800);
  c_ClusterSize->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_ClusterSize->cd(i_pad+1)->SetLeftMargin(0.15);
    c_ClusterSize->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ClusterSize->cd(i_pad+1)->SetTicks(1,1);
    c_ClusterSize->cd(i_pad+1)->SetGrid(0,0);
  }

  string output_start = "./figures/ClusterSize_QA.pdf[";
  c_ClusterSize->Print(output_start.c_str()); // open pdf file

  for(int i_match = 0; i_match < 4; ++i_match)
  {
    c_ClusterSize->cd(i_match+1);
    p_mNHitsR_rP[i_match]->SetStats(0);
    p_mNHitsR_rP[i_match]->GetYaxis()->SetRangeUser(0.5,3.5);
    p_mNHitsR_rP[i_match]->GetXaxis()->SetTitle("<R_{proj}>");
    p_mNHitsR_rP[i_match]->GetXaxis()->SetTitleSize(0.06);
    p_mNHitsR_rP[i_match]->GetYaxis()->SetTitle("# Hits in R");
    p_mNHitsR_rP[i_match]->GetYaxis()->SetTitleSize(0.06);
    p_mNHitsR_rP[i_match]->SetMarkerColor(i_match+1);
    p_mNHitsR_rP[i_match]->SetMarkerStyle(20+i_match);
    p_mNHitsR_rP[i_match]->SetMarkerSize(0.8);
    p_mNHitsR_rP[i_match]->Draw("pE");
    PlotLine(rMin, rMin, 0.5, 3.5, 1, 2, 2);
    PlotLine(rMax, rMax, 0.5, 3.5, 1, 2, 2);
  }
  c_ClusterSize->Print(outputname.c_str()); // print integrated efficiency

  for(int i_match = 0; i_match < 4; ++i_match)
  {
    c_ClusterSize->cd(i_match+1);
    p_mNHitsR_phiP[i_match]->SetStats(0);
    p_mNHitsR_phiP[i_match]->GetYaxis()->SetRangeUser(0.5,3.5);
    p_mNHitsR_phiP[i_match]->GetXaxis()->SetTitle("<#phi_{proj}>");
    p_mNHitsR_phiP[i_match]->GetXaxis()->SetTitleSize(0.06);
    p_mNHitsR_phiP[i_match]->GetYaxis()->SetTitle("# Hits in R");
    p_mNHitsR_phiP[i_match]->GetYaxis()->SetTitleSize(0.06);
    p_mNHitsR_phiP[i_match]->SetMarkerColor(i_match+1);
    p_mNHitsR_phiP[i_match]->SetMarkerStyle(20+i_match);
    p_mNHitsR_phiP[i_match]->SetMarkerSize(0.8);
    p_mNHitsR_phiP[i_match]->Draw("pE");
    PlotLine(phiMin, phiMin, 0.5, 3.5, 1, 2, 2);
    PlotLine(phiMax, phiMax, 0.5, 3.5, 1, 2, 2);
  }
  c_ClusterSize->Update();
  c_ClusterSize->Print(outputname.c_str());

  for(int i_match = 0; i_match < 4; ++i_match)
  {
    c_ClusterSize->cd(i_match+1);
    p_mNHitsPhi_rP[i_match]->SetStats(0);
    p_mNHitsPhi_rP[i_match]->GetYaxis()->SetRangeUser(0.5,3.5);
    p_mNHitsPhi_rP[i_match]->GetXaxis()->SetTitle("<R_{proj}>");
    p_mNHitsPhi_rP[i_match]->GetXaxis()->SetTitleSize(0.06);
    p_mNHitsPhi_rP[i_match]->GetYaxis()->SetTitle("# Hits in #phi");
    p_mNHitsPhi_rP[i_match]->GetYaxis()->SetTitleSize(0.06);
    p_mNHitsPhi_rP[i_match]->SetMarkerColor(i_match+1);
    p_mNHitsPhi_rP[i_match]->SetMarkerStyle(20+i_match);
    p_mNHitsPhi_rP[i_match]->SetMarkerSize(0.8);
    p_mNHitsPhi_rP[i_match]->Draw("pE");
    PlotLine(rMin, rMin, 0.5, 3.5, 1, 2, 2);
    PlotLine(rMax, rMax, 0.5, 3.5, 1, 2, 2);
  }
  c_ClusterSize->Update();
  c_ClusterSize->Print(outputname.c_str()); // print integrated efficiency

  for(int i_match = 0; i_match < 4; ++i_match)
  {
    c_ClusterSize->cd(i_match+1);
    p_mNHitsPhi_phiP[i_match]->SetStats(0);
    p_mNHitsPhi_phiP[i_match]->GetYaxis()->SetRangeUser(0.5,3.5);
    p_mNHitsPhi_phiP[i_match]->GetXaxis()->SetTitle("<#phi_{proj}>");
    p_mNHitsPhi_phiP[i_match]->GetXaxis()->SetTitleSize(0.06);
    p_mNHitsPhi_phiP[i_match]->GetYaxis()->SetTitle("# Hits in #phi");
    p_mNHitsPhi_phiP[i_match]->GetYaxis()->SetTitleSize(0.06);
    p_mNHitsPhi_phiP[i_match]->SetMarkerColor(i_match+1);
    p_mNHitsPhi_phiP[i_match]->SetMarkerStyle(20+i_match);
    p_mNHitsPhi_phiP[i_match]->SetMarkerSize(0.8);
    p_mNHitsPhi_phiP[i_match]->Draw("pE");
    PlotLine(phiMin, phiMin, 0.5, 3.5, 1, 2, 2);
    PlotLine(phiMax, phiMax, 0.5, 3.5, 1, 2, 2);
  }
  c_ClusterSize->Update();
  c_ClusterSize->Print(outputname.c_str()); // print integrated efficiency

  string output_stop =  "./figures/ClusterSize_QA.pdf]";
  c_ClusterSize->Print(output_stop.c_str()); // close pdf file
}

