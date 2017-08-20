#include <Include/HistTools.h>

class HistContainer
{
public:
	TString Type;

	TH1D* h_4e;
	TH1D* h_3e1m;
	TH1D* h_2e2m;
	TH1D* h_1e3m;
	TH1D* h_4m;

	HistContainer( TString _Type )
	{
		this->Type = _Type;
		this->Init();
	}

	void Fill( TString Type_Channel )
	{
		switch( Type_Channel )
		{
			case "4e": this->h_4e->Fill( 0.5 ); break; // -- increase # content in the bin -- //
			case "3e1m": this-h_3e1m->Fill( 0.5 ); break;
			case "2e2m": this-h_2e2m->Fill( 0.5 ); break;
			case "1e3m": this-h_1e3m->Fill( 0.5 ); break;
			case "4m": this-h_4m->Fill( 0.5 ); break;
		}
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_4e->Write();
		this->h_3e1m->Write();
		this->h_2e2m->Write();
		this->h_1e3m->Write();
		this->h_4m->Write();

		cout << "All histograms(" << this->Type << ") are saved in " << f_output->GetName() << endl;
	}

protected:
	void Init()
	{
		this->h_4e = new TH1D("h_4e_"+this->Type, "", 1, 0, 1);
		this->h_3e1m = new TH1D("h_3e1m_"+this->Type, "", 1, 0, 1);
		this->h_2e2m = new TH1D("h_2e2m_"+this->Type, "", 1, 0, 1);
		this->h_1e3m = new TH1D("h_1e3m_"+this->Type, "", 1, 0, 1);
		this->h_4m = new TH1D("h_4m_"+this->Type, "", 1, 0, 1);
	}
}

class Tool_AccEff
{
public:
	TString Path_Sample;
	TString Name_Sample;
	Bool_t Flag_IsSignal;

	Tool_AccEff( TString _Path_Sample, Bool_t _Name_Sample, Bool_t _Flag_IsSignal )
	{
		this->Path_Sample = _Path_Sample;
		this->Name_Sample = _Name_Sample;
		this->Flag_IsSignal = _Flag_IsSignal;
	}

