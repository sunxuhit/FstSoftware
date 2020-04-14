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

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/lib/libFstEvent.dylib)

int plotResidual_ISTHits()
{
  gStyle->SetOptStat(11111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.95);
  gStyle->SetStatW(0.15); gStyle->SetStatH(0.25);

  std::string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstClusters_HV140V_woPed.root";
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;

  TFile *mFile_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_FstEvent = (TTree*)mFile_InPut->Get("mTree_FstEvent");
  FstEvent *mFstEvent = new FstEvent();
  mTree_FstEvent->SetBranchAddress("FstEvent",&mFstEvent);

  TH1F *h_mXResidual_Hits_Before = new TH1F("h_mXResidual_Hits_Before","h_mXResidual_Hits_Before",100,-9.0,9.0);
  TH1F *h_mYResidual_Hits_Before = new TH1F("h_mYResidual_Hits_Before","h_mYResidual_Hits_Before",100,-9.0,9.0);
  TH1F *h_mXResidual_Hits = new TH1F("h_mXResidual_Hits","h_mXResidual_Hits",100,-9.0,9.0);
  TH1F *h_mYResidual_Hits = new TH1F("h_mYResidual_Hits","h_mYResidual_Hits",100,-5.0,5.0);

  long NumOfEvents = (long)mTree_FstEvent->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 500;
  mTree_FstEvent->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_FstEvent->GetEntry(i_event);

    std::vector<FstRawHit *> hitVec_ist1;
    std::vector<FstRawHit *> hitVec_ist2;
    std::vector<FstRawHit *> hitVec_ist3;
    hitVec_ist1.clear();
    hitVec_ist2.clear();
    hitVec_ist3.clear();

    for(int i_hit = 0; i_hit < mFstEvent->getNumRawHits(); ++i_hit)
    { // get Hits info for IST1 IST2 and IST3
      FstRawHit *fstRawHit = mFstEvent->getRawHit(i_hit);
      if(fstRawHit->getLayer() == 1)
      {
	hitVec_ist1.push_back(fstRawHit);
      }
      if(fstRawHit->getLayer() == 2)
      {
	hitVec_ist2.push_back(fstRawHit);
      }
      if(fstRawHit->getLayer() == 3)
      {
	hitVec_ist3.push_back(fstRawHit);
      }
    }

    // double phi_rot = -0.00453642;
    // double x_shift = 0.770364;
    // double y_shift = 0.818041;
    // const double phi_rot_ist1 = -0.000857538;
    // const double phi_rot_ist3 = -0.0100353;
    // const double x_shift      = 0.787765;
    // const double y_shift      = 0.638893;
    const double phi_rot_ist1 = 0.000641529;
    const double phi_rot_ist3 = -0.0073076;
    const double x_shift = 0.745125;
    const double y_shift = 0.930825;
    // const double xMax         = 23.0*FST::pitchColumn + 0.5*FST::pitchColumn;
    // const double xMin         = 20.0*FST::pitchColumn + 0.5*FST::pitchColumn;
    if(hitVec_ist2.size() > 0) // IST2 has at least one hit
    {
      for(int i_ist1 = 0; i_ist1 < hitVec_ist1.size(); ++i_ist1)
      {
	double x1_ist = hitVec_ist1[i_ist1]->getPosX();
	double y1_ist = hitVec_ist1[i_ist1]->getPosY();
	const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;

	// double x1_corr = x1_ist*TMath::Cos(phi_rot) + y1_ist*TMath::Sin(phi_rot) + x_shift;
	// double y1_corr = y1_ist*TMath::Cos(phi_rot) - x1_ist*TMath::Sin(phi_rot) + y_shift;
	double x1_corr = x1_ist*TMath::Cos(phi_rot_ist1) + y1_ist*TMath::Sin(phi_rot_ist1) + x_shift;
	double y1_corr = y1_ist*TMath::Cos(phi_rot_ist1) - x1_ist*TMath::Sin(phi_rot_ist1) + y_shift;

	for(int i_ist3 = 0; i_ist3 < hitVec_ist3.size(); ++i_ist3)
	{
	  double x3_ist = hitVec_ist3[i_ist3]->getPosX();
	  double y3_ist = hitVec_ist3[i_ist3]->getPosY();
	  const double z3_ist = 0.0;

	  double x3_corr = x3_ist*TMath::Cos(phi_rot_ist3) + y3_ist*TMath::Sin(phi_rot_ist3) + x_shift;
	  double y3_corr = y3_ist*TMath::Cos(phi_rot_ist3) - x3_ist*TMath::Sin(phi_rot_ist3) + y_shift;

	  for(int i_ist2 = 0; i_ist2 < hitVec_ist2.size(); ++i_ist2)
	  {
	    double x2_ist = hitVec_ist2[i_ist2]->getPosX();
	    double y2_ist = hitVec_ist2[i_ist2]->getPosY();
	    const double z2_ist = FST::pitchLayer23;

	    double x2_proj = x3_ist + (x1_ist-x3_ist)*z2_ist/z1_ist;
	    double y2_proj = y3_ist + (y1_ist-y3_ist)*z2_ist/z1_ist;
	    h_mXResidual_Hits_Before->Fill(x2_ist-x2_proj);
	    h_mYResidual_Hits_Before->Fill(y2_ist-y2_proj);

	    double x2_corr = x3_corr + (x1_corr-x3_corr)*z2_ist/z1_ist;
	    double y2_corr = y3_corr + (y1_corr-y3_corr)*z2_ist/z1_ist;
	    h_mXResidual_Hits->Fill(x2_ist-x2_corr);
	    h_mYResidual_Hits->Fill(y2_ist-y2_corr);
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
  h_mXResidual_Hits_Before->SetTitle("x-residual before correction");
  h_mXResidual_Hits_Before->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual_Hits_Before->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual_Hits_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual_Hits_Before->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual_Hits_Before->Draw();
  h_mXResidual_Hits_Before->Fit("gaus");

  c_play->cd(2);
  h_mYResidual_Hits_Before->SetTitle("y-residual before correction");
  h_mYResidual_Hits_Before->GetXaxis()->SetTitle("y-residual (mm)");
  h_mYResidual_Hits_Before->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual_Hits_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual_Hits_Before->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual_Hits_Before->Draw();
  h_mYResidual_Hits_Before->Fit("gaus");

  c_play->cd(3);
  h_mXResidual_Hits->SetTitle("x-residual after correction");
  h_mXResidual_Hits->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual_Hits->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual_Hits->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual_Hits->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual_Hits->Draw();
  h_mXResidual_Hits->Fit("gaus");

  c_play->cd(4);
  h_mYResidual_Hits->SetTitle("y-residual after correction");
  h_mYResidual_Hits->GetXaxis()->SetTitle("y-residual (mm)");
  h_mYResidual_Hits->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual_Hits->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual_Hits->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual_Hits->Draw();
  h_mYResidual_Hits->Fit("gaus");

  c_play->SaveAs("Residual_ISTHits.eps");

  return 1;
}
