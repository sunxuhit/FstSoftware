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
class TProfile2D;

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

    int Init();
    int Make(); // subtract pedestal => signal & find Hits
    int Finish();

    // init input TChain
    bool initChain();

    // QA
    void initCounts();
    void fillCounts(FstEvent *fstEvent);
    void writeCounts();

    // ADC
    void initAdcFst();
    void fillAdcFst(FstEvent *fstEvent);
    void writeAdcFst();

  private:
    std::string mHome, mList;
    std::string mOutPutFile;

    TFile *File_mOutPut;

    // QA
    TH1F *h_mCounts_Hits[4]; // 0 for FST, 1-3 for IST
    TH1F *h_mCounts_Clusters[4]; // 0 for FST, 1-3 for IST
    TH2F *h_mCounts_Corr[4];
    TH2F *h_mCounts_RPhi[4];

    // ADC
    TH1F *h_mAdcFst_Hits[4]; // 0-3 r_strip
    TProfile2D *p_mAdcFst_Hits; // r_strip vs phi
    TProfile2D *p_mAdcFst_Column; // cluster mean column vs. hit column
    TProfile2D *p_mAdcFst_Row; // cluster mean row vs. hit row 

    // Time Bin
    TProfile2D *p_mMaxTbFst_Column;
    TProfile2D *p_mMaxTbFst_Row;
    TH1F *h_mTbDiff_Clusters;

    // Position
    TH2F *h_mPositionR_Clusters;
    TH2F *h_mPositionPhi_Clusters;

    // Input TChain for hits and clusters
    TChain *mChainInPut; // input TTree
    FstEvent *mFstEvent_InPut;
    FstRawHit *mFstRawHit;
    FstCluster *mFstCluster;
    FstTrack *mFstTrack;

    ClassDef(FstQAStudy,1)
};

#endif
