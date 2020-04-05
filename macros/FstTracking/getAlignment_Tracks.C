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
typedef std::tuple<double, double, double, double, double, int> tPars; // phi_rot, x_shift, y_shift, xCut, yCut, nOffset

tPars minuitAlignment(dVec x0_orig, dVec y0_orig, dVec x1_orig, dVec y1_orig, dVec x3_orig, dVec y3_orig, tPars cParameters);
tPars updateFitParameters(tPars fitPars, double xCut, double yCut, int nOffset);

R__LOAD_LIBRARY(/Users/xusun/WorkSpace/STAR/ForwardSiliconTracker/FstTracking/lib/libFstEvent.dylib)

int getAlignment_Tracks(int trackType = 1) // 0: alignment with hits | 1: alignment with trakcs
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

  FstEvent *mFstEvent = new FstEvent();
  std::string inputfile = "/Users/xusun/WorkSpace/STAR/Data/ForwardSiliconTracker/FstCosmicTestStand_Mar2020/output/FstClusters_HV140.root";
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;
  TFile *mFile_InPut = TFile::Open(inputfile.c_str());
  TTree *mTree_FstEvent = (TTree*)mFile_InPut->Get("mTree_FstEvent");
  mTree_FstEvent->SetBranchAddress("FstEvent",&mFstEvent);

  long NumOfEvents = (long)mTree_FstEvent->GetEntries();
  cout << "total number of events: " << NumOfEvents << endl;
  // if(NumOfEvents > 1000) NumOfEvents = 1000;
  // NumOfEvents = 500;
  mTree_FstEvent->GetEntry(0);

  if(trackType == 0) cout << "Read in Hits info:" << endl;
  if(trackType == 1) cout << "Read in Clusters info:" << endl;
  int numOfTracks = 0;
  for(int i_event = 0; i_event < NumOfEvents; ++i_event)
  {
    if(i_event%1000==0) cout << "processing events:  " << i_event << "/" << NumOfEvents << endl;
    mTree_FstEvent->GetEntry(i_event);

    for(int i_track = 0; i_track < mFstEvent->getNumTracks(); ++i_track)
    { // get Hits info
      FstTrack *fstTrack = mFstEvent->getTrack(i_track);
      if(fstTrack->getTrackType() == trackType && fstTrack->getId(0) > 0)
      {
	x0_fst.push_back(fstTrack->getPosOrig(0).X());
	y0_fst.push_back(fstTrack->getPosOrig(0).Y());
	x1_ist.push_back(fstTrack->getPosOrig(1).X());
	y1_ist.push_back(fstTrack->getPosOrig(1).Y());
	x3_ist.push_back(fstTrack->getPosOrig(3).X());
	y3_ist.push_back(fstTrack->getPosOrig(3).Y());

	// std::cout << "x0_fst: " << x0_fst[numOfTracks] << ", y0_fst: " << y0_fst[numOfTracks] << ", x1_ist: " << x1_ist[numOfTracks] << ", y1_ist: " << y1_ist[numOfTracks] << ", x3_ist: " << x3_ist[numOfTracks] << ", y3_ist: " << y3_ist[numOfTracks] << endl;
	// std::cout << endl;
	numOfTracks++;
      }
    }
  }
  mFile_InPut->Close();

  cout << "Start Minuit Fit for alignment => " << endl;

  int nFit = 0;
  cout << "Iteration " << nFit << " ==> " << endl;
  tPars initPars = std::make_tuple(0.00,0.0,0.0,1000.0,1000.0,0); // set initial fit parameters
  tPars fitPars = minuitAlignment(x0_fst, y0_fst, x1_ist, y1_ist, x3_ist, y3_ist, initPars);
  cout << endl;

  // reject trakcs with xCut = 50 and yCut = 5
  tPars itPars = updateFitParameters(fitPars, 50, 5, 1);
  int nOffset_temp = std::get<5>(itPars);
  while(nOffset_temp > 0)
  { 
    nFit++;
    cout << "Iteration " << nFit << " ==> " << endl;
    fitPars = minuitAlignment(x0_fst, y0_fst, x1_ist, y1_ist, x3_ist, y3_ist, itPars);
    nOffset_temp = std::get<5>(fitPars);
    itPars = updateFitParameters(fitPars,50,5,nOffset_temp);
    cout << endl;
  }

  // reject trakcs with xCut = 25 and yCut = 2.5 
  itPars = updateFitParameters(fitPars, 25, 2.5, 1);
  nOffset_temp = std::get<5>(itPars);
  while(nOffset_temp > 0)
  {
    nFit++;
    cout << "Iteration " << nFit << " ==> " << endl;
    fitPars = minuitAlignment(x0_fst, y0_fst, x1_ist, y1_ist, x3_ist, y3_ist, itPars);
    nOffset_temp = std::get<5>(fitPars);
    itPars = updateFitParameters(fitPars,25,2.5,nOffset_temp);
    cout << endl;
  }

  cout << "Finish Minuit Fit for alignment => " << endl;

  cout << "Minuit minimization: phi_rot = " << std::get<0>(fitPars) << ", x_shift = " << std::get<1>(fitPars) << ", y_shift = " << std::get<2>(fitPars) << endl;

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

  for(int i_hit = 0; i_hit < x0_orig.size(); ++i_hit)
  { // set hit containers
    x0_temp.push_back(x0_orig[i_hit]);
    y0_temp.push_back(y0_orig[i_hit]);
    x1_temp.push_back(x1_orig[i_hit]);
    y1_temp.push_back(y1_orig[i_hit]);
    x3_temp.push_back(x3_orig[i_hit]);
    y3_temp.push_back(y3_orig[i_hit]);
  }

  const double pitchLayer03 = 134.9375; // mm
  const double pitchLayer12 = 34.925; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
  const double pitchLayer23 = 30.1625; //mm, distances between the 2nd & 3rd Layer

  // reject offset trakcs
  const double phi_rot_temp = std::get<0>(cParameters); // aligment parameters from previous iteration
  const double x_shift_temp  = std::get<1>(cParameters);
  const double y_shift_temp  = std::get<2>(cParameters);
  const double xCut_temp = std::get<3>(cParameters);
  const double yCut_temp = std::get<4>(cParameters);
  cout << "reject trakcs out of xCut = " << xCut_temp << " and yCut: " << yCut_temp << endl;

  int numOfUsedHits = 0;
  for(int i_hit = 0; i_hit < x0_temp.size(); i_hit++)
  {
    double x1_corr_temp = x1_temp[i_hit]*TMath::Cos(phi_rot_temp) + y1_temp[i_hit]*TMath::Sin(phi_rot_temp) + x_shift_temp;
    double y1_corr_temp = y1_temp[i_hit]*TMath::Cos(phi_rot_temp) - x1_temp[i_hit]*TMath::Sin(phi_rot_temp) + y_shift_temp;

    double x3_corr_temp = x3_temp[i_hit]*TMath::Cos(phi_rot_temp) + y3_temp[i_hit]*TMath::Sin(phi_rot_temp) + x_shift_temp;
    double y3_corr_temp = y3_temp[i_hit]*TMath::Cos(phi_rot_temp) - x3_temp[i_hit]*TMath::Sin(phi_rot_temp) + y_shift_temp;

    double x0_proj_temp = x3_corr_temp + (x1_corr_temp-x3_corr_temp)*pitchLayer03/(pitchLayer12+pitchLayer23);
    double y0_proj_temp = y3_corr_temp + (y1_corr_temp-y3_corr_temp)*pitchLayer03/(pitchLayer12+pitchLayer23);

    double dx = x0_temp[i_hit] - x0_proj_temp;
    double dy = y0_temp[i_hit] - y0_proj_temp;
    // cout << "dx = " << dx << ", dy = " << dy << endl;

    if(abs(dx) < xCut_temp && abs(dy) < yCut_temp)
    {
      x0_fit.push_back(x0_temp[i_hit]);
      y0_fit.push_back(y0_temp[i_hit]);
      x1_fit.push_back(x1_temp[i_hit]);
      y1_fit.push_back(y1_temp[i_hit]);
      x3_fit.push_back(x3_temp[i_hit]);
      y3_fit.push_back(y3_temp[i_hit]);
      numOfUsedHits++;
    }
  }
  cout << "numOfUsedTracks = " << numOfUsedHits << endl;

  cout << "Start the Minuit minimization!" << endl;
  auto chi2Function = [&](const Double_t *par)
  { //minimisation function computing the sum of squares of residuals
    double phi_rot = par[0];
    double x_shift = par[1];
    double y_shift = par[2];

    const double x_weight = 1.0;
    const double y_weight = 250.0;
    double f = 0;
    for (int i_hit = 0; i_hit < numOfUsedHits; i_hit++)
    {
      double x1_corr = x1_fit[i_hit]*TMath::Cos(phi_rot) + y1_fit[i_hit]*TMath::Sin(phi_rot) + x_shift;
      double y1_corr = y1_fit[i_hit]*TMath::Cos(phi_rot) - x1_fit[i_hit]*TMath::Sin(phi_rot) + y_shift;

      double x3_corr = x3_fit[i_hit]*TMath::Cos(phi_rot) + y3_fit[i_hit]*TMath::Sin(phi_rot) + x_shift;
      double y3_corr = y3_fit[i_hit]*TMath::Cos(phi_rot) - x3_fit[i_hit]*TMath::Sin(phi_rot) + y_shift;

      double x0_proj = x3_corr + (x1_corr-x3_corr)*pitchLayer03/(pitchLayer12+pitchLayer23);
      double y0_proj = y3_corr + (y1_corr-y3_corr)*pitchLayer03/(pitchLayer12+pitchLayer23);

      double dx = x0_fit[i_hit] - x0_proj;
      double dy = y0_fit[i_hit] - y0_proj;

      f += x_weight*dx*dx + y_weight*dy*dy;
    }
    return f;
  };

  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,3);
  ROOT::Fit::Fitter fitter;

  // double pStart[3] = {0.01,140,-20};
  double pStart[3] = {phi_rot_temp,x_shift_temp,y_shift_temp};
  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("phi_rot");
  fitter.Config().ParSettings(1).SetName("x_shift");
  fitter.Config().ParSettings(2).SetName("y_shift");

  // do the fit
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("find alignment","Minuit Fit failed");
    return std::make_tuple(-1.0, -1.0, -1.0, -1.0, -1.0, -1);
  }

  const ROOT::Fit::FitResult & result = fitter.Result();
  // result.Print(std::cout);
  const double *fitpar = result.GetParams();
  double phi_rot_MF = fitpar[0];
  double x_shift_MF = fitpar[1];
  double y_shift_MF = fitpar[2];

  // calculate nOffset with current fit parameters
  int nOffset = 0;
  for (int i_hit = 0; i_hit < numOfUsedHits; i_hit++)
  {
    double x1_corr_MF = x1_fit[i_hit]*TMath::Cos(phi_rot_MF) + y1_fit[i_hit]*TMath::Sin(phi_rot_MF) + x_shift_MF;
    double y1_corr_MF = y1_fit[i_hit]*TMath::Cos(phi_rot_MF) - x1_fit[i_hit]*TMath::Sin(phi_rot_MF) + y_shift_MF;

    double x3_corr_MF = x3_fit[i_hit]*TMath::Cos(phi_rot_MF) + y3_fit[i_hit]*TMath::Sin(phi_rot_MF) + x_shift_MF;
    double y3_corr_MF = y3_fit[i_hit]*TMath::Cos(phi_rot_MF) - x3_fit[i_hit]*TMath::Sin(phi_rot_MF) + y_shift_MF;

    double x0_proj_MF = x3_corr_MF + (x1_corr_MF-x3_corr_MF)*pitchLayer03/(pitchLayer12+pitchLayer23);
    double y0_proj_MF = y3_corr_MF + (y1_corr_MF-y3_corr_MF)*pitchLayer03/(pitchLayer12+pitchLayer23);

    double dx = x0_fit[i_hit] - x0_proj_MF;
    double dy = y0_fit[i_hit] - y0_proj_MF;

    if( !(abs(dx) < xCut_temp && abs(dy) < yCut_temp) )
    {
      nOffset++;
    }
  }

  tPars tParameters = std::make_tuple(phi_rot_MF, x_shift_MF, y_shift_MF, xCut_temp, yCut_temp, nOffset);

  // cout << "Minuit minimization: phi_rot_ist1 = " << phi_rot_ist1_MF << ", phi_rot_ist3 = " << phi_rot_ist3_MF << ", x_shift = " << x_shift_MF << ", y_shift = " << y_shift_MF << endl;
  cout << "Minuit minimization: phi_rot = " << std::get<0>(tParameters) << ", x_shift = " << (double)std::get<1>(tParameters) << ", y_shift = " << std::get<2>(tParameters) << ", nOffset = " << std::get<5>(tParameters) << endl;

  return tParameters;
}

tPars updateFitParameters(tPars fitPars, double xCut, double yCut, int nOffset)
{
  const double phi_old      = std::get<0>(fitPars);
  const double x_shift_old  = std::get<1>(fitPars);
  const double y_shift_old  = std::get<2>(fitPars);
  const double xCut_old     = std::get<3>(fitPars);
  const double yCut_old     = std::get<4>(fitPars);
  const double nOffset_old  = std::get<5>(fitPars);

  tPars newPars = std::make_tuple(phi_old, x_shift_old, y_shift_old, xCut, yCut, nOffset);

  return newPars;
}
