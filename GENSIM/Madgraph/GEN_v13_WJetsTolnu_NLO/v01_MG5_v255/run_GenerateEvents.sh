#!bin/bash

date

# -- setup the cards -- #
cp run_card.dat ./Local/MG5_Output/Cards/
cp shower_card.dat ./Local/MG5_Output/Cards/
cp madspin_card.dat ./Local/MG5_Output/Cards/

./Local/MG5_Output/bin/aMCatNLO run_GenerateEvents.dat

echo "Generation is finished"

date
