#include "Hist_Validation.h"

void Hist_Validation()
{
	TString BasePath = "/home/kplee/data1/LHE/v20170707_1st_ZprimeTo4L/v20170707_1st_DetSim_Delphes";
	TString FileName = BasePath+"/evts_1gen_m1000_m10_13TeV_200k.root";

	Analyzer *analyzer = new Analyzer(FileName);
	analyzer->Analyze();

}