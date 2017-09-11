#!bin/bash

cd /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width
$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_1M.root /home/kplee/data7/LHE/v20170726_1st_ZZto4L0j_AfterHad_1M/events_PYTHIA8_0.hepmc

echo "finished"