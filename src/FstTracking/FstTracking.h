#ifndef FstTracking_h
#define FstTracking_h

#include <TObject.h>
#include "../FstUtil/FstCons.h"
#include "../FstUtil/FstRawHit.h"
#include "../FstUtil/FstCluster.h"
#include "../FstUtil/FstTrack.h"
#include "../FstUtil/FstEvent.h"
#include <vector>
#include <fstream>

class TFile;
class TChain;
class TTree;
class TH1F;
class TH2F;

class FstTracking : public TObject
{
  public:
    FstTracking();
    ~FstTracking();

    void set_list(std::string inputlist)
    {
      mList = inputlist;
    }
    void set_outputfile(std::string outputfile)
    {
      mOutPutFile = outputfile;
    }

    int Init();
    int Make(); // subtract pedestal => signal & find Hits
    int Finish();

    // init input TChain
    bool initChain();

    // track QA
    bool initTrackingQA_Hits();
    void fillTrackingQA_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    void writeTrackingQA_Hits();

    // incident angle
    void initTrackAngle();
    void fillTrackAngle(FstEvent *fstEvent);
    void writeTrackAngle();

    // track resolution
    void initTracking_Hits();
    void calResolution_Hits(FstEvent *fstEvent);
    void writeTracking_Hits();

    void initTracking_Clusters();
    void calResolution_SimpleClusters(FstEvent *fstEvent);
    void calResolution_ScanClusters(FstEvent *fstEvent);
    void writeTracking_Clusters();

    // efficiency
    void initEfficiency_Hits();
    void calEfficiency_Hits(FstEvent *fstEvent);
    void writeEfficiency_Hits();

    void initEfficiency_Clusters();
    void calEfficiency_SimpleClusters(FstEvent *fstEvent);
    void calEfficiency_ScanClusters(FstEvent *fstEvent);
    void writeEfficiency_Clusters();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Tracking QA based on Hits
    TH2F *h_mHitsCorrXR[4]; // 0: ist1x vs. ist3x | 1: ist1x vs. fstr | 2: ist3x vs. fstr | 3: ist1x+ist3x vs. fstr
    TH2F *h_mHitsCorrYPhi[4]; // 0: ist1y vs. ist3y | 1: ist1y vs. fstphi | 2: ist3y vs. fstphi | 3: ist1y+ist3y vs. fstphi
    TH1F *h_mXResidual_Hits_Before;
    TH1F *h_mYResidual_Hits_Before;
    TH1F *h_mXResidual_Hits;
    TH1F *h_mYResidual_Hits;
    TH1F *h_mRResidual_Hits;
    TH1F *h_mPhiResidual_Hits;

    // incident angle
    TH1F *h_mClustersTrackAngle; // cosmic incident angle on IST

    // Tracking
    // hits
    TH1F *h_mHitsTrackFstResX;
    TH1F *h_mHitsTrackFstResY;
    TH2F *h_mHitsTrackFstResXY; // 2D distribution

    TH1F *h_mHitsTrackFstResR;
    TH1F *h_mHitsTrackFstResPhi;
    TH2F *h_mHitsTrackFstResRPhi; // 2D distribution

    // simple clusters
    TH1F *h_mSimpleClustersTrackFstResX_2Layer;
    TH1F *h_mSimpleClustersTrackFstResY_2Layer;
    TH2F *h_mSimpleClustersTrackFstResXY_2Layer; // 2D distribution

    TH1F *h_mSimpleClustersTrackFstResR_2Layer;
    TH1F *h_mSimpleClustersTrackFstResPhi_2Layer;
    TH2F *h_mSimpleClustersTrackFstResRPhi_2Layer; // 2D distribution
    
    TH1F *h_mSimpleClustersTrackFstResX_2Layer_Rstrips[4];
    TH1F *h_mSimpleClustersTrackFstResY_2Layer_Rstrips[4];
    TH2F *h_mSimpleClustersTrackFstResXY_2Layer_Rstrips[4]; // 2D distribution
    
    TH1F *h_mSimpleClustersTrackFstResR_2Layer_Rstrips[4];
    TH1F *h_mSimpleClustersTrackFstResPhi_2Layer_Rstrips[4];
    TH2F *h_mSimpleClustersTrackFstResRPhi_2Layer_Rstrips[4]; // 2D distribution

    TH1F *h_mSimpleClustersTrackIstResX_2Layer;
    TH1F *h_mSimpleClustersTrackIstResY_2Layer;
    TH2F *h_mSimpleClustersTrackIstResXY_2Layer; // 2D distribution

    TH1F *h_mSimpleClustersTrackFstResX_3Layer;
    TH1F *h_mSimpleClustersTrackFstResY_3Layer;
    TH2F *h_mSimpleClustersTrackFstResXY_3Layer; // 2D distribution

