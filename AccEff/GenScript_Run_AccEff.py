import os

CodeName = "AccEff.cxx"

BasePath = os.environ['KP_DATA_PATH']
List_SamplePath = [
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_1M.root",

DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_200_Msn3_1_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_200_Msn3_50_200k.root",

DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_800_Msn3_1_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_800_Msn3_50_200k.root",

DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_10_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_50_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_300_200k.root",

DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_1_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_20_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_50_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_600_200k.root",


DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_1_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_30_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_50_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_900_200k.root",

DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_1_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_40_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_50_200k.root",
DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_1200_200k.root"
]

if "Local" not in os.listdir("."):
	os.mkdir( "Local" )

cwd = os.getcwd()

f = open("Run_AccEff.sh", "w")
for SamplePath in List_SamplePath:
	SampleName = SamplePath.split("/")[-1].split(".root")[0]
	SubDirPath = "Local/%s" % SampleName
	Flag_IsSignal = 1
	if "ZZto4L" in SampleName: Flag_IsSignal = 0

	cmd_mkdir = "mkdir %s" % SubDirPath
	cmd_cp = "cp %s %s" % (CodeName, SubDirPath)
	cmd_cd = "cd %s" % (SubDirPath)
	cmd_code = 'root -b -q %s("%s", "%s", %d)++ >&log&' % (CodeName, SamplePath, Flag_IsSignal)
	cmd_cp2 = 'cd %s' % cwd

	f.write( "%s\n" % cmd_mkdir )
	f.write( "%s\n" % cmd_cp )
	f.write( "%s\n" % cmd_cd )
	f.write( "%s\n\n" % cmd_cp2 )

f.write('echo "Submission is finished\n\n"')