#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

#include "../../src/FstUtil/FstCons.h"
#include "../../src/FstUtil/FstRawHit.h"
#include "../../src/FstUtil/FstCluster.h"
#include "../../src/FstUtil/FstTrack.h"
#include "../../src/FstUtil/FstEvent.h"

R__LOAD_LIBRARY(../../lib/libFstEvent.dylib)

int plotResidual_ISTClusters()
{
  gStyle->SetOptStat(11111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.15); gStyle->SetStatH(0.25);

  const double phi_rot_ist1 = -0.00113551;
  const double x1_shift = 0.530444;
  const double y1_shift = 1.17133;
  const double phi_rot_ist3 = -0.00379431;
  const double x3_shift = 0.45811;
  const double y3_shift = 1.0116;

  const double xMax = 24.0*FST::pitchColumn; // center of 23 column + 0.5*pitchColumn
  const double xMin = 20.0*FST::pitchColumn; // center of 20 column - 0.5*pitchColumn

  std::string inputfile = "../../output/FstClusters_HV140V_woPed.root";
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;

  TFile *mFile_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_FstEvent = (TTree*)mFile_InPut->Get("mTree_FstEvent");
  FstEvent *mFstEvent = new FstEvent();
  mTree_FstEvent->SetBranchAddress("FstEvent",&mFstEvent);

  TH1F *h_mXResidual_Clusters_Before = new TH1F("h_mXResidual_Clusters_Before","h_mXResidual_Clusters_Before",15,-20.0,20.0);
  TH1F *h_mYResidual_Clusters_Before = new TH1F("h_mYResidual_Clusters_Before","h_mYResidual_Clusters_Before",100,-5.0,5.0);
  TH1F *h_mXResidual_Clusters = new TH1F("h_mXResidual_Clusters","h_mXResidual_Clusters",15,-20.0,20.0);
  TH1F *h_mYResidual_Clusters = new TH1F("h_mYResidual_Clusters","h_mYResidual_Clusters",100,-5.0,5.0);

  long NumOfEvents = (long)mTree_FstEvent->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 500;
  mTree_FstEvent->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_FstEvent->GetEntry(i_event);

    std::vector<FstCluster *> clusterVec_ist1;
    std::vector<FstCluster *> clusterVec_ist2;
    std::vector<FstCluster *> clusterVec_ist3;
    clusterVec_ist1.clear();
    clusterVec_ist2.clear();
    clusterVec_ist3.clear();

    for(int i_cluster = 0; i_cluster < mFstEvent->getNumClusters(); ++i_cluster)
    { // get Clusters info for IST1 IST2 and IST3
      FstCluster *fstCluster = mFstEvent->getCluster(i_cluster);
      if(fstCluster->getLayer() == 1)
      {
	clusterVec_ist1.push_back(fstCluster);
      }
      if(fstCluster->getLayer() == 2)
      {
	clusterVec_ist2.push_back(fstCluster);
      }
      if(fstCluster->getLayer() == 3)
      {
	clusterVec_ist3.push_back(fstCluster);
      }
    }

    if(clusterVec_ist2.size() > 0) // IST2 has at least one hit
    {
      for(int i_ist1 = 0; i_ist1 < clusterVec_ist1.size(); ++i_ist1)
      {
	double x1_ist = clusterVec_ist1[i_ist1]->getMeanX();
	double y1_ist = clusterVec_ist1[i_ist1]->getMeanY();
	const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
	double col1 = clusterVec_ist1[i_ist1]->getMeanColumn();
	double row1 = clusterVec_ist1[i_ist1]->getMeanRow();

	// double x1_corr = x1_ist*TMath::Cos(phi_rot) + y1_ist*TMath::Sin(phi_rot) + x_shift;
	// double y1_corr = y1_ist*TMath::Cos(phi_rot) - x1_ist*TMath::Sin(phi_rot) + y_shift;
	double x1_corr = x1_ist*TMath::Cos(phi_rot_ist1) + y1_ist*TMath::Sin(phi_rot_ist1) + x1_shift;
	double y1_corr = y1_ist*TMath::Cos(phi_rot_ist1) - x1_ist*TMath::Sin(phi_rot_ist1) + y1_shift;

	for(int i_ist3 = 0; i_ist3 < clusterVec_ist3.size(); ++i_ist3)
	{
	  double x3_ist = clusterVec_ist3[i_ist3]->getMeanX();
	  double y3_ist = clusterVec_ist3[i_ist3]->getMeanY();
	  const double z3_ist = 0.0;
	  double col3 = clusterVec_ist3[i_ist3]->getMeanColumn();
	  double row3 = clusterVec_ist3[i_ist3]->getMeanRow();

	  double x3_corr = x3_ist*TMath::Cos(phi_rot_ist3) + y3_ist*TMath::Sin(phi_rot_ist3) + x3_shift;
	  double y3_corr = y3_ist*TMath::Cos(phi_rot_ist3) - x3_ist*TMath::Sin(phi_rot_ist3) + y3_shift;

	  for(int i_ist2 = 0; i_ist2 < clusterVec_ist2.size(); ++i_ist2)
	  {
	    double x2_ist = clusterVec_ist2[i_ist2]->getMeanX();
	    double y2_ist = clusterVec_ist2[i_ist2]->getMeanY();
	    const double z2_ist = FST::pitchLayer23;

	    double x2_proj = x3_ist + (x1_ist-x3_ist)*z2_ist/z1_ist;
	    double y2_proj = y3_ist + (y1_ist-y3_ist)*z2_ist/z1_ist;

	    double x2_corr = x3_corr + (x1_corr-x3_corr)*z2_ist/z1_ist;
	    double y2_corr = y3_corr + (y1_corr-y3_corr)*z2_ist/z1_ist;

	    if( abs(row3-row1) < 17 && x2_proj >= xMin && x2_proj <= xMax)
	    {
	      if( abs(x2_corr - x2_ist) < 6.0 && abs(y2_corr-y2_ist) < 0.6)
	      {
		h_mXResidual_Clusters_Before->Fill(x2_ist-x2_proj);
		h_mYResidual_Clusters_Before->Fill(y2_ist-y2_proj);
		h_mXResidual_Clusters->Fill(x2_ist-x2_corr);
		h_mYResidual_Clusters->Fill(y2_ist-y2_corr);
	      }
	    }
	  }
	}
      }
    }
  }
  // mFile_InPut->Close();

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,800,800);
  c_play->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_play->cd(i_pad+1)->SetLeftMargin(0.15);
    c_play->cd(i_pad+1)->SetBottomMargin(0.15);
    c_play->cd(i_pad+1)->SetTicks(1,1);
    c_play->cd(i_pad+1)->SetGrid(0,0);
  }

  c_play->cd(1);
  h_mXResidual_Clusters_Before->SetTitle("x-residual before correction");
  h_mXResidual_Clusters_Before->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual_Clusters_Before->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual_Clusters_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual_Clusters_Before->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual_Clusters_Before->Draw();
  h_mXResidual_Clusters_Before->Fit("gaus");

  c_play->cd(2);
  h_mYResidual_Clusters_Before->SetTitle("y-residual before correction");
  h_mYResidual_Clusters_Before->GetXaxis()->SetTitle("y-residual (mm)");
  h_mYResidual_Clusters_Before->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual_Clusters_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual_Clusters_Before->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual_Clusters_Before->Draw();
  h_mYResidual_Clusters_Before->Fit("gaus");

  c_play->cd(3);
  h_mXResidual_Clusters->SetTitle("x-residual after correction");
  h_mXResidual_Clusters->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual_Clusters->Draw();
  h_mXResidual_Clusters->Fit("gaus");

  c_play->cd(4);
  h_mYResidual_Clusters->SetTitle("y-residual after correction");
  h_mYResidual_Clusters->GetXaxis()->SetTitle("y-residual (mm)");
  h_mYResidual_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual_Clusters->Draw();
  h_mYResidual_Clusters->Fit("gaus");

  c_play->SaveAs("./figures/Residual_ISTClusters.eps");

  return 1;
}
