#!bin/bash

date

export KP_MADGRAPH_PATH=/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5

$KP_MADGRAPH_PATH/bin/mg5_aMC proc_card.dat

echo "matrix element calculation is finished: ZZTo4L0j_5f_NLO_FXFX directory is created with outputs"
echo "Next step: ZZTo4L0j_5f_NLO_FXFX/bin/aMCatNLO"

date