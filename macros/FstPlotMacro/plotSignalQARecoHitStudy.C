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

void plotSignalQARecoHitStudy(string mod = "Mod03", string hv = "HV70V", bool isSavePed = false, bool isApplyCMNCorr = true, float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2)
{
  //bool isSavePed             = true;
  //bool isApplyCMNCorr        = true;
  
  const int nFHC2            = 3;
  float nFstThresholdCut2[nFHC2] = {2.0,2.5,3.0};
  
  const int nFHC1            = 3;
  float nFstThresholdCut1[nFHC1] = {2.5,3.0,3.5};
  
  //float nFstThresholdCut2    = 2.5;
  //float nFstThresholdCut1    = 3.5;

  std::string pedMode = "withPed";
  if(!isSavePed) pedMode = "woPed";
  std::string cmnMode = "withCMNCorr";
  if(!isApplyCMNCorr) cmnMode = "woCMNCorr";

  TFile *File_InPut[nFHC2][nFHC1];
  TH1F *h_mFstScanClustersSignal[nFHC2][nFHC1][FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersNoise[nFHC2][nFHC1][FST::mFstNumSensorsPerModule];
  TH1F *h_mFstScanClustersSNRatio[nFHC2][nFHC1][FST::mFstNumSensorsPerModule];

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  for(int i_FHC2 = 0; i_FHC2 < nFHC2; ++i_FHC2)
  {
    for(int i_FHC1 = i_FHC2; i_FHC1 < nFHC1; ++i_FHC1)
    {
      string inputfile = Form("../../output/configuration/FstQAStudy_%s_%s_Th%1.1fTb%dPed%1.1fPed%1.1f_%s_%s.root",mod.c_str(),hv.c_str(),nFstHitsCut,numOfUsedTimeBins,nFstThresholdCut2[i_FHC2],nFstThresholdCut1[i_FHC1],pedMode.c_str(),cmnMode.c_str());
      std::cout << "Opening File " << inputfile << std::endl;
      File_InPut[i_FHC2][i_FHC1] = TFile::Open(inputfile.c_str());
      for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
      {
        string HistName;
        HistName = Form("h_mFstScanClustersSignal_Sensor%d",i_sensor);
        h_mFstScanClustersSignal[i_FHC2][i_FHC1][i_sensor] = (TH1F*)File_InPut[i_FHC2][i_FHC1]->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersNoise_Sensor%d",i_sensor);
        h_mFstScanClustersNoise[i_FHC2][i_FHC1][i_sensor] = (TH1F*)File_InPut[i_FHC2][i_FHC1]->Get(HistName.c_str());
        HistName = Form("h_mFstScanClustersSNRatio_Sensor%d",i_sensor);
        h_mFstScanClustersSNRatio[i_FHC2][i_FHC1][i_sensor] = (TH1F*)File_InPut[i_FHC2][i_FHC1]->Get(HistName.c_str());
      }
    }
  }

  //________________________________READING IN DATA AND FILLING HISTOGRAMS________________________________//

  //________________________________PLOTTING SIGNAL QA________________________________//

  TCanvas *c_Signal = new TCanvas("c_Signal","c_Signal",10,10,400*nFHC2,400*nFHC1);
  c_Signal->Divide(nFHC2,nFHC1);
  for(int i_pad = 0; i_pad < nFHC2*nFHC1; ++i_pad)
  {
    c_Signal->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Signal->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Signal->cd(i_pad+1)->SetTicks(1,1);
    c_Signal->cd(i_pad+1)->SetGrid(0,0);
  }

  string outputname = Form("./figures/SignalQARecoHit%s_Th%1.1fTb%d.pdf",hv.c_str(),nFstHitsCut,numOfUsedTimeBins);
  string output_start = Form("%s[",outputname.c_str());
  c_Signal->Print(output_start.c_str());

  for(int i_sensor = 0; i_sensor < FST::mFstNumSensorsPerModule; ++i_sensor)
  {
    for(int i_FHC2 = 0; i_FHC2 < nFHC2; ++i_FHC2)
    {
      for(int i_FHC1 = i_FHC2; i_FHC1 < nFHC1; ++i_FHC1)
      {
        c_Signal->cd(i_FHC1*nFHC2+i_FHC2+1)->Clear();
        c_Signal->cd(i_FHC1*nFHC2+i_FHC2+1);
        string title = Form("Signal Sensor %d Ped %1.1f Ped %1.1f",i_sensor,nFstThresholdCut2[i_FHC2],nFstThresholdCut1[i_FHC1]);

        h_mFstScanClustersSignal[i_FHC2][i_FHC1][i_sensor]->SetTitle(title.c_str());
        h_mFstScanClustersSignal[i_FHC2][i_FHC1][i_sensor]->GetXaxis()->SetTitle("ADC");
  	h_mFstScanClustersSignal[i_FHC2][i_FHC1][i_sensor]->GetXaxis()->SetTitleSize(0.06);
  	h_mFstScanClustersSignal[i_FHC2][i_FHC1][i_sensor]->Draw("HIST"); 
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str());

    for(int i_FHC2 = 0; i_FHC2 < nFHC2; ++i_FHC2)
    {
      for(int i_FHC1 = i_FHC2; i_FHC1 < nFHC1; ++i_FHC1)
      {
        c_Signal->cd(i_FHC1*nFHC2+i_FHC2+1)->Clear();
        c_Signal->cd(i_FHC1*nFHC2+i_FHC2+1);
        string title = Form("Noise Sensor %d Ped %1.1f Ped %1.1f",i_sensor,nFstThresholdCut2[i_FHC2],nFstThresholdCut1[i_FHC1]);

        h_mFstScanClustersNoise[i_FHC2][i_FHC1][i_sensor]->SetTitle(title.c_str());
        h_mFstScanClustersNoise[i_FHC2][i_FHC1][i_sensor]->GetXaxis()->SetTitle("Noise ADC");
  	h_mFstScanClustersNoise[i_FHC2][i_FHC1][i_sensor]->GetXaxis()->SetTitleSize(0.06);
  	h_mFstScanClustersNoise[i_FHC2][i_FHC1][i_sensor]->Draw("HIST"); 
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str());   

    for(int i_FHC2 = 0; i_FHC2 < nFHC2; ++i_FHC2)
    {
      for(int i_FHC1 = i_FHC2; i_FHC1 < nFHC1; ++i_FHC1)
      {
        c_Signal->cd(i_FHC1*nFHC2+i_FHC2+1)->Clear();
        c_Signal->cd(i_FHC1*nFHC2+i_FHC2+1);
        string title = Form("S/N Sensor %d Ped %1.1f Ped %1.1f",i_sensor,nFstThresholdCut2[i_FHC2],nFstThresholdCut1[i_FHC1]);

        h_mFstScanClustersSNRatio[i_FHC2][i_FHC1][i_sensor]->SetTitle(title.c_str());
        h_mFstScanClustersSNRatio[i_FHC2][i_FHC1][i_sensor]->GetXaxis()->SetTitle("S/N");
  	h_mFstScanClustersSNRatio[i_FHC2][i_FHC1][i_sensor]->GetXaxis()->SetTitleSize(0.06);
  	h_mFstScanClustersSNRatio[i_FHC2][i_FHC1][i_sensor]->Draw("HIST"); 
      }
    }
    c_Signal->Update();
    c_Signal->Print(outputname.c_str()); 
 
  }
  
  string output_stop = Form("%s]",outputname.c_str());
  c_Signal->Print(output_stop.c_str()); // close pdf file

}
