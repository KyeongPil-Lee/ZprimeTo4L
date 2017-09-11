
def Write_CMD( f, MZp, Msn3 ):
	nEvent = 200000;
	RelWidth = 0.014 * 0.01 # -- 0.014% -- //
	AbsWidth_Zp = MZp * RelWidth
	AbsWidth_sn3 = Msn3 * RelWidth

	f.write(
"""
generate_events
	shower=PYTHIA8
	set MZp {_MZp}
	set WZp {_AbsWidth_Zp}
	set Msn3 {_Msn3}
	set Wsn3 {_AbsWidth_sn3}
	set nevents {_nEvent}

""".format( _MZp=MZp, _Msn3=Msn3, _AbsWidth_Zp=AbsWidth_Zp, _AbsWidth_sn3=AbsWidth_sn3, _nEvent=nEvent))

# -- main part -- #

Scenarios_Msn3 = {}
# Scenarios_Msn3[200] = [1, 50]
# Scenarios_Msn3[800] = [1, 50]
# Scenarios_Msn3[1000] = [10, 50, 300]
# Scenarios_Msn3[2000] = [1, 20, 50, 600]
# Scenarios_Msn3[3000] = [30, 50, 900]
# Scenarios_Msn3[4000] = [40, 50, 1200]
Scenarios_Msn3[3000] = [1]
Scenarios_Msn3[4000] = [1]

f = open("run_GenerateEvents.dat", "w")
f.write("set automatic_html_opening False\n")
f.write("set nb_core 10\n")

List_MZp = Scenarios_Msn3.keys()
List_MZp.sort()

for MZp in List_MZp:
	for Msn3 in Scenarios_Msn3[MZp]:
		Write_CMD( f, MZp, Msn3 )