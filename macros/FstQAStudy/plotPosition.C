#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>

using namespace std;

void plotPosition()
{
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH2F *h_mPositionR_Clusters = (TH2F*)File_InPut->Get("h_mPositionR_Clusters");
  TH2F *h_mPositionPhi_Clusters = (TH2F*)File_InPut->Get("h_mPositionPhi_Clusters");

  TCanvas *c_pos = new TCanvas("c_pos","c_pos",10,10,800,400);
  c_pos->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++i_pad)
  {
    c_pos->cd(i_pad+1)->SetLeftMargin(0.15);
    c_pos->cd(i_pad+1)->SetBottomMargin(0.15);
    c_pos->cd(i_pad+1)->SetTicks(1,1);
    c_pos->cd(i_pad+1)->SetGrid(0,0);
  }

  c_pos->cd(1);
  h_mPositionR_Clusters->SetStats(0);
  h_mPositionR_Clusters->GetXaxis()->SetTitle("R_{proj}");
  h_mPositionR_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mPositionR_Clusters->GetYaxis()->SetTitle("R_{ro}");
  h_mPositionR_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mPositionR_Clusters->Draw("colz");

  c_pos->cd(2);
  h_mPositionPhi_Clusters->SetStats(0);
  h_mPositionPhi_Clusters->GetXaxis()->SetTitle("#phi_{proj}");
  h_mPositionPhi_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mPositionPhi_Clusters->GetYaxis()->SetTitle("#phi_{ro}");
  h_mPositionPhi_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mPositionPhi_Clusters->Draw("colz");

  c_pos->SaveAs("./figures/positionFst.eps");
}
