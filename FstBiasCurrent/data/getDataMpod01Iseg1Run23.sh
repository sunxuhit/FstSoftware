#!/bin/bash
date +%F

#. ./getDataMpod01Iseg1Run23.sh

if [ $# -eq 0 ]
then

  echo "retrive bias current of fstmpod01 u100-107 for run23"

  echo "retrive bias current of fstmpod01 u100 =====>"
  rm fstmpod01_u100_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u100` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u100_run23.txt

  echo "retrive bias current of fstmpod01 u101 =====>"
  rm fstmpod01_u101_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u101` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u101_run23.txt

  echo "retrive bias current of fstmpod01 u102 =====>"
  rm fstmpod01_u102_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u102` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u102_run23.txt

  echo "retrive bias current of fstmpod01 u103 =====>"
  rm fstmpod01_u103_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u103` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u103_run23.txt

  echo "retrive bias current of fstmpod01 u104 =====>"
  rm fstmpod01_u104_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u104` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u104_run23.txt

  echo "retrive bias current of fstmpod01 u105 =====>"
  rm fstmpod01_u105_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u105` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u105_run23.txt

  echo "retrive bias current of fstmpod01 u106 =====>"
  rm fstmpod01_u106_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u106` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u106_run23.txt

  echo "retrive bias current of fstmpod01 u107 =====>"
  rm fstmpod01_u107_run23.txt
  mysql -h dbbak.starp.bnl.gov --port=3422 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u107` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2023-04-01" AND "2023-08-31" ORDER BY beginTime ASC' > fstmpod01_u107_run23.txt

else
  echo "Wrong Input Parameter! Try: . ./getDataMpod01Iseg1Run23.sh"
fi
