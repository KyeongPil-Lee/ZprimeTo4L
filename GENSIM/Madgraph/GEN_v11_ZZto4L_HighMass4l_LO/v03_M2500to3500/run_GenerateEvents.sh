#!bin/bash

date

# -- setup the cards -- #
cp run_card.dat ./Local/MG5_Output/
#cp shower_card.dat ./Local/MG5_Output/

Local/MG5_Output/bin/madevent run_GenerateEvents.dat

echo "Generation is finished"

date
