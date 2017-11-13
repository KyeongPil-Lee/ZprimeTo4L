#!bin/bash
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO/v01_M500to1500/Local/MG5_Output/Events/run_01/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/ZZto4L_M500to1500.hepmc.gz
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO/v02_M1500to2500/Local/MG5_Output/Events/run_01/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/ZZto4L_M1500to2500.hepmc.gz
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO/v03_M2500to3500/Local/MG5_Output/Events/run_01/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/ZZto4L_M2500to3500.hepmc.gz
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO/v04_M3500toInf/Local/MG5_Output/Events/run_01/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/ZZto4L_M3500toInf.hepmc.gz

cd /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width

gunzip ZZto4L_M500to1500.hepmc.gz &
gunzip ZZto4L_M1500to2500.hepmc.gz &
gunzip ZZto4L_M2500to3500.hepmc.gz &
gunzip ZZto4L_M3500toInf.hepmc.gz &

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_M500to1500.root ZZto4L_M500to1500.hepmc &
$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_M1500to2500.root ZZto4L_M1500to2500.hepmc &
$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_M2500to3500.root ZZto4L_M2500to3500.hepmc &
$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_M3500toInf.root ZZto4L_M3500toInf.hepmc &

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

echo "all jobs are finished"

