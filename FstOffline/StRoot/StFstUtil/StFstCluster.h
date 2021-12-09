#ifndef StFstCluster_hh
#define StFstCluster_hh

#include <map>

#include "StObject.h"
#include "StFstRawHit.h"


/**
 * Data structure for FST cluster (neighboring pads fired by the same track).
 *
 * \author Shenghui Zhang
 * \date Sep 2021
 */
class StFstCluster: public TObject
{
public:

   StFstCluster(int key = -1, int disk = -1, int wedge = -1, int sensor = -1, int apv = -1,
                float meanRStrip = -1, float meanPhiStrip = -1, float totCharge = 0,
                float totChargeErr = 0, int clusteringType = -1);
   ~StFstCluster();

   //accessors
   vector<StFstRawHit *> &getRawHitVec();
   const vector<StFstRawHit *> &getRawHitVec() 	const;
   int                  getKey()            	const;
   unsigned char        getDisk()         	const; //!< 1-3
   unsigned char        getWedge()         	const; //!< 1-36
   unsigned char        getSensor()         	const; //!< 0-2
   unsigned char        getApv()         	const; //!< 0-15
   float                getMeanRStrip()        	const;
   float                getMeanPhiStrip()     	const;
   float                getTotCharge()      	const;
   float                getTotChargeErr()   	const;
   unsigned char        getMaxTimeBin()     	const;
   unsigned char        getClusteringType() 	const;
   unsigned char        getNRawHits()       	const;
   unsigned char        getNRawHitsRPhi()   	const;
   unsigned char        getNRawHitsZ()      	const;
   unsigned short       getIdTruth()        	const;

   void        setDisk(int disk);
   void        setWedge(int wedge);
   void        setSensor(int sensor);
   void        setApv(int apv);
   void        setMeanRStrip(float meanRStrip);
   void        setMeanPhiStrip(float meanPhiStrip);
   void        setTotCharge(float totCharge);
   void        setTotChargeErr(float totChargeErr);
   void        setMaxTimeBin(int tb);
   void        setClusteringType(int clusteringType);
   void        setNRawHits(int nRawHits);
   void        setNRawHitsRPhi(int nRawHitsRPhi);
   void        setNRawHitsZ(int nRawHitsZ);
   void        setIdTruth(unsigned short idTruth);

   virtual void Print(Option_t *opt = "") const;

protected:
   Int_t       mKey;                    	///< Cluster unique label
   Float_t     mMeanRStrip;                	///< Cluster's mean rstrip
   Float_t     mMeanPhiStrip;             	///< Cluster's mean phistrip
   Float_t     mTotCharge;              	///< Charge sum of the cluster
   Float_t     mTotChargeErr;           	///< rMS noise of the cluster
   UShort_t    mIdTruth;                        //!< For embedding, 0 as background
   UChar_t     mDiskId;                       ///< Disk id the cluster belongs to
   UChar_t     mWedgeId;                       ///< Wedge id the cluster belongs to
   UChar_t     mSensorId;                       ///< Sensor id the cluster belongs to
   UChar_t     mApv;                       ///< Apv id the cluster belongs to
   UChar_t     mClusteringType;         	///< Clustering algorithm type
   UChar_t     mMaxTimeBin;             	///< Max ADC time bin index
   UChar_t     mNRawHits;               	///< Cluster size
   UChar_t     mNRawHitsRPhi;           	///< Cluster size in r-phi direction
   UChar_t     mNRawHitsZ;              	///< Cluster size in beam direction
   std::vector<StFstRawHit *> mRawHitVec;	///< Map container to save raw hits who contribute to the cluster

   ClassDef(StFstCluster, 1);
};

#endif
