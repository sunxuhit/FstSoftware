#ifndef IstTracking_h
#define IstTracking_h

#include <TObject.h>
#include <./IstTrackingCons.h>

class TFile;
class TChain;
class TH1F;
class TH2F;
class TH3F;
class TGraph;

typedef struct
{
  int layer;
  int sensor;
  int column;
  int row;
  double maxAdc;
  int maxTb;
  bool filled;
} IstHit;

typedef struct
{
  int layer;
  int sensor;
  double x;
  double y;
  double z;
  double adc;
  bool filled;
} IstCluster;

class IstTracking : public TObject
{
  public:
    IstTracking();
    ~IstTracking();

    void set_list(std::string inputlist)
    {
      mList = inputlist;
    }
    void set_outputfile(std::string outputfile)
    {
      mOutPutFile = outputfile;
    }

    int Init();
    bool initChain();
    bool initPedestal();
    bool initSignal();
    bool initHit();
    bool initHitDisplay();
    bool initCluster();
    bool initTracking_ARMDisplay();

    bool clearHit();
    bool clearSignal();
    bool clearCluster();

    int Make();
    bool calPedestal(); // extract pedestal for each ch and fill TGraphs for ped mean & sigma (noise)
    void fillHitDisplay(IstHit isthit[]);
    bool findCluster_ARMDisplay(IstHit isthit[], int numOfHits);
    void fillTracking_ARMDisplay(IstCluster istcluster[], int numOfCluster);

    int Finish();
    void writePedestal();
    void writeHitDisplay();
    void writeTracking_ARMDisplay();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Pedestal
    double mPed[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
    double mPedStdDev[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
    // double pedRMS[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];

    TGraph *g_mPedMean_Layer1;
    TGraph *g_mPedSigma_Layer1;
    TGraph *g_mPedMean_Layer2;
    TGraph *g_mPedSigma_Layer2;
    TGraph *g_mPedMean_Layer3;
    TGraph *g_mPedSigma_Layer3;

    TH2F *h_mHitDisplay_Layer1; // col & row
    TH2F *h_mHitDisplay_Layer2;
    TH2F *h_mHitDisplay_Layer3;
    TH1F *h_mMaxTb_Layer1; 
    TH1F *h_mMaxTb_Layer2;
    TH1F *h_mMaxTb_Layer3;

    IstHit mIstHit[IST::maxNHits]; // store hit information after ped subtraction
    double mSigPedCorr[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels][IST::numTBins];
    double mRawSig[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels][IST::numTBins];

    IstCluster mIstCluster[IST::maxNHits]; // cluster
    TH1F *h_mXResidual;
    TH1F *h_mYResidual;
    TH1F *h_mAdc_Layer1;
    TH1F *h_mAdc_Layer3;
    TH1F *h_mAdcAngleCorr_Layer1;
    TH1F *h_mAdcAngleCorr_Layer3;
    TH1F *h_mTrackAngle;

    // Utility for tracking
    int getLayer(int arm, int port); // return layer based on arm & port
    int getSensor(int apv); // return sensor based on APV
    int getColumn(int apv, int ch); // return column based on arm & port & apv & ch
    int getRow(int apv, int ch); // return row based on arm & port & apv & ch

    TChain *mChainInPut; // input TTree
    int evt_rdo[IST::numARMs][IST::numPorts][IST::numAPVs];
    int evt_arm[IST::numARMs][IST::numPorts][IST::numAPVs];
    int evt_port[IST::numARMs][IST::numPorts][IST::numAPVs];
    int evt_apv[IST::numARMs][IST::numPorts][IST::numAPVs];
    int hit_ch[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numROChannels];
    int hit_tb[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numROChannels];
    int hit_adc[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numROChannels];

    ClassDef(IstTracking,1)
};

#endif
