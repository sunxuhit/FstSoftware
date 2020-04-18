### this is a code package for 2020 FST Cosmic Test Stand @ UIC

#### list
- run list for FST cosmic test stand
> - FstPed\_*HV*.list: raw ped scan list
> - FstData\_*HV*.list: raw data list
> - FstTracking\_*HV*.list: list for tracking study
> - FstQAStudy\_*HV*.list: list for QA study

---------------

#### src

##### IstTracking
- extract pedestal & noise
- find hit cluster
- extract position dependent tracking resolution and efficiency

##### FstUtil
- provide utility class for analysis
- `FstRawHit`: class of raw hit
- `FstCluster`: class of cluster
- `FstTrack`: class of track of raw hit and cluster
- `FstEvent`: class of all the information saved in one event
- `FstCons`: all constants used in the analysis => geometry & alignment & threshold etc.

##### FstClusterMaker
- identify raw hits (4.5 for FST & 6.0 for IST)
- group raw hits into clusters
- find tracking based on hits and clusters in IST1 & IST3
- save all information to FstEvent
- `FstClusterMaker.cxx` line 216 -- line 593 to change used time bin from 3 to 1

##### FstTracking
- read in FstEvent
- generate histograms for resoluiton and efficiency

##### FstQAStudy
- read in FstEvent
- generate histgram for QA study
- generate Hit Maps 
- generate Cluster Maps
- generate Track Hit Maps

---------------

#### macro
- `IstTracking` extract IST position dependent tracking resolution and efficiency.
- `FstUtil` test FstRawHit, FstCluster, FstTrack and FstEvent classes.
- `FstClusterMaker` analyze raw data and produce RawHits, Clusters and Tracks.
  - root -l -b -q makeFstCluster.C
- `FstTracking` 
>  - doFstTracking.C: produce tracking (Hits & Clusters) from FstClusterMaker
>  - getAlignment\_ISTClusters.C: get alignment parameters of IST1 & IST3 w.r.t. IST2 from IST Clusters 
>  - plotResidual\_ISTClusters.C: plot residuals of IST2 before and after alignment
>  - getAlignment\_FSTClusters.C: get alignment parameters of IST2 w.r.t FST with 2-Layer tracking
>  - plotResidual\_FSTClusters\_2Layer.C: plot residuals of FST before and after alignment based on 2-Layer tracking
>  - plotResidual\_FSTClusters\_3Layer.C: plot residuals of FST before and after alignment based on 3-Layer tracking

- `FstQAStudy`: generate Event Display histograms
- `FstPlotMacro`:
>  - plotQA.sh: produce all QA plots for one specific configuration => Threshold & NumOfTimeBins

---------------

#### compile the code
- mkdir build
- cd ./build
- cmake ..
- make install 
- a shared lib can be found at ${PROJECT\_SOURCE\_DIR}/lib

---------------

#### analysis work flow
1. compile the code
2. `FstClusterMaker`: root -l -b -q makeFstCluster.C
  - produce TTree with FstEvent
  - output file canbe sepcified in the macro
3. `FstTracking`: root -l -b -q doFstTracking.C
  - produce histograms for residual and efficiency
  - output file canbe sepcified in the macro
4. `FstQAStudy`: root -l -b -q doFstQAStudy.C
  - generate TTree for EventDisplay
  - output file canbe sepcified in the macro
5. rename the output with configuration
  - default output file name is *FstClusters\_HV140V\_withPed.root*
  - rename it with configurations: **FstClusters\_HV140V\_withPed\_Th4o5Tb3.root**
    - Th4o5Tb3 stands for 4.5 threshold and 3 used time bins
6. `FstPlotMacro`: . ./plotQA.sh
  - change **hv** and **config** in `plotQA.sh` for different HV and configuration
  - all QA plots can be found in **figures/hv\_config**

