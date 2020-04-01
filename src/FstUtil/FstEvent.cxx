#include <iostream>
#include "./FstEvent.h"

using namespace std;

ClassImp(FstEvent)

//------------------------------------------
FstEvent::FstEvent()
{
  mNumOfHits = 0;
  mNumOfClusters = 0;

  mRawHits = new TClonesArray("FstRawHit", 10);
  mClusters = new TClonesArray("FstCluster", 10);
}

FstEvent::~FstEvent()
{
  delete mRawHits;
  mRawHits = NULL;
  delete mClusters;
  mClusters = NULL;
}
//------------------------------------------

// FstRawHit
FstRawHit* FstEvent::createRawHit()
{
  if(mNumOfHits == mRawHits->GetSize())
    mRawHits->Expand( mNumOfHits + 10 );
  if(mNumOfHits >= 15)
  {
    Fatal( "FstEvent::createRawHit()", "ERROR: Too many hits (>15)!" );
    exit( 2 );
  }

  new((*mRawHits)[mNumOfHits++]) FstRawHit;
  return (FstRawHit*)((*mRawHits)[mNumOfHits - 1]);
}

void FstEvent::clearRawHitsList()
{
  mNumOfHits = 0;
  mRawHits->Clear();
}

int FstEvent::getNumRawHits() const
{
  return mNumOfHits;
}

FstRawHit* FstEvent::getRawHit(int i_hit) const
{
  return i_hit < mNumOfHits ? (FstRawHit*)((*mRawHits)[i_hit]) : NULL;
}
// FstRawHit

// FstCluster
FstCluster* FstEvent::createCluster()
{
  if(mNumOfClusters == mClusters->GetSize())
    mClusters->Expand( mNumOfClusters + 10 );
  if(mNumOfClusters >= 15)
  {
    Fatal( "FstEvent::createCluster()", "ERROR: Too many clusters (>15)!" );
    exit( 2 );
  }

  new((*mClusters)[mNumOfClusters++]) FstCluster;
  return (FstCluster*)((*mClusters)[mNumOfClusters - 1]);
}

void FstEvent::clearClustersList()
{
  mNumOfClusters = 0;
  mClusters->Clear();
}

int FstEvent::getNumClusters() const
{
  return mNumOfClusters;
}

FstCluster* FstEvent::getCluster(int i_cluster) const
{
  return i_cluster < mNumOfClusters ? (FstCluster*)((*mClusters)[i_cluster]) : NULL;
}
// FstCluster
