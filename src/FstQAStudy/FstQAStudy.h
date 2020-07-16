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
    TH1F *h_mAdcFst_Hits[FST::numRStrip]; // 0-3 r_strip
    TH1F *h_mAdcIst_Hits[4]; // 0: FST | 1: ISST

    TH1F *h_mAdcFst_Clusters[FST::numRStrip]; // 0-3 r_strip
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
    // hits
    TProfile2D *p_mPedMap_FST;
    TProfile2D *p_mSigMap_FST;
    TH1F *h_mFstHitsMaxTb[FST::mFstNumSensorsPerModule];
    TH1F *h_mFstHitsSignal[FST::mFstNumSensorsPerModule];
    TH1F *h_mFstHitsNoise[FST::mFstNumSensorsPerModule];
    TH1F *h_mFstHitsSNRatio[FST::mFstNumSensorsPerModule];

    TH1F *h_mFstHitsMaxTb_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstHitsSignal_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstHitsNoise_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstHitsSNRatio_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];

    TH1F *h_mFstHitsSignal_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins]; // time bin differential
    TH1F *h_mFstHitsNoise_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];
    TH1F *h_mFstHitsSNRatio_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];

    //clusters
    TH1F *h_mFstSimpleClustersSignal[FST::mFstNumSensorsPerModule];
    TH1F *h_mFstSimpleClustersSignal_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstSimpleClustersMaxTb_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstSimpleClustersSignal_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];

    TH1F *h_mFstScanClustersSignal[FST::mFstNumSensorsPerModule];
    TH1F *h_mFstScanClustersSignal_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstScanClustersMaxTb_Rstrip[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor];
    TH1F *h_mFstScanClustersSignal_Rstrip_TimeBin[FST::mFstNumSensorsPerModule][FST::mFstNumRstripPerSensor][FST::numTBins];

    // Output TTree for Event Display
    bool mApplyCMNCorr;
    TTree *mTree_EventDisplay;
    int mEventId;
    int mNumOfFstRawHits;
    int mNumOfIst1RawHits;
    int mNumOfIst2RawHits;
    int mNumOfIst3RawHits;
    int mNumOfFstSimpleClusters;
    int mNumOfFstScanClusters;
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

    TH2F *h_mFstSimpleClustersDisplay;
    TH2F *h_mFstScanClustersDisplay;
    TH2F *h_mFstScanClustersDisplay_RawHits; // hits & peds in a cluster
    TH2F *h_mFstScanClustersDisplay_RawPeds;
    TH2F *h_mFstScanClustersDisplay_MaxTb;

    TH2F *h_mHitTracksDisplay;
    TH2F *h_mClusterTracksDisplay;

    TGraph *g_mFstSimpleClustersDisplay; // display only
    TGraph *g_mFstScanClustersDisplay; // display only
    TGraph *g_mHitTracksDisplay;
    TGraph *g_mClusterTracksDisplay;

    TH1F *h_mNumFstRawHitsDisplay;
    TH1F *h_mNumFstSimpleClustersDisplay;
    TH1F *h_mNumFstScanClustersDisplay;

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstQAStudy,1)
};

#endif
