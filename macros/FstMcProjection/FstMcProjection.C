#include <TRandom3.h>
#include <TStyle.h>
#include <TMath.h>
#include "../../src/FstUtil/FstCons.h"

void FstMcProjection()
{
  gRandom->SetSeed();

  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  const double z0_fst = FST::pitchLayer03;
  const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
  const double z2_ist = FST::pitchLayer23;
  const double z3_ist = 0.0;

  std::cout << std::fixed;
  std::cout << std::setprecision(4);

  TH2F *h_mIst3Pixel = new TH2F("h_mIst3Pixel","h_mIst3Pixel",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);
  TH2F *h_mIst1Pixel = new TH2F("h_mIst1Pixel","h_mIst1Pixel",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);
  TH2F *h_mFstPixel  = new TH2F("h_mFstPixel","h_mFstPixel",6,FST::rMin,FST::rMax,FST::numPhiSeg,-1.0*FST::phiMax,1.0*FST::phiMax);

  TH2F *h_mIst3Display = new TH2F("h_mIst3Display","h_mIst3Display",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);
  TH2F *h_mIst1Display = new TH2F("h_mIst1Display","h_mIst1Display",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);

  TH1F *h_mIst3ResX_2Layer = new TH1F("h_mIst3ResX_2Layer","h_mIst3ResX_2Layer",40,-FST::pitchColumn,FST::pitchColumn);
  TH1F *h_mIst3ResY_2Layer = new TH1F("h_mIst3ResY_2Layer","h_mIst3ResY_2Layer",40,-FST::pitchRow,FST::pitchRow);
  TH1F *h_mIst1ResX_2Layer = new TH1F("h_mIst1ResX_2Layer","h_mIst1ResX_2Layer",40,-FST::pitchColumn,FST::pitchColumn);
  TH1F *h_mIst1ResY_2Layer = new TH1F("h_mIst1ResY_2Layer","h_mIst1ResY_2Layer",40,-FST::pitchRow,FST::pitchRow);

  TH1F *h_mIstProjResX_2Layer  = new TH1F("h_mIstProjResX_2Layer","h_mIstProjResX_2Layer",21,-15.75,15.75);
  TH1F *h_mIstProjResY_2Layer  = new TH1F("h_mIstProjResY_2Layer","h_mIstProjResY_2Layer",41,-3.075,3.075);
  TH2F *h_mIstProjResXY_2Layer = new TH2F("h_mIstProjResXY_2Layer","h_mIstProjResXY_2Layer",21,-15.75,15.75,41,-3.075,3.075);

  TH2F *h_mFstDisplay            = new TH2F("h_mFstDisplay","h_mFstDisplay",6,FST::rMin,FST::rMax,FST::numPhiSeg,-1.0*FST::phiMax,1.0*FST::phiMax);
  TH1F *h_mFstProjResR_2Layer    = new TH1F("h_mFstProjResR_2Layer","h_mFstProjResR_2Layer",80,-2.0*FST::pitchR,2.0*FST::pitchR);
  TH1F *h_mFstProjResPhi_2Layer  = new TH1F("h_mFstProjResPhi_2Layer","h_mFstProjResPhi_2Layer",80,-4.0*FST::pitchPhi,4.0*FST::pitchPhi);
  TH2F *h_mFstProjResRPhi_2Layer = new TH2F("h_mFstProjResRPhi_2Layer","h_mFstProjResRPhi_2Layer",80,-2.0*FST::pitchR,2.0*FST::pitchR,80,-4.0*FST::pitchPhi,4.0*FST::pitchPhi);

  for(int i_ran =0; i_ran < 500000; ++i_ran)
  {
    double x3_gen    = gRandom->Rndm()*lengthColumn;
    double y3_gen    = gRandom->Rndm()*lengthRow;
    int binX3        = h_mIst3Pixel->GetXaxis()->FindBin(x3_gen);
    int binY3        = h_mIst3Pixel->GetYaxis()->FindBin(y3_gen);
    double x3_center = h_mIst3Pixel->GetXaxis()->GetBinCenter(binX3);
    double y3_center = h_mIst3Pixel->GetYaxis()->GetBinCenter(binY3);
    h_mIst3Display->Fill(x3_gen,y3_gen);
    h_mIst3ResX_2Layer->Fill(x3_center-x3_gen);
    h_mIst3ResY_2Layer->Fill(y3_center-y3_gen);

    double x1_gen    = gRandom->Rndm()*lengthColumn;
    double y1_gen    = gRandom->Rndm()*lengthRow;
    int binX1        = h_mIst1Pixel->GetXaxis()->FindBin(x1_gen);
    int binY1        = h_mIst1Pixel->GetYaxis()->FindBin(y1_gen);
    double x1_center = h_mIst1Pixel->GetXaxis()->GetBinCenter(binX1);
    double y1_center = h_mIst1Pixel->GetYaxis()->GetBinCenter(binY1);
    h_mIst1Display->Fill(x1_gen,y1_gen);
    h_mIst1ResX_2Layer->Fill(x1_center-x1_gen);
    h_mIst1ResY_2Layer->Fill(y1_center-y1_gen);

    double x0_proj = x3_gen + (x1_gen-x3_gen)*z0_fst/z1_ist;
    double y0_proj = y3_gen + (y1_gen-y3_gen)*z0_fst/z1_ist;

    double x0_cProj = x3_center + (x1_center-x3_center)*z0_fst/z1_ist;
    double y0_cProj = y3_center + (y1_center-y3_center)*z0_fst/z1_ist;

    // IST Projection Error
    h_mIstProjResX_2Layer->Fill(x0_cProj-x0_proj);
    h_mIstProjResY_2Layer->Fill(y0_cProj-y0_proj);
    h_mIstProjResXY_2Layer->Fill(x0_cProj-x0_proj,y0_cProj-y0_proj);

    // FST Residual
    // real position
    double x3_corr = x3_gen + FST::x2_shift; // shift into FST coordinate
    double y3_corr = y3_gen + FST::y2_shift;
    double x1_corr = x1_gen + FST::x2_shift; // shift into FST coordinate
    double y1_corr = y1_gen + FST::y2_shift;
    double x0_corr = x3_corr + (x1_corr-x3_corr)*z0_fst/z1_ist;
    double y0_corr = y3_corr + (y1_corr-y3_corr)*z0_fst/z1_ist;

    double r0_corr     = TMath::Sqrt(x0_corr*x0_corr+y0_corr*y0_corr);
    double phi0_corr   = TMath::ATan2(y0_corr,x0_corr);
    int binR0          = h_mFstPixel->GetXaxis()->FindBin(r0_corr);
    int binPhi0        = h_mFstPixel->GetYaxis()->FindBin(phi0_corr);
    double r0_center   = h_mFstPixel->GetXaxis()->GetBinCenter(binR0);
    double phi0_center = h_mFstPixel->GetYaxis()->GetBinCenter(binPhi0);

    // pixel position
    double x3_CORR = x3_center + FST::x2_shift; // shift into FST coordinate
    double y3_CORR = y3_center + FST::y2_shift;
    double x1_CORR = x1_center + FST::x2_shift; // shift into FST coordinate
    double y1_CORR = y1_center + FST::y2_shift;
    double x0_CORR = x3_CORR + (x1_CORR-x3_CORR)*z0_fst/z1_ist;
    double y0_CORR = y3_CORR + (y1_CORR-y3_CORR)*z0_fst/z1_ist;

    double r0_CORR     = TMath::Sqrt(x0_CORR*x0_CORR+y0_CORR*y0_CORR);
    double phi0_CORR   = TMath::ATan2(y0_CORR,x0_CORR);

    if(r0_center >= FST::rOuter && r0_center < FST::rOuter+4.0*FST::pitchR && phi0_corr >= -FST::phiMax && phi0_corr < FST::phiMax)
    {
      h_mFstDisplay->Fill(r0_CORR,phi0_CORR);
      h_mFstProjResR_2Layer->Fill(r0_center-r0_CORR);
      h_mFstProjResPhi_2Layer->Fill(phi0_center-phi0_CORR);
      h_mFstProjResRPhi_2Layer->Fill(r0_center-r0_CORR,phi0_center-phi0_CORR);
    }
  }

  TFile *File_OutPut = new TFile("../../output/simulation/FstMcProjection.root","RECREATE");
  File_OutPut->cd();
  h_mIst1Display->Write();
  h_mIst3Display->Write();
  h_mIst1ResX_2Layer->Write();
  h_mIst1ResY_2Layer->Write();
  h_mIst3ResX_2Layer->Write();
  h_mIst3ResY_2Layer->Write();

  h_mIstProjResX_2Layer->Write();
  h_mIstProjResY_2Layer->Write();
  h_mIstProjResXY_2Layer->Write();

  h_mFstDisplay->Write();
  h_mFstProjResR_2Layer->Write();
  h_mFstProjResPhi_2Layer->Write();
  h_mFstProjResRPhi_2Layer->Write();
  File_OutPut->Close();
}
