#ifndef StFstHitMaker_hh
#define StFstHitMaker_hh

#include "StMaker.h"

class THashList;


/**
 * Calculates hit global position, and writes FST hits to StFstHitCollection.
 *
 * \author: Shenghui Zhang
 * \date Oct. 2021
 */
class StFstHitMaker : public StMaker
{
public:

   StFstHitMaker( const char *name = "ist_hit" );
   Int_t InitRun(Int_t runnumber);
   Int_t Make();

  virtual const char *GetCVS() const
  {static const char cvs[]="Tag $Name: SL20a $ $Id: StFstHitMaker.h,v 1.15 2015/07/27 18:50:49 huangbc Exp $ built " __DATE__ " " __TIME__  ; return cvs;}

protected:

   THashList *mSensorTransforms; ///< A list of TGeo transformations for each FST sensor

   ClassDef(StFstHitMaker, 0);
};

#endif


/***************************************************************************
* StFstHitMaker.h,v 1.0
* Revision 1.0 2021/10/04 Shenghui Zhang
* Initial version
****************************************************************************/
