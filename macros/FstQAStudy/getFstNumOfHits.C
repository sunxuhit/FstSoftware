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

void getFstNumOfHits(string hv = "HV200V", bool isSavePed = true, bool isApplyCMNCorr = false, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 3)
{
  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  string inputfile = Form("../../output/configuration/FstQAStudy_%s_Th%1.1fTb%d_%s_%s.root",hv.c_str(),nFstHitsCut,numOfUsedTimeBins,pedMode.c_str(),cmnMode.c_str());

  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mSignalHits_Rstrip[4];
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    string HistName = Form("h_mSignalHits_Rstrip%d",i_rstrip);
    h_mSignalHits_Rstrip[i_rstrip] = (TH1F*)File_InPut->Get(HistName.c_str());
  }

  double weight[4];

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,800,800);
  c_Signal->cd()->SetLeftMargin(0.15);
  c_Signal->cd()->SetRightMargin(0.15);
  c_Signal->cd()->SetBottomMargin(0.15);
  c_Signal->cd()->SetTicks(1,1);
  c_Signal->cd()->SetGrid(0,0);

  c_Signal->cd();
  TLegend *leg = new TLegend(0.35,0.65,0.85,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetNdivisions(505);
    h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetLabelSize(0.04);
    h_mSignalHits_Rstrip[i_rstrip]->GetXaxis()->SetTitle("ADC");
    h_mSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetLabelSize(0.04);
    h_mSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetTitle("Counts");
    h_mSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetTitleSize(0.06);
    h_mSignalHits_Rstrip[i_rstrip]->GetYaxis()->SetNdivisions(505);
    h_mSignalHits_Rstrip[i_rstrip]->SetLineColor(i_rstrip+1);
    float inte_orig = h_mSignalHits_Rstrip[0]->Integral(1,h_mSignalHits_Rstrip[0]->GetNbinsX());
    float inte = h_mSignalHits_Rstrip[i_rstrip]->Integral(1,h_mSignalHits_Rstrip[i_rstrip]->GetNbinsX());
    weight[i_rstrip] = inte/inte_orig;

    string LegName = Form("R_strip %d & Counts %1.1f & weight %1.3f",i_rstrip,inte,weight[i_rstrip]);
    leg->AddEntry(h_mSignalHits_Rstrip[i_rstrip],LegName.c_str(),"L");

    if(i_rstrip == 0) 
    {
      h_mSignalHits_Rstrip[i_rstrip]->SetTitle("Signal Hits");
      h_mSignalHits_Rstrip[i_rstrip]->Draw();
    }
    else h_mSignalHits_Rstrip[i_rstrip]->Draw("same");

    cout << "i_rstrip = " << i_rstrip << ", weight = " << weight[i_rstrip] << endl;
  }
  leg->Draw("same");


  string figName = Form("./figures/FstNumOfHits_%s.pdf",cmnMode.c_str());
  c_Signal->SaveAs(figName.c_str());
}


