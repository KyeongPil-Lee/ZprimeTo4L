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

#include "../HistTools.h"

// class HistContainer
// {
// public:
// 	HistContainer_GENLepton *GenLeptonHist_All;
// 	HistContainer_GENLepton *GenLeptonHist_1st;
// 	HistContainer_GENLepton *GenLeptonHist_2nd;
// 	HistContainer_GENLepton *GenLeptonHist_3rd;
// 	HistContainer_GENLepton *GenLeptonHist_4th;

// 	HistContainer_GENLeptonPair* GenPairHist_All;
// 	// HistContainer_GENLeptonPair* GenPairHist_Phi;
// 	// HistContainer_GENLeptonPair* GenPairHist_AntiPhi;
// 	HistContainer_4GENLepton* FourGenLepHist;


// 	HistContainer_Lepton* SingleLeptonHist_All;
// 	HistContainer_Lepton* SingleLeptonHist_1st;
// 	HistContainer_Lepton* SingleLeptonHist_2nd;
// 	HistContainer_Lepton* SingleLeptonHist_3rd;
// 	HistContainer_Lepton* SingleLeptonHist_4th;


// 	HistContainer_LeptonPair* LeptonPairHist_All;
// 	// MuonPairHistContainer* MuonPairHist_Phi;
// 	// MuonPairHistContainer* MuonPairHist_AntiPhi;

// 	HistContainer_4Lepton* FourLeptonHist;

// 	HistContainer()
// 	{
// 		this->Init_Hist();
// 	}

// 	void Fill_GEN(MyGenPair *Pair_Phi, MyGenPair* Pair_AntiPhi)
// 	{
// 		vector< GenParticle* > vec_GenMu;
// 		vec_GenMu.push_back( Pair_Phi->First );
// 		vec_GenMu.push_back( Pair_Phi->Second );
// 		vec_GenMu.push_back( Pair_AntiPhi->First );
// 		vec_GenMu.push_back( Pair_AntiPhi->Second );
// 		sort( vec_GenMu.begin(), vec_GenMu.end(), CompareGenParticle );

// 		GenLeptonHist_All->Fill( vec_GenMu[0] );
// 		GenLeptonHist_All->Fill( vec_GenMu[1] );
// 		GenLeptonHist_All->Fill( vec_GenMu[2] );
// 		GenLeptonHist_All->Fill( vec_GenMu[3] );

// 		GenLeptonHist_1st->Fill( vec_GenMu[0] );
// 		GenLeptonHist_2nd->Fill( vec_GenMu[1] );
// 		GenLeptonHist_3rd->Fill( vec_GenMu[2] );
// 		GenLeptonHist_4th->Fill( vec_GenMu[3] );

// 		GenPairHist_All->Fill( Pair_Phi );
// 		GenPairHist_All->Fill( Pair_AntiPhi );

// 		FourGenLepHist->Fill( Pair_Phi, Pair_AntiPhi );
// 	}


// 	void Fill_RECO(MyLeptonPair *Pair_Phi, MyLeptonPair* Pair_AntiPhi)
// 	{
// 		vector< MyLepton* > vec_Lepton;
// 		vec_Lepton.push_back( Pair_Phi->First );
// 		vec_Lepton.push_back( Pair_Phi->Second );
// 		vec_Lepton.push_back( Pair_AntiPhi->First );
// 		vec_Lepton.push_back( Pair_AntiPhi->Second );
// 		sort( vec_Lepton.begin(), vec_Lepton.end(), CompareLepton );

// 		SingleLeptonHist_All->Fill( vec_Lepton[0] );
// 		SingleLeptonHist_All->Fill( vec_Lepton[1] );
// 		SingleLeptonHist_All->Fill( vec_Lepton[2] );
// 		SingleLeptonHist_All->Fill( vec_Lepton[3] );

// 		SingleLeptonHist_1st->Fill( vec_Lepton[0] );
// 		SingleLeptonHist_2nd->Fill( vec_Lepton[1] );
// 		SingleLeptonHist_3rd->Fill( vec_Lepton[2] );
// 		SingleLeptonHist_4th->Fill( vec_Lepton[3] );

// 		LeptonPairHist_All->Fill( Pair_Phi );
// 		LeptonPairHist_All->Fill( Pair_AntiPhi );

// 		FourLeptonHist->Fill( Pair_Phi, Pair_AntiPhi );
// 	}

// 	void Save( TFile *f_output )
// 	{
// 		f_output->cd();

