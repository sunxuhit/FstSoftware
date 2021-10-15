#!/bin/bash
date

#. ./scan.sh position.txt

if [ $# -eq 1 ]
then
  # while read pos
  # do
  #   echo "read in position ($pos) mm"
  # done < $1

  while IFS=' ' read -r posx posy posz
  do
    echo "x position is $posx mm"
    echo "y position is $posy mm"
    echo "z position is $posz mm"
    echo
  done < $1

else
  echo "Wrong number of parameters"
fi

