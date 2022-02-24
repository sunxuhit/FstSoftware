#include <stdio.h>
#include "TDatime.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TString.h"

void plotRms(int glbElecApvIdx = 259)
{
  char paraDir[256];

  //load external pedstal/RMS value for APV
  sprintf(paraDir, "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstInstallation/qa/rmsInfo_APV%d.txt", glbElecApvIdx);
  std::cout << "Opening RMS File for APV " << glbElecApvIdx << ": " << paraDir << std::endl;

  TGraph *g_totRms = new TGraph();
  TGraph *g_ranRms = new TGraph();
  int nPoint = 0;

  FILE *file_inRms;
  file_inRms = fopen(paraDir, "r");
  if (file_inRms==0) {
    cout << "Cannot find file" << paraDir << endl;
    exit(0);
  }
  else {
    //LOG(U_FST,"loading pedestals from %s ", paraDir);
    while(!feof(file_inRms)) {
      long runNumber;
      char runDate[256], runTime[256];
      int glbApvIdxTemp = 0, rdoIdxTemp = 0, armIdxTemp = 0, apvIdxTemp = 0, tbIdxTemp = 0;
      float totRmsTemp = 0.0, ranRmsTemp = 0.0;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_inRms) == 0) continue ;
      int ret = sscanf(buff,"%ld %s %s %d %d %d %d %d %f %f",&runNumber,&runDate,&runTime,&glbApvIdxTemp,&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&tbIdxTemp,&totRmsTemp,&ranRmsTemp);
      if(ret!=10) continue;

      TString storeTime = Form("%s %s",runDate,runTime);

      TDatime da(storeTime.Data());
      // UInt_t test = da.Convert();
      // cout << "storeTime: " << storeTime.Data() << ", test = " << test << endl;

      g_totRms->SetPoint(nPoint,da.Convert(),totRmsTemp);
      g_ranRms->SetPoint(nPoint,da.Convert(),ranRmsTemp);
      nPoint++;

      // cout << "runNumber = " << runNumber << ", storeTime = " << storeTime.Data() << ", glbApvIdxTemp = " << glbApvIdxTemp << ", rdoIdxTemp = " << rdoIdxTemp << ", armIdxTemp = " << armIdxTemp << ", apvIdxTemp = " << apvIdxTemp << ", tbIdxTemp = " << tbIdxTemp << ", totRmsTemp = " << totRmsTemp << ", ranRmsTem = " << ranRmsTem << endl;

    }
    fclose(file_inRms);
  }

  TCanvas *c_ranRms = new TCanvas("c_ranRms","c_ranRms",10,10,1600,400);
  c_ranRms->SetLeftMargin(0.15);
  c_ranRms->SetBottomMargin(0.15);
  c_ranRms->SetTicks(1,1);
  c_ranRms->SetGrid(0,0);


  g_ranRms->SetMarkerStyle(24);
  g_ranRms->SetMarkerColor(kRed);
  g_ranRms->GetXaxis()->SetTimeDisplay(1);
  g_ranRms->GetXaxis()->SetNdivisions(510);
  g_ranRms->GetXaxis()->SetTimeFormat("%Y-%m-%d");
  g_ranRms->GetXaxis()->SetTimeOffset(0,"gmt");
  g_ranRms->GetYaxis()->SetRangeUser(10,20);
  g_ranRms->Draw("AP");

  g_totRms->SetMarkerStyle(24);
  g_totRms->SetMarkerColor(kGray+3);
  g_totRms->Draw("P same");

  TString title = Form("RMS of APV%d",glbElecApvIdx);
  TLegend *leg = new TLegend(0.6,0.6,0.8,0.8);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->SetFillStyle(0);
  leg->AddEntry((TObject*)0,title.Data(),"");
  leg->AddEntry(g_totRms,"tot RMS","p");
  leg->AddEntry(g_ranRms,"ran RMS","p");
  leg->Draw("same");

  TString FigName = Form("/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/figures/QA/c_ranRms_APV%d.eps",glbElecApvIdx);
  c_ranRms->SaveAs(FigName.Data());
}
