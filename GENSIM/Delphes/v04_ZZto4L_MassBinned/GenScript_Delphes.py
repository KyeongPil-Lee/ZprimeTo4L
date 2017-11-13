import os, sys, json

JSONFileName = sys.argv[1]
print "[JSONFileName]", JSONFileName
with open(JSONFileName) as DelphesInfo_File:
	DelphesInfo = json.load(DelphesInfo_File)

List_CMD_cp = []
List_CMD_gunzip = []
List_CMD_Delphes = []

for sampleInfo in DelphesInfo["SampleInfo"]:
	name = sampleInfo["Name"]
	path = sampleInfo["Path"]
	fullPath = "%s/%s" % (DelphesInfo["InputPathBase"], path)

	cmd_cpToDisk = "cp %s %s/%s.hepmc.gz" % (fullPath, DelphesInfo["OutputPath"], name)
	List_CMD_cp.append( cmd_cpToDisk )

	cmd_gunzip = "gunzip %s.hepmc.gz &" % (name)
	List_CMD_gunzip.append( cmd_gunzip )

	cmd_delphes = "%s %s %s.root %s.hepmc &" % (DelphesInfo["DelphesExe"], DelphesInfo["DelphesCard"], name, name)
	List_CMD_Delphes.append( cmd_delphes )

# -- generate script -- #
f = open("Run_Delphes.sh", "w")
f.write("#!bin/bash\n")

for CMD in List_CMD_cp:
	f.write( CMD+"\n")

f.write("\ncd %s\n\n" % DelphesInfo["OutputPath"] )

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

print "+" * 100
print "[Run_Delphes.sh] is generated"
print "source Run_Delphes.sh >&log_Run_Delphes.log&"
print "+" * 100
