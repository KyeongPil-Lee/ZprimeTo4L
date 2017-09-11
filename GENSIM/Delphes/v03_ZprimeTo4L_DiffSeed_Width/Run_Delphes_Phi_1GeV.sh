#!bin/bash
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v10_ZprimeTo4L_DiffRSeed_Width/zprime_b3ltau_pp-4l/Events/run_18/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/MZp_3000_Msn3_1_200k.hepmc.gz
cp /home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v10_ZprimeTo4L_DiffRSeed_Width/zprime_b3ltau_pp-4l/Events/run_19/tag_1_pythia8_events.hepmc.gz /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width/MZp_4000_Msn3_1_200k.hepmc.gz

cd /home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width

gunzip MZp_3000_Msn3_1_200k.hepmc.gz &
gunzip MZp_4000_Msn3_1_200k.hepmc.gz &

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl MZp_3000_Msn3_1_200k.root MZp_3000_Msn3_1_200k.hepmc &
$KP_DELPHES_PATH/DelphesHepMC /home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl MZp_4000_Msn3_1_200k.root MZp_4000_Msn3_1_200k.hepmc &

WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

echo "all jobs are finished"

