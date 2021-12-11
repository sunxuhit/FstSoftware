#!/bin/sh
starver dev

rm fstPedNoiseTable.dat
root4star -b -q -l 'bfc.C(1,10000,"in,fstDb,fstRawHit","../../../data/st_fstcosmic_adc_22340057_raw_0000009.daq")'
