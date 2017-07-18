#!bin/bash

KP_INPUT_PATH=/home/kplee/Physics/ZZTo4L/Madgraph/GEN_v01_ZZ4mu_10k/ZZTo4L01j_5f_NLO_FXFX/Events/run_03/events.lhe

# -- DelphesHepMC card outputname inputname -- #
$KP_DELPHES_PATH/DelphesLHEF $KP_DELPHES_PATH/cards/delphes_card_CMS.tcl $KP_DELPHESDATA_PATH/ZZTo4mu_10k_BeforeHadronization.root $KP_INPUT_PATH # -- error

echo "simulation is finished"
