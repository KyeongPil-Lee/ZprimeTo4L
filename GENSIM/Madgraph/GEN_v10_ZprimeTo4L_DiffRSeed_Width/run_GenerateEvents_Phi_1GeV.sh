#!bin/bash

date

# -- setup the cards -- #
cp run_card.dat ./zprime_b3ltau_pp-4l/Cards/
# cp shower_card.dat ./zprime_b3ltau_pp-4l/Cards/

zprime_b3ltau_pp-4l/bin/madevent run_GenerateEvents_Phi_1GeV.dat

echo "Generation is finished"

date
