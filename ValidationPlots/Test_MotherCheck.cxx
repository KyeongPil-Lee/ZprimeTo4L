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

#include "HistTools.h"

void Test_MotherCheck()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
	Bool_t Flag_IsSignal = kFALSE;
	// TString InputFileName = DataPath+"/Delphes/ZZTo4mu_10k.root";
	TString InputFileName = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/MZp_1000_Msn3_10_200k.root"; Flag_IsSignal = kTRUE;
	// TString InputFileName = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_200k.root"; Flag_IsSignal = kFALSE;

	TChain* chain = new TChain("Delphes");
	chain->Add(InputFileName);

	// Create object of class ExRootTreeReader
	ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
	Int_t nTotEvent = treeReader->GetEntries();
	cout << "nTotEvent: " << nTotEvent << endl;

	// Get pointers to branches used in this analysis
	TClonesArray *Br_Muon = treeReader->UseBranch("Muon");
	TClonesArray *Br_GenParticle = treeReader->UseBranch("Particle");

	
	Int_t nRealZRealZ = 0;
	Bool_t isFound = kFALSE;
	// nTotEvent = 100;
	for(Int_t i_ev = 0; i_ev < nTotEvent; i_ev++)
	{
		treeReader->ReadEntry(i_ev);

		TString TStr_Channal = "";
		vector< GenParticle* > vec_Lepton_HardProcess;
		if( Flag_IsSignal )
			TStr_Channal = ChannelType_Signal( Br_GenParticle, vec_Lepton_HardProcess );
		else
			TStr_Channal = ChannelType_Bkg( Br_GenParticle, vec_Lepton_HardProcess );

		if( TStr_Channal == "4m" )
		{
			vector< GenParticle* > vec_Lepton_FinalState;
			Find_FinalStateLepton_All( vec_Lepton_HardProcess, Br_GenParticle, vec_Lepton_FinalState );

			Bool_t Flag_Acc = Pass_CMSAcc( vec_Lepton_FinalState );
			// if( Flag_Acc )
			{
				vector< Muon* > vec_MuonMatched;
				Int_t nMuon = Br_Muon->GetEntriesFast();
				if( nMuon >= 4 ) // -- at least 4 muon should be available in the reco.level -- //
				{
					Bool_t Flag_FullyMatched = Matching_RECO_GENMuonHPFS( vec_Lepton_FinalState, Br_Muon, vec_MuonMatched);
					cout << "Flag_FullyMatched: " << Flag_FullyMatched << "\n" << endl;
					if( !Flag_FullyMatched ) isFound = kTRUE;
				}


				// // -- reconstruction level -- //
				// vector< Muon* > vec_MuonFromPhi;
				// vector< Muon* > vec_MuonFromAntiPhi;

				// Int_t nMuon = Br_Muon->GetEntriesFast();
				// // cout << "nMuon: " << nMuon << endl;
				// for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
				// {
				// 	// cout << i_mu << "th reco muon" << endl;

				// 	Muon* RecoMu = (Muon*)Br_Muon->At(i_mu);

				// 	GenParticle* GenMu = (GenParticle*)RecoMu->Particle.GetObject(); // -- corresponding muon at the generator level -- //
				// 	Flag_Matched = Matched_GENMuonHPFS( RecoMu, vec_Lepton_FinalState );

				// 	Int_t motherID = this->GetMotherID( GenMu, Br_GenParticle );
				// 	if( motherID == 1000600 )
				// 	{
				// 		vec_GenLepFromPhi.push_back( GenMu );
				// 		vec_MuonFromPhi.push_back( RecoMu );
				// 	}
				// 	else if( motherID == -1000600 )
				// 	{
				// 		vec_GenLepFromAntiPhi.push_back( GenMu );
				// 		vec_MuonFromAntiPhi.push_back( RecoMu );
				// 	}
				// }
			}
		}





		// Int_t nGenParticle = Br_GenParticle->GetEntriesFast();

		// // PrintAll_GenParticle( Br_GenParticle );

		// vector< GenParticle* > vec_Lepton;
		// vector< GenParticle* > vec_Lepton_HardProcess;
		// vector< GenParticle* > vec_Z_HardProcess;
		// vector< GenParticle* > vec_Lepton_FromZ_NotFinal;
		// for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
		// {
		// 	GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		// 	if( fabs(GenPar->PID) == 11 || fabs(GenPar->PID) == 13 )
		// 	{
		// 		vec_Lepton.push_back( GenPar );
		// 		if( GenPar->Status == 1 || GenPar->Status == 23 ) // -- 23: from hard process; 1: may come from hard process (23 && 1 -> 1 ) -- //
		// 		{
		// 			GenParticle* Mother1 = (GenParticle*)Br_GenParticle->At(GenPar->M1);
		// 			Int_t Status_Mother1 = Mother1->Status;
		// 			Int_t ID_Mother1 = Mother1->PID;

		// 			Int_t Status_Mother2 = -1;
		// 			if( GenPar->M2 > 0 )
		// 			{
		// 				GenParticle* Mother2 = (GenParticle*)Br_GenParticle->At(GenPar->M2);
		// 				Status_Mother2 = Mother2->Status;
		// 			}
		// 			else
		// 				Status_Mother2 = -1;

		// 			// -- (Mother1->Status == 21 && Mother2->Status == 21): 
		// 			// from virtual. mothers should be hardest incoming particles -- //
		// 			// -- Mother1->PID == 23: from Z -- //
		// 			if( (Status_Mother1 == 21 && Status_Mother2 == 21) || ID_Mother1 == 23 )
		// 				vec_Lepton_HardProcess.push_back( GenPar );
		// 		}

		// 		// if( ((GenPar->Status == 1 || GenPar->Status == 23) && fabs(MotherID) <= 6 )  // -- from virtual -- //
		// 		// 	|| ((GenPar->Status == 1 || GenPar->Status == 23) && MotherID == 23) ) // -- from Z -- //
		// 		// {
		// 		// 	vec_Lepton_HardProcess.push_back( GenPar );
		// 		// }
		// 		// else if( GenPar->Status == 23 && MotherID == 23 ) // -- from Z, not final state -- //
		// 		// 	vec_Lepton_FromZ_NotFinal.push_back( GenPar );
		// 	}

		// 	if( GenPar->PID == 23 && GenPar->Status == 22 )
		// 		vec_Z_HardProcess.push_back( GenPar );
		// }

		// // // -- If we found already 4 leptons from virtual & Z with final state: it's fine
		// // // if not, then it means leptons from Z suffer FSR, so they cannot pass GenPar->Status == 1 & MotherID == 23.
		// // // (because Mother will be same with leptonID )
		// // // In this case, we need to take into account the leptons directly from Z, but not in final state.
		// // // If we do in one loop, then it will be double-counted!: leptons from Z, 
		// // if( vec_Lepton_HardProcess.size() < 4 )
		// // {
		// // 	for( const auto& GenPar : vec_Lepton_FromZ_NotFinal )
		// // 		vec_Lepton_HardProcess.push_back( GenPar );
		// // }


		// Int_t nHP = vec_Lepton_HardProcess.size();
		// Int_t nHP_Z = vec_Z_HardProcess.size();
		// Int_t nLep_FromZ_NotFinal = vec_Lepton_FromZ_NotFinal.size();
		// // cout << "nHP: " << nHP << endl;
		// // cout << "nHP_Z: " << nHP_Z << endl;
		// // cout << "nLep_FromZ_NotFinal: " << nLep_FromZ_NotFinal << endl;
		// if( nHP != 4 )
		// {
		// 	isFound = kTRUE;
		// 	for(Int_t i_gen=0; i_gen<vec_Lepton.size(); i_gen++)
		// 	{
		// 		GenParticle* GenPar = (GenParticle*)vec_Lepton[i_gen];
		// 		Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
		// 		cout << "[" << i_gen << "th muon]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << " (MotherID = " << ID_Mother1 << "), GenPar->M2: " << GenPar->M2 << endl;
		// 	}

		// 	PrintAll_GenParticle(Br_GenParticle);
		// }

		// vector< GenParticle* > vec_Lepton_FinalState;
		// Find_FinalStateLepton_All( vec_Lepton_HardProcess, Br_GenParticle, vec_Lepton_FinalState );
		// cout << "final state leptons" << endl;
		// Int_t nLep = vec_Lepton_FinalState.size();
		// for(Int_t i_lep=0; i_lep<nLep; i_lep++)
		// {
		// 	cout << "HP: "; Print_GenParticle( vec_Lepton_HardProcess[i_lep] );
		// 	cout << "FS: "; Print_GenParticle( vec_Lepton_FinalState[i_lep] );
		// 	cout << endl;
		// }
		// cout << endl;
		// cout << endl;

		// Int_t nLep_FromZ_NotFinal = vec_Lepton_FromZ_NotFinal.size();
		// if( nLep_FromZ_NotFinal > 0 )
		// 	cout << "nLep_FromZ_NotFinal: " << nLep_FromZ_NotFinal << endl;

		// if( nHP == 4 )
		// 	PrintAll_GenParticle( Br_GenParticle );

		// vector< GenParticle* > vec_Lepton;
		// vector< GenParticle* > vec_LeptonFromHardProcess;
		// for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
		// {
		// 	GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		// 	if( fabs(GenPar->PID) == 11 || fabs(GenPar->PID) == 13 )
		// 	{
		// 		vec_Lepton.push_back( GenPar );

		// 		Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
		// 		if( (GenPar->Status == 23 || GenPar->Status == 1) && fabs(ID_Mother1) == 1000600 )
		// 			vec_LeptonFromHardProcess.push_back( GenPar );
		// 	}
		// }

		// Int_t nLeptonFromHardProcess = vec_LeptonFromHardProcess.size();
		// if( nLeptonFromHardProcess != 4 )
		// {
		// 	cout << "WARNING: nLeptonFromHardProcess: " << nLeptonFromHardProcess << endl;
		// 	for(Int_t i_gen=0; i_gen<vec_Lepton.size(); i_gen++)
		// 	{
		// 		GenParticle* GenPar = (GenParticle*)vec_Lepton[i_gen];
		// 		Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
		// 		cout << "[" << i_gen << "th muon]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << " (MotherID = " << ID_Mother1 << "), GenPar->M2: " << GenPar->M2 << endl;
		// 	}
		// }


		// Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
		// vector< GenParticle* > vec_Muon;
		// vector< GenParticle* > vec_MuonFinalState;
		// vector< GenParticle* > vec_MuonFinalState_MotherCheck;
		// for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
		// {
		// 	GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);

		// 	if( fabs(GenPar->PID) == 13 || fabs(GenPar->PID) == 11 )
		// 	{
		// 		vec_Muon.push_back( GenPar );

		// 		if( GenPar->Status == 1 )
		// 		{
		// 			vec_MuonFinalState.push_back( GenPar );

		// 			Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
		// 			if( fabs(ID_Mother1) == 1000600 || ID_Mother1 == GenPar->PID )
		// 				vec_MuonFinalState_MotherCheck.push_back( GenPar );
		// 		}
		// 	}

		// }
		// Int_t nMuon = vec_Muon.size();
		// Int_t nMuon_FinalState = vec_MuonFinalState.size();
		// Int_t nMuon_FinalState_MotherCheck = vec_MuonFinalState_MotherCheck.size();

		// cout << "nMuon: " << nMuon << endl;

		// // cout << "nMuon_FinalState: " << nMuon_FinalState << endl;
		// if( nMuon_FinalState != 4 )
		// {
		// 	cout << "\t WARNING! # muons in final state is NOT 4!!" << endl;

		// 	for(Int_t i_gen=0; i_gen<nMuon_FinalState; i_gen++)
		// 	{
		// 		GenParticle* GenPar = (GenParticle*)vec_MuonFinalState[i_gen];
		// 		Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
		// 		cout << "[" << i_gen << "th muon]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << " (MotherID = " << ID_Mother1 << "), GenPar->M2: " << GenPar->M2 << endl;
		// 	}
		// }

		

		// if( nMuon_FinalState_MotherCheck != 4 )
		// {
		// 	isFound = kTRUE;
		// 	cout << "\t WARNING! # muons in final state with mother check is NOT 4!!" << endl;

		// 	for(Int_t i_gen=0; i_gen<nMuon_FinalState; i_gen++)
		// 	{
		// 		GenParticle* GenPar = (GenParticle*)vec_Muon[i_gen];
		// 		Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
		// 		cout << "[" << i_gen << "th muon]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << " (MotherID = " << ID_Mother1 << "), GenPar->M2: " << GenPar->M2 << endl;
		// 	}
		// }

		// cout << "[" << i_ev << "th event]" << endl;
		// Int_t nMuon_MotherZ = 0;
		// for( const auto& Muon : vec_MuonFinalState_MotherCheck )
		// {
		// 	if( GetMotherID( Muon, Br_GenParticle ) == 23 ) nMuon_MotherZ++;
		// 	else if( fabs(GetMotherID( Muon, Br_GenParticle )) == 13 )
		// 	{
		// 		GenParticle* Mother = (GenParticle*)Br_GenParticle->At(Muon->M1);
		// 		Int_t ID_GrandMother = GetMotherID( Mother, Br_GenParticle );
		// 		if( ID_GrandMother == 23 )
		// 		{
		// 			cout << "Mother == 13, GrandMother == 23" << endl;
		// 			nMuon_MotherZ++;
		// 		}
		// 		else if( fabs(ID_GrandMother) == 13 )
		// 		{
		// 			cout << "Mother == 13, and still GrandMother == 13" << endl;
		// 			PrintAll_GenMuon(Br_GenParticle);
		// 		}
		// 		else if( fabs(ID_GrandMother) == 0 )
		// 		{
		// 			cout << "Mother == 13, and GrandMother == 0" << endl;
		// 			PrintAll_GenMuon(Br_GenParticle);
		// 		}
		// 		else
		// 		{
		// 			cout << "Other case! ... ID_GrandMother = " << ID_GrandMother << endl;
		// 			PrintAll_GenMuon(Br_GenParticle);
		// 		}
		// 	}
		// }

		// if( nMuon_MotherZ == 4 )
		// {
		// 	cout << "ZZ case" << endl;
		// 	PrintAll_GenParticle(Br_GenParticle);
		// }






		// MyGenPair *GenPair_Phi = new MyGenPair( vec_GenLepFromPhi[0],vec_GenLepFromPhi[1] );
		// MyGenPair *GenPair_AntiPhi = new MyGenPair( vec_GenLepFromAntiPhi[0],vec_GenLepFromAntiPhi[1] );
		// Hists->Fill_GEN( GenPair_Phi, GenPair_AntiPhi );


		// // -- reconstruction level -- //
		// vector< Muon* > vec_MuonFromPhi;
		// vector< Muon* > vec_MuonFromAntiPhi;

		// Int_t nMuon = Br_Muon->GetEntriesFast();
		// // cout << "nMuon: " << nMuon << endl;
		// for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
		// {
		// 	// cout << i_mu << "th reco muon" << endl;

		// 	Muon* RecoMu = (Muon*)Br_Muon->At(i_mu);

		// 	GenParticle* GenMu = (GenParticle*)RecoMu->Particle.GetObject(); // -- corresponding muon at the generator level -- //
		// 	Int_t motherID = this->GetMotherID( GenMu, Br_GenParticle );
		// 	if( motherID == 1000600 )
		// 	{
		// 		vec_GenLepFromPhi.push_back( GenMu );
		// 		vec_MuonFromPhi.push_back( RecoMu );
		// 	}
		// 	else if( motherID == -1000600 )
		// 	{
		// 		vec_GenLepFromAntiPhi.push_back( GenMu );
		// 		vec_MuonFromAntiPhi.push_back( RecoMu );
		// 	}
		// }


		// if( (Int_t)vec_MuonFromPhi.size() == 2 && (Int_t)vec_MuonFromAntiPhi.size() == 2 ) // -- all 4 muons are reconstructed -- //
		// {
		// 	MyMuonPair *Pair_Phi = new MyMuonPair( vec_MuonFromPhi[0], vec_MuonFromPhi[1] );
		// 	MyMuonPair *Pair_AntiPhi = new MyMuonPair( vec_MuonFromAntiPhi[0], vec_MuonFromAntiPhi[1] );

		// 	Hists->Fill_RECO( Pair_Phi, Pair_AntiPhi );
		// }

	} // -- end of event iteration -- //

	cout << "isFound: " << isFound << endl;
	cout << "nRealZRealZ " << nRealZRealZ << endl;
}

