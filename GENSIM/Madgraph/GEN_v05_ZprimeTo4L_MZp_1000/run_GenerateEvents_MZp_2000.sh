#!bin/bash

date

# -- setup the cards -- #
# cp run_card.dat ./zprime_b3ltau_pp-4l/Cards/
# cp shower_card.dat ./zprime_b3ltau_pp-4l/Cards/

zprime_b3ltau_pp-4l/bin/madevent run_GenerateEvents_MZp_2000.dat

echo "Generation is finished"

date
