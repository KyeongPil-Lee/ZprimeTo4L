#include <AccEff/AccEff.h>

void AccEff( TString Path_Sample, TString Name_Sample, Bool_t Flag_IsSignal )
{
	Tool_AccEff *tool = new Tool_AccEff( Path_Sample, Name_Sample, Flag_IsSignal );
	tool->Analyze();
}