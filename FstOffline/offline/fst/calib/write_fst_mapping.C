void write_fst_mapping() {

	gSystem->Setenv("DB_ACCESS_MODE", "write");
	gROOT->Macro("LoadLogger.C");
	gSystem->Load("St_base.so");
	gSystem->Load("libStDb_Tables.so");
	gSystem->Load("StDbLib.so");

	TString storeTime = "2014-01-29 00:00:01";
	cout <<"The RTS Start date and time is " << storeTime<<endl;  

	StDbManager* mgr = StDbManager::Instance();
	StDbConfigNode* node = mgr->initConfig("Calibrations_fst");
	StDbTable* dbtable = node->addDbTable("fstMapping");
	mgr->setStoreTime(storeTime.Data());

	const int ApvPerPort   = 8;
	const int ApvPerSensor = 2;
	const int ModPerRdo    = 6;     // per RDO
	const int PortPerArm   = 2;     // per ARM
	const int ChPerDisk    = 12288; // 128 channels * 96 APV chips
	const int ChPerMod     = 1024;  // 128 channels * 8 APV chips
	const int PhiSegPerMod = 128;   // per module
	const int ApvPerSec    = 4;
	fstMapping_st table;

	for (int i = 0; i < 36864; i++) {
		table.mapping[i] = i;
	}

	for(int rdoIdx=1; rdoIdx<=6; rdoIdx++) 
	{ // 1-6
		for(int armIdx=0; armIdx<3; armIdx++) 
		{ // 0-2
			for(int refApvIdx=0; refApvIdx<16; refApvIdx++) 
			{ // 0-15
				int portIdx      = refApvIdx/ApvPerPort;                            // 0-1
				int lclApvIdx    = refApvIdx-portIdx*ApvPerPort;                    // 0-7
				int lclSensorIdx = lclApvIdx/ApvPerSensor;                          // 0-3
				int diskIdx   = (rdoIdx-1)/2+1;                                     // 1-3
				int moduleIdx = (rdoIdx-1)%2*ModPerRdo+armIdx*PortPerArm+portIdx+1; // 1-12
				for(int chan=0; chan<128; chan++) {
					int elecId = (rdoIdx-1)*3*16*128+armIdx*16*128+refApvIdx*128+chan;
					int lclRstripIdx = -1; // 0-7
					int lclPhiSegIdx = -1; // 0-127
					// Inner Section: APV 0-1 for sensor 0 | APV 2-3 for sensor 1
					// Outer Section: APV 4-5 for sensor 2 | APV 6-7 for sensor 3
					if(lclApvIdx == 0 || lclApvIdx == 1 || lclApvIdx == 4 || lclApvIdx == 6) lclRstripIdx = chan%4;
					if(lclApvIdx == 2 || lclApvIdx == 3 || lclApvIdx == 5 || lclApvIdx == 7) lclRstripIdx = 3-chan%4;
					if(lclSensorIdx == 2 || lclSensorIdx == 3) lclRstripIdx = lclRstripIdx+4; // outer

					if(lclSensorIdx == 0 || lclSensorIdx == 1) lclPhiSegIdx = lclApvIdx*32+chan/4; // inner
					if(lclSensorIdx == 2 || lclSensorIdx == 3) lclPhiSegIdx = (lclApvIdx-ApvPerSec)*32+chan/4; // outer

					table.mapping[elecId] = (diskIdx-1)*ChPerDisk + (moduleIdx-1)*ChPerMod + lclRstripIdx*PhiSegPerMod + lclPhiSegIdx; // 0-36863
				}
			}
		}
	} 

	// Store data to the StDbTable
	dbtable->SetTable((char*)&table, 1);

	// Store table to database
	mgr->storeDbTable(dbtable);


};

