#include <iostream>
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TString.h>

using namespace std;

void plotCounts()
{
  string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstQAStudy_HV140_woPed.root";
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mCounts_Hits[4];
  TH1F *h_mCounts_Clusters[4];
  TH2F *h_mCounts_Corr[4];
  TH2F *h_mCounts_RPhi[4];

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    string HistName;
    HistName  = Form("h_mCounts_Hits_Layer%d",i_layer);
    h_mCounts_Hits[i_layer] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName  = Form("h_mCounts_Clusters_Layer%d",i_layer);
    h_mCounts_Clusters[i_layer] = (TH1F*)File_InPut->Get(HistName.c_str());
    HistName  = Form("h_mCounts_Corr_Layer%d",i_layer);
    h_mCounts_Corr[i_layer] = (TH2F*)File_InPut->Get(HistName.c_str());
    HistName  = Form("h_mCounts_RPhi_Layer%d",i_layer);
    h_mCounts_RPhi[i_layer] = (TH2F*)File_InPut->Get(HistName.c_str());
  }

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1600,1600);
  c_play->Divide(4,4);
  for(int i_pad = 0; i_pad < 16; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    c_play->cd(4*i_layer+1);
    h_mCounts_Hits[i_layer]->GetXaxis()->SetTitle("Num of Hits");
    h_mCounts_Hits[i_layer]->GetXaxis()->SetTitleSize(0.06);
    h_mCounts_Hits[i_layer]->Draw();

    c_play->cd(4*i_layer+2);
    h_mCounts_Clusters[i_layer]->GetXaxis()->SetTitle("Num of Clusters");
    h_mCounts_Clusters[i_layer]->GetXaxis()->SetTitleSize(0.06);
    h_mCounts_Clusters[i_layer]->Draw();

    c_play->cd(4*i_layer+3);
    h_mCounts_Corr[i_layer]->GetXaxis()->SetTitle("Num of Hits");
    h_mCounts_Corr[i_layer]->GetXaxis()->SetTitleSize(0.06);
    h_mCounts_Corr[i_layer]->GetYaxis()->SetTitle("Num of Clusters");
    h_mCounts_Corr[i_layer]->GetYaxis()->SetTitleSize(0.06);
    h_mCounts_Corr[i_layer]->Draw("colz");

    c_play->cd(4*i_layer+4);
    if(i_layer == 0) 
    {
      h_mCounts_RPhi[i_layer]->GetXaxis()->SetTitle("Size of Clusters in R");
      h_mCounts_RPhi[i_layer]->GetYaxis()->SetTitle("Size of Clusters in #phi");
    }
    else
    {
      h_mCounts_RPhi[i_layer]->GetXaxis()->SetTitle("Size of Clusters in X");
      h_mCounts_RPhi[i_layer]->GetYaxis()->SetTitle("Size of Clusters in Y");
    }
    h_mCounts_RPhi[i_layer]->GetXaxis()->SetTitleSize(0.06);
    h_mCounts_RPhi[i_layer]->GetYaxis()->SetTitleSize(0.06);
    h_mCounts_RPhi[i_layer]->Draw("colz");
  }

  c_play->SaveAs("./figures/ClusterCounts.eps");
}
