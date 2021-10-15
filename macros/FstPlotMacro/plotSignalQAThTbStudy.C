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
#include "../FstPlotMacro/draw.h"
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotSignalQAThTbStudy(string mod = "Mod03", string hv = "HV70V", bool isSavePed = false, bool isApplyCMNCorr = true, float nFstThresholdCut2 = 2.5, float nFstThresholdCut1 = 3.5)
{
  //bool isSavePed             = true;
  //bool isApplyCMNCorr        = true;
  
  const int nFHC             = 4;
  float nFstHitsCut[nFHC]    = {3.0,3.5,4.0,4.5};
  
  const int nTB              = 3;
  int numOfUsedTimeBins[nTB] = {1,2,3};
  
  //const int nFHC             = 2;
  //float nFstHitsCut[nFHC]    = {3.5,4.0};
  
  //const int nTB              = 1;
  //int numOfUsedTimeBins[nTB] = {2};
  
  //float nFstThresholdCut2    = 2.5;
  //float nFstThresholdCut1    = 3.5;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  TFile *File_InPut[nFHC][nTB];
  TH1F *h_mFstScanClustersSignal[nFHC][nTB][FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersNoise[nFHC][nTB][FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersSNRatio[nFHC][nTB][FST::mFstNumSensorsPerModule];

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
  {
    for(int i_TB = 0; i_TB < nTB; ++i_TB)
    {
      string inputfile = Form("../../output/configuration/FstQAStudy_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB],nFstThresholdCut2,nFstThresholdCut1,pedMode.c_str(),cmnMode.c_str());
      std::cout << "Opening File " << inputfile << std::endl;
      File_InPut[i_FHC][i_TB] = TFile::Open(inputfile.c_str());
      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        string HistName;
        HistName = Form("h_mFstScanClustersSignal_Sensor%d",i_sensor);
        h_mFstScanClustersSignal[i_FHC][i_TB][i_sensor] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersNoise_Sensor%d",i_sensor);
        h_mFstScanClustersNoise[i_FHC][i_TB][i_sensor] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersSNRatio_Sensor%d",i_sensor);
        h_mFstScanClustersSNRatio[i_FHC][i_TB][i_sensor] = (TH1F*)File_InPut[i_FHC][i_TB]->Get(HistName.c_str());
      }
    }
  }

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  //________________________________PLOTTING SIGNAL QA________________________________//

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,400*nFHC,400*nTB);
  c_Signal->Divide(nFHC,nTB);
  for(int i_pad = 0; i_pad < nFHC*nTB; ++i_pad)
  {
    c_Signal->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Signal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Signal->cd(i_pad+1)->SetTicks(1,1);
    c_Signal->cd(i_pad+1)->SetGrid(0,0);
  }

  string outputname = Form("./figures/SignalQAThTb%s_Ped%1.1fPed%1.1f.pdf",hv.c_str(),nFstThresholdCut2,nFstThresholdCut1);
  string output_start = Form("%s[",outputname.c_str());
  c_Signal->Print(output_start.c_str());

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
    {
      for(int i_TB = 0; i_TB < nTB; ++i_TB)
      {
        c_Signal->cd(i_TB*nFHC+i_FHC+1)->Clear();
        c_Signal->cd(i_TB*nFHC+i_FHC+1);
        string title = Form("Signal Sensor %d Th %1.1f Tb %d",i_sensor,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);

        h_mFstScanClustersSignal[i_FHC][i_TB][i_sensor]->SetTitle(title.c_str());
        h_mFstScanClustersSignal[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitle("ADC");
  	h_mFstScanClustersSignal[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitleSize(0.06);
  	h_mFstScanClustersSignal[i_FHC][i_TB][i_sensor]->Draw("HIST"); 
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str());

    for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
    {
      for(int i_TB = 0; i_TB < nTB; ++i_TB)
      {
        c_Signal->cd(i_TB*nFHC+i_FHC+1)->Clear();
        c_Signal->cd(i_TB*nFHC+i_FHC+1);
        string title = Form("Noise Sensor %d Th %1.1f Tb %d",i_sensor,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);

        h_mFstScanClustersNoise[i_FHC][i_TB][i_sensor]->SetTitle(title.c_str());
        h_mFstScanClustersNoise[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitle("Noise ADC");
  	h_mFstScanClustersNoise[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitleSize(0.06);
  	h_mFstScanClustersNoise[i_FHC][i_TB][i_sensor]->Draw("HIST"); 
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str());   

    for(int i_FHC = 0; i_FHC < nFHC; ++i_FHC)
    {
      for(int i_TB = 0; i_TB < nTB; ++i_TB)
      {
        c_Signal->cd(i_TB*nFHC+i_FHC+1)->Clear();
        c_Signal->cd(i_TB*nFHC+i_FHC+1);
        string title = Form("S/N Sensor %d Th %1.1f Tb %d",i_sensor,nFstHitsCut[i_FHC],numOfUsedTimeBins[i_TB]);

        h_mFstScanClustersSNRatio[i_FHC][i_TB][i_sensor]->SetTitle(title.c_str());
        h_mFstScanClustersSNRatio[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitle("S/N");
  	h_mFstScanClustersSNRatio[i_FHC][i_TB][i_sensor]->GetXaxis()->SetTitleSize(0.06);
  	h_mFstScanClustersSNRatio[i_FHC][i_TB][i_sensor]->Draw("HIST"); 
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str()); 
 
  }
  
  string output_stop = Form("%s]",outputname.c_str());
  c_Signal->Print(output_stop.c_str()); // close pdf file

}
