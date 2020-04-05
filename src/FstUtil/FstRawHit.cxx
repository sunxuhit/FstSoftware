#include <iostream>
#include "./FstRawHit.h"

using namespace std;

ClassImp(FstRawHit)

//------------------------------------------

FstRawHit::FstRawHit()
{
  Clear();
}

FstRawHit::~FstRawHit()
{
}

//------------------------------------------

// modifiers
void FstRawHit::setLayer(int layer)
{
  mLayer = layer;
}
void FstRawHit::setSensor(int sensor)
{
  mSensor = sensor;
}
void FstRawHit::setAPV(int apv)
{
  mAPV = apv;
}
void FstRawHit::setChannel(int channel)
{
  mChannel = channel;
}
void FstRawHit::setColumn(int column)
{
  mColumn = column;
}
void FstRawHit::setRow(int row)
{
  mRow = row;
}
void FstRawHit::setPosX(double posX)
{
  mPosX = posX;
}
void FstRawHit::setPosY(double posY)
{
  mPosY = posY;
}
void FstRawHit::setPedMean(double pedMean, int tb)
{
  mPedMean[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ] = pedMean;
}
void FstRawHit::setPedStdDev(double pedStdDev, int tb)
{
  mPedStdDev[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ] = pedStdDev;
}
void FstRawHit::setPedRMS(double pedRMS, int tb)
{
  mPedRMS[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ] = pedRMS;
}
void FstRawHit::setCharge(double charge, int tb)
{
  mCharge[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ] = charge;
}
void FstRawHit::setMaxTb(int maxTb)
{
  mMaxTb = maxTb;
}
void FstRawHit::setHitId(int hitId)
{
  mHitId = hitId;
}
void FstRawHit::setDefaultTb(int tb)
{
  mDefaultTb = tb;
}

// accessors
int FstRawHit::getLayer() const
{
  return mLayer;
}
int FstRawHit::getSensor() const
{
  return mSensor;
}
int FstRawHit::getAPV() const
{
  return mAPV;
}
int FstRawHit::getChannel() const
{
  return mChannel;
}
int FstRawHit::getColumn() const
{
  return mColumn;
}
int FstRawHit::getRow() const
{
  return mRow;
}
double FstRawHit::getPosX() const
{
  return mPosX;
}
double FstRawHit::getPosY() const
{
  return mPosY;
}
double FstRawHit::getPedMean(int tb) const
{
  return mPedMean[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ];
}
double FstRawHit::getPedStdDev(int tb) const
{
  return mPedStdDev[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ];
}
double FstRawHit::getPedRMS(int tb) const
{
  return mPedRMS[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ];
}
double FstRawHit::getCharge(int tb) const
{
  return mCharge[ (tb < 0 || tb >= FST::numTBins) ? mDefaultTb : tb ];
}
int FstRawHit::getMaxTb() const
{
  return mMaxTb;
}
int FstRawHit::getHitId() const
{
  return mHitId;
}
int FstRawHit::getDefaultTb() const
{
  return mDefaultTb;
}

//------------------------------------------
void FstRawHit::Print() const
{
  cout << "mLayer = " << mLayer << endl;
  cout << "mSensor = " << mSensor << endl;
  cout << "mAPV = " << mAPV << endl;
  cout << "mChannel = " << mChannel << endl;
  cout << "mColumn = " << mColumn << endl;
  cout << "mRow = " << mRow << endl;
  cout << "mPosX = " << mPosX << endl;
  cout << "mPosY = " << mPosY << endl;
  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    cout << "i_tb = " << i_tb << ", mPedMean = " <<  mPedMean[i_tb] << endl;
    cout << "i_tb = " << i_tb << ", mPedStdDev = " <<  mPedStdDev[i_tb] << endl;
    cout << "i_tb = " << i_tb << ", mPedRMS = " <<  mPedRMS[i_tb] << endl;
    cout << "i_tb = " << i_tb << ", mCharge = " <<  mCharge[i_tb] << endl;
  }
  cout << "mMaxTb =" << mMaxTb << endl;
  cout << "mHitId =" << mHitId << endl;
  cout << "mDefaultTb = " << mDefaultTb  << endl;
  cout << endl;
}

void FstRawHit::Clear()
{
  mLayer = -1;
  mSensor = -1;
  mAPV = -1;
  mChannel = -1;
  mColumn = -1;
  mRow = -1;
  mPosX = -999.9;
  mPosY = -999.9;
  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    mPedMean[i_tb] = -1.0;
    mPedStdDev[i_tb] = -1.0;
    mPedRMS[i_tb] = -1.0;
    mCharge[i_tb] = -1.0;
  }
  mMaxTb = -1;
  mHitId = -1;
  mDefaultTb = FST::defaultTimeBin;
}
