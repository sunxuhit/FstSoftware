#!/bin/bash
date +%F

#. ./getDataMpod01Iseg1Run22.sh

if [ $# -eq 0 ]
then

  echo "retrive bias current of fstmpod01 u100-107 for run22"

  echo "retrive bias current of fstmpod01 u100 =====>"
  rm fstmpod01_u100_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u100` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u100_run22.txt

  echo "retrive bias current of fstmpod01 u101 =====>"
  rm fstmpod01_u101_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u101` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u101_run22.txt

  echo "retrive bias current of fstmpod01 u102 =====>"
  rm fstmpod01_u102_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u102` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u102_run22.txt

  echo "retrive bias current of fstmpod01 u103 =====>"
  rm fstmpod01_u103_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u103` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u103_run22.txt

  echo "retrive bias current of fstmpod01 u104 =====>"
  rm fstmpod01_u104_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u104` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u104_run22.txt

  echo "retrive bias current of fstmpod01 u105 =====>"
  rm fstmpod01_u105_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u105` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u105_run22.txt

  echo "retrive bias current of fstmpod01 u106 =====>"
  rm fstmpod01_u106_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u106` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u106_run22.txt

  echo "retrive bias current of fstmpod01 u107 =====>"
  rm fstmpod01_u107_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod01:measurement_current_u107` from `mq_collector_Conditions_fst`.`fstMPOD01` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod01_u107_run22.txt

else
  echo "Wrong Input Parameter! Try: . ./getDataMpod01Iseg1Run22.sh"
fi
