#!/bin/bash
date +%F

#. ./getDataMpod03Iseg0Run23.sh

if [ $# -eq 0 ]
then

  echo "retrive bias current of fstmpod03 u0-7 for run23"

  echo "retrive bias current of fstmpod03 u0 =====>"
  rm fstmpod03_u0_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u0` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u0_run23.txt

  echo "retrive bias current of fstmpod03 u1 =====>"
  rm fstmpod03_u1_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u1` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u1_run23.txt

  echo "retrive bias current of fstmpod03 u2 =====>"
  rm fstmpod03_u2_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u2` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u2_run23.txt

  echo "retrive bias current of fstmpod03 u3 =====>"
  rm fstmpod03_u3_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u3` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u3_run23.txt

  echo "retrive bias current of fstmpod03 u4 =====>"
  rm fstmpod03_u4_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u4` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u4_run23.txt

  echo "retrive bias current of fstmpod03 u5 =====>"
  rm fstmpod03_u5_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u5` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u5_run23.txt

  echo "retrive bias current of fstmpod03 u6 =====>"
  rm fstmpod03_u6_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u6` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u6_run23.txt

  echo "retrive bias current of fstmpod03 u7 =====>"
  rm fstmpod03_u7_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u7` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u7_run23.txt

else
  echo "Wrong Input Parameter! Try: . ./getDataMpod03Iseg0Run23.sh"
fi
