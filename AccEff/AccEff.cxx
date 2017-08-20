void AccEff( TString Path_Sample, TString Name_Sample, Bool_t Flag_IsSignal )
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	Tool_AccEff *tool = new AccEff( Path_Sample, Name_Sample, Flag_IsSignal );
	tool->Analyze();
}