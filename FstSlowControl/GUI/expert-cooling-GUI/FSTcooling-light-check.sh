#!/bin/bash

i=1
while [ "$i" -lt 400 ]
do
    current_data_time="`date "+%Y-%m-%d %H:%M"`"

    FST_Cooling_level=`caget FST:Cool:Reservoir:level | awk '{print($2)}'`
    FST_Cooling_P=`caget FST:Cool:Reservoir:P | awk '{print($2)}' `
    FST_Cooling_RH=`caget FST:Cool:Reservoir:RH | awk '{print($2)}'`
    FST_Cooling_TEMP=`caget FST:Cool:TC:ConSensTemp | awk '{print($2)}'`
    FST_Cooling_Sup=`caget FST:Cool:Flow:supply | awk '{print($2)}'`
    FST_Cooling_Ret=`caget FST:Cool:Flow:return |  awk '{print($2)}'`
    #echo ${current_data_time} ${FST_Cooling_level} ${FST_Cooling_P} ${FST_Cooling_RH} ${FST_Cooling_TEMP} ${FST_Cooling_Sup} ${FST_Cooling_Ret} >> fst_cooling_leaktest_Jul23.dat
    echo ${current_data_time} ${FST_Cooling_level} ${FST_Cooling_P} ${FST_Cooling_RH} ${FST_Cooling_TEMP} ${FST_Cooling_Sup} ${FST_Cooling_Ret} >> fst_cooling_leaktest_Sep.dat
    i=$(($i+1))
    sleep 15m
done
