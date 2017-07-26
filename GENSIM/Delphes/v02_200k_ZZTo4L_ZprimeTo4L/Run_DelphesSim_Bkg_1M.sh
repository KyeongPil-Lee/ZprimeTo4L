#!bin/bash

$KP_DELPHES_PATH/DelphesHepMC $KP_DELPHES_PATH/cards/delphes_card_CMS.tcl /home/kplee/data7/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_1M.root /home/kplee/data7/LHE/v20170726_1st_ZZto4L0j_AfterHad_1M/events_PYTHIA8_0.hepmc

echo "simulation is finished"
