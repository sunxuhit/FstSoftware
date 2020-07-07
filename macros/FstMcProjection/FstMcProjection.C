#include <TRandom3.h>
#include <TStyle.h>
#include <TMath.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TH1F.h>
#include <TF1.h>
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void genCosmicTrackRandom(TVector2 &vPosIST1, TVector2 &vPosIST3, TVector2 &vPosFST, bool isRot, int rAligned); // cosmic ray simulation | return (x,y) for IST & (r,phi) for FST
void genCosmicTrackAngle(TVector2 &vPosIST1, TVector2 &vPosIST3, TVector2 &vPosFST, TH1F *h_TrackAngle, bool isRot, int rAligned); // cosmic ray simulation | return (x,y) for IST & (r,phi) for FST
void genCosmicTrackAngle(TVector2 &vPosIST1, TVector2 &vPosIST3, TVector2 &vPosFST, TF1 *f_TrackAngle, bool isRot, int rAligned); // cosmic ray simulation | return (x,y) for IST & (r,phi) for FST
TVector2 getProjection(TVector2 vPosIST1, TVector2 vPosIST3, bool isRot, int rAligned); // get projected position on FST from IST1 & IST3 | return (r,phi) for FST
TVector2 getReadOut(TVector2 vPosHit, TH2F *h_pixel, bool isFST); // get readout position from a real hit | return (x,y) for IST & (r,phi) for FST
void printAlignmentInfo(bool isRot, int rAligned);

