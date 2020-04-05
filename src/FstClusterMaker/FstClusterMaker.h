#ifndef FstClusterMaker_h
#define FstClusterMaker_h

#include <TObject.h>
#include "../FstUtil/FstCons.h"
#include <vector>
#include <fstream>

class TFile;
class TChain;
class TTree;
class TH1F;
class TH2F;
class TGraph;
class FstRawHit;
class FstCluster;
class FstTrack;
class FstEvent;

class FstClusterMaker : public TObject
{
  public:
    FstClusterMaker();
    ~FstClusterMaker();

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

    // input TChain
    bool initChain();

    // pedestal & signal
    bool clearPedestal();
    bool initPedestal();
    bool calPedestal(); // extract pedestal for each ch and fill TGraphs for ped mean & sigma (noise)
    void writePedestal();
    bool clearSignal();
    bool initSignal();

    // hit & hit display
    bool initHitDisplay();
    void fillHitDisplay(std::vector<FstRawHit *> rawHitsVec);
    void writeHitDisplay();

    // cluster with Simple Algorithm
    std::vector<FstCluster *> findCluster_Simple(std::vector<FstRawHit *> rawHitsVec_orig);

    // find track
    std::vector<FstTrack *> findTrack_Hits(std::vector<FstRawHit *> rawHitVec_orig);
    std::vector<FstTrack *> findTrack_Clusters(std::vector<FstCluster *> clusterVec_orig);

    // Output TTree for hits and clusters
    bool initTree();
    void writeTree();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Pedestal
    double mPed[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // ped for each time bin
    double mPedStdDev[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // std of ped for each time bin
    double mPedRMS[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // rms of ped for each time bin

    // Pedestal Display
    TGraph *g_mPedMean[4][FST::numTBins]; // 0 for FST, 1-3 for IST
    TGraph *g_mPedSigma[4][FST::numTBins];
    TH2F *h_mPedDisplay[4][FST::numTBins]; // phi & r_strip

    // Signal
    double mRawSig[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins];
    double mSigPedCorr[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins];

    // Hit Display
    TH2F *h_mHitDisplay[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mMaxTb[4]; 

    // Output TTree for hits and clusters
    TTree *mTree_FstEvent;
    FstEvent *mFstEvent;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    // Utility for tracking
    int getLayer(int arm, int port); // return layer based on arm & port
    int getSensor(int arm, int port, int apv); // return sensor based on APV and IST/FST
    int getColumn(int arm, int port, int apv, int ch); // return FST/IST column based on arm & port & apv & ch
    int getRow(int arm, int port, int apv, int ch); // return FST/IST row based on arm & port & apv & ch
    int getPhiSeg(int apv, int ch); // return FST Phi segmentation based on apv & ch
    int getRStrip(int apv, int ch); // return FST R strip based on apv & ch
    double getPosX(int arm, int port, int apv, int ch); // return FST/IST X based on arm & port & apv & ch
    double getPosY(int arm, int port, int apv, int ch); // return FST/IST Y based on arm & port & apv & ch
    bool isBadAPV(int arm, int port, int apv);

    TChain *mChainInPut; // input TTree
    int evt_rdo[FST::numARMs][FST::numPorts][FST::numAPVs];
    int evt_arm[FST::numARMs][FST::numPorts][FST::numAPVs];
    int evt_port[FST::numARMs][FST::numPorts][FST::numAPVs];
    int evt_apv[FST::numARMs][FST::numPorts][FST::numAPVs];
    int hit_ch[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numROChannels];
    int hit_tb[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numROChannels];
    int hit_adc[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numROChannels];

    ClassDef(FstClusterMaker,1)
};

#endif
