#include "Hist_Validation_General.h"

void Hist_Validation_4e()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");

	TString InputFileName_Bkg = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_1M.root";
	Analyzer *analyzer_bkg = new Analyzer(InputFileName_Bkg);
	analyzer_bkg->IsSignal( kFALSE );
	analyzer_bkg->Set_ChannelType( "4e" );
	analyzer_bkg->Analyze();

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

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_1_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_30_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_3000_Msn3_900_200k.root",

		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_1_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_40_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_4000_Msn3_1200_200k.root"
	};

	for( const auto& InputFileName_Signal : vec_InputFileName_Signal )
	{
		cout << "InputFileName_Signal: " << InputFileName_Signal << endl;
		Analyzer *analyzer_sig = new Analyzer(InputFileName_Signal);
		analyzer_sig->IsSignal( kTRUE );
		analyzer_sig->Set_ChannelType( "4e" );
		analyzer_sig->Analyze();
		cout << "Running over " << InputFileName_Signal << "is finished" << endl;
	}
}