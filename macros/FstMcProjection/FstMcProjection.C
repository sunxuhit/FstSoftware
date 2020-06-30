#include <TRandom3.h>
#include <TStyle.h>
#include <TMath.h>
#include "../../src/FstUtil/FstCons.h"

void FstMcProjection(bool isRot = true, int numOfTracks = 500000)
{
  gRandom->SetSeed();

  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  const double z0_fst = FST::pitchLayer03;
  const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
  const double z2_ist = FST::pitchLayer23;
  const double z3_ist = 0.0;

  double x2_shift = 0.0;
  double y2_shift = 0.0;
  double phi_rot_ist2 = 0.0;

  // before rotation
  if( !isRot )
  {
    x2_shift = FST::x2_shift;
    y2_shift = FST::y2_shift;
    phi_rot_ist2 = 0.0;
  }

  // after rotation
  if( isRot )
  {
    x2_shift = 214.839;
    y2_shift = -46.692;
    phi_rot_ist2 = -1.55329;
  }

  cout << "isRot = " << isRot << ", x2_shift = " << x2_shift << ", y2_shift = " << y2_shift << ", phi_rot_ist2 = " << phi_rot_ist2 << endl;

  std::cout << std::fixed;
  std::cout << std::setprecision(4);

  TH2F *h_mIst3Pixel = new TH2F("h_mIst3Pixel","h_mIst3Pixel",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);
  TH2F *h_mIst1Pixel = new TH2F("h_mIst1Pixel","h_mIst1Pixel",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);
  TH2F *h_mFstPixel  = new TH2F("h_mFstPixel","h_mFstPixel",6,FST::rMin,FST::rMax,FST::numPhiSeg*2,-2.0*FST::phiMax,2.0*FST::phiMax);

  TH2F *h_mIst3Display = new TH2F("h_mIst3Display","h_mIst3Display",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);
  TH2F *h_mIst1Display = new TH2F("h_mIst1Display","h_mIst1Display",FST::noColumns,0.0,lengthColumn,FST::noRows,0.0,lengthRow);

  TH1F *h_mIst3ResX_2Layer = new TH1F("h_mIst3ResX_2Layer","h_mIst3ResX_2Layer",40,-FST::pitchColumn,FST::pitchColumn);
  TH1F *h_mIst3ResY_2Layer = new TH1F("h_mIst3ResY_2Layer","h_mIst3ResY_2Layer",40,-FST::pitchRow,FST::pitchRow);
  TH1F *h_mIst1ResX_2Layer = new TH1F("h_mIst1ResX_2Layer","h_mIst1ResX_2Layer",40,-FST::pitchColumn,FST::pitchColumn);
  TH1F *h_mIst1ResY_2Layer = new TH1F("h_mIst1ResY_2Layer","h_mIst1ResY_2Layer",40,-FST::pitchRow,FST::pitchRow);

  // IST projection residual
  TH1F *h_mIstProjResX_2Layer  = new TH1F("h_mIstProjResX_2Layer","h_mIstProjResX_2Layer",21,-15.75,15.75);
  TH1F *h_mIstProjResY_2Layer  = new TH1F("h_mIstProjResY_2Layer","h_mIstProjResY_2Layer",41,-3.075,3.075);
  TH2F *h_mIstProjResXY_2Layer = new TH2F("h_mIstProjResXY_2Layer","h_mIstProjResXY_2Layer",21,-15.75,15.75,41,-3.075,3.075);

  TH2F *h_mFstDisplay            = new TH2F("h_mFstDisplay","h_mFstDisplay",6,FST::rMin,FST::rMax,FST::numPhiSeg,-1.0*FST::phiMax,1.0*FST::phiMax);
  // FST residual
  TH1F *h_mFstProjResR_2Layer    = new TH1F("h_mFstProjResR_2Layer","h_mFstProjResR_2Layer",160,-4.0*FST::pitchR,4.0*FST::pitchR);
  TH1F *h_mFstProjResPhi_2Layer  = new TH1F("h_mFstProjResPhi_2Layer","h_mFstProjResPhi_2Layer",320,-32.0*FST::pitchPhi,32.0*FST::pitchPhi);
  TH2F *h_mFstProjResRPhi_2Layer = new TH2F("h_mFstProjResRPhi_2Layer","h_mFstProjResRPhi_2Layer",160,-4.0*FST::pitchR,4.0*FST::pitchR,320,-32.0*FST::pitchPhi,32.0*FST::pitchPhi);

  TH1F *h_mFstProjResX_2Layer  = new TH1F("h_mFstProjResX_2Layer","h_mFstProjResX_2Layer",50,-160.0,160.0);
  TH1F *h_mFstProjResY_2Layer  = new TH1F("h_mFstProjResY_2Layer","h_mFstProjResY_2Layer",100,-16.0,16.0);
  TH2F *h_mFstProjResXY_2Layer = new TH2F("h_mFstProjResXY_2Layer","h_mFstProjResXY_2Layer",50,-160.0,160.0,100,-16.0,16.0);

  TH1F *h_mFstSimResR_2Layer    = new TH1F("h_mFstSimResR_2Layer","h_mFstSimResR_2Layer",160,-4.0*FST::pitchR,4.0*FST::pitchR);
  TH1F *h_mFstSimResPhi_2Layer  = new TH1F("h_mFstSimResPhi_2Layer","h_mFstSimResPhi_2Layer",320,-32.0*FST::pitchPhi,32.0*FST::pitchPhi);
  TH2F *h_mFstSimResRPhi_2Layer = new TH2F("h_mFstSimResRPhi_2Layer","h_mFstSimResRPhi_2Layer",160,-4.0*FST::pitchR,4.0*FST::pitchR,320,-32.0*FST::pitchPhi,32.0*FST::pitchPhi);

  TH2F *h_mIstCounts_2Layer[8];
  TH2F *h_mFstCounts_2Layer[8];
  for(int i_match = 0; i_match < 8; ++i_match)
  {
    string HistName;
    // simple clusters
    HistName = Form("h_mIstCounts_2Layer_SF%d",i_match);
    h_mIstCounts_2Layer[i_match] = new TH2F(HistName.c_str(),HistName.c_str(),30,FST::rMin,FST::rMax,90,-1.5*FST::phiMax,1.5*FST::phiMax);
    HistName = Form("h_mFstCounts_2Layer_SF%d",i_match);
    h_mFstCounts_2Layer[i_match] = new TH2F(HistName.c_str(),HistName.c_str(),30,FST::rMin,FST::rMax,90,-1.5*FST::phiMax,1.5*FST::phiMax);
  }

  for(int i_track =0; i_track < numOfTracks; ++i_track)
  {
    double x3_gen = gRandom->Rndm()*lengthColumn;
    double y3_gen = gRandom->Rndm()*lengthRow;
    int binX3     = h_mIst3Pixel->GetXaxis()->FindBin(x3_gen);
    int binY3     = h_mIst3Pixel->GetYaxis()->FindBin(y3_gen);
    double x3_ro  = h_mIst3Pixel->GetXaxis()->GetBinCenter(binX3);
    double y3_ro  = h_mIst3Pixel->GetYaxis()->GetBinCenter(binY3);
    h_mIst3Display->Fill(x3_gen,y3_gen);
    h_mIst3ResX_2Layer->Fill(x3_ro-x3_gen);
    h_mIst3ResY_2Layer->Fill(y3_ro-y3_gen);

    double x1_gen = gRandom->Rndm()*lengthColumn;
    double y1_gen = gRandom->Rndm()*lengthRow;
    int binX1     = h_mIst1Pixel->GetXaxis()->FindBin(x1_gen);
    int binY1     = h_mIst1Pixel->GetYaxis()->FindBin(y1_gen);
    double x1_ro  = h_mIst1Pixel->GetXaxis()->GetBinCenter(binX1);
    double y1_ro  = h_mIst1Pixel->GetYaxis()->GetBinCenter(binY1);
    h_mIst1Display->Fill(x1_gen,y1_gen);
    h_mIst1ResX_2Layer->Fill(x1_ro-x1_gen);
    h_mIst1ResY_2Layer->Fill(y1_ro-y1_gen);

    double x0_proj = x3_gen + (x1_gen-x3_gen)*z0_fst/z1_ist;
    double y0_proj = y3_gen + (y1_gen-y3_gen)*z0_fst/z1_ist;

    double x0_cProj = x3_ro + (x1_ro-x3_ro)*z0_fst/z1_ist;
    double y0_cProj = y3_ro + (y1_ro-y3_ro)*z0_fst/z1_ist;

    // IST Projection Error
    h_mIstProjResX_2Layer->Fill(x0_cProj-x0_proj);
    h_mIstProjResY_2Layer->Fill(y0_cProj-y0_proj);
    h_mIstProjResXY_2Layer->Fill(x0_cProj-x0_proj,y0_cProj-y0_proj);

    // FST Residual
    // real position
    double x3_corr = x3_gen*TMath::Cos(phi_rot_ist2) + y3_gen*TMath::Sin(phi_rot_ist2) + x2_shift;
    double y3_corr = y3_gen*TMath::Cos(phi_rot_ist2) - x3_gen*TMath::Sin(phi_rot_ist2) + y2_shift;
    double x1_corr = x1_gen*TMath::Cos(phi_rot_ist2) + y1_gen*TMath::Sin(phi_rot_ist2) + x2_shift;
    double y1_corr = y1_gen*TMath::Cos(phi_rot_ist2) - x1_gen*TMath::Sin(phi_rot_ist2) + y2_shift;
    double x0_corr = x3_corr + (x1_corr-x3_corr)*z0_fst/z1_ist;
    double y0_corr = y3_corr + (y1_corr-y3_corr)*z0_fst/z1_ist;

    // real position in r & phi on FST
    double r0_corr   = TMath::Sqrt(x0_corr*x0_corr+y0_corr*y0_corr);
    double phi0_corr = TMath::ATan2(y0_corr,x0_corr);
    double r0_ro     = -999.0;
    double phi0_ro   = -999.0;
    double x0_ro     = -999.0;
    double y0_ro     = -999.0;
    // check if FST has readout
    if(r0_corr >= FST::rOuter && r0_corr <= FST::rOuter+4.0*FST::pitchR && phi0_corr >= -FST::phiMax && phi0_corr <= FST::phiMax)
    {
      int binR0   = h_mFstPixel->GetXaxis()->FindBin(r0_corr);
      int binPhi0 = h_mFstPixel->GetYaxis()->FindBin(phi0_corr);
      // readout position in r & phi on FST
      r0_ro   = h_mFstPixel->GetXaxis()->GetBinCenter(binR0);
      phi0_ro = h_mFstPixel->GetYaxis()->GetBinCenter(binPhi0);
      x0_ro   = r0_ro*TMath::Cos(phi0_ro);
      y0_ro   = r0_ro*TMath::Sin(phi0_ro);
    }

    // projected position from IST pixel
    double x3_CORR = x3_ro*TMath::Cos(phi_rot_ist2) + y3_ro*TMath::Sin(phi_rot_ist2) + x2_shift;
    double y3_CORR = y3_ro*TMath::Cos(phi_rot_ist2) - x3_ro*TMath::Sin(phi_rot_ist2) + y2_shift;
    double x1_CORR = x1_ro*TMath::Cos(phi_rot_ist2) + y1_ro*TMath::Sin(phi_rot_ist2) + x2_shift;
    double y1_CORR = y1_ro*TMath::Cos(phi_rot_ist2) - x1_ro*TMath::Sin(phi_rot_ist2) + y2_shift;
    double x0_CORR = x3_CORR + (x1_CORR-x3_CORR)*z0_fst/z1_ist;
    double y0_CORR = y3_CORR + (y1_CORR-y3_CORR)*z0_fst/z1_ist;

    double r0_CORR     = TMath::Sqrt(x0_CORR*x0_CORR+y0_CORR*y0_CORR);
    double phi0_CORR   = TMath::ATan2(y0_CORR,x0_CORR);

    // fill Residual
    if(r0_ro > -100.0 && phi0_ro > -100.0)
    {
      h_mFstDisplay->Fill(r0_CORR,phi0_CORR);
      h_mFstProjResR_2Layer->Fill(r0_ro-r0_CORR);
      h_mFstProjResPhi_2Layer->Fill(phi0_ro-phi0_CORR);
      h_mFstProjResRPhi_2Layer->Fill(r0_ro-r0_CORR,phi0_ro-phi0_CORR);

      h_mFstProjResX_2Layer->Fill(x0_ro-x0_CORR);
      h_mFstProjResY_2Layer->Fill(y0_ro-y0_CORR);
      h_mFstProjResXY_2Layer->Fill(x0_ro-x0_CORR,y0_ro-y0_CORR);

      h_mFstSimResR_2Layer->Fill(r0_ro-r0_corr);
      h_mFstSimResPhi_2Layer->Fill(phi0_ro-phi0_corr);
      h_mFstSimResRPhi_2Layer->Fill(r0_ro-r0_corr,phi0_ro-phi0_corr);
    }

    // fill Efficiency
    if(r0_CORR >= FST::rMin && r0_CORR < FST::rMax && phi0_CORR >= -FST::phiMax && phi0_CORR < FST::phiMax)
    {
      for(int i_match = 0; i_match < 8; ++i_match)
      {
	h_mIstCounts_2Layer[i_match]->Fill(r0_CORR,phi0_CORR);
	int nMatchedTrack = 0;
	if(r0_ro > -100.0 && phi0_ro > -100.0)
	{
	  if(i_match == 0)
	  {
	    nMatchedTrack++;
	  }
	  // if( i_match > 0 && abs(r0_ro-r0_CORR) <= i_match*0.5*FST::pitchR && abs(phi0_ro-phi0_CORR) <= 3.0*FST::pitchPhi)
	  if( i_match > 0 && abs(r0_ro-r0_CORR) <= i_match*0.5*FST::pitchR && abs(phi0_ro-phi0_CORR) <= i_match*1.5*FST::pitchPhi)
	  {
	    nMatchedTrack++;
	  }
	}
	if(nMatchedTrack > 0) h_mFstCounts_2Layer[i_match]->Fill(r0_CORR,phi0_CORR);
      }
    }
  }

  string mRot = "woRot";
  if(isRot) mRot = "Rot";
  string outputname = Form("../../output/simulation/FstMcProjection_%s.root",mRot.c_str());
  TFile *File_OutPut = new TFile(outputname.c_str(),"RECREATE");
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

  h_mFstProjResX_2Layer->Write();
  h_mFstProjResY_2Layer->Write();
  h_mFstProjResXY_2Layer->Write();

  h_mFstSimResR_2Layer->Write();
  h_mFstSimResPhi_2Layer->Write();
  h_mFstSimResRPhi_2Layer->Write();

  for(int i_match = 0; i_match < 8; ++i_match)
  {
    h_mIstCounts_2Layer[i_match]->Write();
    h_mFstCounts_2Layer[i_match]->Write();
  }
  File_OutPut->Close();
}
