#include "Hist_Validation_Simple.h"

void Hist_Validation_Simple()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
	TString FileName_sig = DataPath+"/Delphes/v20170707_1st_DetSim_Delphes/evts_1gen_m1000_m10_13TeV_200k.root";
	TString FileName_bkg = DataPath+"/Delphes/"

	vector<TString> vec_FileName = {FileName_sig, FileName_bkg};

	for( const auto& FileName : vec_FileName )
	{
		Analyzer *analyzer = new Analyzer(FileName);
		analyzer->Analyze();
	}
}