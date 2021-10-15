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

void plotSensorPedAdcQA(string module = "Mod04", string hv = "HV140V")
{
  // gStyle->SetOptStat(111111);
  // gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.90); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.35); gStyle->SetStatH(0.35);

  const int probChNumber = 0;
  const int chNumber = 10;
  const int usedTb = 0;

  string inputfile = Form("../../output/noise/%s/TriggerTest/FstPedNoise_%s_%s.root",module.c_str(),module.c_str(),hv.c_str());
  TFile *File_InPut = TFile::Open(inputfile.c_str());
  TH1F *h_mAdc_FST[FST::numAPVs][FST::numChannels][FST::numTBins];
  for(int i_apv = 0; i_apv < FST::numAPVs; ++i_apv)
  {
    for(int i_ch = 0; i_ch < FST::numChannels; ++i_ch)
    {
      for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
      {
	std::string HistName = Form("h_mAdc_FST_Apv%d_Ch%d_Tb%d",i_apv,i_ch,i_tb);
	h_mAdc_FST[i_apv][i_ch][i_tb] = (TH1F*)File_InPut->Get(HistName.c_str());
      }
    }
  }

  TCanvas *c_Adc = new TCanvas("c_Adc","c_Adc",10,10,900,900);
  c_Adc->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++i_pad)
  {
    c_Adc->cd(i_pad+1)->SetLeftMargin(0.1);
    // c_Adc->cd(i_pad+1)->SetRightMargin(0.15);
    c_Adc->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Adc->cd(i_pad+1)->SetTicks(1,1);
    c_Adc->cd(i_pad+1)->SetGrid(0,0);
    c_Adc->cd(i_pad+1)->SetLogy();
  }

  for(int i_apv = 0; i_apv < 8; ++i_apv)
  {
    c_Adc->cd(i_apv+1);
    int maxBin       = h_mAdc_FST[i_apv][chNumber][usedTb]->GetMaximumBin();
    int maxBinCenter = h_mAdc_FST[i_apv][chNumber][usedTb]->GetBinCenter(maxBin);
    cout << "maxBin = " << maxBin << ", maxBinCenter = " << maxBinCenter << endl;

    std::string title = Form("APV %d & Ch %d", i_apv, chNumber);
    if(i_apv == 5) title = Form("APV %d & Ch %d (low Pedestal)", i_apv, chNumber); 
    h_mAdc_FST[i_apv][chNumber][usedTb]->SetTitle(title.c_str());
    h_mAdc_FST[i_apv][chNumber][usedTb]->RebinX(2);
    h_mAdc_FST[i_apv][chNumber][usedTb]->GetXaxis()->SetRangeUser(maxBinCenter-150,maxBinCenter+200);
    h_mAdc_FST[i_apv][chNumber][usedTb]->GetXaxis()->SetNdivisions(505);
    h_mAdc_FST[i_apv][chNumber][usedTb]->GetXaxis()->SetTitle("ADC");
    h_mAdc_FST[i_apv][chNumber][usedTb]->GetYaxis()->SetNdivisions(505);
    h_mAdc_FST[i_apv][chNumber][usedTb]->Draw();
    h_mAdc_FST[i_apv][chNumber][usedTb]->Fit("gaus");
  }

  {
    c_Adc->cd(9);
    int maxBin       = h_mAdc_FST[0][probChNumber][usedTb]->GetMaximumBin();
    int maxBinCenter = h_mAdc_FST[0][probChNumber][usedTb]->GetBinCenter(maxBin);
    cout << "maxBin = " << maxBin << ", maxBinCenter = " << maxBinCenter << endl;

    std::string title = Form("APV %d & Ch %d (Abnormal Noise)", 0, probChNumber);
    h_mAdc_FST[0][probChNumber][usedTb]->SetTitle(title.c_str());
    h_mAdc_FST[0][probChNumber][usedTb]->RebinX(2);
    h_mAdc_FST[0][probChNumber][usedTb]->GetXaxis()->SetRangeUser(maxBinCenter-150,maxBinCenter+200);
    h_mAdc_FST[0][probChNumber][usedTb]->GetXaxis()->SetNdivisions(505);
    h_mAdc_FST[0][probChNumber][usedTb]->GetXaxis()->SetTitle("ADC");
    h_mAdc_FST[0][probChNumber][usedTb]->GetYaxis()->SetNdivisions(505);
    h_mAdc_FST[0][probChNumber][usedTb]->Draw();
    h_mAdc_FST[0][probChNumber][usedTb]->Fit("gaus");
  }

  string outputname = Form("./figures/%s/DAQ/PedAdcQA_%s_%s.pdf",module.c_str(),module.c_str(),hv.c_str());
  c_Adc->SaveAs(outputname.c_str());
}
