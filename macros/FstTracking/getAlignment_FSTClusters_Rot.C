#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include <TMath.h>
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

typedef std::vector<double> dVec;
typedef std::tuple<double, double, double, double, double, int> tPars; // phi_rot_ist2, x2_shift, y2_shift, xCut, yCut, nOffset

tPars minuitAlignment(dVec x0_orig, dVec y0_orig, dVec x1_orig, dVec y1_orig, dVec x3_orig, dVec y3_orig, tPars cParameters);
tPars updateFitParameters(tPars fitPars, double xCut, double yCut, int nOffset);

R__LOAD_LIBRARY(../../lib/libFstEvent.dylib)

int getAlignment_FSTClusters_2Layer()
{
  dVec x0_fst;
  dVec y0_fst;
  dVec x1_ist;
  dVec y1_ist;
  dVec x3_ist;
  dVec y3_ist;

  // clear vector
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

  FstEvent *mFstEvent = new FstEvent();
  std::string inputfile = "../../output/alignment/FstClusters_HV140V_Th4.5Tb3_woPed_withCMNCorr_2Layer.root";
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;
  TFile *mFile_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_FstEvent = (TTree*)mFile_InPut->Get("mTree_FstEvent");
  mTree_FstEvent->SetBranchAddress("FstEvent",&mFstEvent);

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
    std::vector<FstCluster *> clusterVec_ist3;
    clusterVec_fst.clear();
    clusterVec_ist1.clear();
    clusterVec_ist3.clear();

    for(int i_cluster = 0; i_cluster < mFstEvent->getNumClusters(); ++i_cluster)
    { // get Clusters info for IST1 IST2 and IST3
      FstCluster *fstCluster = mFstEvent->getCluster(i_cluster);
      if(fstCluster->getLayer() == 0 && fstCluster->getClusterType() == 1)
      { // use Simple Cluster
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

    if(clusterVec_fst.size() > 0) // FST has at least one hit
    {
      for(int i_ist1 = 0; i_ist1 < clusterVec_ist1.size(); ++i_ist1)
      {
	double x1 = clusterVec_ist1[i_ist1]->getMeanX();
	double y1 = clusterVec_ist1[i_ist1]->getMeanY();
	const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
	double col1 = clusterVec_ist1[i_ist1]->getMeanColumn();
	double row1 = clusterVec_ist1[i_ist1]->getMeanRow();

	double x1_corr = x1*TMath::Cos(phi_rot_ist1) + y1*TMath::Sin(phi_rot_ist1) + x1_shift;
	double y1_corr = y1*TMath::Cos(phi_rot_ist1) - x1*TMath::Sin(phi_rot_ist1) + y1_shift;

	for(int i_ist3 = 0; i_ist3 < clusterVec_ist3.size(); ++i_ist3)
	{
	  double x3 = clusterVec_ist3[i_ist3]->getMeanX();
	  double y3 = clusterVec_ist3[i_ist3]->getMeanY();
	  const double z3_ist = 0.0;
	  double col3 = clusterVec_ist3[i_ist3]->getMeanColumn();
	  double row3 = clusterVec_ist3[i_ist3]->getMeanRow();

	  double x3_corr = x3*TMath::Cos(phi_rot_ist3) + y3*TMath::Sin(phi_rot_ist3) + x3_shift;
	  double y3_corr = y3*TMath::Cos(phi_rot_ist3) - x3*TMath::Sin(phi_rot_ist3) + y3_shift;

	  if( abs(row3-row1) < 17)
	  {
	    for(int i_fst = 0; i_fst < clusterVec_fst.size(); ++i_fst)
	    {
	      double r0 = clusterVec_fst[i_fst]->getMeanX();
	      double phi0 = clusterVec_fst[i_fst]->getMeanY();
	      double x0 = r0*TMath::Cos(phi0);
	      double y0 = r0*TMath::Sin(phi0);
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

  mFile_InPut->Close();

  cout << "Start Minuit Fit for alignment => " << endl;

  int nFit = 0;
  cout << "Iteration " << nFit << " ==> " << endl;
  tPars initPars = std::make_tuple(0.0,0.0,0.0,1000.0,1000.0,0); // set initial fit parameters
  tPars fitPars = minuitAlignment(x0_fst, y0_fst, x1_ist, y1_ist, x3_ist, y3_ist, initPars);
  cout << endl;

  // reject trakcs with xCut = 50 and yCut = 5
  tPars itPars = updateFitParameters(fitPars, 50, 50, 1);
  int nOffset_temp = std::get<5>(itPars);
  while(nOffset_temp > 0)
  { 
    nFit++;
    cout << "Iteration " << nFit << " ==> " << endl;
    fitPars = minuitAlignment(x0_fst, y0_fst, x1_ist, y1_ist, x3_ist, y3_ist, itPars);
    nOffset_temp = std::get<5>(fitPars);
    itPars = updateFitParameters(fitPars,50,50,nOffset_temp);
    cout << endl;
  }

  // reject trakcs with xCut = 25 and yCut = 2.5 
  itPars = updateFitParameters(fitPars, 25, 25, 1);
  nOffset_temp = std::get<5>(itPars);
  while(nOffset_temp > 0)
  {
    nFit++;
    cout << "Iteration " << nFit << " ==> " << endl;
    fitPars = minuitAlignment(x0_fst, y0_fst, x1_ist, y1_ist, x3_ist, y3_ist, itPars);
    nOffset_temp = std::get<5>(fitPars);
    itPars = updateFitParameters(fitPars,15,15,nOffset_temp);
    cout << endl;
  }

  cout << "Finish Minuit Fit for alignment => " << endl;

  cout << "Minuit minimization: phi_rot_ist2 = " << std::get<0>(fitPars) << ", x2_shift = " << std::get<1>(fitPars) << ", y2_shift = " << std::get<2>(fitPars) << endl;

  return 1;
}

tPars minuitAlignment(dVec x0_orig, dVec y0_orig, dVec x1_orig, dVec y1_orig, dVec x3_orig, dVec y3_orig, tPars cParameters)
{
  dVec x0_temp, x0_fit;
  dVec y0_temp, y0_fit;
  dVec x1_temp, x1_fit;
  dVec y1_temp, y1_fit;
  dVec x3_temp, x3_fit;
  dVec y3_temp, y3_fit;

  // clear vector
  x0_temp.clear();
  y0_temp.clear();
  x1_temp.clear();
  y1_temp.clear();
  x3_temp.clear();
  y3_temp.clear();

  x0_fit.clear();
  y0_fit.clear();
  x1_fit.clear();
  y1_fit.clear();
  x3_fit.clear();
  y3_fit.clear();

  for(int i_cluster = 0; i_cluster < x1_orig.size(); ++i_cluster)
  { // set hit containers
    x0_temp.push_back(x0_orig[i_cluster]);
    y0_temp.push_back(y0_orig[i_cluster]);
    x1_temp.push_back(x1_orig[i_cluster]);
    y1_temp.push_back(y1_orig[i_cluster]);
    x3_temp.push_back(x3_orig[i_cluster]);
    y3_temp.push_back(y3_orig[i_cluster]);
  }

  const double pitchLayer03 = 134.9375; // mm
  const double pitchLayer12 = 34.925; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
  const double pitchLayer23 = 30.1625; //mm, distances between the 2nd & 3rd Layer

  // reject offset trakcs
  const double phi_ist2_temp = std::get<0>(cParameters); // aligment parameters from previous iteration
  const double x2_shift_temp = std::get<1>(cParameters);
  const double y2_shift_temp = std::get<2>(cParameters);
  const double xCut_temp     = std::get<3>(cParameters);
  const double yCut_temp     = std::get<4>(cParameters);
  cout << "reject trakcs out of xCut = " << xCut_temp << " and yCut: " << yCut_temp << endl;

  int numOfUsedHits = 0;
  for(int i_cluster = 0; i_cluster < x1_temp.size(); i_cluster++)
  {
    double x1_corr_temp = x1_temp[i_cluster]*TMath::Cos(phi_ist2_temp) + y1_temp[i_cluster]*TMath::Sin(phi_ist2_temp) + x2_shift_temp;
    double y1_corr_temp = y1_temp[i_cluster]*TMath::Cos(phi_ist2_temp) - x1_temp[i_cluster]*TMath::Sin(phi_ist2_temp) + y2_shift_temp;

    double x3_corr_temp = x3_temp[i_cluster]*TMath::Cos(phi_ist2_temp) + y3_temp[i_cluster]*TMath::Sin(phi_ist2_temp) + x2_shift_temp;
    double y3_corr_temp = y3_temp[i_cluster]*TMath::Cos(phi_ist2_temp) - x3_temp[i_cluster]*TMath::Sin(phi_ist2_temp) + y2_shift_temp;

    double x0_proj_temp = x3_corr_temp + (x1_corr_temp-x3_corr_temp)*pitchLayer03/(pitchLayer12+pitchLayer23);
    double y0_proj_temp = y3_corr_temp + (y1_corr_temp-y3_corr_temp)*pitchLayer03/(pitchLayer12+pitchLayer23);

    double dx = x0_temp[i_cluster] - x0_proj_temp;
    double dy = y0_temp[i_cluster] - y0_proj_temp;
    // cout << "dx = " << dx << ", dy = " << dy << endl;

    if(abs(dx) < xCut_temp && abs(dy) < yCut_temp)
    {
      x0_fit.push_back(x0_temp[i_cluster]);
      y0_fit.push_back(y0_temp[i_cluster]);
      x1_fit.push_back(x1_temp[i_cluster]);
      y1_fit.push_back(y1_temp[i_cluster]);
      x3_fit.push_back(x3_temp[i_cluster]);
      y3_fit.push_back(y3_temp[i_cluster]);
      numOfUsedHits++;
    }
  }
  cout << "numOfUsedTracks = " << numOfUsedHits << endl;

  cout << "Start the Minuit minimization!" << endl;
  auto chi2Function = [&](const Double_t *par)
  { //minimisation function computing the sum of squares of residuals
    double phi_rot_ist2 = par[0];
    double x2_shift     = par[1];
    double y2_shift     = par[2];

    const double x_weight = 1.0;
    const double y_weight = 1.0;
    double f = 0;
    for (int i_cluster = 0; i_cluster < numOfUsedHits; i_cluster++)
    {
      double x1_corr = x1_fit[i_cluster]*TMath::Cos(phi_rot_ist2) + y1_fit[i_cluster]*TMath::Sin(phi_rot_ist2) + x2_shift;
      double y1_corr = y1_fit[i_cluster]*TMath::Cos(phi_rot_ist2) - x1_fit[i_cluster]*TMath::Sin(phi_rot_ist2) + y2_shift;

      double x3_corr = x3_fit[i_cluster]*TMath::Cos(phi_rot_ist2) + y3_fit[i_cluster]*TMath::Sin(phi_rot_ist2) + x2_shift;
      double y3_corr = y3_fit[i_cluster]*TMath::Cos(phi_rot_ist2) - x3_fit[i_cluster]*TMath::Sin(phi_rot_ist2) + y2_shift;

      double x0_proj = x3_corr + (x1_corr-x3_corr)*pitchLayer03/(pitchLayer12+pitchLayer23);
      double y0_proj = y3_corr + (y1_corr-y3_corr)*pitchLayer03/(pitchLayer12+pitchLayer23);

      double dx = x0_fit[i_cluster] - x0_proj;
      double dy = y0_fit[i_cluster] - y0_proj;

      f += x_weight*dx*dx + y_weight*dy*dy;
    }
    return f;
  };

  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,3);
  ROOT::Fit::Fitter fitter;

  double pStart[3] = {phi_ist2_temp,x2_shift_temp,y2_shift_temp};
  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("phi_rot_ist2");
  fitter.Config().ParSettings(1).SetName("x2_shift");
  fitter.Config().ParSettings(2).SetName("y2_shift");

  // do the fit
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("find alignment","Minuit Fit failed");
    return std::make_tuple(-1.0, -1.0, -1.0, -1.0, -1.0, -1);
  }

  const ROOT::Fit::FitResult & result = fitter.Result();
  // result.Print(std::cout);
  const double *fitpar = result.GetParams();
  double phi_rot_ist2_MF = fitpar[0];
  double x2_shift_MF     = fitpar[1];
  double y2_shift_MF     = fitpar[2];

  // calculate nOffset with current fit parameters
  int nOffset = 0;
  for (int i_cluster = 0; i_cluster < numOfUsedHits; i_cluster++)
  {
    double x1_corr_MF = x1_fit[i_cluster]*TMath::Cos(phi_rot_ist2_MF) + y1_fit[i_cluster]*TMath::Sin(phi_rot_ist2_MF) + x2_shift_MF;
    double y1_corr_MF = y1_fit[i_cluster]*TMath::Cos(phi_rot_ist2_MF) - x1_fit[i_cluster]*TMath::Sin(phi_rot_ist2_MF) + y2_shift_MF;

    double x3_corr_MF = x3_fit[i_cluster]*TMath::Cos(phi_rot_ist2_MF) + y3_fit[i_cluster]*TMath::Sin(phi_rot_ist2_MF) + x2_shift_MF;
    double y3_corr_MF = y3_fit[i_cluster]*TMath::Cos(phi_rot_ist2_MF) - x3_fit[i_cluster]*TMath::Sin(phi_rot_ist2_MF) + y2_shift_MF;

    double x0_proj_MF = x3_corr_MF + (x1_corr_MF-x3_corr_MF)*pitchLayer03/(pitchLayer12+pitchLayer23);
    double y0_proj_MF = y3_corr_MF + (y1_corr_MF-y3_corr_MF)*pitchLayer03/(pitchLayer12+pitchLayer23);

    double dx = x0_fit[i_cluster] - x0_proj_MF;
    double dy = y0_fit[i_cluster] - y0_proj_MF;

    if( !(abs(dx) < xCut_temp && abs(dy) < yCut_temp) )
    {
      nOffset++;
    }
  }

  tPars tParameters = std::make_tuple(phi_rot_ist2_MF, x2_shift_MF, y2_shift_MF, xCut_temp, yCut_temp, nOffset);

  // cout << "Minuit minimization: phi_rot_ist1 = " << phi_rot_ist1_MF << ", phi_rot_ist3 = " << phi_rot_ist3_MF << ", x_shift = " << x_shift_MF << ", y_shift = " << y_shift_MF << endl;
  cout << "Minuit minimization: phi_rot_ist2 = " << std::get<0>(tParameters) << ", x2_shift = " << (double)std::get<1>(tParameters) << ", y2_shift = " << std::get<2>(tParameters) << ", nOffset = " << std::get<5>(tParameters) << endl;

  return tParameters;
}

tPars updateFitParameters(tPars fitPars, double xCut, double yCut, int nOffset)
{
  const double phi_ist2_old = std::get<0>(fitPars);
  const double x2_shift_old = std::get<1>(fitPars);
  const double y2_shift_old = std::get<2>(fitPars);
  const double xCut_old     = std::get<3>(fitPars);
  const double yCut_old     = std::get<4>(fitPars);
  const double nOffset_old  = std::get<5>(fitPars);

  tPars newPars = std::make_tuple(phi_ist2_old, x2_shift_old, y2_shift_old, xCut, yCut, nOffset);

  return newPars;
}