    TH1F *h_mSimpleClustersTrackFstResR_3Layer;
    TH1F *h_mSimpleClustersTrackFstResPhi_3Layer;
    TH2F *h_mSimpleClustersTrackFstResRPhi_3Layer; // 2D distribution

    TH1F *h_mSimpleClustersTrackFstResX_3Layer_Rstrips[4];
    TH1F *h_mSimpleClustersTrackFstResY_3Layer_Rstrips[4];
    TH2F *h_mSimpleClustersTrackFstResXY_3Layer_Rstrips[4]; // 2D distribution
    
    TH1F *h_mSimpleClustersTrackFstResR_3Layer_Rstrips[4];
    TH1F *h_mSimpleClustersTrackFstResPhi_3Layer_Rstrips[4];
    TH2F *h_mSimpleClustersTrackFstResRPhi_3Layer_Rstrips[4]; // 2D distribution

    TH1F *h_mSimpleClustersTrackIstResX_3Layer;
    TH1F *h_mSimpleClustersTrackIstResY_3Layer;
    TH2F *h_mSimpleClustersTrackIstResXY_3Layer;

    // scan clusters
    TH1F *h_mScanClustersTrackFstResX_2Layer;
    TH1F *h_mScanClustersTrackFstResY_2Layer;
    TH2F *h_mScanClustersTrackFstResXY_2Layer; // 2D distribution

    TH1F *h_mScanClustersTrackFstResR_2Layer;
    TH1F *h_mScanClustersTrackFstResPhi_2Layer;
    TH2F *h_mScanClustersTrackFstResRPhi_2Layer; // 2D distribution
    
    TH1F *h_mScanClustersTrackFstResX_2Layer_Rstrips[4];
    TH1F *h_mScanClustersTrackFstResY_2Layer_Rstrips[4];
    TH2F *h_mScanClustersTrackFstResXY_2Layer_Rstrips[4]; // 2D distribution
    
    TH1F *h_mScanClustersTrackFstResR_2Layer_Rstrips[4];
    TH1F *h_mScanClustersTrackFstResPhi_2Layer_Rstrips[4];
    TH2F *h_mScanClustersTrackFstResRPhi_2Layer_Rstrips[4]; // 2D distribution

    TH1F *h_mScanClustersTrackFstResX_3Layer;
    TH1F *h_mScanClustersTrackFstResY_3Layer;
    TH2F *h_mScanClustersTrackFstResXY_3Layer; // 2D distribution

    TH1F *h_mScanClustersTrackFstResR_3Layer;
    TH1F *h_mScanClustersTrackFstResPhi_3Layer;
    TH2F *h_mScanClustersTrackFstResRPhi_3Layer; // 2D distribution
    
    TH1F *h_mScanClustersTrackFstResX_3Layer_Rstrips[4];
    TH1F *h_mScanClustersTrackFstResY_3Layer_Rstrips[4];
    TH2F *h_mScanClustersTrackFstResXY_3Layer_Rstrips[4]; // 2D distribution
    
    TH1F *h_mScanClustersTrackFstResR_3Layer_Rstrips[4];
    TH1F *h_mScanClustersTrackFstResPhi_3Layer_Rstrips[4];
    TH2F *h_mScanClustersTrackFstResRPhi_3Layer_Rstrips[4]; // 2D distribution

    // Efficiency based on Hits
    // 0: no matching | 1-3 matching within (1-3)*pitchR in r & (1-3)*pitchPhi in phi
    TH2F *h_mHitsTrackIstCounts[8]; // position on FST from IST projection
    TH2F *h_mHitsTrackFstCounts[8]; // position on FST from FST measurement

    // Efficiency based on Clusters
    // simple clusters
    TH2F *h_mSimpleClustersTrackIstCounts_2Layer[8]; // position on FST from IST projection
    TH2F *h_mSimpleClustersTrackFstCounts_2Layer[8]; // position on FST from FST measurement

    TH2F *h_mSimpleClustersTrackIstCounts_3Layer[8]; // position on FST from IST projection
    TH2F *h_mSimpleClustersTrackFstCounts_3Layer[8]; // position on FST from FST measurement

    // scan clusters
    TH2F *h_mScanClustersTrackIstCounts_2Layer[8]; // position on FST from IST projection
    TH2F *h_mScanClustersTrackFstCounts_2Layer[8]; // position on FST from FST measurement

    TH2F *h_mScanClustersTrackIstCounts_3Layer[8]; // position on FST from IST projection
    TH2F *h_mScanClustersTrackFstCounts_3Layer[8]; // position on FST from FST measurement

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstTracking,1)
};

#endif
