#ifndef IstTracking_h
#define IstTracking_h

#include <TObject.h>
#include <./IstTrackingCons.h>

class TFile;
class TChain;
class TH2F;
class TH3F;
class TGraph;

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

    int Make();
    bool calPedestal();

    int Finish();
    void writePedestal();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // Pedestal
    float ped[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
    float pedStdDev[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];
    // float pedRMS[IST::numARMs][IST::numPorts][IST::numAPVs][IST::numChannels];

    TGraph *g_mPedMean_Layer1;
    TGraph *g_mPedSigma_Layer1;
    TGraph *g_mPedMean_Layer2;
    TGraph *g_mPedSigma_Layer2;
    TGraph *g_mPedMean_Layer3;
    TGraph *g_mPedSigma_Layer3;

    TH3F *h_mDisplay_Layer1; // col & row & event => first 100 events
    TH3F *h_mDisplay_Layer2;
    TH3F *h_mDisplay_Layer3;

    // Utility for tracking
    int getLayer(int arm, int port); // return layer based on arm & port
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
