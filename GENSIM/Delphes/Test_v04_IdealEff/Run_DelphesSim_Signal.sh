#!bin/bash
$KP_DELPHES_PATH/DelphesHepMC delphes_card_CMS_IdealEff.tcl /home/kplee/data7/Delphes/v20170807_Test_2nd_IdealEff/MZp_4000_Msn3_1_200k.root /home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization/MZp_4000_Msn3_1_200k.hepmc >&log1&
$KP_DELPHES_PATH/DelphesHepMC delphes_card_CMS_IdealEff.tcl /home/kplee/data7/Delphes/v20170807_Test_2nd_IdealEff/MZp_4000_Msn3_40_200k.root /home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization/MZp_4000_Msn3_40_200k.hepmc >&log2&
$KP_DELPHES_PATH/DelphesHepMC delphes_card_CMS_IdealEff.tcl /home/kplee/data7/Delphes/v20170807_Test_2nd_IdealEff/MZp_4000_Msn3_50_200k.root /home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization/MZp_4000_Msn3_50_200k.hepmc >&log3&
$KP_DELPHES_PATH/DelphesHepMC delphes_card_CMS_IdealEff.tcl /home/kplee/data7/Delphes/v20170807_Test_2nd_IdealEff/MZp_4000_Msn3_1200_200k.root /home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization/MZp_4000_Msn3_1200_200k.hepmc >&log4&

echo "finished"