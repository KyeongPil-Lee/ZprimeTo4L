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



class HistContainer
{
public:
	vector<TH1D*> vec_Hist;
	TH1D* h_mass_2l2j;
	// -- single? -- //
	// -- mm, em, ee -- //

	HistContainer()
	{

	}

	void Fill( MyLeptonPair* lepPair, MyJetPair *jetPair, Double_t weight )
	{
		TLorentzVector LVec_2l2j = lepPair->LVec_P + jetPair->LVec_P;
		Double_t mass_2l2j = LVec_2l2j.M();

		this->h_mass_2l2j->Fill( mass_2l2j, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();
		for(const auto& h : this->vec_Hist )
			h->Write();
	}

private:
	void Init()
	{
		this->h_mass_2l2j = new TH1D("h_mass_2l2j", "", 10000, 0, 10000 );
		this->vec_Hist.push_back( this->h_mass_2l2j );
	}
};

class HistProducer
{
public:
	TString sampleName;
	HistContainer *hists;
	vector<TString> vec_DataPath;

	HistProducer( TString _sampleName )
	{
		this->sampleName = _sampleName;
	}

	void AddDataPath( TString _value )
	{
		this->vec_DataPath.push_back( _value );
	}

	void Produce()
	{
		this->hists = new HistContainer();

		TChain* chain = new TChain("Delphes");
		for( const &auto dataPath : this->vec_DataPath )
			chain->Add(dataPath);

		// Create object of class ExRootTreeReader
		ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
		Int_t nTotEvent = treeReader->GetEntries();
		cout << "nTotEvent: " << nTotEvent << endl;

		// Get pointers to branches used in this analysis
		TClonesArray *Br_Event = treeReader->UseBranch("Event");
		TClonesArray *Br_Jet = treeReader->UseBranch("Jet");
		TClonesArray *Br_Muon = treeReader->UseBranch("Muon");
		TClonesArray *Br_Electron = treeReader->UseBranch("Electron");
		TClonesArray *Br_GenParticle = treeReader->UseBranch("Particle");

		// nTotEvent = 10000;
		for(Int_t i_ev = 0; i_ev < nTotEvent; i_ev++)
		{
			treeReader->ReadEntry(i_ev);

			Double_t genWeight = Br_Event->Weight < 0 ? -1 : 1;


			////////////////////////////////
			// -- Reco-level selection -- //
			////////////////////////////////
			vector< MyLepton* > vec_RECOLepton = MakeVector_MyLepton( Br_Electron, Br_Muon );

			MyLeptonPair* lepPair;
			Bool_t isLeptonSelPass = this->LeptonSelection( vec_RECOLepton, lepPair );

			vector< Jet* > vec_Jet;
			Int_t nJet = Br_Jet->GetEntriesFast();
			for(Int_t i_jet=0; i_jet<nJet; i_jet++)
			{
				Jet* RecoJet = (Jet*)Br_Jet->At(i_jet);
				vec_Jet.push_back( RecoJet );
			}

			MyJetPair *jetPair;
			Bool_t isJetSelPass = this->JetSelection( vec_Jet, jetPair );

			if( isLeptonSelPass && isJetSelPass )
			{
				this->hists->Fill( lepPair, jetPair, genWeight );
			}

		} // -- end of event iteration -- //

		TString fileName = TString::Format("ROOTFile_ProdHist_ControlPlots_%s.root", this->type.Data() );
		TFile *f_output = TFile::Open( fileName, "RECREATE" );
		this->hists->Save( f_output );
		f_output->Close();

		cout << "finished" << endl;
	}

private:
	Bool_t LeptonSelection( vector< MyLepton* > vec_RECOLepton, MyLeptonPair*& lepPair )
	{
		Bool_t flag = kFALSE;

		vector< MyLeptonPair* > vec_LeptonPair;

		Int_t nLepton = (Int_t)vec_RECOLepton.size();
		for(Int_t i_lep=0; i_lep<nLepton; i_lep++)
		{
			MyLepton *lepton_ith = vec_RECOLepton[i_lep];
			for(Int_t j_lep=i_lep+1; j_lep<nLepton; j_lep++)
			{
				MyLepton *lepton_jth = vec_RECOLepton[j_lep];

				MyLeptonPair *lepPairTemp = new MyLeptonPair( lepton_ith, lepton_jth );
				Bool_t isAccPass = lepPairTemp->TestAcc( 45, 30, 2.4, 2.4 );
				if( isAccPass )
					vec_LeptonPair.push_back(lepPairTemp);
			}
		}

		Int_t nPair = (Int_t)vec_LeptonPair.size();
		if( nPair > 0 )
		{
			flag = kTRUE;
			if( nPair > 1 )
				std::sort( vec_LeptonPair.begin(), vec_LeptonPair.end(), CompareMyLeptonPair ); // -- highest mass -- //

			lepPair = new MyLeptonPair( vec_LeptonPair[0]->First, vec_LeptonPair[0]->Second );
		}

		return flag;
	}

	Bool_t JetSelection( vector< Jet* > vec_RECOJet, MyJetPair*& jetPair )
	{
		Bool_t flag = kFALSE;

		vector< MyJetPair* > vec_JetPair;

		Int_t nJet = (Int_t)vec_RECOJet.size();
		for(Int_t i_jet=0; i_jet<nJet; i_jet++)
		{
			Jet *jet_ith = vec_RECOJet[i_jet];
			for(Int_t j_jet=i_jet+1; j_jet<nJet; j_jet++)
			{
				Jet *jet_jth = vec_RECOJet[j_jet];

				MyJetPair *jetPairTemp = new MyJetPair( jet_ith, jet_jth );
				Bool_t isAccPass = jetPairTemp->TestAcc( 45, 30, 2.5, 2.5 );
				if( isAccPass )
					vec_JetPair.push_back(jetPairTemp);
			}
		}

		Int_t nPair = (Int_t)vec_JetPair.size();
		if( nPair > 0 )
		{
			flag = kTRUE;
			if( nPair > 1 )
				std::sort( vec_JetPair.begin(), vec_JetPair.end(), CompareMyJetPair ); // -- highest mass -- //

			jetPair = new MyJetPair( vec_JetPair[0]->First, vec_JetPair[0]->Second );
		}

		return flag;
	}

};


void ProdHist_ControlPlots()
{
	HistProducer *producer = new HistProducer("DYJetsToLL");
	TString dataPathBase = gSystem->Getenv("KP_DATA_PATH");
	TString dataPath = dataPathBase+"/Zprime4LSamples/v20171121_v01_2l2j/DYJetsToLL_NLO.root";
	producer->AddDataPath( dataPath );
	producer->produce();
}