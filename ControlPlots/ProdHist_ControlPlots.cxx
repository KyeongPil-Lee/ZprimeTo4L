#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>
#include <TClonesArray.h>
#include <TSystem.h>

// -- Delphes -- //
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"

#include <Include/HistTools.h>

class Tool_ControlPlots
{
public:
	TString Path_Sample;
	TString Name_Sample;
	Bool_t Flag_IsSignal;

	Tool_ControlPlots( TString _Path_Sample, TString _Name_Sample, Bool_t _Flag_IsSignal )
	{
		this->Path_Sample = _Path_Sample;
		this->Name_Sample = _Name_Sample;
		this->Flag_IsSignal = _Flag_IsSignal;
	}

	void Analyze()
	{
		vector< TString > vec_ChannelType = {"4e", "3e1m", "2e2m", "1e3m", "4m"};
		vector< HistContainer_4GENLepton* > vec_GENHist_4LepHS;
		vector< HistContainer_4GENLepton* > vec_GENHist_4Lep;
		vector< HistContainer_4Lepton* > vec_RECOHist_4Lep;
		for( unsigned int i=0; i<vec_ChannelType.size(); i++ )
		{
			TString ChannelType = vec_ChannelType[i];

			HistContainer_4GENLepton* GENHistHS = new HistContainer_4GENLepton( "HS_"+ChannelType );
			vec_GENHist_4LepHS.push_back( GENHistHS );

			HistContainer_4GENLepton* GENHist = new HistContainer_4GENLepton( ChannelType );
			vec_GENHist_4Lep.push_back( GENHist );

			HistContainer_4Lepton* RECOHist = new HistContainer_4Lepton( ChannelType );
			vec_RECOHist_4Lep.push_back( RECOHist );
		}

		TChain* chain = new TChain("Delphes");
		chain->Add(this->Path_Sample);

		// Create object of class ExRootTreeReader
		ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
		Int_t nTotEvent = treeReader->GetEntries();
		cout << "nTotEvent: " << nTotEvent << endl;

		// Get pointers to branches used in this analysis
		TClonesArray *Br_Muon = treeReader->UseBranch("Muon");
		TClonesArray *Br_Electron = treeReader->UseBranch("Electron");
		TClonesArray *Br_GenParticle = treeReader->UseBranch("Particle");

		// nTotEvent = 10000;
		for(Int_t i_ev = 0; i_ev < nTotEvent; i_ev++)
		{
			treeReader->ReadEntry(i_ev);

			///////////////////////////
			// -- generator-level -- //
			///////////////////////////
			TString TStr_Channal = "";
			vector< GenParticle* > vec_GENLepton_HardProcess;
			if( this->Flag_IsSignal )
				TStr_Channal = ChannelType_Signal( Br_GenParticle, vec_GENLepton_HardProcess );
			else
				TStr_Channal = ChannelType_Bkg( Br_GenParticle, vec_GENLepton_HardProcess );

			vector< GenParticle* > vec_GENLepton_FinalState;
			Find_FinalStateLepton_All( vec_GENLepton_HardProcess, Br_GenParticle, vec_GENLepton_FinalState );

			My4GenPair *FourGenPair_HS = new My4GenPair(
				vec_GENLepton_HardProcess[0], vec_GENLepton_HardProcess[1],
				vec_GENLepton_HardProcess[2], vec_GENLepton_HardProcess[3] );

			My4GenPair* FourGenPair_FS = new My4GenPair(
				vec_GENLepton_FinalState[0], vec_GENLepton_FinalState[1],
				vec_GENLepton_FinalState[2], vec_GENLepton_FinalState[3] );

			//////////////////////
			// -- reco-level -- //
			//////////////////////
			vector< MyLepton* > vec_RECOLepton = MakeVector_MyLepton( Br_Electron, Br_Muon );
			Int_t nRECOLepton = vec_RECOLepton.size();
			Bool_t Flag_PassSelection = kFALSE;
			My4LeptonPair* SelectedPair = this->Test_PassSelection( vec_RECOLepton, Flag_PassSelection );

			// -- fill -- //
			Double_t weight = 1.0;
			for( unsigned int i=0; i<vec_ChannelType.size(); i++ )
			{
				TString ChannelType = vec_ChannelType[i];

				if( ChannelType == TStr_Channal )
				{
					vec_GENHist_4LepHS[i]->Fill( FourGenPair_HS, weight );
					vec_GENHist_4Lep[i]->Fill( FourGenPair_FS, weight );

					if( Flag_PassSelection )
						vec_RECOHist_4Lep[i]->Fill( SelectedPair, weight );

					break;
				}
			}

		} // -- end of event iteration -- //

		TString FileName_Output = TString::Format("ROOTFile_ControlPlots_%s.root", this->Name_Sample.Data());

		TFile *f_output = TFile::Open(FileName_Output, "RECREATE");
		f_output->cd();
		for( unsigned int i=0; i<vec_ChannelType.size(); i++ )
		{
			vec_GENHist_4LepHS[i]->Save( f_output );
			vec_GENHist_4Lep[i]->Save( f_output );
			vec_RECOHist_4Lep[i]->Save( f_output );
		}
	}

protected:
	Bool_t Test_Acc( vector<GenParticle*> vec_GENLepton_FinalState, TString TStr_Channal )
	{
		Bool_t Flag = kFALSE;
		std::sort( vec_GENLepton_FinalState.begin(), vec_GENLepton_FinalState.end(), CompareGenParticle );

		if( vec_GENLepton_FinalState[0]->PT > 45 && fabs(vec_GENLepton_FinalState[0]->Eta) < 2.4 &&
			vec_GENLepton_FinalState[1]->PT > 45 && fabs(vec_GENLepton_FinalState[1]->Eta) < 2.4 &&
			vec_GENLepton_FinalState[2]->PT > 30 && fabs(vec_GENLepton_FinalState[2]->Eta) < 2.4 &&
			vec_GENLepton_FinalState[3]->PT > 30 && fabs(vec_GENLepton_FinalState[3]->Eta) < 2.4
			)
			Flag = kTRUE;

		return Flag;
	}

