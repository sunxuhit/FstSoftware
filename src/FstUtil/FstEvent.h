#ifndef FstEvent_h
#define FstEvent_h

#include <TObject.h>
#include <TClonesArray.h>
#include "./FstRawHit.h"
#include "./FstCluster.h"
#include "./FstTrack.h"

class FstEvent : public TObject
{
  private:
    int mNumOfHits;
    int mNumOfClusters;
    int mNumOfTracks;

    TClonesArray *mRawHits;      //->
    TClonesArray *mClusters;     //->
    TClonesArray *mTracks;       //->

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

    FstTrack* createTrack();
    void clearTracksList();
    int getNumTracks() const;
    FstTrack* getTrack(int i_track) const;

    ClassDef(FstEvent,1)
};

#endif

