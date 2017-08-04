#include "../Hist_Validation_General.h"

void Hist_Validation_General( TString ChannelType )
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");

	vector< TString > vec_InputFileName_Signal = 
	{
		DataPath+"/Delphes/v20170804Test_1st_SameElecEff_Barrel_Endcap/MZp_4000_Msn3_1_200k.root",
		DataPath+"/Delphes/v20170804Test_1st_SameElecEff_Barrel_Endcap/MZp_4000_Msn3_40_200k.root",
		DataPath+"/Delphes/v20170804Test_1st_SameElecEff_Barrel_Endcap/MZp_4000_Msn3_50_200k.root",
		DataPath+"/Delphes/v20170804Test_1st_SameElecEff_Barrel_Endcap/MZp_4000_Msn3_1200_200k.root"
	};

	for( const auto& InputFileName_Signal : vec_InputFileName_Signal )
	{
		cout << "InputFileName_Signal: " << InputFileName_Signal << endl;
		Analyzer *analyzer_sig = new Analyzer(InputFileName_Signal);
		analyzer_sig->IsSignal( kTRUE );
		analyzer_sig->Set_ChannelType( ChannelType );
		analyzer_sig->Analyze();
	}
}

void Hist_Validation_General()
{
	Hist_Validation_General( "4e" );
}