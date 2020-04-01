#ifndef FstRawHit_h
#define FstRawHit_h

#include <TObject.h>
#include "./FstCons.h"

class FstRawHit : public TObject
{
  private:
    int mLayer; // 0 for FST | 1-3 for IST
    int mSensor;
    int mColumn; // phi for FST | x for IST
    int mRow; // r_strip for FST | y for IST
    double mCharge[FST::numTBins]; // pedestal subtracted ADC for each time bin
    int mMaxTb;
    int mHitId; // hit id
    int mDefaultTb;

  public:
    FstRawHit();
    ~FstRawHit();

    void setLayer(int layer);
    void setSensor(int sensor);
    void setColumn(int column);
    void setRow(int row);
    void setCharge(double charge, int tb);
    void setMaxTb(int maxTb);
    void setHitId(int hitId);
    void setDefaultTb(int tb);

    int getLayer() const;
    int getSensor() const;
    int getColumn() const;
    int getRow() const;
    double getCharge(int tb = 0) const;
    int getMaxTb() const;
    int getHitId() const;
    int getDefaultTb() const;

    void Print() const;
    void Clear();

    ClassDef(FstRawHit,1)
};

#endif
