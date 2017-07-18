#!bin/bash

export KP_INPUT_PATH=/home/kplee/Physics/ZZTo4L/Madgraph/GEN_v01_ZZ4mu_10k/ZZTo4L01j_5f_NLO_FXFX/Events/run_02/input.hep

# -- DelphesHepMC card outputname inputname -- #
$KP_DELPHES_PATH/DelphesHepMC $KP_DELPHES_PATH/cards/delphes_card_CMS.tcl $KP_DELPHESDATA_PATH/ZZTo4mu_10k.root $KP_INPUT_PATH # -- error

echo "simulation is finished"