void FstMcProjection(bool isRot = true, int rAligned = 0, int numOfTracks = 500000)
{
  printAlignmentInfo(isRot,rAligned);

  // cosmic angle distribution
  TFile *File_InPut = TFile::Open("../../output/configuration/FstTracking_HV200V_Th4.0Tb2Ped2.5Ped3.5_withPed_withCMNCorr.root");
  TH1F *h_mClustersTrackAngle = (TH1F*)File_InPut->Get("h_mClustersTrackAngle")->Clone();

  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  const double z0_fst = FST::pitchLayer03;
  const double z1_ist = FST::pitchLayer12 + FST::pitchLayer23;
  const double z2_ist = FST::pitchLayer23;
  const double z3_ist = 0.0;

  const double phiGap = 1.0*TMath::Pi()/180.0;

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

  // Binning
  int numBinX   = 50;
  float maxX    = 160.0;
  int numBinY   = 100;
  float maxY    = 16.0;
  int numBinR   = 50;
  float maxR    = 160.0;
  int numBinPhi = 100;
  float maxPhi  = 0.05;
  if(isRot)
  {
    numBinX   = 160;
    maxX      = 160.0;
    numBinY   = 75;
    maxY      = 25.0;
    numBinR   = 160;
    maxR      = 160.0;
    numBinPhi = 75;
    maxPhi    = 0.15;
  }

  TH2F *h_mFstDisplay = new TH2F("h_mFstDisplay","h_mFstDisplay",6,FST::rMin,FST::rMax,FST::numPhiSeg,-1.0*FST::phiMax,1.0*FST::phiMax);
  // FST residual
  TH1F *h_mFstProjResX_2Layer  = new TH1F("h_mFstProjResX_2Layer","h_mFstProjResX_2Layer",numBinX,-maxX,maxX);
  TH1F *h_mFstProjResY_2Layer  = new TH1F("h_mFstProjResY_2Layer","h_mFstProjResY_2Layer",numBinY,-maxY,maxY);
  TH2F *h_mFstProjResXY_2Layer = new TH2F("h_mFstProjResXY_2Layer","h_mFstProjResXY_2Layer",numBinX,-maxX,maxX,numBinY,-maxY,maxY);

  TH1F *h_mFstProjResR_2Layer    = new TH1F("h_mFstProjResR_2Layer","h_mFstProjResR_2Layer",numBinR,-maxR,maxR);
  TH1F *h_mFstProjResPhi_2Layer  = new TH1F("h_mFstProjResPhi_2Layer","h_mFstProjResPhi_2Layer",numBinPhi,-maxPhi,maxPhi);
  TH2F *h_mFstProjResRPhi_2Layer = new TH2F("h_mFstProjResRPhi_2Layer","h_mFstProjResRPhi_2Layer",numBinR,-maxR,maxR,numBinPhi,-maxPhi,maxPhi);
    
  TH1F *h_mFstProjResX_2Layer_Rstrips[4];
  TH1F *h_mFstProjResY_2Layer_Rstrips[4];
  TH2F *h_mFstProjResXY_2Layer_Rstrips[4]; // 2D distribution

  TH1F *h_mFstProjResR_2Layer_Rstrips[4];
  TH1F *h_mFstProjResPhi_2Layer_Rstrips[4];
  TH2F *h_mFstProjResRPhi_2Layer_Rstrips[4]; // 2D distribution
  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    string HistName;
    HistName = Form("h_mFstProjResX_2Layer_Rstrip%d",i_rstrip);
    h_mFstProjResX_2Layer_Rstrips[i_rstrip]  = new TH1F(HistName.c_str(),HistName.c_str(),numBinX,-maxX,maxX);
    HistName = Form("h_mFstProjResY_2Layer_Rstrip%d",i_rstrip);
    h_mFstProjResY_2Layer_Rstrips[i_rstrip]  = new TH1F(HistName.c_str(),HistName.c_str(),numBinY,-maxY,maxY);
    HistName = Form("h_mFstProjResXY_2Layer_Rstrip%d",i_rstrip);
    h_mFstProjResXY_2Layer_Rstrips[i_rstrip] = new TH2F(HistName.c_str(),HistName.c_str(),numBinX,-maxX,maxX,numBinY,-maxY,maxY);
    
    HistName = Form("h_mFstProjResR_2Layer_Rstrip%d",i_rstrip);
    h_mFstProjResR_2Layer_Rstrips[i_rstrip]    = new TH1F(HistName.c_str(),HistName.c_str(),numBinR,-maxR,maxR);
    HistName = Form("h_mFstProjResPhi_2Layer_Rstrip%d",i_rstrip);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrip]  = new TH1F(HistName.c_str(),HistName.c_str(),numBinPhi,-maxPhi,maxPhi);
    HistName = Form("h_mFstProjResRPhi_2Layer_Rstrip%d",i_rstrip);
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrip] = new TH2F(HistName.c_str(),HistName.c_str(),numBinR,-maxR,maxR,numBinPhi,-maxPhi,maxPhi);
  }

  TH1F *h_mFstSimResR_2Layer    = new TH1F("h_mFstSimResR_2Layer","h_mFstSimResR_2Layer",160,-4.0*FST::pitchR,4.0*FST::pitchR);
  TH1F *h_mFstSimResPhi_2Layer  = new TH1F("h_mFstSimResPhi_2Layer","h_mFstSimResPhi_2Layer",320,-32.0*FST::pitchPhi,32.0*FST::pitchPhi);
  TH2F *h_mFstSimResRPhi_2Layer = new TH2F("h_mFstSimResRPhi_2Layer","h_mFstSimResRPhi_2Layer",160,-4.0*FST::pitchR,4.0*FST::pitchR,320,-32.0*FST::pitchPhi,32.0*FST::pitchPhi);

  int numBinEffR = 30;
  if(isRot) numBinEffR = 120;
  TH2F *h_mIstCounts_2Layer[8];
  TH2F *h_mFstCounts_2Layer[8];
  for(int i_match = 0; i_match < 8; ++i_match)
  {
    string HistName;
    // simple clusters
    HistName = Form("h_mIstCounts_2Layer_SF%d",i_match);
    h_mIstCounts_2Layer[i_match] = new TH2F(HistName.c_str(),HistName.c_str(),numBinEffR,FST::rMin,FST::rMax,90,-1.5*FST::phiMax,1.5*FST::phiMax);
    HistName = Form("h_mFstCounts_2Layer_SF%d",i_match);
    h_mFstCounts_2Layer[i_match] = new TH2F(HistName.c_str(),HistName.c_str(),numBinEffR,FST::rMin,FST::rMax,90,-1.5*FST::phiMax,1.5*FST::phiMax);
  }

  TH1F *h_mRecoAngle = new TH1F("h_mRecoAngle","h_mRecoAngle",25,0.0,0.5*TMath::Pi());
  TH1F *h_mPolarAngle = new TH1F("h_mPolarAngle","h_mPolarAngle",100,0.0,0.5*TMath::Pi());
  TH1F *h_mAzimuthalAngle = new TH1F("h_mAzimuthalAngle","h_mAzimuthalAngle",100,-TMath::Pi(),TMath::Pi());
  TF1 *f_mClustersTrackAngle = new TF1("f_mClustersTrackAngle","[0]*cos(x)",0.0,0.5*TMath::Pi());
  f_mClustersTrackAngle->SetParameter(0,1.0);

  // Cosmic Simulation
  for(int i_track =0; i_track < numOfTracks; ++i_track)
  {
    TVector2 vHitGen_IST1, vHitGen_IST3, vHitGen_FST;
    vHitGen_IST1.Set(-999.0,-999.0);
    vHitGen_IST3.Set(-999.0,-999.0);
    vHitGen_FST.Set(-999.0,-999.0);

    // randomly generated cosmic
    // genCosmicTrackRandom(vHitGen_IST1, vHitGen_IST3, vHitGen_FST, isRot, rAligned);
    // genCosmicTrackAngle(vHitGen_IST1, vHitGen_IST3, vHitGen_FST, h_mClustersTrackAngle, isRot, rAligned);
    genCosmicTrackAngle(vHitGen_IST1, vHitGen_IST3, vHitGen_FST, f_mClustersTrackAngle, isRot, rAligned);
    TVector2 vHitRo_IST3 = getReadOut(vHitGen_IST3, h_mIst3Pixel, false); // RO position at IST3
    TVector2 vHitRo_IST1 = getReadOut(vHitGen_IST1, h_mIst1Pixel, false); // RO position at IST1

    // cosmic incident angle simulation
    double theta = f_mClustersTrackAngle->GetRandom(); // random generated theta from input cosmic angle distribution
    double phi = gRandom->Uniform(-TMath::Pi(),TMath::Pi()); // random generated phi distribution
    h_mPolarAngle->Fill(theta);
    h_mAzimuthalAngle->Fill(phi);

    if( !(vHitRo_IST1.X() > -100.0 && vHitRo_IST1.Y() > -100.0) ) continue;
    if( !(vHitRo_IST3.X() > -100.0 && vHitRo_IST3.Y() > -100.0) ) continue;

    TVector2 vHitRo_FST  = getReadOut(vHitGen_FST, h_mFstPixel, true); // RO position at FST
    TVector2 vRoProj_FST = getProjection(vHitRo_IST1, vHitRo_IST3, isRot, rAligned); // projected position on FST through the readout position from IST1 & IST3

    double x3_gen = vHitGen_IST3.X();
    double y3_gen = vHitGen_IST3.Y();
    double x3_ro  = vHitRo_IST3.X(); 
    double y3_ro  = vHitRo_IST3.Y();
    // h_mIst3Display->Fill(x3_gen,y3_gen);
    h_mIst3Display->Fill(x3_ro,y3_ro);
    h_mIst3ResX_2Layer->Fill(x3_ro-x3_gen);
    h_mIst3ResY_2Layer->Fill(y3_ro-y3_gen);

    double x1_gen = vHitGen_IST1.X();
    double y1_gen = vHitGen_IST1.Y();
    double x1_ro  = vHitRo_IST1.X(); 
    double y1_ro  = vHitRo_IST1.Y();
    // h_mIst1Display->Fill(x1_gen,y1_gen);
    h_mIst1Display->Fill(x1_ro,y1_ro);
    h_mIst1ResX_2Layer->Fill(x1_ro-x1_gen);
    h_mIst1ResY_2Layer->Fill(y1_ro-y1_gen);

    // QA for incident angle
    TVector3 vPos_IST1, vPos_IST3, normVec;
    vPos_IST1.SetXYZ(x1_gen,y1_gen,z1_ist);
    vPos_IST3.SetXYZ(x3_gen,y3_gen,z3_ist);
    TVector3 istTrack = vPos_IST1 - vPos_IST3;
    normVec.SetXYZ(0.0,0.0,vPos_IST1.Z()-vPos_IST3.Z());
    double pAngle = istTrack.Angle(normVec);
    h_mRecoAngle->Fill(pAngle);

    // projected position (w/o alignment) through the randomly generated hit position from IST1 & IST3
    double x0_genIST = x3_gen + (x1_gen-x3_gen)*z0_fst/z1_ist;
    double y0_genIST = y3_gen + (y1_gen-y3_gen)*z0_fst/z1_ist;

    // projected position (w/o alignment) through the readout position from IST1 & IST3
    double x0_roIST = x3_ro + (x1_ro-x3_ro)*z0_fst/z1_ist;
    double y0_roIST = y3_ro + (y1_ro-y3_ro)*z0_fst/z1_ist;

    // IST Projection Error
    h_mIstProjResX_2Layer->Fill(x0_roIST-x0_genIST);
    h_mIstProjResY_2Layer->Fill(y0_roIST-y0_genIST);
    h_mIstProjResXY_2Layer->Fill(x0_roIST-x0_genIST,y0_roIST-y0_genIST);

    // FST Residual
    // real position
    double r0_gen   = vHitGen_FST.X();
    double phi0_gen = vHitGen_FST.Y();
    // read out position
    double r0_ro     = vHitRo_FST.X();
    double phi0_ro   = vHitRo_FST.Y();
    double x0_ro     = r0_ro*TMath::Cos(phi0_ro);
    double y0_ro     = r0_ro*TMath::Sin(phi0_ro);

    // projected position from IST pixel
    double r0_proj   = vRoProj_FST.X();
    double phi0_proj = vRoProj_FST.Y();
    double x0_proj   = r0_proj*TMath::Cos(phi0_proj);
    double y0_proj   = r0_proj*TMath::Sin(phi0_proj);

    // fill Residual
    if(r0_ro > -100.0 && phi0_ro > -100.0)
    {
      h_mFstDisplay->Fill(r0_ro,phi0_ro);
      // h_mFstDisplay->Fill(r0_proj,phi0_proj);
      h_mFstProjResR_2Layer->Fill(r0_ro-r0_proj);
      h_mFstProjResPhi_2Layer->Fill(phi0_ro-phi0_proj);
      h_mFstProjResRPhi_2Layer->Fill(r0_ro-r0_proj,phi0_ro-phi0_proj);

      h_mFstProjResX_2Layer->Fill(x0_ro-x0_proj);
      h_mFstProjResY_2Layer->Fill(y0_ro-y0_proj);
      h_mFstProjResXY_2Layer->Fill(x0_ro-x0_proj,y0_ro-y0_proj);

      for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
      {
	if(r0_proj > FST::rOuter + FST::pitchR*i_rstrip && r0_proj <= FST::rOuter + FST::pitchR*(i_rstrip+1))
	{ // check the position of the projected r is within a specific r_strip and fill accordingly
	  h_mFstProjResX_2Layer_Rstrips[i_rstrip]->Fill(x0_ro-x0_proj);
	  h_mFstProjResY_2Layer_Rstrips[i_rstrip]->Fill(y0_ro-y0_proj);
	  h_mFstProjResXY_2Layer_Rstrips[i_rstrip]->Fill(x0_ro-x0_proj,y0_ro-y0_proj);

	  h_mFstProjResR_2Layer_Rstrips[i_rstrip]->Fill(r0_ro-r0_proj);
	  h_mFstProjResPhi_2Layer_Rstrips[i_rstrip]->Fill(phi0_ro-phi0_proj);
	  h_mFstProjResRPhi_2Layer_Rstrips[i_rstrip]->Fill(r0_ro-r0_proj,phi0_ro-phi0_proj);
	}
      }

      h_mFstSimResR_2Layer->Fill(r0_proj-r0_gen);
      h_mFstSimResPhi_2Layer->Fill(phi0_proj-phi0_gen);
      h_mFstSimResRPhi_2Layer->Fill(r0_proj-r0_gen,phi0_proj-phi0_gen);
    }

    double phiMatchingCut = 0.01; // before totation
    if(isRot) phiMatchingCut = 0.05;
    // fill Efficiency
    // if(r0_proj >= FST::rMin && r0_proj < FST::rMax && phi0_proj >= -FST::phiMax && phi0_proj < FST::phiMax)
    if(r0_proj >= FST::rMin && r0_proj < FST::rMax && phi0_proj >= 0.0 && phi0_proj < FST::phiMax)
    {
      for(int i_match = 0; i_match < 8; ++i_match)
      {
	h_mIstCounts_2Layer[i_match]->Fill(r0_proj,phi0_proj);
	int nMatchedTrack = 0;
	if(r0_ro > -100.0 && phi0_ro > -100.0)
	{
	  if(i_match == 0)
	  {
	    nMatchedTrack++;
	  }
	  if( i_match > 0 && abs(r0_ro-r0_proj) <= i_match*0.5*FST::pitchR && abs(phi0_ro-phi0_proj) <= phiMatchingCut)
	  {
	    nMatchedTrack++;
	  }
	}
	if(nMatchedTrack > 0) h_mFstCounts_2Layer[i_match]->Fill(r0_proj,phi0_proj);
      }
    }
  }

  string outputname = "../../output/simulation/FstMcProjection_woRot.root";
  if(isRot) 
  {
    outputname = Form("../../output/simulation/FstMcProjection_Rot_AlignedRstrip%d.root",rAligned);
  }

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

  for(int i_rstrip = 0; i_rstrip < 4; ++i_rstrip)
  {
    h_mFstProjResX_2Layer_Rstrips[i_rstrip]->Write();
    h_mFstProjResY_2Layer_Rstrips[i_rstrip]->Write();
    h_mFstProjResXY_2Layer_Rstrips[i_rstrip]->Write();

    h_mFstProjResR_2Layer_Rstrips[i_rstrip]->Write();
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrip]->Write();
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrip]->Write();
  }

  h_mFstSimResR_2Layer->Write();
  h_mFstSimResPhi_2Layer->Write();
  h_mFstSimResRPhi_2Layer->Write();

  for(int i_match = 0; i_match < 8; ++i_match)
  {
    h_mIstCounts_2Layer[i_match]->Write();
    h_mFstCounts_2Layer[i_match]->Write();
  }

  h_mRecoAngle->Write();
  h_mPolarAngle->Write();
  h_mAzimuthalAngle->Write();
  File_OutPut->Close();
}

