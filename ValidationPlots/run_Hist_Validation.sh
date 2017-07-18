#!bin/bash

root -l -b << EOF

TString DelphesPath = gSystem->Getenv("KP_DELPHES_PATH")
gSystem->Load(DelphesPath+"/libDelphes.so")
.x Hist_Validation.cxx++

.q

EOF

echo "finished"
