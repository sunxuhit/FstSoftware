#!/bin/sh
starver dev

#root4star -b -q -l 'bfc.C(1,200,"in,ry2014,fstRawHit,fstDb,fstCluster,fstHit,fstQA","st_cosmic_22315089_raw_2000001.daq")'
# root4star -b -q -l 'bfc.C(1,200,"in,fstRawHit,fstDb,fstCluster,fstHit,fstQA","st_cosmic_22315089_raw_2000001.daq")'
root4star -b -q -l 'bfc.C(1,500,"in,fstRawHit,fstDb,fstCalib","/star/u/sunxuhit/ForwardSiliconTracker/Data/FstInstallation/daqtest/st_fstcosmic_adc_22338008_raw_0000002.daq")'

