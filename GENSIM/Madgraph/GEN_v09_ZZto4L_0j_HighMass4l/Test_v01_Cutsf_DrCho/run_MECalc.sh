#!bin/bash

date

export KP_MADGRAPH_PATH=/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5

# -- cuts.f with mass cut -- #
cp cuts_DrCho.f $KP_MADGRAPH_PATH/Template/NLO/SubProcesses/cuts.f

$KP_MADGRAPH_PATH/bin/mg5_aMC proc_card.dat

echo "matrix element calculation is finished: ZZTo4L0j_5f_NLO_FXFX directory is created with outputs"
echo "Next step: ZZTo4L0j_5f_NLO_FXFX/bin/aMCatNLO"

# -- change with the orignal one -- #
cp cuts_original.f $KP_MADGRAPH_PATH/Template/NLO/SubProcesses/cuts.f

date