void genCosmicTrackRandom(TVector2 &vPosIST1, TVector2 &vPosIST3, TVector2 &vPosFST, bool isRot = false, int rAligned = 0)
{
  // gRandom->SetSeed();

  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  // generated position on IST
  double x3_gen = gRandom->Rndm()*lengthColumn;
  double y3_gen = gRandom->Rndm()*lengthRow;
  vPosIST3.Set(x3_gen,y3_gen);

  double x1_gen = gRandom->Rndm()*lengthColumn;
  double y1_gen = gRandom->Rndm()*lengthRow;
  vPosIST1.Set(x1_gen,y1_gen);

  vPosFST = getProjection(vPosIST1,vPosIST3,isRot,rAligned);
}

void genCosmicTrackAngle(TVector2 &vPosIST1, TVector2 &vPosIST3, TVector2 &vPosFST, TH1F *h_TrackAngle, bool isRot = false, int rAligned = 0)
{
  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  // random generated position on IST
  double x3_gen = gRandom->Rndm()*lengthColumn;
  double y3_gen = gRandom->Rndm()*lengthRow;
  vPosIST3.Set(x3_gen,y3_gen);

  // double theta = h_TrackAngle->GetRandom(); // random generated theta from input cosmic angle distribution
  double theta = h_TrackAngle->GetRandom(); // random generated theta from input cosmic angle distribution
  double phi = gRandom->Uniform(-TMath::Pi(),TMath::Pi()); // random generated phi distribution
  double x1_gen = x3_gen + (FST::pitchLayer12 + FST::pitchLayer23)*TMath::Tan(theta)*TMath::Cos(phi);
  double y1_gen = y3_gen + (FST::pitchLayer12 + FST::pitchLayer23)*TMath::Tan(theta)*TMath::Sin(phi);
  vPosIST1.Set(x1_gen,y1_gen);

  vPosFST = getProjection(vPosIST1,vPosIST3,isRot,rAligned);
}

