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

class Analyzer
{
public:
	TString SamplePath;
	TString SampleType;
	Bool_t Flag_IsSignal;

	Analyzer( TString _SamplePath, TString _SampleType )
	{
		this->SamplePath = _SamplePath;
		this->SampleType = _SampleType;
		Flag_IsSignal = kFALSE; // -- initial value -- //
	}

	void IsSignal( Bool_t flag )
	{
		this->Flag_IsSignal = flag;
	}

	void Analyze()
	{
		TChain* chain = new TChain("Delphes");
		chain->Add(this->SamplePath);

		// Create object of class ExRootTreeReader
		ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
		Int_t nTotEvent = treeReader->GetEntries();
		cout << "nTotEvent: " << nTotEvent << endl;

		// Get pointers to branches used in this analysis
		TClonesArray *Br_Muon = treeReader->UseBranch("Muon");
		TClonesArray *Br_Electron = treeReader->UseBranch("Electron");
		TClonesArray *Br_GenParticle = treeReader->UseBranch("Particle");

		Int_t nEvent_WithZprime = 0;
		Int_t nEvent_WithoutZprime = 0;
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

			vector< GenParticle* > vec_Lepton_HardProcess_temp;
			Bool_t isFound_Zprime = kFALSE;
			Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
			for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
			{
				GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
				if( GenPar->PID == 1000623) // -- Z' -- //
				{
					isFound_Zprime = kTRUE;
				}
			}

			if( isFound_Zprime ) nEvent_WithZprime++;
			else
			{
				nEvent_WithoutZprime++;

				cout << "Event with no Z': ChannelType = " << TStr_Channal << endl;
				for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
				{
					GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
					if( fabs(GenPar->PID) == 1000600 && GenPar->Status == 22 ) // -- phi, hard process -- //
					{
						Int_t MotherID = GetMotherID( GenPar, Br_GenParticle );
						cout << "\tPhi (" << GenPar->PID << "): Mother ID = " << MotherID << endl;
					}
				}
			}

		} // -- end of event iteration -- //

		cout << "# events with Z': " << nEvent_WithZprime << endl;
		cout << "# events without Z': " << nEvent_WithoutZprime << endl;
		cout << "# total events: " << nEvent_WithoutZprime + nEvent_WithZprime << endl;
	}
};

void GENCheck_Zprime( TString SamplePath, TString SampleType, Bool_t Flag_IsSignal )
{
	Analyzer *analyzer = new Analyzer(SamplePath, SampleType);
	analyzer->IsSignal( Flag_IsSignal );
	analyzer->Analyze();
}
