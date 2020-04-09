#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TF1.h>
#include <TProfile.h>

using namespace std;

void plotClusterSize()
{
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TProfile *p_mNHitsR_meanColumn   = (TProfile*)File_InPut->Get("p_mNHitsR_meanColumn");
  TProfile *p_mNHitsR_meanRow      = (TProfile*)File_InPut->Get("p_mNHitsR_meanRow");
  TProfile *p_mNHitsPhi_meanColumn = (TProfile*)File_InPut->Get("p_mNHitsPhi_meanColumn");
  TProfile *p_mNHitsPhi_meanRow    = (TProfile*)File_InPut->Get("p_mNHitsPhi_meanRow");
  TProfile *p_mTbDiffR             = (TProfile*)File_InPut->Get("p_mTbDiffR");
  TProfile *p_mTbDiffPhi           = (TProfile*)File_InPut->Get("p_mTbDiffPhi");

  TCanvas *c_ClusterSize = new TCanvas("c_ClusterSize","c_ClusterSize",10,10,800,1200);
  c_ClusterSize->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_ClusterSize->cd(i_pad+1)->SetLeftMargin(0.15);
    c_ClusterSize->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ClusterSize->cd(i_pad+1)->SetTicks(1,1);
    c_ClusterSize->cd(i_pad+1)->SetGrid(0,0);
  }

  c_ClusterSize->cd(1);
  p_mNHitsR_meanColumn->SetTitle("Cluster Size in R");
  p_mNHitsR_meanColumn->SetStats(0);
  p_mNHitsR_meanColumn->GetXaxis()->SetRangeUser(4.0,8.0);
  p_mNHitsR_meanColumn->GetXaxis()->SetTitle("<R_strip>");
  p_mNHitsR_meanColumn->GetXaxis()->SetTitleSize(0.06);
  p_mNHitsR_meanColumn->GetYaxis()->SetTitle("# Hits in R");
  p_mNHitsR_meanColumn->GetYaxis()->SetTitleSize(0.06);
  p_mNHitsR_meanColumn->Draw();

  c_ClusterSize->cd(2);
  p_mNHitsR_meanRow->SetTitle("Cluster Size in R");
  p_mNHitsR_meanRow->SetStats(0);
  p_mNHitsR_meanRow->GetXaxis()->SetRangeUser(0.0,63.0);
  p_mNHitsR_meanRow->GetXaxis()->SetTitle("<#phi seg.>");
  p_mNHitsR_meanRow->GetXaxis()->SetTitleSize(0.06);
  p_mNHitsR_meanRow->GetYaxis()->SetTitle("# Hits in R");
  p_mNHitsR_meanRow->GetYaxis()->SetTitleSize(0.06);
  p_mNHitsR_meanRow->Draw();

  c_ClusterSize->cd(3);
  p_mNHitsPhi_meanColumn->SetTitle("Cluster Size in #phi");
  p_mNHitsPhi_meanColumn->SetStats(0);
  p_mNHitsPhi_meanColumn->GetXaxis()->SetRangeUser(4.0,8.0);
  p_mNHitsPhi_meanColumn->GetXaxis()->SetTitle("<R_strip>");
  p_mNHitsPhi_meanColumn->GetXaxis()->SetTitleSize(0.06);
  p_mNHitsPhi_meanColumn->GetYaxis()->SetTitle("# Hits in #phi");
  p_mNHitsPhi_meanColumn->GetYaxis()->SetTitleSize(0.06);
  p_mNHitsPhi_meanColumn->Draw();

  c_ClusterSize->cd(4);
  p_mNHitsPhi_meanRow->SetTitle("Cluster Size in #phi");
  p_mNHitsPhi_meanRow->SetStats(0);
  p_mNHitsPhi_meanRow->GetXaxis()->SetRangeUser(0.0,63.0);
  p_mNHitsPhi_meanRow->GetXaxis()->SetTitle("<#phi seg.>");
  p_mNHitsPhi_meanRow->GetXaxis()->SetTitleSize(0.06);
  p_mNHitsPhi_meanRow->GetYaxis()->SetTitle("# Hits in #phi");
  p_mNHitsPhi_meanRow->GetYaxis()->SetTitleSize(0.06);
  p_mNHitsPhi_meanRow->Draw();

  c_ClusterSize->cd(5);
  p_mTbDiffR->SetTitle("Time Bin Difference");
  p_mTbDiffR->SetStats(0);
  p_mTbDiffR->GetXaxis()->SetRangeUser(4.0,8.0);
  p_mTbDiffR->GetXaxis()->SetTitle("<R_strip>");
  p_mTbDiffR->GetXaxis()->SetTitleSize(0.06);
  p_mTbDiffR->GetYaxis()->SetTitle("Tb(MaxAdc)-Tb(other hits)");
  p_mTbDiffR->GetYaxis()->SetTitleSize(0.06);
  p_mTbDiffR->Draw();

  c_ClusterSize->cd(6);
  p_mTbDiffPhi->SetTitle("Time Bin Difference");
  p_mTbDiffPhi->SetStats(0);
  p_mTbDiffPhi->GetXaxis()->SetRangeUser(0.0,63.0);
  p_mTbDiffPhi->GetXaxis()->SetTitle("<#phi seg.>");
  p_mTbDiffPhi->GetXaxis()->SetTitleSize(0.06);
  p_mTbDiffPhi->GetYaxis()->SetTitle("Tb(MaxAdc)-Tb(other hits)");
  p_mTbDiffPhi->GetYaxis()->SetTitleSize(0.06);
  p_mTbDiffPhi->Draw();

  c_ClusterSize->SaveAs("./figures/ClusterSize.eps");
}

