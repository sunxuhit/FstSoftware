#include <iostream>
#include "./FstRawHit.h"

using namespace std;

ClassImp(FstRawHit)

//------------------------------------------

FstRawHit::FstRawHit()
{
  mLayer = -1;
  mSensor = -1;
  mColumn = -1;
  mRow = -1;
  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    mCharge[i_tb] = -1.0;
  }
  mMaxTb = -1;
  mHitId = -1;

  mDefaultTb = FST::defaultTimeBin; // use 3 for now
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
void FstRawHit::setColumn(int column)
{
  mColumn = column;
}
void FstRawHit::setRow(int row)
{
  mRow = row;
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

// accessors
int FstRawHit::getLayer() const
{
  return mLayer;
}
int FstRawHit::getSensor() const
{
  return mSensor;
}
int FstRawHit::getColumn() const
{
  return mColumn;
}
int FstRawHit::getRow() const
{
  return mRow;
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

//------------------------------------------
void FstRawHit::Print() const
{
  cout << "mLayer = " << mLayer << endl;
  cout << "mSensor = " << mSensor << endl;
  cout << "mColumn = " << mColumn << endl;
  cout << "mRow = " << mRow << endl;
  for(int i_tb = 0; i_tb < FST::numTBins; ++i_tb)
  {
    cout << "i_tb = " << i_tb << ", mCharge = " <<  mCharge[i_tb] << endl;
  }
  cout << "mMaxTb =" << mMaxTb << endl;
  cout << "mHitId =" << mHitId << endl;
  cout << "mDefaultTb = " << mDefaultTb  << endl;
  cout << endl;
}