// 		GenLeptonHist_All->Save( f_output );
// 		GenLeptonHist_1st->Save( f_output );
// 		GenLeptonHist_2nd->Save( f_output );
// 		GenLeptonHist_3rd->Save( f_output );
// 		GenLeptonHist_4th->Save( f_output );

// 		GenPairHist_All->Save( f_output );
// 		FourGenLepHist->Save( f_output );
		

// 		SingleLeptonHist_All->Save( f_output );
// 		SingleLeptonHist_1st->Save( f_output );
// 		SingleLeptonHist_2nd->Save( f_output );
// 		SingleLeptonHist_3rd->Save( f_output );
// 		SingleLeptonHist_4th->Save( f_output );

// 		LeptonPairHist_All->Save( f_output );
// 		FourLeptonHist->Save( f_output );
// 	}

// 	void Init_Hist()
// 	{
// 		this->GenLeptonHist_All = new HistContainer_GENLepton( "All" );
// 		this->GenLeptonHist_1st = new HistContainer_GENLepton( "1st" );
// 		this->GenLeptonHist_2nd = new HistContainer_GENLepton( "2nd" );
// 		this->GenLeptonHist_3rd = new HistContainer_GENLepton( "3rd" );
// 		this->GenLeptonHist_4th = new HistContainer_GENLepton( "4th" );

// 		this->GenPairHist_All = new HistContainer_GENLeptonPair( "All" );
// 		this->FourGenLepHist = new HistContainer_4GENLepton( "All" );


// 		this->SingleLeptonHist_All = new HistContainer_Lepton( "All" );
// 		this->SingleLeptonHist_1st = new HistContainer_Lepton( "1st" );
// 		this->SingleLeptonHist_2nd = new HistContainer_Lepton( "2nd" );
// 		this->SingleLeptonHist_3rd = new HistContainer_Lepton( "3rd" );
// 		this->SingleLeptonHist_4th = new HistContainer_Lepton( "4th" );

// 		this->LeptonPairHist_All = new HistContainer_LeptonPair( "All" );
// 		this->FourLeptonHist = new HistContainer_4Lepton( "All" );
// 	}
// };

class Analyzer
{
public:
	TString InputFileName;
	Bool_t Flag_IsSignal;

	// -- 4e, 4m, 2e2m, 3e1m, 1e3m -- //
	TString ChannelType;

	Analyzer( TString _InputFileName )
	{
		this->InputFileName = _InputFileName;
		Flag_IsSignal = kFALSE; // -- initial value -- //
	}

	void IsSignal( Bool_t flag )
	{
		this->Flag_IsSignal = flag;
	}

	void Set_ChannelType( TString _Channel )
	{
		this->ChannelType = _Channel;
	}

