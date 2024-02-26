#!/bin/bash
date +%F

#. ./getDataMpod03Iseg1Run23.sh

if [ $# -eq 0 ]
then

  echo "retrive bias current of fstmpod03 u100-107 for run23"

  echo "retrive bias current of fstmpod03 u100 =====>"
  rm fstmpod03_u100_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u100` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u100_run23.txt

  echo "retrive bias current of fstmpod03 u101 =====>"
  rm fstmpod03_u101_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u101` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u101_run23.txt

  echo "retrive bias current of fstmpod03 u102 =====>"
  rm fstmpod03_u102_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u102` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u102_run23.txt

  echo "retrive bias current of fstmpod03 u103 =====>"
  rm fstmpod03_u103_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u103` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u103_run23.txt

  echo "retrive bias current of fstmpod03 u104 =====>"
  rm fstmpod03_u104_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u104` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u104_run23.txt

  echo "retrive bias current of fstmpod03 u105 =====>"
  rm fstmpod03_u105_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u105` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u105_run23.txt

  echo "retrive bias current of fstmpod03 u106 =====>"
  rm fstmpod03_u106_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u106` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u106_run23.txt

  echo "retrive bias current of fstmpod03 u107 =====>"
  rm fstmpod03_u107_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod03:measurement_current_u107` from `mq_collector_Conditions_fst`.`fstMPOD03` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod03_u107_run23.txt

else
  echo "Wrong Input Parameter! Try: . ./getDataMpod03Iseg1Run23.sh"
fi
