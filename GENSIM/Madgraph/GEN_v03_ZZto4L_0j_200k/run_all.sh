#!bin/bash

mkdir Local

source run_MECalc.sh >&Local/log_MECalc

source run_GenerateEvents.sh >&Local/log_GenEvent

echo "finished"