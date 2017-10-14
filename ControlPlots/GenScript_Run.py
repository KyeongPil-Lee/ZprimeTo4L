import os, sys, time, json

JSONFileName = sys.argv[1]
print "[JSONFileName]", JSONFileName
with open(JSONFileName) as JobInfo_File:
	JobInfo = json.load(JobInfo_File)

CodeName = JobInfo["CodeName"]
print "CodeName: %s" % CodeName

TIME = time.strftime('%Y%m%d_%H%M%S', time.localtime(time.time()))

DataPathBase = os.environ['KP_DATA_PATH']

if "Local" not in os.listdir("."):
	os.mkdir( "Local" )

cwd = os.getcwd()

CodeType = CodeName.split(".cxx")[0]
MainDir = "Local/%s_v%s" % (CodeType, TIME)
os.mkdir( MainDir )

f = open("%s/Run.sh" % MainDir, "w")
f.write("#!bin/bash\n\n")

ScriptName = "Run_%s.sh" % CodeType # -- will be used later -- #

for SampleInfo in JobInfo["List_SampleInfo"]:
	SampleType = SampleInfo["Tag"]
	SamplePath = "%s/%s" % (DataPathBase, SampleInfo["SubPath"])
	Flag_IsSignal = SampleInfo["isSignal"]

	SubDirPath = "%s/%s" % (MainDir, SampleType)

	os.mkdir( SubDirPath )
	os.chdir( SubDirPath )

	cmd_cp = "cp %s ./" % (cwd+"/"+CodeName)
	os.system( cmd_cp )

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

	cmd_cd = "cd %s" % SampleType
	cmd_script = "source %s >&../log_%s&" % (ScriptName, SampleType)
	# cmd_cd_backToCWD = 'cd %s' % cwd
	cmd_cd_backToCWD = 'cd ..'

	f.write( "%s\n" % cmd_cd )
	f.write( "%s\n" % cmd_script )
	f.write( "%s\n\n" % cmd_cd_backToCWD )

	os.chdir( cwd )

f.write('echo "Submission is finished"\n\n')
f.close()

print "+" * 100
print "Run all jobs:"
print "cd %s; source Run.sh;" % MainDir
print "+" * 100