void genCosmicTrackAngle(TVector2 &vPosIST1, TVector2 &vPosIST3, TVector2 &vPosFST, TF1 *f_TrackAngle, bool isRot = false, int rAligned = 0)
{
  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  // random generated position on IST
  double x3_gen = gRandom->Rndm()*lengthColumn;
  double y3_gen = gRandom->Rndm()*lengthRow;
  vPosIST3.Set(x3_gen,y3_gen);

  // double theta = h_TrackAngle->GetRandom(); // random generated theta from input cosmic angle distribution
  double theta = f_TrackAngle->GetRandom(); // random generated theta from input cosmic angle distribution
  double phi = gRandom->Uniform(-TMath::Pi(),TMath::Pi()); // random generated phi distribution
  double x1_gen = x3_gen + (FST::pitchLayer12 + FST::pitchLayer23)*TMath::Tan(theta)*TMath::Cos(phi);
  double y1_gen = y3_gen + (FST::pitchLayer12 + FST::pitchLayer23)*TMath::Tan(theta)*TMath::Sin(phi);
  vPosIST1.Set(x1_gen,y1_gen);

  vPosFST = getProjection(vPosIST1,vPosIST3,isRot,rAligned);
}

TVector2 getProjection(TVector2 vPosIST1, TVector2 vPosIST3, bool isRot = false, int rAligned = 0)
{
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
  if( isRot && rAligned == 0)
  {
    // Aligned with RStrip3
    x2_shift = 209.361;
    y2_shift = -48.0745;
    phi_rot_ist2 = -1.50174;
  }
  if( isRot && rAligned == 3)
  {
    // Aligned with RStrip3
    x2_shift = 264.505;
    y2_shift = -48.2426;
    phi_rot_ist2 = -1.5185; 
  }

  double x3_gen = vPosIST3.X();
  double y3_gen = vPosIST3.Y();

  double x1_gen = vPosIST1.X();
  double y1_gen = vPosIST1.Y();

  // FST Residual
  // real position
  double x3_corr = x3_gen*TMath::Cos(phi_rot_ist2) + y3_gen*TMath::Sin(phi_rot_ist2) + x2_shift;
  double y3_corr = y3_gen*TMath::Cos(phi_rot_ist2) - x3_gen*TMath::Sin(phi_rot_ist2) + y2_shift;
  double x1_corr = x1_gen*TMath::Cos(phi_rot_ist2) + y1_gen*TMath::Sin(phi_rot_ist2) + x2_shift;
  double y1_corr = y1_gen*TMath::Cos(phi_rot_ist2) - x1_gen*TMath::Sin(phi_rot_ist2) + y2_shift;
  double x0_corr = x3_corr + (x1_corr-x3_corr)*z0_fst/z1_ist;
  double y0_corr = y3_corr + (y1_corr-y3_corr)*z0_fst/z1_ist;
  double r0_corr   = TMath::Sqrt(x0_corr*x0_corr+y0_corr*y0_corr);
  double phi0_corr = TMath::ATan2(y0_corr,x0_corr);
  // cout << "x0_corr = " << x0_corr << ", y0_corr = " << y0_corr << endl;
  // cout << "r0_corr = " << r0_corr << ", phi0_corr = " << phi0_corr << endl;

  TVector2 vPosFST;
  vPosFST.Set(r0_corr,phi0_corr);

  return vPosFST;
}

