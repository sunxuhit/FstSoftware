#!/bin/bash
date +%F

#. ./getDataMpod01Iseg0.sh

if [ $# -eq 0 ]
then

  echo "retrive bias current of fstmpod01 u0-7 for run22"

  echo "retrive bias current of fstmpod01 u0 =====>"
  rm fstmpod01_u0_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u0` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u0_run22.txt

  echo "retrive bias current of fstmpod01 u1 =====>"
  rm fstmpod01_u1_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u1` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u1_run22.txt

  echo "retrive bias current of fstmpod01 u2 =====>"
  rm fstmpod01_u2_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u2` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u2_run22.txt

  echo "retrive bias current of fstmpod01 u3 =====>"
  rm fstmpod01_u3_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u3` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u3_run22.txt

  echo "retrive bias current of fstmpod01 u4 =====>"
  rm fstmpod01_u4_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u4` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u4_run22.txt

  echo "retrive bias current of fstmpod01 u5 =====>"
  rm fstmpod01_u5_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u5` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u5_run22.txt

  echo "retrive bias current of fstmpod01 u6 =====>"
  rm fstmpod01_u6_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u6` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u6_run22.txt

  echo "retrive bias current of fstmpod01 u7 =====>"
  rm fstmpod01_u7_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u7` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u7_run22.txt

else
  echo "Wrong Input Parameter! Try: . ./getDataMpod01Iseg0.sh"
fi
