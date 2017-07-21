#include "Hist_Validation.h"

void Hist_Validation()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
	TString FileName = DataPath+"/Delphes/v20170707_1st_DetSim_Delphes/evts_1gen_m1000_m10_13TeV_200k.root";
	Bool_t _IsSignal = kFALSE;

	Analyzer *analyzer = new Analyzer(FileName);
	analyzer->IsSignal( _IsSignal );
	analyzer->Analyze();
}