TVector2 getReadOut(TVector2 vPosHit, TH2F *h_pixel, bool isFST)
{
  const float lengthColumn = FST::noColumns*FST::pitchColumn; // length of IST Column
  const float lengthRow = FST::noRows*FST::pitchRow; // length of IST Row

  TVector2 vPosRO;
  vPosRO.Set(-999.0,-999.0);

  double x_hit = vPosHit.X();
  double y_hit = vPosHit.Y();

  if( !isFST )
  {
    double x_ro = -999.0;
    double y_ro = -999.0;
    if(x_hit >= 0 && x_hit < lengthColumn && y_hit >=0 && y_hit < lengthRow)
    {
      int binX    = h_pixel->GetXaxis()->FindBin(x_hit);
      int binY    = h_pixel->GetYaxis()->FindBin(y_hit);
      x_ro = h_pixel->GetXaxis()->GetBinCenter(binX);
      y_ro = h_pixel->GetYaxis()->GetBinCenter(binY);
    }
    vPosRO.Set(x_ro,y_ro);
  }
  if( isFST )
  {
    double r_hit   = x_hit;
    double phi_hit = y_hit;
    double r_ro    = -999.0;
    double phi_ro  = -999.0;
    double x_ro    = -999.0;
    double y_ro    = -999.0;
    // check if FST has readout
    // if(r_hit >= FST::rOuter && r_hit <= FST::rOuter+4.0*FST::pitchR && phi_hit >= -FST::phiMax && phi_hit <= FST::phiMax)
    if(r_hit >= FST::rOuter && r_hit <= FST::rOuter+4.0*FST::pitchR && phi_hit >= 0.0 && phi_hit <= FST::phiMax)
    {
      int binR   = h_pixel->GetXaxis()->FindBin(r_hit);
      int binPhi = h_pixel->GetYaxis()->FindBin(phi_hit);
      r_ro       = h_pixel->GetXaxis()->GetBinCenter(binR);
      phi_ro     = h_pixel->GetYaxis()->GetBinCenter(binPhi);
    }
    vPosRO.Set(r_ro,phi_ro);
  }

  return vPosRO;
}

void printAlignmentInfo(bool isRot, int rAligned = 0)
{
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
  if( isRot && rAligned == 0)
  {
    // Aligned with RStrip3
    x2_shift = 209.361;
    y2_shift = -48.0745;
    phi_rot_ist2 = -1.50174;
  }
  if( isRot && rAligned == 3)
  {
    // Aligned with RStrip3
    x2_shift = 264.505;
    y2_shift = -48.2426;
    phi_rot_ist2 = -1.5185; 
  }

  cout << "isRot = " << isRot << ", x2_shift = " << x2_shift << ", y2_shift = " << y2_shift << ", phi_rot_ist2 = " << phi_rot_ist2 << endl;
}
