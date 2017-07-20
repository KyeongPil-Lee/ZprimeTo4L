import os
import sys

BasePath_HepMC = "/home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization"
BasePath_DelphesOutput = "/home/kplee/data7/Delphes/v20170720_1st_DetSim_Delphes_200k"
Exe_Delphes = "$KP_DELPHES_PATH/DelphesHepMC"
Card_Delphes = "$KP_DELPHES_PATH/cards/delphes_card_CMS.tcl"

Scenarios_Msn3 = {}
Scenarios_Msn3[200] = [1, 50]
Scenarios_Msn3[800] = [1, 50]
Scenarios_Msn3[1000] = [10, 50, 300]
Scenarios_Msn3[2000] = [1, 20, 50, 600]
Scenarios_Msn3[3000] = [30, 50, 900]
Scenarios_Msn3[4000] = [40, 50, 1200]

List_MZp = Scenarios_Msn3.keys()

List_CMD = []
for MZp in List_MZp:
	for Msn3 in Scenarios_Msn3[MZp]:
		Name_Base = "MZp_%s_Msn3_%s_200k" % (MZp, Msn3)

		Name_HepMC = "%s/%s.hepmc" % (BasePath_HepMC, Name_Base)
		Name_Output = "%s/%s.root" % (BasePath_DelphesOutput, Name_Base)

		CMD = "%s %s %s %s" % (Exe_Delphes, Card_Delphes, Name_Output, Name_HepMC)
		List_CMD.append( CMD )

# -- shell script -- #
ScriptName = "Run_DelphesSim_Signal.sh"
f = open("./"+ScriptName, "w")
f.write( "#!bin/bash\n" )
for CMD in List_CMD:
	f.write( CMD )
	f.write( "\n" )

f.write( 'echo "finished"' )
f.close()
print ScriptName+" is generated"