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
	TH1D* h_leadLeptonPt;
	TH1D* h_subLeptonPt;
	TH1D* h_leadLeptonEta;
	TH1D* h_subLeptonEta;
	TH1D* h_leadLeptonPhi;
	TH1D* h_subLeptonPhi;

	TH1D* h_leadJetPt;
	TH1D* h_subJetPt;
	TH1D* h_leadJetEta;
	TH1D* h_subJetEta;
	TH1D* h_leadJetPhi;
	TH1D* h_subJetPhi;

	// -- single? -- //
	// -- mm, em, ee -- //

	HistContainer()
	{
		this->Init();
	}

	~HistContainer()
	{
		for(const auto& h : this->vec_Hist )
			delete h;
	}

	void Fill( MyLeptonPair* lepPair, MyJetPair *jetPair, Double_t weight )
	{
		TLorentzVector LVec_2l2j = lepPair->LVec_P + jetPair->LVec_P;
		Double_t mass_2l2j = LVec_2l2j.M();

		this->h_mass_2l2j->Fill( mass_2l2j, weight );

		this->h_leadLeptonPt->Fill( lepPair->First->Pt, weight );
		this->h_leadLeptonEta->Fill( lepPair->First->Eta, weight );
		this->h_leadLeptonPhi->Fill( lepPair->First->Phi, weight );

		this->h_subLeptonPt->Fill( lepPair->Second->Pt, weight );
		this->h_subLeptonEta->Fill( lepPair->Second->Eta, weight );
		this->h_subLeptonPhi->Fill( lepPair->Second->Phi, weight );

		this->h_leadJetPt->Fill( jetPair->First->PT, weight );
		this->h_leadJetEta->Fill( jetPair->First->Eta, weight );
		this->h_leadJetPhi->Fill( jetPair->First->Phi, weight );

		this->h_subJetPt->Fill( jetPair->Second->PT, weight );
		this->h_subJetEta->Fill( jetPair->Second->Eta, weight );
		this->h_subJetPhi->Fill( jetPair->Second->Phi, weight );
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

		this->h_leadLeptonPt = new TH1D("h_leadLeptonPt", "", 10000, 0, 10000); this->vec_Hist.push_back( this->h_leadLeptonPt );
		this->h_leadLeptonEta = new TH1D("h_leadLeptonEta", "", 60, -3, 3); this->vec_Hist.push_back( this->h_leadLeptonEta );
		this->h_leadLeptonPhi = new TH1D("h_leadLeptonPhi", "", 80, -4, 4); this->vec_Hist.push_back( this->h_leadLeptonPhi );

		this->h_subLeptonPt = new TH1D("h_subLeptonPt", "", 10000, 0, 10000); this->vec_Hist.push_back( this->h_subLeptonPt );
		this->h_subLeptonEta = new TH1D("h_subLeptonEta", "", 60, -3, 3); this->vec_Hist.push_back( this->h_subLeptonEta );
		this->h_subLeptonPhi = new TH1D("h_subLeptonPhi", "", 80, -4, 4); this->vec_Hist.push_back( this->h_subLeptonPhi );

		this->h_leadJetPt = new TH1D("h_leadJetPt", "", 10000, 0, 10000); this->vec_Hist.push_back( this->h_leadJetPt );
		this->h_leadJetEta = new TH1D("h_leadJetEta", "", 60, -3, 3); this->vec_Hist.push_back( this->h_leadJetEta );
		this->h_leadJetPhi = new TH1D("h_leadJetPhi", "", 80, -4, 4); this->vec_Hist.push_back( this->h_leadJetPhi );

		this->h_subJetPt = new TH1D("h_subJetPt", "", 10000, 0, 10000); this->vec_Hist.push_back( this->h_subJetPt );
		this->h_subJetEta = new TH1D("h_subJetEta", "", 60, -3, 3); this->vec_Hist.push_back( this->h_subJetEta );
		this->h_subJetPhi = new TH1D("h_subJetPhi", "", 80, -4, 4); this->vec_Hist.push_back( this->h_subJetPhi );
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
		cout << "++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "HistProducer::Produce()" << endl;
		cout << "\tSample name = " << this->sampleName << endl;
		for( const auto& dataPath : this->vec_DataPath )
			cout << "\tData path = " << dataPath << endl;

		this->hists = new HistContainer();

		TChain* chain = new TChain("Delphes");
		for( const auto& dataPath : this->vec_DataPath )
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

		Double_t sumWeight = 0;
		// nTotEvent = 10000;
		for(Int_t i_ev = 0; i_ev < nTotEvent; i_ev++)
		{
			treeReader->ReadEntry(i_ev);

			HepMCEvent *event = (HepMCEvent*)Br_Event->At(0);
			Double_t genWeight = event->Weight < 0 ? -1 : 1;
			sumWeight += genWeight;

			// printf("[%d event] weight = %lf\n", i_ev, event->Weight );

			////////////////////////////////
			// -- Reco-level selection -- //
			////////////////////////////////
			vector< MyLepton* > vec_RECOLepton = MakeVector_MyLepton( Br_Electron, Br_Muon );

			MyLeptonPair* lepPair = NULL;
			Bool_t isLeptonSelPass = this->LeptonSelection( vec_RECOLepton, lepPair );
			// if( isLeptonSelPass )
			// 	printf("[LeptonPair] (M, pt, rap) = (%lf, %lf, %lf)]\n", lepPair->M, lepPair->Pt, lepPair->Rap);

			vector< Jet* > vec_Jet;
			Int_t nJet = Br_Jet->GetEntriesFast();
			for(Int_t i_jet=0; i_jet<nJet; i_jet++)
			{
				Jet* RecoJet = (Jet*)Br_Jet->At(i_jet);
				vec_Jet.push_back( RecoJet );
			}

			MyJetPair *jetPair = NULL;
			Bool_t isJetSelPass = this->JetSelection( vec_Jet, jetPair );
			// if( isJetSelPass )
			// 	printf("[JetPair] (M, pt, rap) = (%lf, %lf, %lf)]\n", jetPair->M, jetPair->Pt, jetPair->Rap);

			if( isLeptonSelPass && isJetSelPass )
				this->hists->Fill( lepPair, jetPair, genWeight );

		} // -- end of event iteration -- //

		TString fileName = TString::Format("ROOTFile_ProdHist_ControlPlots_%s.root", this->sampleName.Data() );
		TFile *f_output = TFile::Open( fileName, "RECREATE" );
		this->hists->Save( f_output );
		f_output->Close();

		delete this->hists;
		this->hists = NULL;

		printf("[Total sum(weight) = %lf]\n", sumWeight);

		cout << "HistProducer::Produce() is completed" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++\n" << endl;
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
	TString dataPathBase = gSystem->Getenv("KP_DATA_PATH");

	vector<TString> vec_SampleName;
	vector<TString> vec_DataPath;

	vec_SampleName.push_back("DYJetsToLL"); vec_DataPath.push_back(dataPathBase+"/Zprime4LSamples/v20171121_v01_2l2j/DYJetsToLL_NLO.root");
	vec_SampleName.push_back("WJetsTolnu"); vec_DataPath.push_back(dataPathBase+"/Zprime4LSamples/v20171121_v01_2l2j/WJetsTolnu_NLO.root");
	vec_SampleName.push_back("WZTo2l2j"); vec_DataPath.push_back(dataPathBase+"/Zprime4LSamples/v20171121_v01_2l2j/WZTo2l2j_NLO.root");
	vec_SampleName.push_back("ZZTo2l2j"); vec_DataPath.push_back(dataPathBase+"/Zprime4LSamples/v20171121_v01_2l2j/ZZTo2l2j_NLO.root");

	Int_t nSample = (Int_t)vec_SampleName.size();

	for(Int_t i=0; i<nSample; i++)
	{
		TString sampleName = vec_SampleName[i];
		TString dataPath = vec_DataPath[i];

		HistProducer *producer = new HistProducer(sampleName);
		producer->AddDataPath( dataPath );
		producer->Produce();

		delete producer;
	}
}