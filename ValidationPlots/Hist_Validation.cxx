#include "Hist_Validation.h"

void Hist_Validation()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
<<<<<<< HEAD

	// TString InputFileName_Bkg = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_200k.root";
	// Analyzer *analyzer_bkg = new Analyzer(InputFileName_Bkg);
	// analyzer_bkg->IsSignal( kFALSE );
	// analyzer_bkg->Analyze();
=======
	TString InputFileName = "";
	Bool_t Flag_IsSignal = kFALSE;

	InputFileName = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_1M.root"; Flag_IsSignal = kFALSE;
>>>>>>> 50fd7a196ca06c0ed9715a70dcbae0513d03007a

	vector< TString > vec_InputFileName_Signal = 
	{
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_200_Msn3_1_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_200_Msn3_50_200k.root",

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_800_Msn3_1_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_800_Msn3_50_200k.root",

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_10_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_300_200k.root",

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_1_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_20_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_2000_Msn3_600_200k.root",

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_30_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_900_200k.root",

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_40_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_1200_200k.root"
	};

	for( const auto& InputFileName_Signal : vec_InputFileName_Signal )
	{
		cout << "InputFileName_Signal: " << InputFileName_Signal << endl;
		Analyzer *analyzer_sig = new Analyzer(InputFileName_Signal);
		analyzer_sig->IsSignal( kTRUE );
		analyzer_sig->Analyze();
	}
}