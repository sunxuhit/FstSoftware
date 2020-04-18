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

int plotResidual_FSTClusters_2Layer()
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

  // const double phi_rot_ist2 = 0.0392119; // aligned to FST with 3-layer tracking
  // const double x2_shift = 127.899;
  // const double y2_shift = -21.3293;
  const double phi_rot_ist2 = 0.0438495; // aligned IST2 to FST with 2-layer tracking
  const double x2_shift = 134.99;
  const double y2_shift = -20.7848;

  // std::string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstClusters_HV140V_woPed.root";
  std::string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/OutPut/FstClusters_HV140V_withPed.root";
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;

  TFile *mFile_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_FstEvent = (TTree*)mFile_InPut->Get("mTree_FstEvent");
  FstEvent *mFstEvent = new FstEvent();
  mTree_FstEvent->SetBranchAddress("FstEvent",&mFstEvent);

  TH1F *h_mXResidual_Clusters_Before   = new TH1F("h_mXResidual_Clusters_Before","h_mXResidual_Clusters_Before",100,0.0,320.0);
  TH1F *h_mYResidual_Clusters_Before   = new TH1F("h_mYResidual_Clusters_Before","h_mYResidual_Clusters_Before",100,-50.0,10.0);
  TH1F *h_mRResidual_Clusters_Before   = new TH1F("h_mRResidual_Clusters_Before","h_mRResidual_Clusters_Before",100,0.0,320.0);
  TH1F *h_mPhiResidual_Clusters_Before = new TH1F("h_mPhiResidual_Clusters_Before","h_mPhiResidual_Clusters_Before",100,-1.5,1.5);

  TH1F *h_mXResidual_Clusters          = new TH1F("h_mXResidual_Clusters","h_mXResidual_Clusters",50,-160,160.0);
  TH1F *h_mYResidual_Clusters          = new TH1F("h_mYResidual_Clusters","h_mYResidual_Clusters",100,-16.0,16.0);
  TH1F *h_mRResidual_Clusters          = new TH1F("h_mRResidual_Clusters","h_mRResidual_Clusters",50,-160.0,160.0);
  TH1F *h_mPhiResidual_Clusters        = new TH1F("h_mPhiResidual_Clusters","h_mPhiResidual_Clusters",100,-0.05,0.05);

  long NumOfEvents = (long)mTree_FstEvent->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 500;
  mTree_FstEvent->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_FstEvent->GetEntry(i_event);

    std::vector<FstCluster *> clusterVec_fst;
    std::vector<FstCluster *> clusterVec_ist1;
    std::vector<FstCluster *> clusterVec_ist2;
    std::vector<FstCluster *> clusterVec_ist3;
    clusterVec_fst.clear();
    clusterVec_ist1.clear();
    clusterVec_ist2.clear();
    clusterVec_ist3.clear();

    for(int i_cluster = 0; i_cluster < mFstEvent->getNumClusters(); ++i_cluster)
    { // get Clusters info for IST1 IST2 and IST3
      FstCluster *fstCluster = mFstEvent->getCluster(i_cluster);
      if(fstCluster->getLayer() == 0)
      {
	clusterVec_fst.push_back(fstCluster);
      }
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

    if(clusterVec_fst.size() > 0) // FST has at least one hit
    {
      for(int i_ist1 = 0; i_ist1 < clusterVec_ist1.size(); ++i_ist1)
      {
	double x1_ist = clusterVec_ist1[i_ist1]->getMeanX();
	double y1_ist = clusterVec_ist1[i_ist1]->getMeanY();
	const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
	double col1 = clusterVec_ist1[i_ist1]->getMeanColumn();
	double row1 = clusterVec_ist1[i_ist1]->getMeanRow();

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

	  double x0_proj = x3_corr + (x1_corr-x3_corr)*FST::pitchLayer03/z1_ist; // projected position after aligned to IST2
	  double y0_proj = y3_corr + (y1_corr-y3_corr)*FST::pitchLayer03/z1_ist;
	  double r0_proj = TMath::Sqrt(x0_proj*x0_proj+y0_proj*y0_proj);
	  double phi0_proj = TMath::ATan2(y0_proj,x0_proj);

	  double x0_corr = x0_proj*TMath::Cos(phi_rot_ist2) + y0_proj*TMath::Sin(phi_rot_ist2) + x2_shift; // projected position after aligned to FST
	  double y0_corr = y0_proj*TMath::Cos(phi_rot_ist2) - x0_proj*TMath::Sin(phi_rot_ist2) + y2_shift;
	  double r0_corr = TMath::Sqrt(x0_corr*x0_corr+y0_corr*y0_corr);
	  double phi0_corr = TMath::ATan2(y0_corr,x0_corr);

	  if( abs(row3-row1) < 17)
	  {
	    for(int i_fst = 0; i_fst < clusterVec_fst.size(); ++i_fst)
	    {
	      double r0_fst = clusterVec_fst[i_fst]->getMeanX(); // r for FST
	      double phi0_fst = clusterVec_fst[i_fst]->getMeanY(); // phi for FST
	      double x0_fst = r0_fst*TMath::Cos(phi0_fst);
	      double y0_fst = r0_fst*TMath::Sin(phi0_fst);

	      h_mXResidual_Clusters_Before->Fill(x0_fst-x0_proj);
	      h_mYResidual_Clusters_Before->Fill(y0_fst-y0_proj);
	      h_mRResidual_Clusters_Before->Fill(r0_fst-r0_proj);
	      h_mPhiResidual_Clusters_Before->Fill(phi0_fst-phi0_proj);

	      h_mXResidual_Clusters->Fill(x0_fst-x0_corr);
	      h_mYResidual_Clusters->Fill(y0_fst-y0_corr);
	      h_mRResidual_Clusters->Fill(r0_fst-r0_corr);
	      h_mPhiResidual_Clusters->Fill(phi0_fst-phi0_corr);
	    }
	  }
	}
      }
    }
  }

  TCanvas *c_play = new TCanvas("c_play","c_play",10,10,1600,800);
  c_play->Divide(4,2);
  for(int i_pad = 0; i_pad < 8; ++i_pad)
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

  c_play->cd(5);
  h_mRResidual_Clusters_Before->SetTitle("r-residual before correction");
  h_mRResidual_Clusters_Before->GetXaxis()->SetTitle("r-residual (mm)");
  h_mRResidual_Clusters_Before->GetXaxis()->SetTitleSize(0.06);
  h_mRResidual_Clusters_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mRResidual_Clusters_Before->GetYaxis()->SetTitleSize(0.06);
  h_mRResidual_Clusters_Before->Draw();
  h_mRResidual_Clusters_Before->Fit("gaus");

  c_play->cd(6);
  h_mPhiResidual_Clusters_Before->SetTitle("#phi-residual before correction");
  h_mPhiResidual_Clusters_Before->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mPhiResidual_Clusters_Before->GetXaxis()->SetTitleSize(0.06);
  h_mPhiResidual_Clusters_Before->GetYaxis()->SetTitle("No. Tracks");
  h_mPhiResidual_Clusters_Before->GetYaxis()->SetTitleSize(0.06);
  h_mPhiResidual_Clusters_Before->Draw();
  h_mPhiResidual_Clusters_Before->Fit("gaus");

  c_play->cd(7);
  h_mRResidual_Clusters->SetTitle("r-residual after correction");
  h_mRResidual_Clusters->GetXaxis()->SetTitle("r-residual (mm)");
  h_mRResidual_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mRResidual_Clusters->GetXaxis()->SetRangeUser(-20.0,40.0);
  h_mRResidual_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mRResidual_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mRResidual_Clusters->DrawCopy();
  h_mRResidual_Clusters->Fit("gaus");
  h_mRResidual_Clusters->GetXaxis()->SetRangeUser(-160.0,160.0);
  h_mRResidual_Clusters->DrawCopy("same");

  c_play->cd(8);
  h_mPhiResidual_Clusters->SetTitle("#phi-residual after correction");
  h_mPhiResidual_Clusters->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mPhiResidual_Clusters->GetXaxis()->SetTitleSize(0.06);
  h_mPhiResidual_Clusters->GetYaxis()->SetTitle("No. Tracks");
  h_mPhiResidual_Clusters->GetYaxis()->SetTitleSize(0.06);
  h_mPhiResidual_Clusters->Draw();
  h_mPhiResidual_Clusters->Fit("gaus");

  // c_play->SaveAs("./figures/Residual_FSTClusters_2Layer.eps");
  c_play->SaveAs("./figures/Residual_FSTClusters_2Layer.pdf");

  return 1;
}
