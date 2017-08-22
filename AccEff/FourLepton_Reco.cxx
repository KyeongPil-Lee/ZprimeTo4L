#include <AccEff/FourLepton_Reco.h>

void FourLepton_Reco( TString Path_Sample, TString Name_Sample, Bool_t Flag_IsSignal )
{
	Tool_AccEff *tool = new Tool_AccEff( Path_Sample, Name_Sample, Flag_IsSignal );
	tool->Analyze();
}