# Example of Common Mode Noise Calculation

### genCMNGroup.C
- generate `cmnGroup.txt` of CMN group for each APV & channel
- format: apvIdx(0-23)   channelIdx(0-127)    groupIdx(0-3)

### genPedestal.C
- code example to calculate pedestal, total noise and random noise
- CMN is calculated event-by-event
- generate `pedestal.txt` for pedestal information
- fomart: rdoIdx(1-6)    armIdx(0-2)    apvIdx(0-23)    channelIdx(0-127)     tbIdx(0-8)     pedestal     totRMS    ranRMS
- the input file could be found on RCF: /star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/daqtest/testEventAdc.txt 

### testOnlineZS.C
- code example to apply zero suppression
- read in `pedestal.txt`
- calculate CMN event-by-event
- subtract pedestal and CMN from adc
- zero out the channel didn't pass the hit cuts
