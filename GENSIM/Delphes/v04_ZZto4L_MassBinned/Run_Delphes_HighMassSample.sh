#!bin/bash
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO/v05_M3500to5000/Local/MG5_Output/Events/run_02/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/ZZto4L_M3500to5000.hepmc.gz
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO/v06_M3500to4500/Local/MG5_Output/Events/run_02/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/ZZto4L_M3500to4500.hepmc.gz

cd /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width

gunzip ZZto4L_M3500to5000.hepmc.gz &
gunzip ZZto4L_M3500to4500.hepmc.gz &

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_M3500to5000.root ZZto4L_M3500to5000.hepmc &
$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl ZZto4L_M3500to4500.root ZZto4L_M3500to4500.hepmc &

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

echo "all jobs are finished"

