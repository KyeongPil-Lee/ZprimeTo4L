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

Bool_t CompareGenParticle( GenParticle* Object1, GenParticle* Object2 )
{
	return Object1->PT > Object2->PT;
}

Bool_t CompareMuon( Muon* Muon1, Muon* Muon2 )
{
	return Muon1->PT > Muon2->PT;
}

class MyGenPair
{
public:
	GenParticle *First;
	GenParticle *Second;

	Double_t M;
	Double_t Pt;
	Double_t Rap;
	TLorentzVector LVec_P;

	MyGenPair( GenParticle* GenPar1, GenParticle* GenPar2 )
	{
		if( GenPar1->PT > GenPar2->PT )
		{
			this->First = GenPar1;
			this->Second = GenPar2;
		}
		else
		{
			this->First = GenPar2;
			this->Second = GenPar1;
		}

		this->Calc_Var();
	}

	void Calc_Var()
	{
		TLorentzVector LVec_First = this->First->P4();
		TLorentzVector LVec_Second = this->Second->P4();
		this->LVec_P = LVec_First + LVec_Second;

		this->M = this->LVec_P.M();
		this->Pt = this->LVec_P.Pt();
		this->Rap = this->LVec_P.Rapidity();
	}
};

class MyMuonPair
{
public:
	Muon *First;
	Muon *Second;

	Double_t M;
	Double_t Pt;
	Double_t Rap;
	TLorentzVector LVec_P;

	MyMuonPair( Muon* mu1, Muon* mu2 )
	{
		if( mu1->PT > mu2->PT )
		{
			this->First = mu1;
			this->Second = mu2;
		}
		else
		{
			this->First = mu2;
			this->Second = mu1;
		}

		this->Calc_Var();
	}

	void Calc_Var()
	{
		TLorentzVector LVec_First = First->P4();
		TLorentzVector LVec_Second = Second->P4();
		this->LVec_P = LVec_First + LVec_Second;

		this->M = this->LVec_P.M();
		this->Pt = this->LVec_P.Pt();
		this->Rap = this->LVec_P.Rapidity();
	}
};

