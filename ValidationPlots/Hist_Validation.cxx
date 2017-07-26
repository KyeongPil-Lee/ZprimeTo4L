#include "Hist_Validation.h"

void Hist_Validation()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
	TString InputFileName = "";
	Bool_t Flag_IsSignal = kFALSE;

	InputFileName = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_1M.root"; Flag_IsSignal = kFALSE;

	Analyzer *analyzer = new Analyzer(InputFileName);
	analyzer->IsSignal( _IsSignal );
	analyzer->Analyze();
}