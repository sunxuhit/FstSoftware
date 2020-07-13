#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include <TStyle.h>
#include <TMath.h>
#include <TCanvas.h>
#include "Fit/Fitter.h"
#include "Fit/Chi2FCN.h"
#include <Math/Functor.h>
#include "Math/WrappedParamFunction.h"
#include "Math/AdaptiveIntegratorMultiDim.h"

#include "../../src/FstUtil/FstCons.h"
#include "../../src/FstUtil/FstRawHit.h"
#include "../../src/FstUtil/FstCluster.h"
#include "../../src/FstUtil/FstTrack.h"
#include "../../src/FstUtil/FstEvent.h"
#include "../FstPlotConfiguration/draw.h"

R__LOAD_LIBRARY(../../lib/libFstEvent.dylib)

typedef std::vector<double> dVec;
typedef std::tuple<double, double, double, double, double, int> tPars; // phi_rot_ist2, x2_shift, y2_shift, xCut, yCut, nOffset

tPars minuitAlignment(dVec x0_orig, dVec y0_orig, dVec x1_orig, dVec y1_orig, dVec x3_orig, dVec y3_orig, tPars cParameters);
tPars updateFitParameters(tPars fitPars, double xCut, double yCut, int nOffset);

double gaussian(double *var, double *par)
{
  double residual = var[0];

  double norm  = par[0];
  double mean  = par[1];
  double sigma = par[2];
  double binwidth = par[3];

  double sigmaSquare = sigma*sigma;
  double Norm = norm/(sqrt(2.0*TMath::Pi())*sigma);
  double Power = -0.5*(residual-mean)*(residual-mean)/sigmaSquare;

  double y = Norm*exp(Power)*binwidth;

  return y;
}

