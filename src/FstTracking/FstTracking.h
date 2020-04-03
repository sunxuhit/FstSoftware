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

    // hit display
    bool initHitDisplay();
    void fillHitDisplay(std::vector<FstRawHit *> rawHitVec_orig);
    void writeHitDisplay();

    bool initClusterDisplay();
    void fillClusterDisplay(std::vector<FstCluster *> clusterVec_orig);
    void writeClusterDisplay();

    // track QA
    bool initTrackingQA_Hits();
    void fillTrackingQA_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    void writeTrackingQA_Hits();

    // track resolution
    void initTracking_Hits();
    void calResolution_Hits(std::vector<FstTrack *> trackHitVec_orig);
    void writeTracking_Hits();

    void initTracking_Clusters();
    void calResolution_Clusters(std::vector<FstTrack *> trackClusterVec_orig);
    void writeTracking_Clusters();

    // efficiency
    void initEfficiency_Hits();
    void calEfficiency_Hits(std::vector<FstTrack *> trackHitVec_orig);
    void writeEfficiency_Hits();

    void initEfficiency_Clusters();
    void calEfficiency_Clusters(std::vector<FstTrack *> trackClusterVec_orig);
    void writeEfficiency_Clusters();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Hit Display
    TH2F *h_mHitDisplay[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mMaxTb[4]; 
    TH2F *h_mClusterDisplay[4]; // 0 for FST, 1-3 for IST

    // Tracking QA based on Hits
    TH2F *h_mHitsCorrXR[4]; // 0: ist1x vs. ist3x | 1: ist1x vs. fstr | 2: ist3x vs. fstr | 3: ist1x+ist3x vs. fstr
    TH2F *h_mHitsCorrYPhi[4]; // 0: ist1y vs. ist3y | 1: ist1y vs. fstphi | 2: ist3y vs. fstphi | 3: ist1y+ist3y vs. fstphi
    TH1F *h_mXResidual_Hits_Before;
    TH1F *h_mYResidual_Hits_Before;
    TH1F *h_mXResidual_Hits;
    TH1F *h_mYResidual_Hits;
    TH1F *h_mRResidual_Hits;
    TH1F *h_mPhiResidual_Hits;

    TH1F *h_mTrackXRes_Hits;
    TH1F *h_mTrackYRes_Hits;
    TH1F *h_mTrackRRes_Hits;
    TH1F *h_mTrackPhiRes_Hits;

    TH1F *h_mTrackXRes_Clusters;
    TH1F *h_mTrackYRes_Clusters;
    TH1F *h_mTrackRRes_Clusters;
    TH1F *h_mTrackPhiRes_Clusters;

#if 0
    // Tracking based on Clusters
    TH1F *h_mAdc_Simple[4];
    TH1F *h_mAdcAngleCorr_Simple[4];
    TH1F *h_mTrackAngle_Simple;
    TH1F *h_mXResidual_Simple;
    TH1F *h_mYResidual_Simple;
#endif

    // Efficiency based on Hits
    TH2F *h_mTrackHits_IST; // position on FST from IST projection
    TH2F *h_mTrackHits_FST; // position on FST from FST measurement

    TH2F *h_mTrackClusters_IST; // position on FST from IST projection
    TH2F *h_mTrackClusters_FST; // position on FST from FST measurement

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstTracking,1)
};

#endif
