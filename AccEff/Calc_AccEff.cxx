#include "Calc_AccEff.h"

void Calc_AccEff()
{
	if( gSystem->mkdir( "./Local" ) == 0 ) printf("Directory [Local] is created\n");

	TString version = "AccEff_v20170820_232127";

	Tool_CalcAccEff *tool = new Tool_CalcAccEff( version );
	tool->Draw_All();
}