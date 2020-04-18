### this is a code package for 2020 FST Cosmic Test Stand @ UIC

#### macro
- `IstTracking` extract IST position dependent tracking resolution and efficiency.
- `FstUtil` test FstRawHit, FstCluster, FstTrack and FstEvent classes.
- `FstClusterMaker` analyze raw data and produce RawHits, Clusters and Tracks.
  - root -l -b -q makeFstCluster.C

---------------

#### list
- run list for FST cosmic test stand

---------------

#### src

##### IstTracking
- extract pedestal & noise
- find hit cluster
- extract position dependent tracking resolution and efficiency

##### FstTracking
- use IST as a scope
- extract pedestal & noise
- find hit cluster
- extract position dependent tracking resolution and efficiency

---------------

#### compile the code
- mkdir build
- cd ./build
- cmake ..
- make install 
- a shared lib can be found at ${PROJECT\_SOURCE\_DIR}/lib