class FourGenLepHistContainer
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;

	FourGenLepHistContainer(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( MyGenPair* GenPair1, MyGenPair* GenPair2, Double_t weight = 1.0 )
	{
		TLorentzVector LVec_P_4Lep = GenPair1->LVec_P + GenPair2->LVec_P;

		Double_t Pt = LVec_P_4Lep.Pt();
		Double_t Rap = LVec_P_4Lep.Rapidity();
		Double_t Mass = LVec_P_4Lep.M();

		this->h_Pt->Fill( Pt, weight );
		this->h_Rap->Fill( Rap, weight );
		this->h_Mass->Fill( Mass, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Rap->Write();
		this->h_Mass->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_4GenLep_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_4GenLep_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_4GenLep_"+this->Type, "", 80, -4, 4);
	}
};

class FourMuonHistContainer
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;

	FourMuonHistContainer(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( MyMuonPair* MuonPair1, MyMuonPair* MuonPair2, Double_t weight = 1.0 )
	{
		TLorentzVector LVec_P_4Lep = MuonPair1->LVec_P + MuonPair2->LVec_P;

		Double_t Pt = LVec_P_4Lep.Pt();
		Double_t Rap = LVec_P_4Lep.Rapidity();
		Double_t Mass = LVec_P_4Lep.M();

		this->h_Pt->Fill( Pt, weight );
		this->h_Rap->Fill( Rap, weight );
		this->h_Mass->Fill( Mass, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Rap->Write();
		this->h_Mass->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_4Muon_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_4Muon_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_4Muon_"+this->Type, "", 80, -4, 4);
	}
};

class GenLepPairHistContainer
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;

	GenLepPairHistContainer(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( MyGenPair* GenPair, Double_t weight = 1.0 )
	{
		Double_t Pt = GenPair->Pt;
		Double_t Rap = GenPair->Rap;
		Double_t Mass = GenPair->M;

		this->h_Pt->Fill( Pt, weight );
		this->h_Rap->Fill( Rap, weight );
		this->h_Mass->Fill( Mass, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Rap->Write();
		this->h_Mass->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_2GenLep_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_2GenLep_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_2GenLep_"+this->Type, "", 80, -4, 4);
	}
};

class MuonPairHistContainer
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;

	MuonPairHistContainer(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( MyMuonPair* MuonPair, Double_t weight = 1.0 )
	{
		Double_t Pt = MuonPair->Pt;
		Double_t Rap = MuonPair->Rap;
		Double_t Mass = MuonPair->M;

		this->h_Pt->Fill( Pt, weight );
		this->h_Rap->Fill( Rap, weight );
		this->h_Mass->Fill( Mass, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Rap->Write();
		this->h_Mass->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_2Muon_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_2Muon_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_2Muon_"+this->Type, "", 80, -4, 4);
	}
};

class GenLepHistContainer
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Eta;
	TH1D* h_Phi;

	GenLepHistContainer(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( GenParticle* GenPar, Double_t weight = 1.0 )
	{
		Double_t Pt = GenPar->PT;
		Double_t Eta = GenPar->Eta;
		Double_t Phi = GenPar->Phi;

		this->h_Pt->Fill( Pt, weight );
		this->h_Eta->Fill( Eta, weight );
		this->h_Phi->Fill( Phi, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Eta->Write();
		this->h_Phi->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_GEN_"+this->Type, "", 10000, 0, 10000);
		this->h_Eta = new TH1D("h_Eta_GEN_"+this->Type, "", 200, -10, 10);
		this->h_Phi = new TH1D("h_Phi_GEN_"+this->Type, "", 80, -4, 4);
	}

};


class SingleMuHistContainer
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Eta;
	TH1D* h_Phi;
	TH1D* h_Pt_GEN;
	TH1D* h_Eta_GEN;
	TH1D* h_Phi_GEN;
	TH1D* h_RelDiff_Pt;
	TH1D* h_RelDiff_Eta;
	TH1D* h_RelDiff_Phi;

	SingleMuHistContainer( TString _Type )
	{
		this->Type = _Type;

		this->Init_Hist();
	}

	void Fill( Muon *Mu, Double_t weight = 1.0 )
	{
		Double_t Pt_RECO = Mu->PT;
		Double_t Eta_RECO = Mu->Eta;
		Double_t Phi_RECO = Mu->Phi;

		GenParticle* GenMu = (GenParticle*)Mu->Particle.GetObject();
		Double_t Pt_GEN = GenMu->PT;
		Double_t Eta_GEN = GenMu->Eta;
		Double_t Phi_GEN = GenMu->Phi;

		Double_t RelDiff_Pt = (Pt_RECO - Pt_GEN) / Pt_GEN;
		Double_t RelDiff_Eta = (Eta_RECO - Eta_GEN) / Eta_GEN;
		Double_t RelDiff_Phi = (Phi_RECO - Phi_GEN) / Phi_GEN;

		this->h_Pt->Fill( Pt_RECO, weight );
		this->h_Eta->Fill( Eta_RECO, weight );
		this->h_Phi->Fill( Phi_RECO, weight );

		this->h_Pt_GEN->Fill( Pt_GEN, weight );
		this->h_Eta_GEN->Fill( Eta_GEN, weight );
		this->h_Phi_GEN->Fill( Phi_GEN, weight );

		this->h_RelDiff_Pt->Fill( RelDiff_Pt, weight );
		this->h_RelDiff_Eta->Fill( RelDiff_Eta, weight );
		this->h_RelDiff_Phi->Fill( RelDiff_Phi, weight );
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_"+this->Type, "", 10000, 0, 10000);
		this->h_Eta = new TH1D("h_Eta_"+this->Type, "", 60, -3, 3);
		this->h_Phi = new TH1D("h_Phi_"+this->Type, "", 80, -4, 4);

		this->h_Pt_GEN = new TH1D("h_Pt_MatchedGEN_"+this->Type, "", 10000, 0, 10000);
		this->h_Eta_GEN = new TH1D("h_Eta_MatchedGEN_"+this->Type, "", 60, -3, 3);
		this->h_Phi_GEN = new TH1D("h_Phi_MatchedGEN_"+this->Type, "", 80, -4, 4);

		this->h_RelDiff_Pt = new TH1D("h_RelDiff_Pt_"+this->Type, "", 1000, -1, 1);
		this->h_RelDiff_Eta = new TH1D("h_RelDiff_Eta_"+this->Type, "", 1000, -1, 1);
		this->h_RelDiff_Phi = new TH1D("h_RelDiff_Phi_"+this->Type, "", 1000, -1, 1);
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Eta->Write();
		this->h_Phi->Write();
		this->h_Pt_GEN->Write();
		this->h_Eta_GEN->Write();
		this->h_Phi_GEN->Write();
		this->h_RelDiff_Pt->Write();
		this->h_RelDiff_Eta->Write();
		this->h_RelDiff_Phi->Write();
	}
};

class HistContainer
{
public:
	GenLepHistContainer *GenMuHist_All;
	GenLepHistContainer *GenMuHist_1st;
	GenLepHistContainer *GenMuHist_2nd;
	GenLepHistContainer *GenMuHist_3rd;
	GenLepHistContainer *GenMuHist_4th;

	SingleMuHistContainer* SingleMuHist_All;
	SingleMuHistContainer* SingleMuHist_1st;
	SingleMuHistContainer* SingleMuHist_2nd;
	SingleMuHistContainer* SingleMuHist_3rd;
	SingleMuHistContainer* SingleMuHist_4th;

	// GenLepPairHistContainer* GenPairHist_All;
	// GenLepPairHistContainer* GenPairHist_Phi;
	// GenLepPairHistContainer* GenPairHist_AntiPhi;

	// MuonPairHistContainer* MuonPairHist_All;
	// MuonPairHistContainer* MuonPairHist_Phi;
	// MuonPairHistContainer* MuonPairHist_AntiPhi;

	// FourGenLepHistContainer* FourGenLepHist;
	// FourMuonHistContainer* FourMuonHist;

	HistContainer()
	{
		this->Init_Hist();
	}

	void Fill_GEN(MyGenPair *Pair_Phi, MyGenPair* Pair_AntiPhi)
	{
		vector< GenParticle* > vec_GenMu;
		vec_GenMu.push_back( Pair_Phi->First );
		vec_GenMu.push_back( Pair_Phi->Second );
		vec_GenMu.push_back( Pair_AntiPhi->First );
		vec_GenMu.push_back( Pair_AntiPhi->Second );
		sort( vec_GenMu.begin(), vec_GenMu.end(), CompareGenParticle );

		GenMuHist_All->Fill( vec_GenMu[0] );
		GenMuHist_All->Fill( vec_GenMu[1] );
		GenMuHist_All->Fill( vec_GenMu[2] );
		GenMuHist_All->Fill( vec_GenMu[3] );

		GenMuHist_1st->Fill( vec_GenMu[0] );
		GenMuHist_2nd->Fill( vec_GenMu[1] );
		GenMuHist_3rd->Fill( vec_GenMu[2] );
		GenMuHist_4th->Fill( vec_GenMu[3] );
	}


	void Fill_RECO(MyMuonPair *Pair_Phi, MyMuonPair* Pair_AntiPhi)
	{
		vector< Muon* > vec_Muon;
		vec_Muon.push_back( Pair_Phi->First );
		vec_Muon.push_back( Pair_Phi->Second );
		vec_Muon.push_back( Pair_AntiPhi->First );
		vec_Muon.push_back( Pair_AntiPhi->Second );
		sort( vec_Muon.begin(), vec_Muon.end(), CompareMuon );

		SingleMuHist_All->Fill( vec_Muon[0] );
		SingleMuHist_All->Fill( vec_Muon[1] );
		SingleMuHist_All->Fill( vec_Muon[2] );
		SingleMuHist_All->Fill( vec_Muon[3] );

		SingleMuHist_1st->Fill( vec_Muon[0] );
		SingleMuHist_2nd->Fill( vec_Muon[1] );
		SingleMuHist_3rd->Fill( vec_Muon[2] );
		SingleMuHist_4th->Fill( vec_Muon[3] );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		GenMuHist_All->Save( f_output );
		GenMuHist_1st->Save( f_output );
		GenMuHist_2nd->Save( f_output );
		GenMuHist_3rd->Save( f_output );
		GenMuHist_4th->Save( f_output );
		
		SingleMuHist_All->Save( f_output );
		SingleMuHist_1st->Save( f_output );
		SingleMuHist_2nd->Save( f_output );
		SingleMuHist_3rd->Save( f_output );
		SingleMuHist_4th->Save( f_output );
	}

	void Init_Hist()
	{
		this->GenMuHist_All = new GenLepHistContainer( "All" );
		this->GenMuHist_1st = new GenLepHistContainer( "1st" );
		this->GenMuHist_2nd = new GenLepHistContainer( "2nd" );
		this->GenMuHist_3rd = new GenLepHistContainer( "3rd" );
		this->GenMuHist_4th = new GenLepHistContainer( "4th" );

		this->SingleMuHist_All = new SingleMuHistContainer( "All" );
		this->SingleMuHist_1st = new SingleMuHistContainer( "1st" );
		this->SingleMuHist_2nd = new SingleMuHistContainer( "2nd" );
		this->SingleMuHist_3rd = new SingleMuHistContainer( "3rd" );
		this->SingleMuHist_4th = new SingleMuHistContainer( "4th" );
	}
};

class Analyzer
{
public:
	TString InputFileName;

	Analyzer( TString _InputFileName )
	{
		this->InputFileName = _InputFileName;
	}

	void Analyze()
	{
		HistContainer* Hists = new HistContainer();

		TChain* chain = new TChain("Delphes");
		chain->Add(this->InputFileName);

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

			TString Type_Channel = this->ChannelType( Br_GenParticle );

			if( Type_Channel == "4m" ) // -- only focus on 4 mu channel only -- //
			{
				// -- generator level, full acceptance -- //
				vector< GenParticle* > vec_GenMu;

				Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
				for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
				{
					GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
					
					if( GenPar->Status == 1 ) // -- final state letpons  -- //
						vec_GenMu.push_back( GenPar );
				}
				
				MyGenPair *GenPair_Phi = new MyGenPair( vec_GenMu[0],vec_GenMu[1] );
				MyGenPair *GenPair_AntiPhi = new MyGenPair( vec_GenMu[2],vec_GenMu[3] );
				Hists->Fill_GEN( GenPair_Phi, GenPair_AntiPhi );


				// -- reconstruction level -- //
				vector< Muon* > vec_Muon;

				Int_t nMuon = Br_Muon->GetEntriesFast();
				// cout << "nMuon: " << nMuon << endl;
				for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
				{
					// cout << i_mu << "th reco muon" << endl;

					Muon* RecoMu = (Muon*)Br_Muon->At(i_mu);
					vec_Muon.push_back( RecoMu );

					// GenParticle* GenMu = (GenParticle*)RecoMu->Particle.GetObject(); // -- corresponding muon at the generator level -- //
					// Int_t motherID = this->GetMotherID( GenMu, Br_GenParticle );
					// if( motherID == 1000600 )
					// {
					// 	vec_GenLepFromPhi.push_back( GenMu );
					// 	vec_MuonFromPhi.push_back( RecoMu );
					// }
					// else if( motherID == -1000600 )
					// {
					// 	vec_GenLepFromAntiPhi.push_back( GenMu );
					// 	vec_MuonFromAntiPhi.push_back( RecoMu );
					// }
				}

				if( (Int_t)vec_Muon.size() == 4 )
				{
					MyMuonPair *Pair_Phi = new MyMuonPair( vec_Muon[0], vec_Muon[1] );
					MyMuonPair *Pair_AntiPhi = new MyMuonPair( vec_Muon[2], vec_Muon[3] );

					Hists->Fill_RECO( Pair_Phi, Pair_AntiPhi );
				}


				// if( (Int_t)vec_MuonFromPhi.size() == 2 && (Int_t)vec_MuonFromAntiPhi.size() == 2 ) // -- all 4 muons are reconstructed -- //
				// {
				// 	MyMuonPair *Pair_Phi = new MyMuonPair( vec_MuonFromPhi[0], vec_MuonFromPhi[1] );
				// 	MyMuonPair *Pair_AntiPhi = new MyMuonPair( vec_MuonFromAntiPhi[0], vec_MuonFromAntiPhi[1] );

				// 	Hists->Fill_RECO( Pair_Phi, Pair_AntiPhi );
				// }
			}
		} // -- end of event iteration -- //

		TString OutputFileName = this->InputFileName;
		OutputFileName.ReplaceAll(".root", "_ValidationPlots.root");
		TFile *f_output = TFile::Open(OutputFileName, "RECREATE");
		f_output->cd();
		Hists->Save( f_output );
	}

	TString ChannelType(TClonesArray *Br_GenParticle)
	{
		TString TStr_Channel = "";

		Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
		Int_t nGenMuon = 0;
		Int_t nGenElectron = 0;
		for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
		{
			GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
			
			if( GenPar->Status == 1 ) // -- final state letpons  -- //
			{
				// Int_t MotherID = this->GetMotherID( GenPar, Br_GenParticle );
				// cout << "MotherID: " << MotherID << endl;

				// if( fabs(MotherID) == 1000600 ) // -- from intermediate particle -- //
				{
					if( fabs(GenPar->PID) == 13)
						nGenMuon++;
					else if( fabs(GenPar->PID) == 11 )
						nGenElectron++;
				}
			}
		}

		if( nGenElectron == 4 && nGenMuon == 0 ) TStr_Channel = "4e";
		else if( nGenElectron == 3 && nGenMuon == 1 ) TStr_Channel = "3e1m";
		else if( nGenElectron == 2 && nGenMuon == 2 ) TStr_Channel = "2e2m";
		else if( nGenElectron == 1 && nGenMuon == 3 ) TStr_Channel = "1e3m";
		else if( nGenElectron == 0 && nGenMuon == 4 ) TStr_Channel = "4m";
		else cout << "Unexpected case: # muon = " << nGenMuon << ", # electron = " << nGenElectron << endl;

		return TStr_Channel;
	}

	void PrintAll_GenParticle(TClonesArray *Br_GenParticle )
	{
		Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
		for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
		{
			GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
			cout << "[" << i_gen << "th gen-particle]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << ", GenPar->M2: " << GenPar->M2 << endl;
		}
		cout << endl;
	}

	Int_t GetMotherID( GenParticle* GenPar, TClonesArray *Br_GenParticle )
	{
		if( GenPar->M1 != GenPar->M2 )
			cout << "WARNING: M1 and M2 is different! this method (GetMotherID) will only check M1" << endl;

		GenParticle* Mother = (GenParticle*)Br_GenParticle->At(GenPar->M1);
		Int_t motherID = Mother->PID;
		return motherID;
	}
};
