#!bin/bash

export ROOTSYS="/programs/root"
source $ROOTSYS/bin/thisroot.sh

root -l -b << EOF

TString DelphesPath = gSystem->Getenv("KP_DELPHES_PATH")
gSystem->Load(DelphesPath+"/libDelphes.so")
.x Hist_Validation.cxx++

.q

EOF

export ROOTSYS="/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed6/"
source $ROOTSYS/bin/thisroot.sh

echo "finished"