	void Analyze()
	{
		// HistContainer* Hists = new HistContainer();

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

		Int_t nTotal = 0;
		Int_t nPass_Eta[4] = {0};
		Int_t nPass_Pt[4] = {0};
		Int_t nPass_All = 0;

		Int_t nTotal_Reco = 0;
		Int_t nEvent_MoreThan4RecoElec = 0;
		Int_t nEvent_MoreThan4RecoMu = 0;

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

			if( TStr_Channal == this->ChannelType )
			{
				vector< GenParticle* > vec_GENLepton_FinalState;
				Find_FinalStateLepton_All( vec_GENLepton_HardProcess, Br_GenParticle, vec_GENLepton_FinalState );

				if( (Int_t)vec_GENLepton_FinalState.size() != 4 )
				{
					cout << "Something goes wrong! ... # leptons in the final state is not 4!" << endl;
					cout << "vec_GENLepton_FinalState.size() = " << vec_GENLepton_FinalState.size() << endl;
					break;
				}

				nTotal++;

				Bool_t Flag_Acc = kTRUE;
				sort( vec_GENLepton_FinalState.begin(), vec_GENLepton_FinalState.end(), CompareGenParticle );
				for(Int_t i=0; i<4; i++)
				{
					if( vec_GENLepton_FinalState[i]->PT > 10 ) nPass_Pt[i]++;
					else Flag_Acc = kFALSE;

					if( fabs(vec_GENLepton_FinalState[i]->Eta) < 2.4 ) nPass_Eta[i]++;
					else Flag_Acc = kFALSE;
				}
				if( Flag_Acc ) nPass_All++;



				// //////////////////////
				// // -- gen. level -- //
				// //////////////////////
				// // -- signal: ordering is already adjusted to have same mother in vec_Lepton_HardProcess level -- //
				// // -- bkg: no meaning: dimuon variable distribution will not be used -- //
				// MyGenPair *GenPair1 = new MyGenPair( vec_GENLepton_FinalState[0],vec_GENLepton_FinalState[1] );
				// MyGenPair *GenPair2 = new MyGenPair( vec_GENLepton_FinalState[2],vec_GENLepton_FinalState[3] );
				// Hists->Fill_GEN( GenPair1, GenPair2 );

				///////////////////////
				// -- reco. level -- //
				///////////////////////
				vector< MyLepton* > vec_RECOLepton = this->MakeVector_MyLepton( Br_Electron, Br_Muon );

				// -- matching reco-level leptons and gen-level leptons from hard process final state one by one -- //
				Bool_t Flag_FullyMatched = kFALSE;
				vector< MyLepton* > vec_LeptonMatched;
				Int_t nRECOLepton = vec_RECOLepton.size();
				if( nRECOLepton >= 4 ) // -- at least 4 leptons should be available in the reco.level -- //
					Flag_FullyMatched = Matching_RECO_GENMuonHPFS( vec_GENLepton_FinalState, vec_RECOLepton, vec_LeptonMatched);

				if( Flag_FullyMatched ) // -- if "all" reco-lepton are matched, then histograms will be filled -- //
				{
					nTotal_Reco++;
				}

				Int_t nElecRECO = 0;
				Int_t nMuRECO = 0;
				for( const auto &Lepton : vec_RECOLepton )
				{
					if( Lepton.isElec ) nElecRECO++;
					if( Lepton.isMu ) nMuRECO++;
				}

				if( nElecRECO >= 4 ) nEvent_MoreThan4RecoElec++;
				if( nMuRECO >= 4 ) nEvent_MoreThan4RecoMu++;

				if( nElecRECO >= 4 && nMuRECO >= 4)
				{
					cout << "Both reco-level # muon and electron is more than 4!" << endl;
					cout << "\tnElecReco: " << nElecRECO << endl;
					cout << "\tnMuRECO: " << nMuRECO << endl;
				}
			} // -- end of if( TStr_Channal == this->ChannelType ) -- //

		} // -- end of event iteration -- //

		cout << "##### Summary #####" << endl;
		for(Int_t i=0; i<4; i++)
		{
			Double_t ratio_Pt = (Double_t)nPass_Pt[i] / nTotal;
			Double_t ratio_Eta = (Double_t)nPass_Eta[i] / nTotal;
			printf("%d lepton: [Pt] nPass/nTotal = %d/%d = %.5lf, [eta] nPass/nTotal = %d/%d = %.5lf\n", i, nPass_Pt[i], nTotal, ratio_Pt, nPass_Eta[i], nTotal, ratio_Eta);
		}
		cout << endl;

		Double_t ratio_total = (Double_t)nPass_All/nTotal;
		printf("[Total] nPass/nTotal = %d/%d = %.5lf\n", nPass_All, nTotal, ratio_total);

		Double_t ratio_Reco = (Double_t)nTotal_Reco/nTotal;
		printf("[reco] nTotal_Reco/nTotal = %d/%d = %.5lf\n", nTotal_Reco, nTotal, ratio_Reco);

		Double_t ratio_RecoElec = (Double_t)nEvent_MoreThan4RecoElec/nTotal;
		printf("[reco] nEvent (# elec >= 4)/nTotal = %d/%d = %.5lf\n", nEvent_MoreThan4RecoElec, nTotal, ratio_RecoElec);

		Double_t ratio_RecoMu = (Double_t)nEvent_MoreThan4RecoMu/nTotal;
		printf("[reco] nEvent (# mu >= 4)/nTotal = %d/%d = %.5lf\n\n", nEvent_MoreThan4RecoMu, nTotal, ratio_RecoMu);
	}

	vector< MyLepton* > MakeVector_MyLepton( TClonesArray* Br_Electron, TClonesArray* Br_Muon )
	{
		vector< MyLepton* > vec_MyLepton;

		Int_t nElec = Br_Electron->GetEntriesFast();
		for(Int_t i_elec=0; i_elec<nElec; i_elec++)
		{
			Electron* RecoElec = (Electron*)Br_Electron->At(i_elec);
			MyLepton *RecoLepton = new MyLepton( RecoElec );
			vec_MyLepton.push_back( RecoLepton );
		}

		Int_t nMuon = Br_Muon->GetEntriesFast();
		for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
		{
			Muon* RecoMu = (Muon*)Br_Muon->At(i_mu);
			MyLepton *RecoLepton = new MyLepton( RecoMu );
			vec_MyLepton.push_back( RecoLepton );
		}

		return vec_MyLepton;
	}
};
