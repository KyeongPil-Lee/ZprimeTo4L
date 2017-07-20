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

void PrintAll_GenParticle(TClonesArray *Br_GenParticle )
{
	Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
	cout << "[nGenParticle = " << nGenParticle << "]" << endl;
	for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
	{
		GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		cout << "[" << i_gen << "th gen-particle]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << ", GenPar->M2: " << GenPar->M2 << endl;
	}
	cout << endl;
}

void Test_MotherCheck()
{
	TString DataPath = gSystem->Getenv("KP_DATA_PATH");
	TString InputFileName = DataPath+"/Delphes/ZZTo4mu_10k.root";
	// TString InputFileName = DataPath+"/Delphes/v20170720_1st_DetSim_Delphes_200k/ZZto4L0j_200k.root";

	TChain* chain = new TChain("Delphes");
	chain->Add(InputFileName);

	// Create object of class ExRootTreeReader
	ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
	Int_t nTotEvent = treeReader->GetEntries();
	cout << "nTotEvent: " << nTotEvent << endl;

	// Get pointers to branches used in this analysis
	TClonesArray *Br_Muon = treeReader->UseBranch("Muon");
	TClonesArray *Br_GenParticle = treeReader->UseBranch("Particle");

	


	// nTotEvent = 100;
	for(Int_t i_ev = 0; i_ev < nTotEvent; i_ev++)
	{
		treeReader->ReadEntry(i_ev);

		// PrintAll_GenParticle( Br_GenParticle );

		Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
		Int_t nMuon_FinalState = 0;
		for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
		{
			GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);

			if( GenPar->Status == 1 && fabs(GenPar->ID) == 13)
				nMuon_FinalState++;
			
			if( GenPar->Status == 1 ) // -- final state letpons  -- //
			{
				Int_t MotherID = this->GetMotherID( GenPar, Br_GenParticle );
				if( MotherID == 1000600 )
					vec_GenLepFromPhi.push_back( GenPar );
				else if( MotherID == -1000600 )
					vec_GenLepFromAntiPhi.push_back( GenPar );
			}
		}

		cout << "nMuon_FinalState: " << nMuon_FinalState << endl;
		if( nMuon_FinalState != 4 )
			cout << "\t WARNING! # muons in final state is NOT 4!!" << endl;


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
}