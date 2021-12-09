/* $Id: StFstDb.h */

#ifndef StFstDb_hh
#define StFstDb_hh

#include "StObject.h"
#include "THashList.h"
#include "TGeoMatrix.h"
//#include "StEvent/StEnumerations.h"
#include "StFstUtil/StFstConsts.h"

class Survey_st;
class fstPedNoise_st;
class fstControl_st;
class fstGain_st;
class fstMapping_st;
class fstChipConfig_st;
//class fstSimPar_st;


/**
 * FST calibration/geometry database access proxy.
 *
 * relation within STAR frame
 * FstOnGlobal = Tpc2Magnet * Fst2Tpc *    Hss2Fst     * Wedge2Hss * Sensor2Wedge * PS

 * Naming of roatation matrices in this maker :
 * positionGlobal  = tpc2Global * fst2Tpc * hss2Fst * wedge2Hss * sensor2Wedge * positionOnSensor

 * numbering
 * Id  = 1000 + (wedge-1)*3 + sensor
 * 1<= wedge <= 36
 * 0<= sensor <= 2
 *
 *
 * \author Shenghui Zhang
 * \date Oct 2021
 */
class StFstDb : public StObject
{

public:
   StFstDb();
   THashList *getRotations() const                                                {return mgRotList; }
   const TGeoHMatrix *getGeoHMatrixTpcOnGlobal() const                            {return mGeoHMatrixTpcOnGlobal; }
   const TGeoHMatrix *getGeoHMatrixFstOnTpc() const                               {return &mGeoHMatrixFstOnTpc; }
   const TGeoHMatrix *getGeoHMatrixHssOnFst() const                               {return &mGeoHMatrixHssOnFst; }
   const TGeoHMatrix *getGeoHMatrixWedgeOnHss() const                             {return &mGeoHMatrixWedgeOnHss; }
   const TGeoHMatrix *getGeoHMatrixSensorOnWedge(Int_t wedge, Int_t sensor) const {return &mGeoHMatrixSensorOnWedge[wedge - 1][sensor]; }
   static const TGeoHMatrix *getHMatrixSensorOnGlobal(int wedge, int sensor);

   const fstPedNoise_st *getPedNoise() const 		{return mFstPedNoise;}
   const fstGain_st *getGain() const     		{return mFstGain;    }
   const fstMapping_st *getMapping() const  		{return mFstMapping; }
   const fstControl_st *getControl() const  		{return mFstControl; }
   const fstChipConfig_st *getChipStatus() const 	{return mFstChipStatus; }

   /*! Simulator parameters -hft efficiency for both fast and slow*/
   //const fstSimPar_st *fstSimPar() {return mFstSimPar;}
      
   Int_t setGeoHMatrices(Survey_st **tables);
   void setPedNoise(fstPedNoise_st *pedNoise) 	    {mFstPedNoise = pedNoise;}
   void setGain(fstGain_st *gain)		    {mFstGain     = gain;}
   void setMapping(fstMapping_st *mapping)    	    {mFstMapping  = mapping;}
   void setControl(fstControl_st *control)    	    {mFstControl  = control;}
   void setChipStatus(fstChipConfig_st *chipStatus) {mFstChipStatus = chipStatus;}
   //void setFstSimPar(fstSimPar_st *fstSimPar) {mFstSimPar = fstSimPar;}
   
   virtual void Print(Option_t *opt = "") const;

   virtual const char *GetCVS() const
   {static const char cvs[] = "Tag $Name: SL20a $ $Id: StFstDb.h,v 1.13 2018/03/15 21:35:48 dongx Exp $ built " __DATE__ " " __TIME__ ; return cvs;}

private:
   static THashList 	*mgRotList; ///< A list of TGeoHMatrix transormations for each FST sensor
   TGeoHMatrix *mGeoHMatrixTpcOnGlobal;
   TGeoHMatrix mGeoHMatrixFstOnTpc;
   TGeoHMatrix mGeoHMatrixHssOnFst;
   TGeoHMatrix mGeoHMatrixWedgeOnHss;
   TGeoHMatrix mGeoHMatrixSensorOnWedge[kFstNumWedges][kFstNumSensorsPerWedge];

   fstPedNoise_st 	*mFstPedNoise;
   fstGain_st 		*mFstGain;
   fstMapping_st 	*mFstMapping;
   fstControl_st 	*mFstControl;
   fstChipConfig_st 	*mFstChipStatus;
   //fstSimPar_st 	*mFstSimPar; ///< simulator parameters - hit efficiency
   
   ClassDef(StFstDb, 1)
};

#endif


/***************************************************************************
* StFstDb.h,v 1.0
* Revision 1.0 2021/10/18 Shenghui Zhang
* Initial version
****************************************************************************/