	void Analyze()
	{
		HistContainer* Hist_AccTotal = new HistContainer("AccTotal");
		HistContainer* Hist_AccPass = new HistContainer("AccPass");
		HistContainer* Hist_EffPass = new HistContainer("EffPass");

		TChain* chain = new TChain("Delphes");
		chain->Add(this->InputFileName);

		// Create object of class ExRootTreeReader
		ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
		Int_t nTotEvent = treeReader->GetEntries();
		cout << "nTotEvent: " << nTotEvent << endl;

		// Get pointers to branches used in this analysis
		TClonesArray *Br_Muon = treeReader->UseBranch("Muon");
		TClonesArray *Br_Electron = treeReader->UseBranch("Electron");
		TClonesArray *Br_GenParticle = treeReader->UseBranch("Particle");

		// nTotEvent = 100;
		for(Int_t i_ev = 0; i_ev < nTotEvent; i_ev++)
		{
			treeReader->ReadEntry(i_ev);

			TString TStr_Channal = "";
			vector< GenParticle* > vec_GENLepton_HardProcess;
			if( this->Flag_IsSignal )
				TStr_Channal = ChannelType_Signal( Br_GenParticle, vec_GENLepton_HardProcess );
			else
				TStr_Channal = ChannelType_Bkg( Br_GenParticle, vec_GENLepton_HardProcess );

			vector< GenParticle* > vec_GENLepton_FinalState;
			Find_FinalStateLepton_All( vec_GENLepton_HardProcess, Br_GenParticle, vec_GENLepton_FinalState );

			Hist_AccTotal->Fill( TStr_Channal );
			Bool_t Flag_PassAcc = this->Test_Acc( vec_GENLepton_FinalState, TStr_Channal );
			if( Flag_PassAcc )
			{
				Hist_AccPass->Fill( TStr_Channal );

				///////////////////////
				// -- reco. level -- //
				///////////////////////
				vector< MyLepton* > vec_RECOLepton = this->MakeVector_MyLepton( Br_Electron, Br_Muon );
				Int_t nRECOLepton = vec_RECOLepton.size();
				Flag Flag_PassSelection = this->Test_PassSelection( vec_RECOLepton, TStr_Channal );
				if( Flag_PassSelection )
				{
					Hist_EffPass->Fill( TStr_Channal );
				}



				// -- matching reco-level leptons and gen-level leptons from hard process final state one by one -- //
				// Bool_t Flag_FullyMatched = kFALSE;
				// vector< MyLepton* > vec_LeptonMatched;
				// Int_t nRECOLepton = vec_RECOLepton.size();

				// if( nRECOLepton >= 4 ) // -- at least 4 leptons should be available in the reco.level -- //
				// 	Flag_FullyMatched = Matching_RECO_GENMuonHPFS( vec_GENLepton_FinalState, vec_RECOLepton, vec_LeptonMatched);

				// if( Flag_FullyMatched ) // -- if "all" reco-lepton are matched, then histograms will be filled -- //
				// {
				// 	// -- pairing -- // 
				// 	MyLeptonPair *LepPair1 = new MyLeptonPair( vec_LeptonMatched[0], vec_LeptonMatched[1] );
				// 	MyLeptonPair *LepPair2 = new MyLeptonPair( vec_LeptonMatched[2], vec_LeptonMatched[3] );

				// 	Hists->Fill_RECO( LepPair1, LepPair2 );
				// }

			}

			// //////////////////////
			// // -- gen. level -- //
			// //////////////////////
			// // -- signal: ordering is already adjusted to have same mother in vec_Lepton_HardProcess level -- //
			// // -- bkg: no meaning: dimuon variable distribution will not be used -- //
			// MyGenPair *GenPair1 = new MyGenPair( vec_GENLepton_FinalState[0],vec_GENLepton_FinalState[1] );
			// MyGenPair *GenPair2 = new MyGenPair( vec_GENLepton_FinalState[2],vec_GENLepton_FinalState[3] );
			// Hists->Fill_GEN( GenPair1, GenPair2 );

			// ///////////////////////
			// // -- reco. level -- //
			// ///////////////////////
			// vector< MyLepton* > vec_RECOLepton = this->MakeVector_MyLepton( Br_Electron, Br_Muon );

			// // -- matching reco-level leptons and gen-level leptons from hard process final state one by one -- //
			// Bool_t Flag_FullyMatched = kFALSE;
			// vector< MyLepton* > vec_LeptonMatched;
			// Int_t nRECOLepton = vec_RECOLepton.size();
			// if( nRECOLepton >= 4 ) // -- at least 4 leptons should be available in the reco.level -- //
			// 	Flag_FullyMatched = Matching_RECO_GENMuonHPFS( vec_GENLepton_FinalState, vec_RECOLepton, vec_LeptonMatched);

			// if( Flag_FullyMatched ) // -- if "all" reco-lepton are matched, then histograms will be filled -- //
			// {
			// 	// -- pairing -- // 
			// 	MyLeptonPair *LepPair1 = new MyLeptonPair( vec_LeptonMatched[0], vec_LeptonMatched[1] );
			// 	MyLeptonPair *LepPair2 = new MyLeptonPair( vec_LeptonMatched[2], vec_LeptonMatched[3] );

			// 	Hists->Fill_RECO( LepPair1, LepPair2 );
			// }

		} // -- end of event iteration -- //

		TString FileName_Output = TString::Format("ROOTFile_AccEff_%s.root", this->Name_Sample.Data());

		TFile *f_output = TFile::Open(FileName_Output, "RECREATE");
		f_output->cd();
		Hist_AccTotal->Save( f_output );
		Hist_AccPass->Save( f_output );
		Hist_EffPass->Save( f_output );
	}

protected:
	Bool_t Test_Acc( vector<GenParticle*> vec_GENLepton_FinalState, TString TStr_Channal )
	{
		Bool_t Flag = kFALSE;
		std::sort( vec_GENLepton_FinalState.begin(), vec_GENLepton_FinalState.end(), CompareGenParticle );

		if( vec_GENLepton_FinalState[0]->PT > 45 && fabs(vec_GENLepton_FinalState[0].Eta) < 2.4 &&
			vec_GENLepton_FinalState[1]->PT > 45 && fabs(vec_GENLepton_FinalState[1].Eta) < 2.4 &&
			vec_GENLepton_FinalState[2]->PT > 30 && fabs(vec_GENLepton_FinalState[2].Eta) < 2.4 &&
			vec_GENLepton_FinalState[3]->PT > 30 && fabs(vec_GENLepton_FinalState[3].Eta) < 2.4
			)
			Flag = kTRUE;

		return Flag;
	}

	Bool_t Test_PassSelection( vector<MyLepton*> vec_RECOLepton, TString TStr_Channal )
	{
		Bool_t Flag = kFALSE:

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
						Bool_t Flag_SameChannel = (TStr_Channal == FourLPair->FlavorType) ? kTRUE : kFALSE;

						if( Flag_Acc && Flag_PassZVeto && Flag_PassIso && Flag_SameChannel )
						{
							Flag = kTRUE;
							break;
						}
					} // -- l-th lepton iteration -- //
				} // -- k-th lepton iteration -- //
			} // -- j-th lepton iteration -- //
		} // -- i-th lepton iteration -- //	

		return Flag;
	}
}