#include <iostream>
#include "./FstTrack.h"

using namespace std;

ClassImp(FstTrack)

//------------------------------------------

FstTrack::FstTrack()
{
  Clear();
}

FstTrack::~FstTrack()
{
}

//------------------------------------------
// modifiers
void FstTrack::setTrackId(int trackId)
{
  mTrackId = trackId;
}
void FstTrack::setTrackType(int trackType)
{
  mTrackType = trackType;
}
void FstTrack::setId(int layer, int id)
{
  mId[layer] = id;
}
void FstTrack::setPosition(int layer, TVector3 pos)
{
  mPosition[layer].SetXYZ(pos.X(),pos.Y(),pos.Z());
}
void FstTrack::setProjection(int layer, TVector3 pos)
{
  mProjection[layer].SetXYZ(pos.X(),pos.Y(),pos.Z());
}
void FstTrack::setPosOrig(int layer, TVector3 pos)
{
  mPosOrig[layer].SetXYZ(pos.X(),pos.Y(),pos.Z());
}


// accessors
int FstTrack::getTrackId() const
{
  return mTrackId;
}
int FstTrack::getTrackType() const
{
  return mTrackType;
}
int FstTrack::getId(int layer) const
{
  return mId[layer];
}
TVector3 FstTrack::getPosition(int layer) const
{
  return mPosition[layer];
}
TVector3 FstTrack::getProjection(int layer) const
{
  return mProjection[layer];
}
TVector3 FstTrack::getPosOrig(int layer) const
{
  return mPosOrig[layer];
}
//------------------------------------------
void FstTrack::Print() const
{
  cout << "mTrackId = " << mTrackId << endl;
  cout << "mTrackType = " << mTrackType << endl;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    cout << "layer = " << i_layer << ", Hit/Cluster Id is: " << mId[i_layer] << endl;
    cout << "layer = " << i_layer << ", Hit/Cluster ReadOut Position =>" << endl; 
    cout << "X = " << mPosition[i_layer].X() << ", Y = " << mPosition[i_layer].Y() << ", Z = " << mPosition[i_layer].Z() << endl;;
    if(i_layer == 0 || i_layer == 2)
    {
      cout << "layer = " << i_layer << ", Hit/Cluster Projected Position =>" << endl; 
      cout << "X = " << mProjection[i_layer].X() << ", Y = " << mProjection[i_layer].Y() << ", Z = " << mProjection[i_layer].Z() << endl;;
    }
  }
}

void FstTrack::Clear()
{
  mTrackId = -1;
  mTrackType = -1;

  for(int i_layer = 0; i_layer < 4; ++i_layer)
  {
    mId[i_layer] = -1;
    mPosition[i_layer].SetXYZ(-999.9,-999.9,-999.9);
    mProjection[i_layer].SetXYZ(-999.9,-999.9,-999.9);
    mPosOrig[i_layer].SetXYZ(-999.9,-999.9,-999.9);
  }
}
//------------------------------------------
