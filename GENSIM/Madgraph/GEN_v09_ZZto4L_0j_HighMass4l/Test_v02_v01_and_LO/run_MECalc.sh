#!bin/bash

date

export KP_MADGRAPH_PATH=/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5

# -- cuts.f with mass cut -- #
cp cuts_DrCho.f $KP_MADGRAPH_PATH/Template/LO/SubProcesses/cuts.f

$KP_MADGRAPH_PATH/bin/mg5_aMC proc_card.dat

echo "matrix element calculation is finished: Local/MG5_Output directory is created with outputs"
echo "Next step: Local/MG5_Output/bin/aMCatNLO"

# -- change with the orignal one -- #
cp cuts_original_LO.f $KP_MADGRAPH_PATH/Template/LO/SubProcesses/cuts.f

date