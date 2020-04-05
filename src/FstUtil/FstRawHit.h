#ifndef FstRawHit_h
#define FstRawHit_h

#include <TObject.h>
#include "./FstCons.h"

class FstRawHit : public TObject
{
  private:
    int mLayer; // 0 for FST | 1-3 for IST
    int mSensor;
    int mAPV;
    int mChannel;
    int mColumn; // phi for FST | x for IST
    int mRow; // r_strip for FST | y for IST
    double mPosX; // extracted from column & row
    double mPosY;
    double mPedMean[FST::numTBins]; // mean pedestal for each time bin
    double mPedStdDev[FST::numTBins]; // rms of pedestal for each time bin => noise level
    double mPedRMS[FST::numTBins]; // rms of pedestal for each time bin => noise level
    double mCharge[FST::numTBins]; // pedestal subtracted ADC for each time bin
    int mMaxTb;
    int mHitId; // hit id
    int mDefaultTb;

  public:
    FstRawHit();
    ~FstRawHit();

    void setLayer(int layer);
    void setSensor(int sensor);
    void setAPV(int apv);
    void setChannel(int channel);
    void setColumn(int column);
    void setRow(int row);
    void setPosX(double posX);
    void setPosY(double posY);
    void setPedMean(double pedMean, int tb);
    void setPedStdDev(double pedStdDev, int tb);
    void setPedRMS(double pedRMS, int tb);
    void setCharge(double charge, int tb);
    void setMaxTb(int maxTb);
    void setHitId(int hitId);
    void setDefaultTb(int tb);

    int getLayer() const;
    int getSensor() const;
    int getAPV() const;
    int getChannel() const;
    int getColumn() const;
    int getRow() const;
    double getPosX() const;
    double getPosY() const;
    double getPedMean(int tb = 0) const;
    double getPedStdDev(int tb = 0) const;
    double getPedRMS(int tb = 0) const;
    double getCharge(int tb = 0) const;
    int getMaxTb() const;
    int getHitId() const;
    int getDefaultTb() const;

    void Print() const;
    void Clear();

    ClassDef(FstRawHit,1)
};

#endif
