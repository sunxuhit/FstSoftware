#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <TMath.h>
#include "Fit/Fitter.h"
#include "Fit/Chi2FCN.h"
#include <Math/Functor.h>
#include "Math/WrappedParamFunction.h"
#include "Math/AdaptiveIntegratorMultiDim.h"

// typedef std::vector<double> dVec;

// int minuitRingRadius(dVec x0_fst, dVec y0_fst, dVec x1_ist, dVec y1_ist, dVec x3_ist, dVec y3_ist);

int getAlignment()
{
  const int maxHits = 500;
  double x0_fst[maxHits];
  double y0_fst[maxHits];
  double x1_ist[maxHits];
  double y1_ist[maxHits];
  double x3_ist[maxHits];
  double y3_ist[maxHits];
  // double x0_proj[maxHits];
  // double y0_proj[maxHits];

  for(int i_hit = 0; i_hit < maxHits; ++i_hit)
  {
    x0_fst[i_hit] = -1.0;
    y0_fst[i_hit] = -1.0;
    x1_ist[i_hit] = -1.0;
    y1_ist[i_hit] = -1.0;
    x3_ist[i_hit] = -1.0;
    y3_ist[i_hit] = -1.0;
    // x0_proj[i_hit] = -1.0;
    // y0_proj[i_hit] = -1.0;
  }

  std::string inputfile = "./hits.txt";
  std::cout << "inputfile = " << inputfile.c_str() << std::endl;
  std::ifstream file_hits ( inputfile.c_str() );
  if ( !file_hits.is_open() )
  {
    std::cout << "Abort. Fail to read in run list: " << inputfile << std::endl;
    return -1;
  }

  double temp_x0_fst = 0.0;
  double temp_y0_fst = 0.0;
  double temp_x1_ist = 0.0;
  double temp_y1_ist = 0.0;
  double temp_x3_ist = 0.0;
  double temp_y3_ist = 0.0;
  // double temp_x0_proj = 0.0;
  // double temp_y0_proj = 0.0;
  int numOfHits = 0;

  std::cout << "read in hits info: " << std::endl;
  // while (file_hits >> temp_x0_fst >> temp_y0_fst >> temp_x0_proj >> temp_y0_proj)
  while (file_hits >> temp_x0_fst >> temp_y0_fst >> temp_x1_ist >> temp_y1_ist >> temp_x3_ist >> temp_y3_ist)
  {
    x0_fst[numOfHits] = temp_x0_fst;
    y0_fst[numOfHits] = temp_y0_fst;
    x1_ist[numOfHits] = temp_x1_ist;
    y1_ist[numOfHits] = temp_y1_ist;
    x3_ist[numOfHits] = temp_x3_ist;
    y3_ist[numOfHits] = temp_y3_ist;
    // x0_proj[numOfHits] = temp_x0_proj;
    // y0_proj[numOfHits] = temp_y0_proj;

    // std::cout << "temp_x0_fst: " << temp_x0_fst  << ", temp_y0_fst: " << temp_y0_fst  << ", temp_x0_proj: " << temp_x0_proj << ", temp_y0_proj: " << temp_y0_proj << endl;
    // std::cout << "x0_fst: " << x0_fst[numOfHits] << ", y0_fst: " << y0_fst[numOfHits] << ", x0_proj: " << x0_proj[numOfHits] << ", y0_proj: " << y0_proj[numOfHits] << endl;
    // std::cout << endl;

    // std::cout << "temp_x0_fst: " << temp_x0_fst  << ", temp_y0_fst: " << temp_y0_fst  << ", temp_x1_ist: " << temp_x1_ist << ", temp_y1_ist: " << temp_y1_ist << ", temp_x3_ist: " << temp_x3_ist << ", temp_y3_ist: " << temp_y3_ist << endl;
    // std::cout << "x0_fst: " << x0_fst[numOfHits] << ", y0_fst: " << y0_fst[numOfHits] << ", x1_ist: " << x1_ist[numOfHits] << ", y1_ist: " << y1_ist[numOfHits] << ", x3_ist: " << x3_ist[numOfHits] << ", y3_ist: " << y3_ist[numOfHits] << endl;
    // std::cout << endl;

    numOfHits++;
  }
  file_hits.close();
  std::cout << "close hits.txt. " << std::endl;
  cout << "total number of hits: " << numOfHits << endl;

  cout << "Start the Minuit minimization!" << endl;
  auto chi2Function = [&](const Double_t *par)
  { //minimisation function computing the sum of squares of residuals
    double phi_rot_ist1 = par[0];
    double phi_rot_ist3 = par[1];
    double x_shift = par[2];
    double y_shift = par[3];

    const double pitchLayer03 = 134.9375; // mm
    const double pitchLayer12 = 34.925; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
    const double pitchLayer23 = 30.1625; //mm, distances between the 2nd & 3rd Layer
    const double x_weight = 1.0;
    const double y_weight = 250.0;
    
    double f = 0;
    for (int i_hit = 0; i_hit < numOfHits; i_hit++)
    {
      double x1_corr = x1_ist[i_hit]*TMath::Cos(phi_rot_ist1) + y1_ist[i_hit]*TMath::Sin(phi_rot_ist1) + x_shift;
      double y1_corr = y1_ist[i_hit]*TMath::Cos(phi_rot_ist1) - x1_ist[i_hit]*TMath::Sin(phi_rot_ist1) + y_shift;

      double x3_corr = x3_ist[i_hit]*TMath::Cos(phi_rot_ist3) + y3_ist[i_hit]*TMath::Sin(phi_rot_ist3) + x_shift;
      double y3_corr = y3_ist[i_hit]*TMath::Cos(phi_rot_ist3) - x3_ist[i_hit]*TMath::Sin(phi_rot_ist3) + y_shift;

      double x0_proj = x3_corr + (x1_corr-x3_corr)*pitchLayer03/(pitchLayer12+pitchLayer23);
      double y0_proj = y3_corr + (y1_corr-y3_corr)*pitchLayer03/(pitchLayer12+pitchLayer23);

      double dx = x0_fst[i_hit] - x0_proj;
      double dy = y0_fst[i_hit] - y0_proj;
      f += x_weight*dx*dx + y_weight*dy*dy;
    }
    return f;
  };

  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,4);
  ROOT::Fit::Fitter fitter;

  double pStart[4] = {0.01,0.01,120,-10};
  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("phi_rot_ist1");
  fitter.Config().ParSettings(1).SetName("phi_rot_ist3");
  fitter.Config().ParSettings(2).SetName("x_shift");
  fitter.Config().ParSettings(3).SetName("y_shift");

  // do the fit
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("find alignment","Minuit Fit failed");
    return -1;
  }

  const ROOT::Fit::FitResult & result = fitter.Result();
  // result.Print(std::cout);
  const double *fitpar = result.GetParams();
  double phi_rot_ist1_MinuitFit = fitpar[0];
  double phi_rot_ist3_MinuitFit = fitpar[1];
  double x_shift_MinuitFit = fitpar[2];
  double y_shift_MinuitFit = fitpar[3];

  cout << "Minuit minimization: phi_rot_ist1 = " << phi_rot_ist1_MinuitFit << ", phi_rot_ist3 = " << phi_rot_ist3_MinuitFit << ", x_shift = " << x_shift_MinuitFit << ", y_shift = " << y_shift_MinuitFit << endl;

  return 0;
}
