#include <iostream>
#include "TString.h"
#include "TDatime.h"
#include "TGraph.h"

void plotBiasCurrent(int mpodId = 1, int isegId = 1, int runId = 23)
{
  string inputfile = Form("./data/fstmpod0%d_u%d_run%d.txt", mpodId, isegId, runId);
  FILE *file_timeInfo;
  file_timeInfo = fopen(inputfile.c_str(), "r");

  int iPoint = 0;
  TGraph *g_bCurrent = new TGraph();

  char line[80];
  char dt[20];
  float bCurrent;
  if (file_timeInfo == 0) {
    cout << "Cannot find file " << inputfile.c_str() << endl;
    exit(0);
  }
  else {
    while(!feof(file_timeInfo)) {
      if(fgets(line, sizeof(line), file_timeInfo) == 0) continue;
      int ret = sscanf(&line[20], "%f", &bCurrent);
      if(ret!=1) continue;
      strncpy(dt, line, sizeof(dt)-1);
      dt[19] = '\0';
      // cout << "dt = " << dt << ", time = " << TDatime(dt).Convert() << ", bCurrent = " << bCurrent << endl;

      g_bCurrent->SetPoint(iPoint,TDatime(dt).Convert(),bCurrent);
      iPoint++;
    }
    fclose(file_timeInfo);
  }

  // Draw the graph
  TCanvas *c_bCurrent = new TCanvas("c_bCurrent", "c_bCurrent", 1000, 500);
  c_bCurrent->SetLeftMargin(0.07);
  c_bCurrent->SetRightMargin(0.04);

  // Make the X axis labelled with time.
  g_bCurrent->GetXaxis()->SetTimeDisplay(1);
  g_bCurrent->GetXaxis()->CenterTitle();
  g_bCurrent->GetXaxis()->SetTimeFormat("%m/%d/%y");
  g_bCurrent->GetXaxis()->SetTimeOffset(0);
  g_bCurrent->GetXaxis()->SetNdivisions(-219);
  g_bCurrent->GetXaxis()->SetLimits(TDatime(2022, 1, 1, 0, 0, 0).Convert(), TDatime(2023, 6, 30, 0, 0, 0).Convert());
  g_bCurrent->GetXaxis()->SetLabelSize(0.025);
  g_bCurrent->GetXaxis()->CenterLabels();

  TH1F *h_play = new TH1F("h_play","h_play",1000,0.0,1000.0);
  for(int iBin = 0; iBin < h_play->GetNbinsX(); ++iBin)
  {
    h_play->SetBinContent(iBin+1, -1000.0);
    h_play->SetBinError(iBin+1, 1.0);
  }
  h_play->GetXaxis()->SetTimeDisplay(1);
  h_play->GetXaxis()->CenterTitle();
  h_play->GetXaxis()->SetTimeFormat("%m/%d/%y");
  h_play->GetXaxis()->SetTimeOffset(0);
  h_play->GetXaxis()->SetNdivisions(-219);
  h_play->GetXaxis()->SetLimits(TDatime(2022, 1, 1, 0, 0, 0).Convert(), TDatime(2023, 6, 30, 0, 0, 0).Convert());
  h_play->GetXaxis()->SetLabelSize(0.025);
  h_play->GetXaxis()->CenterLabels();
  h_play->GetYaxis()->SetRangeUser(0.0,0.0002);
  h_play->Draw("hE");

  g_bCurrent->SetLineWidth(3);
  g_bCurrent->SetLineColor(kBlue);
  g_bCurrent->Draw("l same");
}
