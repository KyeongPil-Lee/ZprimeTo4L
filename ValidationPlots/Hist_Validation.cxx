#include "Hist_Validation.h"

void Hist_Validation()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
	TString InputFileName = DataPath+"/Delphes/v20170707_1st_DetSim_Delphes/evts_1gen_m1000_m10_13TeV_200k.root";
	Bool_t _IsSignal = kFALSE;

	InputFileName = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_200k.root"; Flag_IsSignal = kFALSE;

	Analyzer *analyzer = new Analyzer(InputFileName);
	analyzer->IsSignal( _IsSignal );
	analyzer->Analyze();
}