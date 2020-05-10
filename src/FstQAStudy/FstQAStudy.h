#ifndef FstQAStudy_h
#define FstQAStudy_h

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
class TH3F;
class TProfile;
class TProfile2D;
class TGraph;

class FstQAStudy : public TObject
{
  public:
    FstQAStudy();
    ~FstQAStudy();

    void set_list(std::string inputlist)
    {
      mList = inputlist;
    }
    void set_outputfile(std::string outputfile)
    {
      mOutPutFile = outputfile;
    }
    void set_cmncorrection(bool applyCMNCorr)
    {
      mApplyCMNCorr = applyCMNCorr;
    }

    int Init();
    int Make(); // subtract pedestal => signal & find Hits
    int Finish();

    // init input TChain
    bool initChain();

    // Counts
    void initCounts();
    void fillCounts(FstEvent *fstEvent);
    void writeCounts();

    // ADC
    void initAdc_Hits();
    void fillAdc_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    void writeAdc_Hits();

    void initAdc_Clusters();
    void fillAdc_Clusters(std::vector<FstCluster *> clusterVec_orig);
    void writeAdc_Cluster();

    // cluster size
    void initClusterSize();
    void fillClusterSize(std::vector<FstCluster *> clusterVec_orig);
    void writeClusterSize();

    void initClusterSize_TrackClusters();
    void fillClusterSize_TrackClusters(FstEvent *fstEvent);
    void writeClusterSize_TrackClusters();
    
    // signal & noise
    void initSignalQA();
    void fillSignalQA(FstEvent *fstEvent);
    void writeSignalQA();

    // Event Display
    void initEventDisplay_TrackClusters();
    void clearEventDisplay_TrackClusters();
    void fillEventDisplay_TrackClusters(FstEvent *fstEvent);
    void writeEventDisplay_TrackClusters();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Counts
    TH1F *h_mCounts_Hits[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mCounts_Clusters[4]; // 0 for FST, 1-3 for IST
    TH2F *h_mCounts_Corr[4];
    TH2F *h_mCounts_RPhi[4];

    // ADC
    TH1F *h_mAdcFst_Hits[4]; // 0-3 r_strip
    TH1F *h_mAdcIst_Hits[4]; // 0: FST | 1: ISST

    TH1F *h_mAdcFst_Clusters[4]; // 0-3 r_strip
    TH1F *h_mAdcIst_Clusters[4]; // 0: FST | 1: ISST
    TProfile2D *p_mAdcFst_Column; // cluster mean column vs. hit column
    TProfile2D *p_mAdcFst_Row; // cluster mean row vs. hit row 

    // cluster size
    TProfile *p_mNHitsR_meanColumn; // <R>
    TProfile *p_mNHitsR_meanRow; // <phi>
    TProfile *p_mNHitsPhi_meanColumn;
    TProfile *p_mNHitsPhi_meanRow;
    TProfile *p_mTbDiffR;
    TProfile *p_mTbDiffPhi;

    // Cluster size with track 
    TProfile *p_mNHitsR_rP[4]; // 0: no matching | 1-3 matching within (1-3)*pitchR in r & (1-3)*pitchPhi in phi
    TProfile *p_mNHitsR_phiP[4];
    TProfile *p_mNHitsPhi_rP[4];
    TProfile *p_mNHitsPhi_phiP[4];

    // Signal & noise for max time bin
    TProfile2D *p_mPedMap_FST;
    TProfile2D *p_mSigMap_FST;
    TH1F *h_mSignalHits_FST;
    TH1F *h_mNoiseHits_FST;
    TH1F *h_mSNRatioHits_FST;
    TH1F *h_mSignalClusters_FST;

    TH1F *h_mMaxTbHits_Rstrip[4];
    TH1F *h_mMaxTbClusters_Rstrip[4];
    TH1F *h_mSignalHits_Rstrip[4];
    TH1F *h_mNoiseHits_Rstrip[4];
    TH1F *h_mSNRatioHits_Rstrip[4];
    TH1F *h_mSignalClusters_Rstrip[4];
    TH1F *h_mSignalHits_Rstrip_TimeBin[4][FST::numTBins]; // time bin differential
    TH1F *h_mNoiseHits_Rstrip_TimeBin[4][FST::numTBins];
    TH1F *h_mSNRatioHits_Rstrip_TimeBin[4][FST::numTBins];
    TH1F *h_mSignalClusters_Rstrip_TimeBin[4][FST::numTBins];

    TH1F *h_mMaxTbHits_Apv[2][4][2]; // APV chips & R & phi: 0-63 & 64-127
    TH1F *h_mMaxTbClusters_Apv[2][4][2];
    TH1F *h_mSignalHits_Apv[2][4][2];
    TH1F *h_mNoiseHits_Apv[2][4][2];
    TH1F *h_mSNRatioHits_Apv[2][4][2];
    TH1F *h_mSignalClusters_Apv[2][4][2];

    // Output TTree for Event Display
    bool mApplyCMNCorr;
    TTree *mTree_EventDisplay;
    int mEventId;
    int mNumOfFstRawHits;
    int mNumOfIst1RawHits;
    int mNumOfIst2RawHits;
    int mNumOfIst3RawHits;
    int mNumOfFstClusters;
    int mNumOfIst1Clusters;
    int mNumOfIst2Clusters;
    int mNumOfIst3Clusters;
    int mNumOfHitTracks;
    int mNumOfClusterTracks; // all tracks with 2-Layer tracking
    int mNumOfClusterTracks_2Layer; // 2-Layer tracking within area of interest
    int mNumOfClusterTracks_3Layer; // 3-Layer tracking within area of interest

    // Event Display
    TH2F *h_mFstRawHitsDisplay; // save event display histogram into a TTree
    TH2F *h_mFstRawPedsDisplay;
    TH2F *h_mFstMaxTbDisplay;

    TH2F *h_mFstRawHitsDisplay_bTh; // save event display histogram into a TTree
    TH2F *h_mFstRawPedsDisplay_bTh;
    TH2F *h_mFstMaxTbDisplay_bTh;

    TH2F *h_mFstClustersDisplay;
    TH2F *h_mHitTracksDisplay;
    TH2F *h_mClusterTracksDisplay;

    TGraph *g_mFstClustersDisplay; // display only
    TGraph *g_mHitTracksDisplay;
    TGraph *g_mClusterTracksDisplay;

    TH1F *h_mNumFstRawHitsDisplay;
    TH1F *h_mNumFstClustersDisplay;

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstQAStudy,1)
};

#endif
