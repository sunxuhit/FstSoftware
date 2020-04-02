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

    // tracking
    bool initTracking_Hits();
    void fillTrackingQA_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    void writeTracking_Hits();

    std::vector<FstTrack *> findTrack_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    // FstTrack *findTrack_Clusters(std::vector<FstCluster *> clusterVec_orig);

#if 0
    bool initTracking_Clusters();
    bool doTracking_Clusters(std::vector<FstCluster *> clusterVec_orig);
    void writeTracking_Clusters();

    // efficiency
    bool initEfficiency_Hits();
    bool doEfficiency_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    void writeEfficiency_Hits();
#endif

    // Output TTree for hits and clusters
    bool initTree();
    void writeTree();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Hit Display
    TH2F *h_mHitDisplay[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mMaxTb[4]; 

    // Tracking based on Hits
    TH2F *h_mHitsCorrXR[4]; // 0: ist1x vs. ist3x | 1: ist1x vs. fstr | 2: ist3x vs. fstr | 3: ist1x+ist3x vs. fstr
    TH2F *h_mHitsCorrYPhi[4]; // 0: ist1y vs. ist3y | 1: ist1y vs. fstphi | 2: ist3y vs. fstphi | 3: ist1y+ist3y vs. fstphi
    TH1F *h_mXResidual_Hits_Before;
    TH1F *h_mYResidual_Hits_Before;
    TH1F *h_mXResidual_Hits;
    TH1F *h_mYResidual_Hits;
    TH1F *h_mRResidual_Hits;
    TH1F *h_mPhiResidual_Hits;

#if 0
    // Tracking based on Clusters
    TH1F *h_mAdc_Simple[4];
    TH1F *h_mAdcAngleCorr_Simple[4];
    TH1F *h_mTrackAngle_Simple;
    TH1F *h_mXResidual_Simple;
    TH1F *h_mYResidual_Simple;

    // Efficiency based on Hits
    TH2F *h_mHits_IST; // position on FST from IST projection
    TH2F *h_mHits_FST; // position on FST from FST measurement

    TH2F *h_mHits_IST13;
    TH2F *h_mHits_IST2;
#endif

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;

    TTree *mTree_FstEvent_OutPut;
    FstEvent *mFstEvent_OutPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstTracking,1)
};

#endif
