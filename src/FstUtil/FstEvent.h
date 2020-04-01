#ifndef FstEvent_h
#define FstEvent_h

#include <TObject.h>
#include <TClonesArray.h>
#include "./FstRawHit.h"
#include "./FstCluster.h"

class FstEvent : public TObject
{
  private:
    int mNumOfHits;
    int mNumOfClusters;

    TClonesArray *mRawHits;      //->
    TClonesArray *mClusters;     //->

  public:
    FstEvent();
    ~FstEvent();

    FstRawHit* createRawHit();
    void clearRawHitsList();
    int getNumRawHits() const;
    FstRawHit* getRawHit(int i_hit) const;

    FstCluster* createCluster();
    void clearClustersList();
    int getNumClusters() const;
    FstCluster* getCluster(int i_cluster) const;

    ClassDef(FstEvent,1)
};

#endif

