import os
# -- setting by hand -- #
BasePath_Input = \
"/home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v11_ZZto4L_HighMass4l_LO"
BasePath_Output = \
"/home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width"
Exe_Delphes = "$KP_DELPHES_PATH/DelphesHepMC"
Card_Delphes = "/home/kplee/Physics/ZprimeTo4L/GENSIM/Delphes/v03_ZprimeTo4L_DiffSeed_Width/delphes_card_CMS_NoUniqueFinder.tcl"
###################################################################



List_MassBinName = [
"M500to1500",
"M1500to2500",
"M2500to3500",
"M3500toInf"
]

List_CMD_cp = []
List_CMD_gunzip = []
List_CMD_Delphes = []

i_sample=1
for MassBinName in List_MassBinName:
	Name_Base = "ZZto4L_%s" % MassBinName
	InputName = "%s/v%02d_%s/Local/MG5_Output/Events/run_01/tag_1_pythia8_events.hepmc.gz" % (BasePath_Input, i_sample, MassBinName)
	i_sample = i_sample+1;

	cmd_cp_to_data7 = "cp %s %s/%s.hepmc.gz" % (InputName, BasePath_Output, Name_Base)
	print cmd_cp_to_data7
	List_CMD_cp.append( cmd_cp_to_data7 )

	cmd_gunzip = "gunzip %s.hepmc.gz &" % (Name_Base)
	print cmd_gunzip
	List_CMD_gunzip.append( cmd_gunzip )

	cmd_delphes = "%s %s %s.root %s.hepmc &" % (Exe_Delphes, Card_Delphes, Name_Base, Name_Base)
	List_CMD_Delphes.append( cmd_delphes )

f = open("Run_Delphes.sh", "w")
f.write("#!bin/bash\n")

for CMD in List_CMD_cp:
	f.write( CMD+"\n")

f.write("\ncd %s\n\n" % BasePath_Output )

for CMD in List_CMD_gunzip:
	f.write( CMD+"\n")

f.write(
"""
WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

""")

for CMD in List_CMD_Delphes:
	f.write(CMD+"\n")

f.write(
"""
WORK_PID=`jobs -l | awk '{print $2}'` # -- WORK_PID: all PIDs in background -- #
wait $WORK_PID

""")

f.write( 'echo "all jobs are finished"\n\n' )

f.close()
