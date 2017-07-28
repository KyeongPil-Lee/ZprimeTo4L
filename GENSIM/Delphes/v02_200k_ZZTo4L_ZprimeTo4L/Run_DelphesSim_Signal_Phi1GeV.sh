#!bin/bash
$KP_DELPHES_PATH/DelphesHepMC $KP_DELPHES_PATH/cards/delphes_card_CMS.tcl /home/kplee/data7/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_1_200k.root /home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization/MZp_3000_Msn3_1_200k.hepmc
$KP_DELPHES_PATH/DelphesHepMC $KP_DELPHES_PATH/cards/delphes_card_CMS.tcl /home/kplee/data7/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_1_200k.root /home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization/MZp_4000_Msn3_1_200k.hepmc
echo "finished"