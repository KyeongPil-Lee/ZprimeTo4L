#!bin/bash
if [ $KP_ANALYZER_PATH ]; then
    echo "KP_ANALYZER_PATH is already defined: use a clean shell!"
    return 1
fi

# -- analyzer path (do not insert "/" in the end of the path)-- #
export KP_ANALYZER_PATH=$(pwd)

# -- cms1.snu.ac.kr -- #
export SCRAM_ARCH=slc6_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
cd ~/CMSSW_8_0_25
eval `scramv1 runtime -sh`
cd $KP_ANALYZER_PATH

export KP_MADGRAPH_PATH="/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5"
export KP_DELPHES_PATH="/home/kplee/Physics/Delphes/Delphes-3.4.1"
export KP_DELPHESDATA_PATH="/data7/Users/kplee/Delphes"
export KP_DATA_PATH="/data7/Users/kplee"
export PYTHIA8DATA="/home/kplee/Physics/Madgraph/MG5_aMC_v2_5_5/HEPTools/pythia8/share/Pythia8/xmldoc"

echo "Setup for Z'->4l analysis is finished"
