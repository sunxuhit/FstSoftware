TDataSet *CreateTable() {
  // ====================================================================
  if (!gROOT->GetClass("St_fstControl")) return 0;
  fstControl_st row;
  St_fstControl *tableSet = new St_fstControl("fstControl",1);
  memset(&row,0,tableSet->GetRowSize());
  row.kFstChanMaxRmsNoiseLevel    = 50.0;
  row.kFstChanMinRmsNoiseLevel    = 5.0;
  row.kFstApvMaxCmNoiseLevel      = 25.0;
  row.kFstPedCutDefault           = 3.0;
  row.kFstMinHitCutDefault        = 2.5;
  row.kFstMedHitCutDefault        = 3.5;
  row.kFstMaxHitCutDefault        = 4.0;
  row.kFstCMNCutDefault           = 3.0;
  row.kFstMinNumOfRawHits         = 1;
  row.kFstMaxNumOfRawHits         = 460;
  row.kFstAlldata                 = 0;
  row.kFstADCdata                 = 1;
  row.kFstZSdata                  = 2;
  row.kFstDefaultTimeBin          = 2;
  row.kFstCurrentTimeBinNum|      = 9;
  tableSet->AddAt(&row,0);
  return (TDataSet *)tableSet;
}
