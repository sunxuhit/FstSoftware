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

    // cluster display
    bool initClusterDisplay();
    void fillClusterDisplay(std::vector<FstCluster *> clusterVec_orig);
    void writeClusterDisplay();

    // track QA
    bool initTrackingQA_Hits();
    void fillTrackingQA_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    void writeTrackingQA_Hits();

    // track resolution
    void initTracking_Hits();
    void calResolution_Hits(FstEvent *fstEvent);
    void writeTracking_Hits();

    void initTracking_Clusters();
    void calResolution_Clusters(FstEvent *fstEvent);
    void writeTracking_Clusters();

    // efficiency
    void initEfficiency_Hits();
    void calEfficiency_Hits(FstEvent *fstEvent);
    void writeEfficiency_Hits();

    void initEfficiency_Clusters();
    void calEfficiency_Clusters(FstEvent *fstEvent);
    void writeEfficiency_Clusters();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // QA
    TH1F *h_mCounts_Hits[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mCounts_Clusters[4]; // 0 for FST, 1-3 for IST
    TH2F *h_mCounts_Corr[4];

    // hit Display
    TH2F *h_mHitDisplay[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mMaxTb[4]; 
    // cluster Display
    TH2F *h_mClusterDisplay[4]; // 0 for FST, 1-3 for IST

    // cluster QA

    // Tracking QA based on Hits
    TH2F *h_mHitsCorrXR[4]; // 0: ist1x vs. ist3x | 1: ist1x vs. fstr | 2: ist3x vs. fstr | 3: ist1x+ist3x vs. fstr
    TH2F *h_mHitsCorrYPhi[4]; // 0: ist1y vs. ist3y | 1: ist1y vs. fstphi | 2: ist3y vs. fstphi | 3: ist1y+ist3y vs. fstphi
    TH1F *h_mXResidual_Hits_Before;
    TH1F *h_mYResidual_Hits_Before;
    TH1F *h_mXResidual_Hits;
    TH1F *h_mYResidual_Hits;
    TH1F *h_mRResidual_Hits;
    TH1F *h_mPhiResidual_Hits;

    // Tracking
    TH1F *h_mTrackXRes_Hits;
    TH1F *h_mTrackYRes_Hits;
    TH2F *h_mTrackXYRes_Hits; // 2D distribution

    TH1F *h_mTrackRRes_Hits;
    TH1F *h_mTrackPhiRes_Hits;
    TH2F *h_mTrackRPhiRes_Hits; // 2D distribution

    TH1F *h_mTrackXRes_Clusters_2Layer;
    TH1F *h_mTrackYRes_Clusters_2Layer;
    TH2F *h_mTrackXYRes_Clusters_2Layer; // 2D distribution

    TH1F *h_mTrackRRes_Clusters_2Layer;
    TH1F *h_mTrackPhiRes_Clusters_2Layer;
    TH2F *h_mTrackRPhiRes_Clusters_2Layer; // 2D distribution

    TH1F *h_mTrackXResIST_2Layer;
    TH1F *h_mTrackYResIST_2Layer;
    TH2F *h_mTrackXYResIST_2Layer; // 2D distribution

    TH1F *h_mTrackXRes_Clusters_3Layer;
    TH1F *h_mTrackYRes_Clusters_3Layer;
    TH2F *h_mTrackXYRes_Clusters_3Layer; // 2D distribution

    TH1F *h_mTrackRRes_Clusters_3Layer;
    TH1F *h_mTrackPhiRes_Clusters_3Layer;
    TH2F *h_mTrackRPhiRes_Clusters_3Layer; // 2D distribution

    TH1F *h_mTrackXResIST_3Layer;
    TH1F *h_mTrackYResIST_3Layer;
    TH2F *h_mTrackXYResIST_3Layer;

    // Efficiency based on Hits
    // 0: no matching | 1-3 matching within (1-3)*pitchR in r & (1-3)*pitchPhi in phi
    TH2F *h_mTrackHits_IST[8]; // position on FST from IST projection
    TH2F *h_mTrackHits_FST[8]; // position on FST from FST measurement

    TH2F *h_mTrackClusters_IST_2Layer[8]; // position on FST from IST projection
    TH2F *h_mTrackClusters_FST_2Layer[8]; // position on FST from FST measurement

    TH2F *h_mTrackClusters_IST_3Layer[8]; // position on FST from IST projection
    TH2F *h_mTrackClusters_FST_3Layer[8]; // position on FST from FST measurement

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstTracking,1)
};

#endif
