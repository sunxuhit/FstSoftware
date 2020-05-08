#!/bin/bash
date

#. ./copyRCF.sh pdf

if [ $# -eq 1 ]
then
  pdf=$1

  echo copy $pdf HV70V to RCF
  scp ./figures/HV70V_Th4o5Tb3/${pdf}_HV70V_Th4o5Tb3.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV70V_Th4o5Tb3
  scp ./figures/HV70V_Th4o5Tb2/${pdf}_HV70V_Th4o5Tb2.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV70V_Th4o5Tb2
  scp ./figures/HV70V_Th4o5Tb1/${pdf}_HV70V_Th4o5Tb1.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV70V_Th4o5Tb1
  scp ./figures/HV70V_Th3Tb3/${pdf}_HV70V_Th3Tb3.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV70V_Th3Tb3
  scp ./figures/HV70V_Th3Tb2/${pdf}_HV70V_Th3Tb2.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV70V_Th3Tb2
  scp ./figures/HV70V_Th3Tb1/${pdf}_HV70V_Th3Tb1.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV70V_Th3Tb1

  echo copy $pdf HV140V to RCF
  scp ./figures/HV140V_Th4o5Tb3/${pdf}_HV140V_Th4o5Tb3.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV140V_Th4o5Tb3
  scp ./figures/HV140V_Th4o5Tb2/${pdf}_HV140V_Th4o5Tb2.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV140V_Th4o5Tb2
  scp ./figures/HV140V_Th4o5Tb1/${pdf}_HV140V_Th4o5Tb1.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV140V_Th4o5Tb1
  scp ./figures/HV140V_Th3Tb3/${pdf}_HV140V_Th3Tb3.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV140V_Th3Tb3
  scp ./figures/HV140V_Th3Tb2/${pdf}_HV140V_Th3Tb2.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV140V_Th3Tb2
  scp ./figures/HV140V_Th3Tb1/${pdf}_HV140V_Th3Tb1.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV140V_Th3Tb1

  echo copy $pdf HV200V to RCF
  scp ./figures/HV200V_Th4o5Tb3/${pdf}_HV200V_Th4o5Tb3.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV200V_Th4o5Tb3
  scp ./figures/HV200V_Th4o5Tb2/${pdf}_HV200V_Th4o5Tb2.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV200V_Th4o5Tb2
  scp ./figures/HV200V_Th4o5Tb1/${pdf}_HV200V_Th4o5Tb1.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV200V_Th4o5Tb1
  scp ./figures/HV200V_Th3Tb3/${pdf}_HV200V_Th3Tb3.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV200V_Th3Tb3
  scp ./figures/HV200V_Th3Tb2/${pdf}_HV200V_Th3Tb2.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV200V_Th3Tb2
  scp ./figures/HV200V_Th3Tb1/${pdf}_HV200V_Th3Tb1.pdf rcf_star:/star/u/sunxuhit/WWW/FstCosmicTestQA/HV200V_Th3Tb1

fi
