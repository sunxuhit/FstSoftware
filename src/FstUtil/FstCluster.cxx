#include <iostream>
#include "./FstCluster.h"

using namespace std;

ClassImp(FstCluster)

//------------------------------------------

FstCluster::FstCluster()
{
  Clear();

  // mNumOfRawHits = 0;
  // mFstRawHits = new TClonesArray("FstRawHit", 10);
}

FstCluster::~FstCluster()
{
}

//------------------------------------------

// modifiers
void FstCluster::setLayer(int layer)
{
  mLayer = layer;
}
void FstCluster::setSensor(int sensor)
{
  mSensor = sensor;
}
void FstCluster::setMeanColumn(double column)
{
  mMeanColumn = column;
}
void FstCluster::setMeanRow(double row)
{
  mMeanRow = row;
}
void FstCluster::setTotCharge(double charge)
{
  mTotCharge = charge;
}
void FstCluster::setMaxTb(int maxTb)
{
  mMaxTb = maxTb;
}
void FstCluster::setClusterType(int type)
{
  mClusterType = type;
}
void FstCluster::setNRawHits(int nRawHits)
{
  mNRawHits = nRawHits;
}
void FstCluster::setNRawHitsR(int nRawHitsR)
{
  mNRawHitsR = nRawHitsR;
}
void FstCluster::setNRawHitsPhi(int nRawHitsPhi)
{
  mNRawHitsPhi = nRawHitsPhi;
}
void FstCluster::addRawHit(FstRawHit *fstRawHit)
{
  mRawHitVec.push_back(fstRawHit);
}
void FstCluster::setHitId(int i_hit, int hitId)
{
  mHitId[i_hit] = hitId;
}

// accessors
int FstCluster::getLayer() const
{
  return mLayer;
}
int FstCluster::getSensor() const
{
  return mSensor;
}
double FstCluster::getMeanColumn() const
{
  return mMeanColumn;
}
double FstCluster::getMeanRow() const
{
  return mMeanRow;
}
double FstCluster::getTotCharge() const
{
  return mTotCharge;
}
int FstCluster::getMaxTb() const
{
  return mMaxTb;
}
int FstCluster::getClusterType() const
{
  return mClusterType;
}
int FstCluster::getNRawHits() const
{
  return mNRawHits;
}
int FstCluster::getNRawHitsR() const
{
  return mNRawHitsR;
}
int FstCluster::getNRawHitsPhi() const
{
  return mNRawHitsPhi;
}
std::vector<FstRawHit *> FstCluster::getRawHitVec() const
{
  return mRawHitVec;
}
int FstCluster::getHitId(int i_hit) const
{
  return mHitId[i_hit];
}

//------------------------------------------

void FstCluster::Print() const
{
  cout << "mLayer = " << mLayer << endl;
  cout << "mSensor = " << mSensor << endl;
  cout << "mMeanColumn = " << mMeanColumn << endl;
  cout << "mMeanRow = " << mMeanRow << endl;
  cout << "mTotCharge = " << mTotCharge << endl;
  cout << "mMaxTb =" << mMaxTb << endl;
  cout << "mClusterType =" << mClusterType << endl;
  cout << "mNRawHits = " << mNRawHits << endl;
  cout << "mNRawHitsR = " << mNRawHitsR << endl;
  cout << "mNRawHitsPhi = " << mNRawHitsPhi << endl;
  for(int i_hit = 0; i_hit < mNRawHits; ++i_hit)
  {
    cout << "mHitId = " << mHitId[i_hit] << endl;
  }
  cout << endl;
}

void FstCluster::Clear()
{
  mLayer = -1;
  mSensor = -1;
  mMeanColumn = -1;
  mMeanRow = -1;
  mTotCharge = -1;
  mMaxTb = -1;
  mClusterType = -1;
  mNRawHits = -1;
  mNRawHitsR = -1;
  mNRawHitsPhi = -1;
  mRawHitVec.clear();
  for(int i_hit = 0; i_hit < FST::maxNHits; ++i_hit)
  {
    mHitId[i_hit] = -1;
  }
}

#if 0
// FstRawHit
FstRawHit* FstCluster::createRawHit()
{
  if(mNumOfRawHits == mFstRawHits->GetSize())
    mFstRawHits->Expand( mNumOfRawHits + 10 );
  if(mNumOfRawHits >= 15)
  {
    Fatal( "FstEvent::createRawHit()", "ERROR: Too many hits (>15)!" );
    exit( 2 );
  }

  new((*mFstRawHits)[mNumOfRawHits++]) FstRawHit;
  return (FstRawHit*)((*mFstRawHits)[mNumOfRawHits - 1]);
}

void FstCluster::clearRawHitsList()
{
  mNumOfRawHits = 0;
  mFstRawHits->Clear();
}

int FstCluster::getNumRawHits() const
{
  return mNumOfRawHits;
}

FstRawHit* FstCluster::getRawHit(int i_hit) const
{
  return i_hit < mNumOfRawHits ? (FstRawHit*)((*mFstRawHits)[i_hit]) : NULL;
}
// FstRawHit
#endif
