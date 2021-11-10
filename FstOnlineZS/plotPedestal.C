#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TString.h>

using namespace std;

void plotPedestal(string runId = "01107734")
{
  // read in ADC info for all events
  float pedestals[6][3][24][128][9];
  float totStdDev[6][3][24][128][9];
  float ranStdDev[6][3][24][128][9];
  for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
  {
    for(int i_arm = 0; i_arm < 3; ++i_arm)
    {
      for(int i_apv = 0; i_apv < 24; ++i_apv)
      {
	for(int i_ch = 0; i_ch < 128; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < 9; ++i_tb)
	  {
	    pedestals[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1;
	    totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1;
	    ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb] = -1;
	  }
	}
      }
    }
  }

  FILE *file_pedestals;
  char inputfile[256]; 
  // read in pedestal file on fst01
  sprintf(inputfile, "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstInstallation/daqtest/fst_s1_pedestals_%s_GOOD.txt",runId.c_str());
  cout << "Read in pedestal from fst01: " << inputfile << endl;
  file_pedestals = fopen(inputfile,"r");
  if(file_pedestals == 0) 
  {
    cout << "can't open input file " << inputfile << endl;;
  }
  else
  {
    cout << "Start to read in " << inputfile << endl;;
    while(!feof(file_pedestals)) 
    {
      int rdoIdxTemp=-1, armIdxTemp=-1, apvIdxTemp=-1, chanIdxTemp=-1, tbIdxTemp=-1; 
      float pedestalsTemp=-1.0, totStdDevTemp=-1.0, ranStdDevTemp=-1.0;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_pedestals) == NULL) continue ;
      int ret = sscanf(buff,"%d %d %d %d %d %f %f %f",&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&pedestalsTemp,&totStdDevTemp,&ranStdDevTemp);
      if(ret != 8) continue;
      pedestals[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = pedestalsTemp;
      totStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = totStdDevTemp;
      ranStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = ranStdDevTemp;
      // if(rdoIdxTemp == 1 && armIdxTemp == 0 && apvIdxTemp == 14 && chanIdxTemp == 24 && tbIdxTemp == 5) cout << "pedestalsTemp = " << pedestalsTemp << ", totStdDevTemp = " << totStdDevTemp << ", ranStdDevTemp = " << ranStdDevTemp << endl;
      // if(rdoIdxTemp == 1 && armIdxTemp == 0 && apvIdxTemp == 14 && chanIdxTemp == 24 && tbIdxTemp == 5) cout << "pedestals = " << pedestals[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << ", totStdDev = " << totStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << ", ranStdDev = " << ranStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << endl;
    }
  }
  fclose(file_pedestals);

  // read in pedestal file on fst02
  sprintf(inputfile, "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstInstallation/daqtest/fst_s2_pedestals_%s_GOOD.txt",runId.c_str());
  cout << "Read in pedestal from fst02: " << inputfile << endl;
  file_pedestals = fopen(inputfile,"r");
  if(file_pedestals == 0) 
  {
    cout << "can't open input file " << inputfile << endl;;
  }
  else
  {
    cout << "Start to read in " << inputfile << endl;;
    while(!feof(file_pedestals)) 
    {
      int rdoIdxTemp=-1, armIdxTemp=-1, apvIdxTemp=-1, chanIdxTemp=-1, tbIdxTemp=-1; 
      float pedestalsTemp=-1.0, totStdDevTemp=-1.0, ranStdDevTemp=-1.0;
      char buff[256];

      if(fgets(buff,sizeof(buff),file_pedestals) == NULL) continue ;
      int ret = sscanf(buff,"%d %d %d %d %d %f %f %f",&rdoIdxTemp,&armIdxTemp,&apvIdxTemp,&chanIdxTemp,&tbIdxTemp,&pedestalsTemp,&totStdDevTemp,&ranStdDevTemp);
      if(ret != 8) continue;
      pedestals[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = pedestalsTemp;
      totStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = totStdDevTemp;
      ranStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] = ranStdDevTemp;
      // if(rdoIdxTemp == 2 && armIdxTemp == 0 && apvIdxTemp == 14 && chanIdxTemp == 24 && tbIdxTemp == 5) cout << "pedestalsTemp = " << pedestalsTemp << ", totStdDevTemp = " << totStdDevTemp << ", ranStdDevTemp = " << ranStdDevTemp << endl;
      // if(rdoIdxTemp == 2 && armIdxTemp == 0 && apvIdxTemp == 14 && chanIdxTemp == 24 && tbIdxTemp == 5) cout << "pedestals = " << pedestals[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << ", totStdDev = " << totStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << ", ranStdDev = " << ranStdDev[rdoIdxTemp-1][armIdxTemp][apvIdxTemp][chanIdxTemp][tbIdxTemp] << endl;
    }
  }
  fclose(file_pedestals);

  TGraph *g_pedestals[36][9];
  TGraph *g_totStdDev[36][9];
  TGraph *g_ranStdDev[36][9];
  for(int i_mod = 0; i_mod < 36; ++i_mod)
  {
    for(int i_tb = 0; i_tb < 9; ++i_tb)
    {
      g_pedestals[i_mod][i_tb] = new TGraph();
      g_totStdDev[i_mod][i_tb] = new TGraph();
      g_ranStdDev[i_mod][i_tb] = new TGraph();
    }
  }

  TH1F *h_frame = new TH1F("h_frame","h_frame",1536,-0.5,1535.5);
  for(int i_bin = 0; i_bin < 1536; ++i_bin)
  {
    h_frame->SetBinContent(i_bin+1, -100.0);
    h_frame->SetBinError(i_bin+1, 1.0);
  }
  h_frame->SetStats(0);
  h_frame->GetXaxis()->SetTitle("channel Id");;
  h_frame->GetXaxis()->SetNdivisions(-12);
  h_frame->GetYaxis()->SetTitle("ADC");

  for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
  {
    for(int i_arm = 0; i_arm < 3; ++i_arm)
    {
      for(int i_apv = 0; i_apv < 24; ++i_apv)
      {
	int portIdx = 0;
	if(i_apv >= 12) portIdx = 1;

	int diskIdx   = i_rdo/2+1;                       // 1-3
        int moduleIdx = i_rdo%2*6 + i_arm*2 + portIdx+1; // 1-12
	int glbModuleIdx  = (diskIdx-1)*12 + moduleIdx;  // 1-36
	// cout << "i_rdo = " << i_rdo << ", i_arm = " << i_arm << ", i_apv = " << i_apv << endl;
	// cout << "diskIdx = " << diskIdx << ", moduleIdx = " << moduleIdx << ", glbModuleIdx = " << glbModuleIdx << endl;
	for(int i_ch = 0; i_ch < 128; ++i_ch)
	{
	  for(int i_tb = 0; i_tb < 9; ++i_tb)
	  {
	    int i_point = (i_apv - 12*portIdx)*128 + i_ch;
	    g_pedestals[glbModuleIdx-1][i_tb]->SetPoint(i_point,i_point,pedestals[i_rdo][i_arm][i_apv][i_ch][i_tb]);
	    g_totStdDev[glbModuleIdx-1][i_tb]->SetPoint(i_point,i_point,totStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb]);
	    g_ranStdDev[glbModuleIdx-1][i_tb]->SetPoint(i_point,i_point,ranStdDev[i_rdo][i_arm][i_apv][i_ch][i_tb]);
	  }
	}
      }
    }
  }

  string outputname, outputStart, outputStop;
  outputname = Form("./figures/fst_pedestals_%s.pdf",runId.c_str());
  TCanvas *c_pedestals = new TCanvas("c_pedestals","c_pedestals",10,10,800,600);
  c_pedestals->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_pedestals->cd(i_pad+1)->SetLeftMargin(0.1);
    c_pedestals->cd(i_pad+1)->SetBottomMargin(0.15);
    c_pedestals->cd(i_pad+1)->SetTicks(1,1);
    c_pedestals->cd(i_pad+1)->SetGrid(1,0);
  }
  outputStart = outputname + "[";
  c_pedestals->Print(outputStart.c_str());
  for(int i_tb = 0; i_tb < 9; ++i_tb)
  {
    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_mod = 0; i_mod < 6; ++i_mod)
      {
	int glbModuleIdx = i_rdo*6 + i_mod + 1;
        int lclModuleIdx = i_rdo%2*6 + i_mod + 1; // 1-12
	c_pedestals->cd(i_mod+1);
	string title = Form("Pedestal: RDO%d Module%d TB%d", i_rdo+1, lclModuleIdx, i_tb);
	h_frame->SetTitle(title.c_str());
	h_frame->GetYaxis()->SetRangeUser(0,4096);
	h_frame->DrawCopy("pE");
	g_pedestals[glbModuleIdx-1][i_tb]->Draw("P same");
      }
      c_pedestals->Update();
      c_pedestals->Print(outputname.c_str());
    }
  }
  outputStop = outputname + "]";
  c_pedestals->Print(outputStop.c_str());

  outputname = Form("./figures/fst_totStdDev_%s.pdf",runId.c_str());
  TCanvas *c_totStdDev = new TCanvas("c_totStdDev","c_totStdDev",10,10,800,600);
  c_totStdDev->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_totStdDev->cd(i_pad+1)->SetLeftMargin(0.1);
    c_totStdDev->cd(i_pad+1)->SetBottomMargin(0.15);
    c_totStdDev->cd(i_pad+1)->SetTicks(1,1);
    c_totStdDev->cd(i_pad+1)->SetGrid(1,0);
  }
  outputStart = outputname + "[";
  c_totStdDev->Print(outputStart.c_str());
  for(int i_tb = 0; i_tb < 9; ++i_tb)
  {
    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_mod = 0; i_mod < 6; ++i_mod)
      {
	int glbModuleIdx = i_rdo*6 + i_mod + 1;
        int lclModuleIdx = i_rdo%2*6 + i_mod + 1; // 1-12
	c_totStdDev->cd(i_mod+1);
	string title = Form("Total Noise: RDO%d Module%d TB%d", i_rdo+1, lclModuleIdx, i_tb);
	h_frame->SetTitle(title.c_str());
	h_frame->GetYaxis()->SetRangeUser(-10,100);
	h_frame->DrawCopy("pE");
	g_totStdDev[glbModuleIdx-1][i_tb]->Draw("P same");
      }
      c_totStdDev->Update();
      c_totStdDev->Print(outputname.c_str());
    }
  }
  outputStop = outputname + "]";
  c_totStdDev->Print(outputStop.c_str());


  outputname = Form("./figures/fst_ranStdDev_%s.pdf",runId.c_str());
  TCanvas *c_ranStdDev = new TCanvas("c_ranStdDev","c_ranStdDev",10,10,800,600);
  c_ranStdDev->Divide(2,3);
  for(int i_pad = 0; i_pad < 6; ++i_pad)
  {
    c_ranStdDev->cd(i_pad+1)->SetLeftMargin(0.1);
    c_ranStdDev->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ranStdDev->cd(i_pad+1)->SetTicks(1,1);
    c_ranStdDev->cd(i_pad+1)->SetGrid(1,0);
  }
  outputStart = outputname + "[";
  c_ranStdDev->Print(outputStart.c_str());
  for(int i_tb = 0; i_tb < 9; ++i_tb)
  {
    for(int i_rdo = 0; i_rdo < 6; ++i_rdo)
    {
      for(int i_mod = 0; i_mod < 6; ++i_mod)
      {
	int glbModuleIdx = i_rdo*6 + i_mod + 1;
        int lclModuleIdx = i_rdo%2*6 + i_mod + 1; // 1-12
	c_ranStdDev->cd(i_mod+1);
	string title = Form("Random Noise: RDO%d Module%d TB%d", i_rdo+1, lclModuleIdx, i_tb);
	h_frame->SetTitle(title.c_str());
	h_frame->GetYaxis()->SetRangeUser(-10,100);
	h_frame->DrawCopy("pE");
	g_ranStdDev[glbModuleIdx-1][i_tb]->Draw("P same");
      }
      c_ranStdDev->Update();
      c_ranStdDev->Print(outputname.c_str());
    }
  }
  outputStop = outputname + "]";
  c_ranStdDev->Print(outputStop.c_str());
}
