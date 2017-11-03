#!bin/bash

date

export KP_MADGRAPH_PATH=/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5

$KP_MADGRAPH_PATH/bin/mg5_aMC proc_card.dat

echo "MECalc: finished"

date