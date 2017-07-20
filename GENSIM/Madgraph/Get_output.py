import os
import sys

BasePath_Output = "/home/kplee/data7/LHE/v20170719_1st_ZprimeTo4L_AfterHadronization"

DirNames = {}
DirNames[200] = "GEN_v07_ZprimeTo4L_MZp_200_800"
DirNames[800] = "GEN_v07_ZprimeTo4L_MZp_200_800"
DirNames[1000] = "GEN_v05_ZprimeTo4L_MZp_1000"
DirNames[2000] = "GEN_v06_ZprimeTo4L_MZp_2000"
DirNames[3000] = "GEN_v08_ZprimeTo4L_MZp_3000_4000"
DirNames[4000] = "GEN_v08_ZprimeTo4L_MZp_3000_4000"

Scenarios_Msn3 = {}
Scenarios_Msn3[200] = [1, 50]
Scenarios_Msn3[800] = [1, 50]
Scenarios_Msn3[1000] = [10, 50, 300]
Scenarios_Msn3[2000] = [1, 20, 50, 600]
Scenarios_Msn3[3000] = [30, 50, 900]
Scenarios_Msn3[4000] = [40, 50, 1200]

List_MZp = [200, 800, 1000, 2000, 3000, 4000]
List_OutputNames = {}
for MZp in List_MZp:
	List_OutputNames[ DirNames[MZp] ] = [] # -- initialization -- #

for MZp in List_MZp:
	for Msn3 in Scenarios_Msn3[MZp]:
		OutputName = "MZp_%s_Msn3_%s_200k.hepmc.gz" % (MZp, Msn3)
		List_OutputNames[ DirNames[MZp] ].append( OutputName )

List_Path_Output = []
List_CMD = []
for DirName in List_OutputNames.keys():
	print "DirName: ", DirName
	i_run = 1;

	for OutputName in List_OutputNames[DirName]:
		print "\t", OutputName

		DirPath_Input = "%s/zprime_b3ltau_pp-4l/Events/run_%02d" % (DirName, i_run)
		List_File = os.listdir( DirPath_Input )
		InputName = ""
		for File in List_File:
			if ".hepmc" in File:
				if InputName == "":
					InputName = File
				else:
					print "More than 2 .hepmc file! ... %s and %s" % (InputName, File)
					sys.exit()

		Path_Input = "%s/%s" % (DirPath_Input, InputName)
		i_run = i_run+1
		print "\t\t", Path_Input

		Path_Output = "%s/%s" % (BasePath_Output, OutputName)
		List_Path_Output.append( Path_Output )

		print "\t\t", Path_Output

		CMD = "cp %s %s" % (Path_Input, Path_Output);
		# print "\t\t", CMD
		List_CMD.append( CMD )

# -- shell script -- #
f = open("./Move_Output.sh", "w")
f.write( "#!bin/bash\n" )
for CMD in List_CMD:
	f.write( CMD )
	f.write( "\n" )
f.write("\n")

for Path_Output in List_Path_Output:
	f.write( "gunzip %s" % Path_Output )
	f.write( "\n" )
f.write("\n")

f.write( 'echo "finished"' )
f.close()
print "Move_Output.sh is generated"