	My4LeptonPair* Test_PassSelection( vector<MyLepton*> vec_RECOLepton, Bool_t& Flag )
	{
		Flag = kFALSE;

		vector< My4LeptonPair* > vec_SelectedPair;

		// -- make all possible 4 lepton candidate -- //
		Int_t nLepton = (Int_t)vec_RECOLepton.size();
		for(Int_t i_lep=0; i_lep<nLepton; i_lep++)
		{
			MyLepton *Lepton_ith = vec_RECOLepton[i_lep];
			for(Int_t j_lep=i_lep+1; j_lep<nLepton; j_lep++)
			{
				MyLepton *Lepton_jth = vec_RECOLepton[j_lep];
				for(Int_t k_lep=j_lep+1; k_lep<nLepton; k_lep++)
				{
					MyLepton *Lepton_kth = vec_RECOLepton[k_lep];
					for(Int_t l_lep=k_lep+1; l_lep<nLepton; l_lep++)
					{
						MyLepton *Lepton_lth = vec_RECOLepton[l_lep];

						My4LeptonPair *FourLPair = new My4LeptonPair( Lepton_ith, Lepton_jth, Lepton_kth, Lepton_lth );
						Bool_t Flag_Acc = FourLPair->Test_Acc( 45, 45, 30, 30, 2.4, 2.4, 2.4, 2.4 );
						Bool_t Flag_PassZVeto = FourLPair->Test_PassZVeto(89, 93);
						Bool_t Flag_PassIso = FourLPair->Test_Isolation( 0.10 );
						// Bool_t Flag_SameChannel = (TStr_Channal == FourLPair->FlavorType) ? kTRUE : kFALSE;

						if( Flag_Acc && Flag_PassZVeto && Flag_PassIso )
						{
							Flag = kTRUE;
							vec_SelectedPair.push_back( FourLPair );
						}
					} // -- l-th lepton iteration -- //
				} // -- k-th lepton iteration -- //
			} // -- j-th lepton iteration -- //
		} // -- i-th lepton iteration -- //	

		if( Flag == kFALSE )
		{
			My4LeptonPair *GenPair_temp = new My4LeptonPair(); // -- dummy -- //
			vec_SelectedPair.push_back( GenPair_temp );
		}
		else // -- pass the selection -- //
		{
			if( vec_SelectedPair.size() > 1 )
				std::sort( vec_SelectedPair.begin(), vec_SelectedPair.end(), Compare4LeptonPair );
		}

		return vec_SelectedPair[0];
	}
};

void ProdHist_ControlPlots( TString Path_Sample, TString Name_Sample, Bool_t Flag_IsSignal )
{
	Tool_ControlPlots *tool = new Tool_ControlPlots( Path_Sample, Name_Sample, Flag_IsSignal );
	tool->Analyze();

}