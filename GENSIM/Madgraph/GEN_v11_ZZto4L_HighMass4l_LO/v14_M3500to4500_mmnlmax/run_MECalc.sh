#!bin/bash

date

export KP_MADGRAPH_PATH=/home/kplee/Physics/Madgraph/MG5_aMC_v2_6_0

$KP_MADGRAPH_PATH/bin/mg5_aMC proc_card.dat

echo "matrix element calculation is finished: Local/MG5_Output directory is created with outputs"
#echo "Next step: Local/MG5_Output/bin/aMCatNLO"

date