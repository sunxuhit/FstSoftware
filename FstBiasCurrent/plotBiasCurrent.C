#include <iostream>
#include "TString.h"
#include "TDatime.h"
#include "TGraph.h"
#include "TStyle.h"

void plotBiasCurrent(int mpodId = 2, int runId = 22)
{
  // read in inner sensor of Disk2 module 4: isegId = 6
  const int iIsegId = 6;
  string inputfileIS = Form("./data/fstmpod0%d_u%d_run%d.txt", mpodId, iIsegId, runId);
  FILE *file_InnerSensor;
  file_InnerSensor = fopen(inputfileIS.c_str(), "r");

  long iPointIS = 0;
  TGraph *g_bCurrentIS = new TGraph();
  if (file_InnerSensor == 0) {
    cout << "Cannot find file " << inputfileIS.c_str() << endl;
    exit(0);
  }
  else {
    char line[80], dt[20];
    long timestamp;
    float bCurrent;
    while(!feof(file_InnerSensor)) {
      if(fgets(line, sizeof(line), file_InnerSensor) == 0) continue;
      int ret = sscanf(&line[20], "%ld %f", &timestamp, &bCurrent);
      if(ret!=2) continue;
      strncpy(dt, line, sizeof(dt)-1);
      dt[19] = '\0';

      // if(bCurrent < 0 || bCurrent > 0.0002) continue;
      if(bCurrent < 0 || bCurrent > 0.0002) continue;
      // cout << "dt = " << dt << ", time = " << TDatime(dt).Convert() << ", timestamp = " << timestamp << ", bCurrent = " << bCurrent << endl;

      g_bCurrentIS->SetPoint(iPointIS,TDatime(dt).Convert(),bCurrent*1.0e6);
      // g_bCurrentIS->SetPoint(iPointIS,timestamp,bCurrent); // GMT time
      iPointIS++;
    }
    fclose(file_InnerSensor);
  }

  // read in outer sensor of Disk2 module 4: isegId = 7
  const int oIsegId = 7;
  string inputfileOS = Form("./data/fstmpod0%d_u%d_run%d.txt", mpodId, oIsegId, runId);
  FILE *file_OuterSensor;
  file_OuterSensor = fopen(inputfileOS.c_str(), "r");

  long iPointOS = 0;
  TGraph *g_bCurrentOS = new TGraph();
  if (file_OuterSensor == 0) {
    cout << "Cannot find file " << inputfileOS.c_str() << endl;
    exit(0);
  }
  else {
    char line[80], dt[20];
    long timestamp;
    float bCurrent;
    while(!feof(file_OuterSensor)) {
      if(fgets(line, sizeof(line), file_OuterSensor) == 0) continue;
      int ret = sscanf(&line[20], "%ld %f", &timestamp, &bCurrent);
      if(ret!=2) continue;
      strncpy(dt, line, sizeof(dt)-1);
      dt[19] = '\0';

      if(bCurrent < 0 || bCurrent > 0.0002) continue;
      // cout << "dt = " << dt << ", time = " << TDatime(dt).Convert() << ", timestamp = " << timestamp << ", bCurrent = " << bCurrent << endl;

      g_bCurrentOS->SetPoint(iPointOS,TDatime(dt).Convert(),bCurrent*1.0e6);
      // g_bCurrentOS->SetPoint(iPointOS,timestamp,bCurrent); // GMT time
      iPointOS++;
    }
    fclose(file_OuterSensor);
  }

  // Draw the graph
  const int pixelScale = 4;
  // gStyle->SetGridStyle(2);
  gStyle->SetGridWidth(1*pixelScale);
  gStyle->SetFrameLineWidth(2*pixelScale);

  TCanvas *c_bCurrent = new TCanvas("c_bCurrent", "c_bCurrent", 2000*pixelScale, 1000*pixelScale);
  c_bCurrent->SetLeftMargin(0.08);
  c_bCurrent->SetRightMargin(0.05);
  c_bCurrent->SetBottomMargin(0.15);
  c_bCurrent->SetGrid(1,0);

  TH1F *h_play = new TH1F("h_play","h_play",1000,0.0,1000.0);
  for(int iBin = 0; iBin < h_play->GetNbinsX(); ++iBin)
  {
    h_play->SetBinContent(iBin+1, -1000.0);
    h_play->SetBinError(iBin+1, 1.0);
  }
  h_play->SetTitle("FST Disk2 Module4");
  h_play->SetStats(0);
  h_play->GetXaxis()->SetTitle("EST Time");
  h_play->GetXaxis()->CenterTitle();
  h_play->GetXaxis()->SetTitleSize(0.05);
  h_play->GetXaxis()->SetTimeDisplay(1);
  h_play->GetXaxis()->SetTimeFormat("%m/%d/%y");
  h_play->GetXaxis()->SetTimeOffset(0);
  h_play->GetXaxis()->SetNdivisions(214);
  // h_play->GetXaxis()->SetLimits(TDatime(2021, 12, 1, 0, 0, 0).Convert(), TDatime(2023, 8, 15, 0, 0, 0).Convert());
  h_play->GetXaxis()->SetLimits(TDatime(2021, 10, 30, 0, 0, 0).Convert(), TDatime(2022, 4, 22, 0, 0, 0).Convert());
  h_play->GetXaxis()->SetLabelSize(0.035);
  // h_play->GetXaxis()->CenterLabels();
  h_play->GetYaxis()->SetTitle("Leak Current (#muA)");
  h_play->GetYaxis()->CenterTitle();
  h_play->GetYaxis()->SetTitleSize(0.05);
  h_play->GetYaxis()->SetTitleOffset(0.5);
  h_play->GetYaxis()->SetLabelSize(0.04);
  h_play->GetYaxis()->SetRangeUser(0.0,0.00003*1.0e6);
  h_play->Draw("hE");

  long tsShift  = 60*60*24*2; // shift 2 days
  const long tsCosmic = TDatime(2021, 11, 11, 0, 0, 0).Convert();
  TLatex *lCosmic;
  lCosmic = new TLatex(tsCosmic+tsShift, 4.7,"Cosmic Start");
  lCosmic->SetTextFont(42);
  lCosmic->SetTextSize(0.04);
  lCosmic->SetTextColor(kRed-7);
  // lCosmic->SetLineWidth(2);
  lCosmic->Draw();
  TArrow *aCosmic = new TArrow(tsCosmic, 1.5, tsCosmic, 4.5, 0.010,"<|");
  aCosmic->SetFillColor(kRed-7);
  aCosmic->SetFillStyle(1002);
  aCosmic->SetLineColor(kRed-7);
  aCosmic->SetLineWidth(3*pixelScale);
  aCosmic->Draw();

  tsShift  = 60*60*24*2; // shift 2 days
  const long tsBeam = TDatime(2021, 12, 15, 0, 0, 0).Convert();
  TLatex *lBeam;
  lBeam = new TLatex(tsBeam+tsShift, 4.7,"Beam Start");
  lBeam->SetTextFont(42);
  lBeam->SetTextSize(0.04);
  lBeam->SetTextColor(kRed-7);
  // lBeam->SetLineWidth(2);
  lBeam->Draw();
  TArrow *aBeam = new TArrow(tsBeam, 1.5, tsBeam, 4.5, 0.010,"<|");
  aBeam->SetFillColor(kRed-7);
  aBeam->SetFillStyle(1002);
  aBeam->SetLineColor(kRed-7);
  aBeam->SetLineWidth(3*pixelScale);
  aBeam->Draw();

  tsShift  = 60*60*24*20; // shift 20 days
  const long tsRunStop = TDatime(2022, 4, 18, 12, 0, 0).Convert();
  TLatex *lRunStop;
  lRunStop = new TLatex(tsRunStop-tsShift, 20.7,"Run22 Stop");
  lRunStop->SetTextFont(42);
  lRunStop->SetTextSize(0.04);
  lRunStop->SetTextColor(kRed-7);
  // lRunStop->SetLineWidth(2);
  lRunStop->Draw();
  TArrow *aRunStop = new TArrow(tsRunStop, 17.5, tsRunStop, 20.5, 0.010,"<|");
  aRunStop->SetFillColor(kRed-7);
  aRunStop->SetFillStyle(1002);
  aRunStop->SetLineColor(kRed-7);
  aRunStop->SetLineWidth(3*pixelScale);
  aRunStop->Draw();

  g_bCurrentIS->SetLineWidth(3*pixelScale);
  g_bCurrentIS->SetLineColor(kRed);
  g_bCurrentIS->Draw("l same");

  g_bCurrentOS->SetLineWidth(3*pixelScale);
  g_bCurrentOS->SetLineColor(kBlue);
  g_bCurrentOS->Draw("l same");

  TLegend *leg = new TLegend(0.25,0.7,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(10);
  leg->AddEntry(g_bCurrentIS,"Inner Sensor","l");
  leg->AddEntry(g_bCurrentOS,"Outer Sensor","l");
  leg->Draw("same");

  c_bCurrent->SaveAs("./figures/Run22LeakCurrent.eps");
  c_bCurrent->SaveAs("./figures/Run22LeakCurrent.png");
}
