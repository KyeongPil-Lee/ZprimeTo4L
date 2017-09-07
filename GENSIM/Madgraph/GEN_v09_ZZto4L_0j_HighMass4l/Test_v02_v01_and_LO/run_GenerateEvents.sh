#!bin/bash

date

# -- setup the cards -- #
#cp run_card.dat ./ZZTo4L0j_5f_NLO_FXFX/Cards/
#cp shower_card.dat ./ZZTo4L0j_5f_NLO_FXFX/Cards/

Local/MG5_Output/bin/madevent run_GenerateEvents.dat

echo "Generation is finished"

date
