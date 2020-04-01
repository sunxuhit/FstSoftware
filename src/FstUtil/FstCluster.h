#ifndef FstCluster_h
#define FstCluster_h

#include <TObject.h>
#include <TClonesArray.h>
#include <vector>
#include "./FstRawHit.h"
#include "./FstCons.h"

class FstCluster : public TObject
{
  private:
    int mLayer; // 0 for FST | 1-3 for IST
    int mSensor;
    double mMeanColumn; // phi for FST | x for IST
    double mMeanRow; // r_strip for FST | y for IST
    double mTotCharge; // sum of pedestal subtracted ADC for max time bin of each hit in the cluster
    int mMaxTb;
    int mClusterType; // 0 for ARMDisplay | 1 for Simple | 2 for Scan
    int mNRawHits; // number of hits in the cluster
    int mNRawHitsR; // number of hits in the cluster along the R direction
    int mNRawHitsPhi; // number of hits in the cluster along the phi direction
    std::vector<FstRawHit *> mRawHitVec; // container to save raw hits which contribute to the cluster
    int mHitId[FST::maxNHits];

    // int mNumOfRawHits; // number of raw hits in the cluster
    // TClonesArray *mFstRawHits; //->

  public:
    FstCluster();
    ~FstCluster();

    void setLayer(int layer);
    void setSensor(int sensor);
    void setMeanColumn(double column);
    void setMeanRow(double row);
    void setTotCharge(double charge);
    void setMaxTb(int maxTb);
    void setClusterType(int type);
    void setNRawHits(int nRawHits);
    void setNRawHitsR(int nRawHitsR);
    void setNRawHitsPhi(int nRawHitsPhi);
    void addRawHit(FstRawHit *fstRawHit);
    void setHitId(int i_hit, int hitId);

    int getLayer() const;
    int getSensor() const;
    double getMeanColumn() const;
    double getMeanRow() const;
    double getTotCharge() const;
    int getMaxTb() const;
    int getClusterType() const;
    int getNRawHits() const;
    int getNRawHitsR() const;
    int getNRawHitsPhi() const;
    std::vector<FstRawHit *> getRawHitVec() const;
    int getHitId(int i_hit) const;

#if 0
    FstRawHit* createRawHit();
    void clearRawHitsList();
    int getNumRawHits() const;
    FstRawHit* getRawHit(int i_hit) const;
#endif

    void Print() const;
    void Clear();

    ClassDef(FstCluster,1)
};

#endif
