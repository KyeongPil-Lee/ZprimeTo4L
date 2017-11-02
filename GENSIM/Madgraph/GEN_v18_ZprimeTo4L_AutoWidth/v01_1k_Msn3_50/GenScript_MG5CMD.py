
def Write_CMD( f, nEvent, MZp, Msn3 ):
	f.write(
"""
generate_events
	shower=PYTHIA8
	set MZp {_MZp}
	set WZp Auto
	set Msn3 {_Msn3}
	set Wsn3 Auto
	set nevents {_nEvent}
	set dynamical_scale_choice 4
	set cut_decays True
	set pta 0
	set ptj 20
	set ptl 15
	set etal 2.5
	set etaa 0
	set etaj 5.
	set draa 0
	set draj 0
	set dral 0
	set drll 0

""".format( _MZp=MZp, _Msn3=Msn3, _nEvent=nEvent) )

# -- main part -- #
nGenEvent = 1000;

Scenarios_Msn3 = {}
# Scenarios_Msn3[200] = [1, 50]
# Scenarios_Msn3[800] = [1, 50]
# Scenarios_Msn3[1000] = [1, 10, 50, 300]
# Scenarios_Msn3[2000] = [1, 20, 50, 600]
# Scenarios_Msn3[3000] = [1, 30, 50, 900]
# Scenarios_Msn3[4000] = [1, 40, 50, 1200]
Scenarios_Msn3[200] = [50]
Scenarios_Msn3[800] = [50]
Scenarios_Msn3[1000] = [50]
Scenarios_Msn3[2000] = [50]
Scenarios_Msn3[3000] = [50]
Scenarios_Msn3[4000] = [50]

f = open("run_GenerateEvents.dat", "w")
f.write("set automatic_html_opening False\n")
f.write("set nb_core 30\n")

List_MZp = Scenarios_Msn3.keys()
List_MZp.sort()


for MZp in List_MZp:
	for Msn3 in Scenarios_Msn3[MZp]:
		Write_CMD( f, nGenEvent, MZp, Msn3 )