#!/bin/bash
date +%F

#. ./getDataMpod02Iseg2.sh

if [ $# -eq 0 ]
then

  echo "retrive bias current of fstmpod02 u200-207 for run22"

  echo "retrive bias current of fstmpod02 u200 =====>"
  rm fstmpod02_u200_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u200` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u200_run22.txt

  echo "retrive bias current of fstmpod02 u201 =====>"
  rm fstmpod02_u201_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u201` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u201_run22.txt

  echo "retrive bias current of fstmpod02 u202 =====>"
  rm fstmpod02_u202_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u202` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u202_run22.txt

  echo "retrive bias current of fstmpod02 u203 =====>"
  rm fstmpod02_u203_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u203` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u203_run22.txt

  echo "retrive bias current of fstmpod02 u204 =====>"
  rm fstmpod02_u204_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u204` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u204_run22.txt

  echo "retrive bias current of fstmpod02 u205 =====>"
  rm fstmpod02_u205_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u205` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u205_run22.txt

  echo "retrive bias current of fstmpod02 u206 =====>"
  rm fstmpod02_u206_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u206` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u206_run22.txt

  echo "retrive bias current of fstmpod02 u207 =====>"
  rm fstmpod02_u207_run22.txt
  mysql -h db04.star.bnl.gov --port=3421 -s -e 'SELECT beginTime, UNIX_TIMESTAMP(beginTime), `fstmpod02:measurement_current_u207` from `mq_collector_Conditions_fst`.`fstMPOD02` WHERE beginTime BETWEEN "2021-11-01" AND "2022-04-30" ORDER BY beginTime ASC' > fstmpod02_u207_run22.txt

else
  echo "Wrong Input Parameter! Try: . ./getDataMpod02Iseg2.sh"
fi
