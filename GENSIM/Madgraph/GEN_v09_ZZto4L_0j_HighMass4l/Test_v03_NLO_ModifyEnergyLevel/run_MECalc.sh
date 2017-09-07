#!bin/bash

date

export KP_MADGRAPH_PATH=/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5

# -- cuts.f with mass cut -- #
cp cuts_with4LMassCut.f $KP_MADGRAPH_PATH/Template/NLO/SubProcesses/cuts.f
cp setcuts_modify_taumin_s.f $KP_MADGRAPH_PATH/Template/NLO/SubProcesses/setcuts.f

$KP_MADGRAPH_PATH/bin/mg5_aMC proc_card.dat

echo "matrix element calculation is finished: Local/MG5_Output directory is created with outputs"
echo "Next step: Local/MG5_Output/bin/aMCatNLO"

# -- change with the orignal one -- #
cp cuts_original.f $KP_MADGRAPH_PATH/Template/NLO/SubProcesses/cuts.f
cp setcuts_original.f $KP_MADGRAPH_PATH/Template/NLO/SubProcesses/setcuts.f

date