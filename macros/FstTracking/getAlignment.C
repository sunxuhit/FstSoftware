#include <iostream>
#include <fstream>
#include <string>

#include <TMath.h>
#include "Fit/Fitter.h"
#include "Fit/Chi2FCN.h"
#include <Math/Functor.h>
#include "Math/WrappedParamFunction.h"
#include "Math/AdaptiveIntegratorMultiDim.h"

int getAlignment()
{
  const int maxHits = 500;
  double x0_fst[maxHits];
  double y0_fst[maxHits];
  double x0_proj[maxHits];
  double y0_proj[maxHits];

  for(int i_hit = 0; i_hit < maxHits; ++i_hit)
  {
    x0_fst[i_hit] = -1.0;
    y0_fst[i_hit] = -1.0;
    x0_proj[i_hit] = -1.0;
    y0_proj[i_hit] = -1.0;
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
  double temp_x0_proj = 0.0;
  double temp_y0_proj = 0.0;
  int numOfHits = 0;

  std::cout << "read in hits info: " << std::endl;
  while (file_hits >> temp_x0_fst >> temp_y0_fst >> temp_x0_proj >> temp_y0_proj)
  {
    x0_fst[numOfHits] = temp_x0_fst;
    y0_fst[numOfHits] = temp_y0_fst;
    x0_proj[numOfHits] = temp_x0_proj;
    y0_proj[numOfHits] = temp_y0_proj;

    // std::cout << "temp_x0_fst: " << temp_x0_fst  << ", temp_y0_fst: " << temp_y0_fst  << ", temp_x0_proj: " << temp_x0_proj << ", temp_y0_proj: " << temp_y0_proj << endl;
    // std::cout << "x0_fst: " << x0_fst[numOfHits] << ", y0_fst: " << y0_fst[numOfHits] << ", x0_proj: " << x0_proj[numOfHits] << ", y0_proj: " << y0_proj[numOfHits] << endl;
    // std::cout << endl;

    numOfHits++;
  }
  file_hits.close();
  std::cout << "close hits.txt. " << std::endl;
  cout << "total number of hits: " << numOfHits << endl;

  cout << "Start the Minuit minimization!" << endl;
  auto chi2Function = [&](const Double_t *par)
  {
    //minimisation function computing the sum of squares of residuals
    double f = 0;
    double phi_rot = par[0];
    double x_shift = par[1];
    double y_shift = par[2];

    for (int i_hit = 0; i_hit < numOfHits; i_hit++)
    {
      double dx = x0_fst[i_hit] - (x0_proj[i_hit]*TMath::Cos(phi_rot) + y0_proj[i_hit]*TMath::Sin(phi_rot) + x_shift);
      double dy = y0_fst[i_hit] - (y0_proj[i_hit]*TMath::Cos(phi_rot) - x0_proj[i_hit]*TMath::Sin(phi_rot) + y_shift);
      f += dx*dx + dy*dy;
    }
    return f;
  };

  // wrap chi2 funciton in a function object for the fit
  // 3 is the number of fit parameters (size of array par)
  ROOT::Math::Functor fcn(chi2Function,3);
  ROOT::Fit::Fitter fitter;

  double pStart[3] = {0.01,140,-20};
  fitter.SetFCN(fcn, pStart);
  fitter.Config().ParSettings(0).SetName("phi_rot");
  fitter.Config().ParSettings(1).SetName("x_shift");
  fitter.Config().ParSettings(2).SetName("y_shift");

  // do the fit
  bool ok = fitter.FitFCN();
  if (!ok) {
    Error("minuitRingRadius","Minuit Fit failed");
    return -1;
  }

  const ROOT::Fit::FitResult & result = fitter.Result();
  // result.Print(std::cout);
  const double *fitpar = result.GetParams();
  double phi_rot_MinuitFit = fitpar[0];
  double x_shift_MinuitFit = fitpar[1];
  double y_shift_MinuitFit = fitpar[2];

  cout << "Minuit minimization: phi_rot = " << phi_rot_MinuitFit << ", x_shift = " << x_shift_MinuitFit << ", y_shift = " << y_shift_MinuitFit << endl;

  return 0;
}
