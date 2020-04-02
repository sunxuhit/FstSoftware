#ifndef FstTrack_h
#define FstTrack_h

#include <TObject.h>
#include <TVector3.h>

class FstTrack : public TObject
{
  private:
    int mTrackId; // 0 => reconstructed from hits | 100 => reconstructed from simple cluster | 200 => reconstructed from scan cluster
    int mTrackType; // 0: reconstructed from hits | 1: reconstructed from simple cluster | 2: reconstructed from scan cluster

    int mId[4]; // track/cluster Id for each layer
    TVector3 mPosition[4]; // measured position on each layer: 0 for FST | 1-3 for IST
    TVector3 mProjection[4]; // projected position on layer 0 & 2
    TVector3 mPosOrig[4]; // measured position on each layer: 0 for FST | 1-3 for IST

  public:
    FstTrack();
    ~FstTrack();

    void setTrackId(int trackId);
    void setTrackType(int trackType);
    void setId(int layer, int id);
    void setPosition(int layer, TVector3 pos);
    void setProjection(int layer, TVector3 pos);
    void setPosOrig(int layer, TVector3 pos);

    int getTrackId() const;
    int getTrackType() const;
    int getId(int layer) const;
    TVector3 getPosition(int layer) const;
    TVector3 getProjection(int layer) const;
    TVector3 getPosOrig(int layer) const;

    void Print() const;
    void Clear();

    ClassDef(FstTrack,1)
};

#endif