int plotResidual_3Layer(float nFstHitsCut = 4.0, int numOfUsedTimeBins = 2, bool isInner = true)
{
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1001);
  gStyle->SetStatX(0.95); gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.20); gStyle->SetStatH(0.20);

  dVec r0_fst;
  dVec phi0_fst;
  dVec x0_fst;
  dVec y0_fst;
  dVec x1_ist;
  dVec y1_ist;
  dVec x3_ist;
  dVec y3_ist;

  // clear vector
  r0_fst.clear();
  phi0_fst.clear();
  x0_fst.clear();
  y0_fst.clear();
  x1_ist.clear();
  y1_ist.clear();
  x3_ist.clear();
  y3_ist.clear();

  const double phi_rot_ist1 = -0.00113551; // aligned IST1 & IST3 to IST2
  const double x1_shift = 0.530444;
  const double y1_shift = 1.17133;
  const double phi_rot_ist3 = -0.00379431;
  const double x3_shift = 0.45811;
  const double y3_shift = 1.0116;

  string mSector = "Inner";
  if(!isInner) mSector = "Outer";
  // std::string inputlist = Form("../../list/FST/alignment/FstCluster_Th%1.1fTb%d_%s.list",nFstHitsCut,numOfUsedTimeBins,mSector.c_str());
  std::string inputlist = Form("../../list/FST/alignment/FstCluster_Th%1.1fTb%d_InOut.list",nFstHitsCut,numOfUsedTimeBins);
  cout << "input list set to: " << inputlist.c_str() << endl;
  TChain *mChainInPut = new TChain("mTree_FstEvent");
  if (!inputlist.empty())   // if input file is ok
  {
    cout << "Open input probability file list" << endl;
    ifstream in(inputlist.c_str());  // input stream
    if(in)
    {
      cout << "input file probability list is ok" << endl;
      char str[255];       // char array for each file name
      Long64_t entries_save = 0;
      while(in)
      {
	in.getline(str,255);  // take the lines of the file list
	if(str[0] != 0)
	{
	  string addfile;
	  addfile = str;
	  mChainInPut->AddFile(addfile.c_str(),-1,"mTree_FstEvent");
	  Long64_t file_entries = mChainInPut->GetEntries();
	  cout << "File added to data chain: " << addfile.c_str() << " with " << (file_entries-entries_save) << " entries" << endl;
	  entries_save = file_entries;
	}
      }
    }
    else
    {
      cout << "WARNING: input probability file input is problemtic" << endl;
      return false;
    }
  }

  FstEvent *mFstEvent = new FstEvent();
  mChainInPut->SetBranchAddress("FstEvent",&mFstEvent);
  long NumOfEvents = (long)mChainInPut->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;

  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 500;
  mChainInPut->GetEntry(0);

  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mChainInPut->GetEntry(i_event);

    std::vector<FstCluster *> clusterVec_fst;
    std::vector<FstCluster *> clusterVec_ist1;
    std::vector<FstCluster *> clusterVec_ist3;
    clusterVec_fst.clear();
    clusterVec_ist1.clear();
    clusterVec_ist3.clear();

    for(int i_cluster = 0; i_cluster < mFstEvent->getNumClusters(); ++i_cluster)
    { // get Clusters info for IST1 IST2 and IST3
      FstCluster *fstCluster = mFstEvent->getCluster(i_cluster);
      if(fstCluster->getLayer() == 0 && fstCluster->getClusterType() == 2 && fstCluster->getIsSeed())
      { // use Scan cluster
	clusterVec_fst.push_back(fstCluster);
      }
      if(fstCluster->getLayer() == 1)
      {
	clusterVec_ist1.push_back(fstCluster);
      }
      if(fstCluster->getLayer() == 3)
      {
	clusterVec_ist3.push_back(fstCluster);
      }
    }

    for(int i_ist1 = 0; i_ist1 < clusterVec_ist1.size(); ++i_ist1)
    {
      double x1 = clusterVec_ist1[i_ist1]->getMeanX();
      double y1 = clusterVec_ist1[i_ist1]->getMeanY();
      const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
      double row1 = clusterVec_ist1[i_ist1]->getMeanRow();

      double x1_corr = x1*TMath::Cos(phi_rot_ist1) + y1*TMath::Sin(phi_rot_ist1) + x1_shift;
      double y1_corr = y1*TMath::Cos(phi_rot_ist1) - x1*TMath::Sin(phi_rot_ist1) + y1_shift;

      for(int i_ist3 = 0; i_ist3 < clusterVec_ist3.size(); ++i_ist3)
      {
	double x3 = clusterVec_ist3[i_ist3]->getMeanX();
	double y3 = clusterVec_ist3[i_ist3]->getMeanY();
	const double z3_ist = 0.0;
	double row3 = clusterVec_ist3[i_ist3]->getMeanRow();

	double x3_corr = x3*TMath::Cos(phi_rot_ist3) + y3*TMath::Sin(phi_rot_ist3) + x3_shift;
	double y3_corr = y3*TMath::Cos(phi_rot_ist3) - x3*TMath::Sin(phi_rot_ist3) + y3_shift;

	if( abs(row3-row1) < 17)
	{
	  if(clusterVec_fst.size() > 0) // FST has at least one hit
	  {
	    for(int i_fst = 0; i_fst < clusterVec_fst.size(); ++i_fst)
	    {
	      double r0 = clusterVec_fst[i_fst]->getMeanX();
	      double phi0 = clusterVec_fst[i_fst]->getMeanY();
	      double rMinTest = FST::rInner;
	      double rMaxTest = FST::rInner + 4.0*FST::pitchR;
	      double phiMinTest = -FST::phiMax;
	      double phiMaxTest = FST::phiMax;
	      if(!isInner) rMinTest = FST::rOuter;
	      if(!isInner) rMaxTest = FST::rOuter + 4.0*FST::pitchR;
	      if(!isInner) phiMinTest = 0.0;
	      if(!isInner) phiMaxTest = FST::phiMax;
	      if(r0 > rMinTest && r0 < rMaxTest)
	      {
		if(phi0 > phiMinTest && phi0 < phiMaxTest)
		{
		  double x0 = r0*TMath::Cos(phi0);
		  double y0 = r0*TMath::Sin(phi0);
		  r0_fst.push_back(r0);
		  phi0_fst.push_back(phi0);
		  x0_fst.push_back(x0);
		  y0_fst.push_back(y0);
		  x1_ist.push_back(x1_corr); // aligned w.r.t. IST2
		  y1_ist.push_back(y1_corr);
		  x3_ist.push_back(x3_corr);
		  y3_ist.push_back(y3_corr);
		}
	      }
	    }
	  }
	}
      }
    }
  }

  // mFile_InPut->Close();

  cout << "Start Minuit Fit for alignment => " << endl;
  // -------------------------------------
  // for plotting
  int numBinX   = 50;
  double maxX   = 160.0;
  double minX   = -160.0;
  int numBinY   = 100;
  double maxY   = 16.0;
  double minY   = -16.0;
  int numBinR   = 50;
  double maxR   = 160.0;
  double minR   = -160.0;
  int numBinPhi = 100;
  double maxPhi = 0.1;
  double minPhi = -0.1;
  TH1F *h_mXResidual      = new TH1F("h_mXResidual","h_mXResidual",numBinX,minX,maxX);
  TH1F *h_mYResidual      = new TH1F("h_mYResidual","h_mYResidual",numBinY,minY,maxY);
  TH1F *h_mRResidual      = new TH1F("h_mRResidual","h_mRResidual",numBinR,minR,maxR);
  TH1F *h_mPhiResidual    = new TH1F("h_mPhiResidual","h_mPhiResidual",numBinPhi,minPhi,maxPhi);

  h_mXResidual->SetTitle("Corrected X-residual FST Scan Clusters");
  h_mXResidual->GetXaxis()->SetTitle("x-residual (mm)");
  h_mXResidual->GetXaxis()->SetTitleSize(0.06);
  h_mXResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mXResidual->GetYaxis()->SetTitleSize(0.06);
  h_mXResidual->SetLineColor(1);
  h_mXResidual->SetLineWidth(1);

  h_mYResidual->SetTitle("Corrected Y-residual FST Scan Clusters");
  h_mYResidual->GetXaxis()->SetTitle("Y-residual (mm)");
  h_mYResidual->GetXaxis()->SetTitleSize(0.06);
  h_mYResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mYResidual->GetYaxis()->SetTitleSize(0.06);
  h_mYResidual->SetLineColor(1);
  h_mYResidual->SetLineWidth(1);

  h_mRResidual->SetTitle("Corrected R-residual FST Scan Clusters");
  h_mRResidual->GetXaxis()->SetTitle("r-residual (mm)");
  h_mRResidual->GetXaxis()->SetTitleSize(0.06);
  h_mRResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mRResidual->GetYaxis()->SetTitleSize(0.06);
  h_mRResidual->SetLineColor(1);
  h_mRResidual->SetLineWidth(1);

  h_mPhiResidual->SetTitle("Corrected #phi-residual FST Scan Clusters");
  h_mPhiResidual->GetXaxis()->SetTitle("#phi-residual (rad)");
  h_mPhiResidual->GetXaxis()->SetTitleSize(0.06);
  h_mPhiResidual->GetYaxis()->SetTitle("No. Tracks");
  h_mPhiResidual->GetYaxis()->SetTitleSize(0.06);
  h_mPhiResidual->SetLineColor(1);
  h_mPhiResidual->SetLineWidth(1);
  // -------------------------------------

  TCanvas *c_Residual = new TCanvas("c_Residual","c_Residual",10,10,800,800);
  c_Residual->Divide(2,2);
  for(int i_pad = 0; i_pad < 4; ++i_pad)
  {
    c_Residual->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Residual->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Residual->cd(i_pad+1)->SetTicks(1,1);
    c_Residual->cd(i_pad+1)->SetGrid(0,0);
  }

  cout << "Finish Minuit Fit for alignment => " << endl;
  // plotting final alignment
  {
    double phi_ist2_temp = FST::phi_rot_ist2; // aligment parameters from previous iteration
    double x2_shift_temp = FST::x2_shift;
    double y2_shift_temp = FST::y2_shift;

    for(int i_cluster = 0; i_cluster < x1_ist.size(); i_cluster++)
    {
      double x1_corr_temp = x1_ist[i_cluster]*TMath::Cos(phi_ist2_temp) + y1_ist[i_cluster]*TMath::Sin(phi_ist2_temp) + x2_shift_temp;
      double y1_corr_temp = y1_ist[i_cluster]*TMath::Cos(phi_ist2_temp) - x1_ist[i_cluster]*TMath::Sin(phi_ist2_temp) + y2_shift_temp;

      double x3_corr_temp = x3_ist[i_cluster]*TMath::Cos(phi_ist2_temp) + y3_ist[i_cluster]*TMath::Sin(phi_ist2_temp) + x2_shift_temp;
      double y3_corr_temp = y3_ist[i_cluster]*TMath::Cos(phi_ist2_temp) - x3_ist[i_cluster]*TMath::Sin(phi_ist2_temp) + y2_shift_temp;

      double x0_proj_temp   = x3_corr_temp + (x1_corr_temp-x3_corr_temp)*FST::pitchLayer03/(FST::pitchLayer12+FST::pitchLayer23);
      double y0_proj_temp   = y3_corr_temp + (y1_corr_temp-y3_corr_temp)*FST::pitchLayer03/(FST::pitchLayer12+FST::pitchLayer23);
      double r0_proj_temp   = TMath::Sqrt(x0_proj_temp*x0_proj_temp+y0_proj_temp*y0_proj_temp);
      double phi0_proj_temp = TMath::ATan2(y0_proj_temp,x0_proj_temp);

      double dx   = x0_fst[i_cluster] - x0_proj_temp;
      double dy   = y0_fst[i_cluster] - y0_proj_temp;
      double dr   = r0_fst[i_cluster] - r0_proj_temp;
      double dphi = phi0_fst[i_cluster] - phi0_proj_temp;
      // cout << "dx = " << dx << ", dy = " << dy << endl;

      h_mXResidual->Fill(dx);
      h_mYResidual->Fill(dy);
      h_mRResidual->Fill(dr);
      h_mPhiResidual->Fill(dphi);
    }

    c_Residual->cd();
    string TitleName = "Final Alignment";
    TPaveLabel* canvas_title = new TPaveLabel(0.0,0.98,0.1,1.0,TitleName.c_str());
    canvas_title->SetBorderSize(1);
    canvas_title->Draw();
    c_Residual->cd(1);
    h_mXResidual->DrawCopy("hE");
    TF1 *f_gausX = new TF1("f_gausX",gaussian,-150.0,150.0,4);
    f_gausX->SetParameter(0,100.0);
    f_gausX->SetParameter(1,0.0);
    f_gausX->SetParameter(2,10.0);
    f_gausX->FixParameter(3,h_mXResidual->GetBinWidth(1));
    f_gausX->SetRange(-30,30);
    h_mXResidual->Fit(f_gausX,"R");
    f_gausX->Draw("l same");

    c_Residual->cd(2);
    h_mYResidual->DrawCopy("hE");
    TF1 *f_gausY = new TF1("f_gausY",gaussian,-150.0,150.0,4);
    f_gausY->SetParameter(0,100.0);
    f_gausY->SetParameter(1,0.0);
    f_gausY->SetParameter(2,10.0);
    f_gausY->FixParameter(3,h_mYResidual->GetBinWidth(1));
    f_gausY->SetRange(-4.0,4.0);
    h_mYResidual->Fit(f_gausY,"R");
    f_gausY->Draw("l same");

    c_Residual->cd(3);
    h_mRResidual->DrawCopy("hE");
    TF1 *f_gausR = new TF1("f_gausR",gaussian,-150.0,150.0,4);
    f_gausR->SetParameter(0,100.0);
    f_gausR->SetParameter(1,0.0);
    f_gausR->SetParameter(2,10.0);
    f_gausR->FixParameter(3,h_mRResidual->GetBinWidth(1));
    f_gausR->SetRange(-30,30);
    h_mRResidual->Fit(f_gausR,"R");
    f_gausR->Draw("l same");

    c_Residual->cd(4);
    h_mPhiResidual->DrawCopy("hE");
    TF1 *f_gausPhi = new TF1("f_gausPhi",gaussian,-1.0,1.0,4);
    f_gausPhi->SetParameter(0,100.0);
    f_gausPhi->SetParameter(1,0.0);
    f_gausPhi->SetParameter(2,10.0);
    f_gausPhi->FixParameter(3,h_mPhiResidual->GetBinWidth(1));
    f_gausPhi->SetRange(-0.03,0.03);
    h_mPhiResidual->Fit(f_gausPhi,"R");
    f_gausPhi->Draw("l same");
  }

  string outputname = Form("./figures/%sTestResidual_FSTClusters_3Layer.pdf",mSector.c_str()); 
  c_Residual->SaveAs(outputname.c_str());

  return 1;
}

