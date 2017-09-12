import os
import sys
import time

CodeName = sys.argv[1]
print "CodeName: %s" % CodeName

TIME = time.strftime('%Y%m%d_%H%M%S', time.localtime(time.time()))

DataPath = os.environ['KP_DATA_PATH']
BasePath = DataPath + "/Zprime4LSamples/v20170911_1st_DiffSeed_Width"

List_SamplePath = [
# BasePath+"/ZZto4L_1M.root",

# BasePath+"/MZp_200_Msn3_1_200k.root",
# BasePath+"/MZp_200_Msn3_50_200k.root",

# BasePath+"/MZp_800_Msn3_1_200k.root",
# BasePath+"/MZp_800_Msn3_50_200k.root",

# BasePath+"/MZp_1000_Msn3_10_200k.root",
# BasePath+"/MZp_1000_Msn3_50_200k.root",
# BasePath+"/MZp_1000_Msn3_300_200k.root",

# BasePath+"/MZp_2000_Msn3_1_200k.root",
# BasePath+"/MZp_2000_Msn3_20_200k.root",
# BasePath+"/MZp_2000_Msn3_50_200k.root",
# BasePath+"/MZp_2000_Msn3_600_200k.root",

BasePath+"/MZp_3000_Msn3_1_200k.root",
# BasePath+"/MZp_3000_Msn3_30_200k.root",
# BasePath+"/MZp_3000_Msn3_50_200k.root",
# BasePath+"/MZp_3000_Msn3_900_200k.root",

BasePath+"/MZp_4000_Msn3_1_200k.root",
# BasePath+"/MZp_4000_Msn3_40_200k.root",
# BasePath+"/MZp_4000_Msn3_50_200k.root",
# BasePath+"/MZp_4000_Msn3_1200_200k.root"
]

if "Local" not in os.listdir("."):
	os.mkdir( "Local" )

cwd = os.getcwd()

f = open("Run.sh", "w")
f.write("#!bin/bash\n\n")

CodeType = CodeName.split(".cxx")[0]
MainDir = "Local/%s_v%s" % (CodeType, TIME)
os.mkdir( MainDir )

ScriptName = "Run_%s.sh" % CodeType # -- will be used later -- #

for SamplePath in List_SamplePath:
	SampleType = SamplePath.split("/")[-1].split(".root")[0]
	SubDirPath = "%s/%s" % (MainDir, SampleType)
	os.mkdir( SubDirPath )
	os.chdir( SubDirPath )

	cmd_cp = "cp %s ./" % (cwd+"/"+CodeName)
	os.system( cmd_cp )

	Flag_IsSignal = 1
	if "ZZto4L" in SampleType: Flag_IsSignal = 0
	cmd_ROOT = '.x %s++("%s", "%s", %d)' % (CodeName, SamplePath, SampleType, Flag_IsSignal)

	f_temp = open(ScriptName, "w");
	f_temp.write("""
#!bin/bash

root -l -b << EOF

TString DelphesPath = gSystem->Getenv("KP_DELPHES_PATH")
gSystem->Load(DelphesPath+"/libDelphes.so")
%s

.q

EOF

echo "finished"
	""" % cmd_ROOT)
	f_temp.close()

	cmd_cd = "cd %s" % SubDirPath
	cmd_script = "source %s >&../log_%s&" % (ScriptName, SampleType)
	cmd_cd_backToCWD = 'cd %s' % cwd

	f.write( "%s\n" % cmd_cd )
	f.write( "%s\n" % cmd_script )
	f.write( "%s\n\n" % cmd_cd_backToCWD )

	os.chdir( cwd )

f.write('echo "Submission is finished"\n\n')
f.close()

print "Working directory is made: %s" % MainDir
