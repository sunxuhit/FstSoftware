#ifndef FstNoiseStudy_h
#define FstNoiseStudy_h

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

class FstNoiseStudy : public TObject
{
  public:
    FstNoiseStudy();
    ~FstNoiseStudy();

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

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Pedestal
    double mPed[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // ped for each time bin
    double mPedStdDev[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // std of ped for each time bin
    double mPedRMS[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // rms of ped for each time bin

    // Commen Mode Noise => only meaningful for FST
    double mCMNMean[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins]; // CMN for each time bin
    double mCMNStdDev[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // CMN for each time bin

    double mCMNMean_Diff[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numRStrip][FST::numTBins]; // CMN for each time bin
    double mCMNStdDev_Diff[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // CMN for each time bin
    double mDiffStdDev[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numChannels][FST::numTBins]; // std of ped for each time bin

    // Pedestal Display
    TGraph *g_mPedMean[4][FST::numTBins]; // 0 for FST, 1-3 for IST
    TGraph *g_mPedSigma[4][FST::numTBins];
    TH2F *h_mPedDisplay[4][FST::numTBins]; // phi & r_strip
    TH1F *h_mPedMean_FST[4][FST::numTBins]; // for RStrip
    TH1F *h_mPedSigma_FST[4][FST::numTBins];

    TGraph *g_mNoiseCMN[FST::numTBins]; // for FST
    TH1F *h_mCMNSigma_FST[4][FST::numTBins]; // RStrip & TimeBin

    TGraph *g_mNoiseDiff[FST::numTBins];
    TH1F *h_mDiffSIgma_FST[4][FST::numTBins]; // RStrip & TimeBin

    // Utility for tracking
    int getLayer(int arm, int port); // return layer based on arm & port
    int getSensor(int arm, int port, int apv); // return sensor based on APV and IST/FST
    int getColumn(int arm, int port, int apv, int ch); // return FST/IST column based on arm & port & apv & ch
    int getRow(int arm, int port, int apv, int ch); // return FST/IST row based on arm & port & apv & ch
    int getPhiSeg(int apv, int ch); // return FST Phi segmentation based on apv & ch
    int getRStrip(int apv, int ch); // return FST R strip based on apv & ch
    double getPosX(int arm, int port, int apv, int ch); // return FST/IST X based on arm & port & apv & ch
    double getPosY(int arm, int port, int apv, int ch); // return FST/IST Y based on arm & port & apv & ch
    double getMeanX(int layer, double meanColumn); // return FST/IST meanX based on arm & port
    double getMeanY(int layer, double meanRow); // return FST/IST meanY based on arm & port
    bool isBadAPV(int arm, int port, int apv);

    TChain *mChainInPut; // input TTree
    int evt_rdo[FST::numARMs][FST::numPorts][FST::numAPVs];
    int evt_arm[FST::numARMs][FST::numPorts][FST::numAPVs];
    int evt_port[FST::numARMs][FST::numPorts][FST::numAPVs];
    int evt_apv[FST::numARMs][FST::numPorts][FST::numAPVs];
    int hit_ch[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numROChannels];
    int hit_tb[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numROChannels];
    int hit_adc[FST::numARMs][FST::numPorts][FST::numAPVs][FST::numROChannels];

    ClassDef(FstNoiseStudy,1)
};

#endif
