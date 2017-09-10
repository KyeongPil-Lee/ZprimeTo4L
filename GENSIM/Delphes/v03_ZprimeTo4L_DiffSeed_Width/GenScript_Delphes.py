import os
# -- setting by hand -- #
BasePath_Input = \
"/home/kplee/Physics/ZprimeTo4L/GENSIM/Madgraph/GEN_v10_ZprimeTo4L_DiffRSeed_Width/zprime_b3ltau_pp-4l/Events"
BasePath_Output = \
"/home/kplee/data7/Zprime4LSamples/v20170911_1st_DiffSeed_Width"
Exe_Delphes = "$KP_DELPHES_PATH/DelphesHepMC"
Card_Delphes = os.getcwd()+"/delphes_card_CMS_NoUniqueFinder.tcl"
###################################################################

Scenarios_Msn3 = {}
Scenarios_Msn3[200] = [1, 50]
Scenarios_Msn3[800] = [1, 50]
Scenarios_Msn3[1000] = [10, 50, 300]
Scenarios_Msn3[2000] = [1, 20, 50, 600]
Scenarios_Msn3[3000] = [30, 50, 900]
Scenarios_Msn3[4000] = [40, 50, 1200]

List_MZp = Scenarios_Msn3.keys()
List_MZp.sort()

print List_MZp

List_CMD_cp = []
List_CMD_gunzip = []
List_CMD_Delphes = []

i_sample=1
for MZp in List_MZp:
	for Msn3 in Scenarios_Msn3[MZp]:
		Name_Base = "MZp_%s_Msn3_%s_200k" % (MZp, Msn3)

		InputName = "%s/run_%02d/tag_1_pythia8_events.hepmc.gz" % (BasePath_Input, i_sample)
		i_sample = i_sample+1

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
