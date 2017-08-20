Bool_t CompareGenParticle( GenParticle* Object1, GenParticle* Object2 )
{
	return Object1->PT > Object2->PT;
}

Bool_t CompareMuon( Muon* Muon1, Muon* Muon2 )
{
	return Muon1->PT > Muon2->PT;
}

Bool_t CompareElectron( Electron* Electron1, Electron* Electron2 )
{
	return Electron1->PT > Electron2->PT;
}

Bool_t CompareMyLepton( MyLepton* MyLepton1, MyLepton* MyLepton2 )
{
	return MyLepton1->Pt > MyLepton2->Pt;
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


class MyLepton
{
public:
	Double_t Pt;
	Double_t Eta;
	Double_t Phi;
	TLorentzVector LVec_P;
	Double_t Charge;
	Double_t RelIso;

	Bool_t isMu;
	Muon *Mu;

	Bool_t isElec;
	Electron *Elec;
	GenParticle* GenLepton;

	MyLepton()
	{
		this->Pt = 0;
		this->Eta = 0;
		this->Phi = 0;
		this->LVec_P.SetPxPyPzE(0,0,0,0);

		this->Charge = -999;
		this->RelIso = -1;

		this->isMu = kFALSE;
		this->Mu = NULL;

		this->isElec = kFALSE;
		this->Elec = NULL;

		this->GenLepton = NULL;
	}

	MyLepton( Muon *_Muon ) : MyLepton()
	{
		this->Pt = _Muon->PT;
		this->Eta = _Muon->Eta;
		this->Phi = _Muon->Phi;
		this->LVec_P = _Muon->P4();

		this->Charge = _Muon->Charge;
		this->RelIso = _Muon->IsolationVar;

		this->isMu = kTRUE;
		this->Mu = _Muon;
		this->GenLepton = (GenParticle*)_Muon->Particle.GetObject();
	}

	MyLepton( Electron *_Electron ) : MyLepton()
	{
		this->Pt = _Electron->PT;
		this->Eta = _Electron->Eta;
		this->Phi = _Electron->Phi;
		this->LVec_P = _Electron->P4();

		this->Charge = _Electron->Charge;
		this->RelIso = _Electron->IsolationVar;


		this->isElec = kTRUE;
		this->Elec = _Electron;
		this->GenLepton = (GenParticle*)_Electron->Particle.GetObject();
	}

	Muon* GetMuon() { return Mu; }
	Electron* GetElectron() { return Elec; }
};

Bool_t CompareLepton( MyLepton* Lepton1, MyLepton* Lepton2 )
{
	return Lepton1->Pt > Lepton2->Pt;
}

class MyLeptonPair
{
public:
	MyLepton *First;
	MyLepton *Second;

	Double_t M;
	Double_t Pt;
	Double_t Rap;
	TLorentzVector LVec_P;

	Bool_t Flag_SameFlavor;
	Bool_t Flag_OS;

	MyGenPair* GENPair_postFSR;


	MyLeptonPair( MyLepton* Lepton1, MyLepton* Lepton2 )
	{
		if( Lepton1->Pt > Lepton2->Pt )
		{
			this->First = Lepton1;
			this->Second = Lepton2;
		}
		else
		{
			this->First = Lepton2;
			this->Second = Lepton1;
		}

		this->Calc_Var();
	}

	void Calc_Var()
	{
		TLorentzVector LVec_First = First->LVec_P;
		TLorentzVector LVec_Second = Second->LVec_P;
		this->LVec_P = LVec_First + LVec_Second;

		this->M = this->LVec_P.M();
		this->Pt = this->LVec_P.Pt();
		this->Rap = this->LVec_P.Rapidity();

		this->Flag_OS = kFALSE;
		if( First->Charge != Second->Charge ) this->Flag_OS = kTRUE;

		this->Flag_SameFlavor = kFALSE;
		if( (First->isMu && Second->isMu) || (First->isElec && Second->isElec) ) this->Flag_SameFlavor = kTRUE;

		// -- matched Gen-pair @ post-FSR level (final state) -- //
		GenParticle* GenLepton1 = this->First->GenLepton;
		GenParticle* GenLepton2 = this->Second->GenLepton;
		this->GENPair_postFSR = new MyGenPair( GenLepton1, GenLepton2 );
	}
};

class My4LeptonPair
{
public:
	vector< MyLepton* > vec_Lepton;
	MyLepton *First;
	MyLepton *Second;
	MyLepton *Third;
	MyLepton *Fourth;

	Double_t M;
	Double_t Pt;
	Double_t Rap;
	TLorentzVector LVec_P;

	TString FlavorType;

	My4LeptonPair( MyLepton* Lepton1, MyLepton* Lepton2, MyLepton* Lepton3, MyLepton* Lepton4 )
	{
		this->vec_Lepton.push_back( Lepton1 );
		this->vec_Lepton.push_back( Lepton2 );
		this->vec_Lepton.push_back( Lepton3 );
		this->vec_Lepton.push_back( Lepton4 );

		std::sort( vec_Lepton.begin(), vec_Lepton.end(), CompareLepton );
		this->First = vec_Lepton[0];
		this->Second = vec_Lepton[1];
		this->Third = vec_Lepton[2];
		this->Fourth = vec_Lepton[3];

		this->Calc_Var();
	}

	void Calc_Var()
	{
		TLorentzVector LVec_First = First->LVec_P;
		TLorentzVector LVec_Second = Second->LVec_P;
		TLorentzVector LVec_Third = Third->LVec_P;
		TLorentzVector LVec_Fourth = Fourth->LVec_P;
		this->LVec_P = LVec_First + LVec_Second + LVec_Third + LVec_Fourth;

		this->M = this->LVec_P.M();
		this->Pt = this->LVec_P.Pt();
		this->Rap = this->LVec_P.Rapidity();

		// -- decide the flavor type: ex> 4m -- //
		Int_t nMu = 0;
		Int_t nElec = 0;
		for(const auto & lepton : this->vec_Lepton )
		{
			if( lepton->isMu ) nMu++;
			if( lepton->isElec ) nElec++;
		}
		if( nMu == 4 ) this->FlavorType = "4m";
		else if( nMu == 3 && nElec == 1) this->FlavorType = "1e3m";
		else if( nMu == 2 && nElec == 2) this->FlavorType = "2e2m";
		else if( nMu == 1 && nElec == 3) this->FlavorType = "3e2m";
		else if( nElec == 4) this->FlavorType = "4e";
		else
		{
			cout << "# mu: " << nMu << ", # elec: " << nElec << " -> there's no corresponding flavor type" << endl;
			this->FlavorType = "None";
		}
	}

	Bool_t Test_Acc( Double_t PtCut_1st, PtCut_2nd, PtCut_3rd, PtCut_4th, EtaCut_1st, EtaCut_2nd, EtaCut_3rd, EtaCut_4th )
	{
		Bool_t Flag_PassAcc = kFALSE;
		if( this->First->Pt > PtCut_1st && fabs(this->First->Eta) < EtaCut_1st &&
			this->Second->Pt > PtCut_2nd && fabs(this->Second->Eta) < EtaCut_2nd &&
			this->Third->Pt > PtCut_3rd && fabs(this->Third->Eta) < EtaCut_3rd &&
			this->Fourth->Pt > PtCut_4th && fabs(this->Fourth->Eta) < EtaCut_4th
			)
			Flag_PassAcc = kTRUE;

		return Flag_PassAcc;
	}

	Bool_t Test_PassZVeto( Double_t ZMass_min = 89, Double_t ZMass_max = 93 )
	{
		Bool_t Flag_PassZVeto = kTRUE; // -- default: true, and it will be changed if a lepton pair close to Z mass is found -- //

		// -- search for all possible pair -- //
		for(Int_t i_lep=0; i_lep<nLepton; i_lep++)
		{
			MyLepton *Lepton_ith = this->vec_Lepton[i_lep];
			for(Int_t j_lep=i_lep+1, j_lep<nLepton; j_lep++)
			{
				MyLepton* Lepton_jth = this->vec_Lepton[j_lep];

				MyLeptonPair *LepPair = new MyLeptonPair( Lepton_ith[0], Lepton_jth[1] );
				if( LepPair->Flag_SameFlavor && LepPair->Flag_OS ) // -- only check same flavor, opposite sign pair -- //
				{
					if( LepPair->M > ZMass_min && LepPair->M < ZMass_max ) Flag_PassZVeto = kFALSE;
				}
			}
		}

		return Flag_PassZVeto;
	}

	Bool_t Test_Isolation( Double_t IsoCut )
	{
		Bool_t Flag_PassIso = kFALSE;
		if( this->First->RelIso < IsoCut &&
			this->Second->RelIso < IsoCut &&
			this->Third->RelIso < IsoCut &&
			this->Fourth->RelIso < IsoCut )
			Flag_PassIso = kTURE;

		return Flag_PassIso;
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

	MyGenPair* GENPair_postFSR;


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

		// -- matched Gen-pair @ post-FSR level (final state) -- //
		GenParticle* GenMu1 = (GenParticle*)this->First->Particle.GetObject();
		GenParticle* GenMu2 = (GenParticle*)this->Second->Particle.GetObject();
		this->GENPair_postFSR = new MyGenPair( GenMu1, GenMu2 );
	}
};

class HistContainer_4GENLepton
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;

	HistContainer_4GENLepton(TString _Type)
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
		this->h_Pt = new TH1D("h_Pt_4Lepton_GEN_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_4Lepton_GEN_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_4Lepton_GEN_"+this->Type, "", 10000, 0, 10000);
	}
};

class HistContainer_4Lepton
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;
	TH1D* h_RelDiff_Mass;

	HistContainer_4Lepton(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( MyLeptonPair* LeptonPair1, MyLeptonPair* LeptonPair2, Double_t weight = 1.0 )
	{
		TLorentzVector LVec_P_4Lep = LeptonPair1->LVec_P + LeptonPair2->LVec_P;

		Double_t Pt = LVec_P_4Lep.Pt();
		Double_t Rap = LVec_P_4Lep.Rapidity();
		Double_t Mass = LVec_P_4Lep.M();

		Double_t Mass_GEN = (LeptonPair1->GENPair_postFSR->LVec_P + LeptonPair2->GENPair_postFSR->LVec_P).M();
		Double_t RelDiff_Mass = (Mass - Mass_GEN) / Mass_GEN;

		this->h_Pt->Fill( Pt, weight );
		this->h_Rap->Fill( Rap, weight );
		this->h_Mass->Fill( Mass, weight );
		this->h_RelDiff_Mass->Fill( RelDiff_Mass, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Rap->Write();
		this->h_Mass->Write();
		this->h_RelDiff_Mass->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_4Lepton_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_4Lepton_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_4Lepton_"+this->Type, "", 10000, 0, 10000);
		this->h_RelDiff_Mass = new TH1D("h_RelDiff_Mass_4Lepton_"+this->Type, "", 1000, -1, 1);
	}
};

class HistContainer_GENLeptonPair
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;

	HistContainer_GENLeptonPair(TString _Type)
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
		this->h_Pt = new TH1D("h_Pt_2Lepton_GEN_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_2Lepton_GEN_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_2Lepton_GEN_"+this->Type, "", 10000, 0, 10000);
	}
};

class HistContainer_LeptonPair
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Rap;
	TH1D* h_Mass;
	TH1D* h_RelDiff_Mass;

	HistContainer_LeptonPair(TString _Type)
	{
		this->Type = _Type;
		this->Init_Hist();
	}

	void Fill( MyLeptonPair* LeptonPair, Double_t weight = 1.0 )
	{
		Double_t Pt = LeptonPair->Pt;
		Double_t Rap = LeptonPair->Rap;
		Double_t Mass = LeptonPair->M;

		Double_t Mass_GEN = LeptonPair->GENPair_postFSR->M;
		Double_t RelDiff_Mass = (Mass - Mass_GEN ) / Mass_GEN; // -- difference with "post-FSR" gen-mass -- //

		this->h_Pt->Fill( Pt, weight );
		this->h_Rap->Fill( Rap, weight );
		this->h_Mass->Fill( Mass, weight );
		this->h_RelDiff_Mass->Fill( RelDiff_Mass, weight );
	}

	void Save( TFile *f_output )
	{
		f_output->cd();

		this->h_Pt->Write();
		this->h_Rap->Write();
		this->h_Mass->Write();
		this->h_RelDiff_Mass->Write();
	}

	void Init_Hist()
	{
		this->h_Pt = new TH1D("h_Pt_2Lepton_"+this->Type, "", 10000, 0, 10000);
		this->h_Rap = new TH1D("h_Rap_2Lepton_"+this->Type, "", 200, -10, 10);
		this->h_Mass = new TH1D("h_Mass_2Lepton_"+this->Type, "", 10000, 0, 10000);
		this->h_RelDiff_Mass = new TH1D("h_RelDiff_Mass_2Lepton_"+this->Type, "", 1000, -1, 1);
	}
};

class HistContainer_GENLepton
{
public:
	TString Type;

	TH1D* h_Pt;
	TH1D* h_Eta;
	TH1D* h_Phi;

	HistContainer_GENLepton(TString _Type)
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


class HistContainer_Lepton
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

	HistContainer_Lepton( TString _Type )
	{
		this->Type = _Type;

		this->Init_Hist();
	}

	void Fill( MyLepton *Lepton, Double_t weight = 1.0 )
	{
		Double_t Pt_RECO = Lepton->Pt;
		Double_t Eta_RECO = Lepton->Eta;
		Double_t Phi_RECO = Lepton->Phi;

		GenParticle* GenLepton = Lepton->GenLepton;
		Double_t Pt_GEN = GenLepton->PT;
		Double_t Eta_GEN = GenLepton->Eta;
		Double_t Phi_GEN = GenLepton->Phi;

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


void Print_GenParticle( GenParticle* GenPar )
{
	printf("[ID=%03d, Status=%02d]: (M1, M2) = (%03d, %03d), (D1, D2) = (%03d, %03d), (Pt, eta, phi) = (%9.3lf, %9.3lf, %9.3lf)\n",
		GenPar->PID, GenPar->Status, GenPar->M1, GenPar->M2, GenPar->M1, GenPar->M2, GenPar->PT, GenPar->Eta, GenPar->Phi);
}

void PrintAll_GenParticle(TClonesArray *Br_GenParticle )
{
	Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
	cout << "[nGenParticle = " << nGenParticle << "]" << endl;
	for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
	{
		GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		cout << "[" << i_gen << "th gen-particle]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << ", GenPar->M2: " << GenPar->M2 
		<< ", GenPar->D1: " << GenPar->D1 << ", GenPar->D2: " << GenPar->D2 << endl;
	}
	cout << endl;
}

void PrintAll_GenMuon(TClonesArray *Br_GenParticle )
{
	Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
	cout << "[nGenParticle = " << nGenParticle << "]" << endl;
	for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
	{
		GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		if( fabs(GenPar->PID) == 13 )
		cout << "[" << i_gen << "th gen-particle]: ID = " << GenPar->PID << ", status = " << GenPar->Status << ", GenPar->M1: " << GenPar->M1 << ", GenPar->M2: " << GenPar->M2 << endl;
	}
	cout << endl;
}

Int_t GetMotherID( GenParticle* GenPar, TClonesArray *Br_GenParticle )
{
	if( GenPar->M1 != GenPar->M2 && GenPar->M2 > 0 )
		cout << "WARNING: M1 and M2 is different!: M1 = " << GenPar->M1 << ", M2 = " << GenPar->M2 << ". this method (GetMotherID) will only check M1" << endl;

	Int_t motherID = -1;
	if( GenPar->M1 < 0 )
		cout << "GenPar->M1 = " << GenPar->M1 << " < 0" << endl;
	else
	{
		GenParticle* Mother = (GenParticle*)Br_GenParticle->At(GenPar->M1);
		motherID = Mother->PID;
	}
	return motherID;
}

TString ChannelType_Signal(TClonesArray *Br_GenParticle, vector< GenParticle* >& vec_Lepton_HardProcess)
{
	TString TStr_Channel = "";

	Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
	Int_t nGenMuon = 0;
	Int_t nGenElectron = 0;

	vector< GenParticle* > vec_Lepton_HardProcess_temp;
	for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
	{
		GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		if( fabs(GenPar->PID) == 11 || fabs(GenPar->PID) == 13 )
		{
			Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
			if( (GenPar->Status == 23 || GenPar->Status == 1) && fabs(ID_Mother1) == 1000600 )
			{
				vec_Lepton_HardProcess_temp.push_back( GenPar );
				if( fabs(GenPar->PID) == 13)
					nGenMuon++;
				else if( fabs(GenPar->PID) == 11 )
					nGenElectron++;
			}
		}
	}

	// -- re-ordering: ([0] and [1]) -> from 1000600, ([2] and [3]) -> from -1000600 -- // 
	vector< GenParticle* > vec_FromPhi;
	vector< GenParticle* > vec_FromAntiPhi;
	for( const auto& GenPar : vec_Lepton_HardProcess_temp )
	{
		Int_t MotherID = GetMotherID( GenPar, Br_GenParticle );
		if( MotherID == 1000600 )
			vec_FromPhi.push_back( GenPar );
		else if( MotherID == -1000600 )
			vec_FromAntiPhi.push_back( GenPar );
	}

	vec_Lepton_HardProcess.push_back( vec_FromPhi[0] );
	vec_Lepton_HardProcess.push_back( vec_FromPhi[1] );
	vec_Lepton_HardProcess.push_back( vec_FromAntiPhi[0] );
	vec_Lepton_HardProcess.push_back( vec_FromAntiPhi[1] );

	if( nGenElectron == 4 && nGenMuon == 0 ) TStr_Channel = "4e";
	else if( nGenElectron == 3 && nGenMuon == 1 ) TStr_Channel = "3e1m";
	else if( nGenElectron == 2 && nGenMuon == 2 ) TStr_Channel = "2e2m";
	else if( nGenElectron == 1 && nGenMuon == 3 ) TStr_Channel = "1e3m";
	else if( nGenElectron == 0 && nGenMuon == 4 ) TStr_Channel = "4m";
	else cout << "Unexpected case: # muon = " << nGenMuon << ", # electron = " << nGenElectron << endl;

	return TStr_Channel;
}

TString ChannelType_Bkg(TClonesArray *Br_GenParticle, vector< GenParticle* >& vec_Lepton_HardProcess)
{
	TString TStr_Channel = "";

	Int_t nGenParticle = Br_GenParticle->GetEntriesFast();
	Int_t nGenMuon = 0;
	Int_t nGenElectron = 0;

	for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
	{
		GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		if( fabs(GenPar->PID) == 11 || fabs(GenPar->PID) == 13 )
		{
			// -- 23: from hard process -- //
			// -- 1: maybe hard process (23 && 1 -> 1 ), but not always (ex> from hadron decays) -- //
			if( GenPar->Status == 1 || GenPar->Status == 23 ) 
			{
				GenParticle* Mother1 = (GenParticle*)Br_GenParticle->At(GenPar->M1);
				Int_t Status_Mother1 = Mother1->Status;
				Int_t ID_Mother1 = Mother1->PID;

				Int_t Status_Mother2 = -1;
				if( GenPar->M2 > 0 )
				{
					GenParticle* Mother2 = (GenParticle*)Br_GenParticle->At(GenPar->M2);
					Status_Mother2 = Mother2->Status;
				}
				else
					Status_Mother2 = -1;

				// -- (Mother1->Status == 21 && Mother2->Status == 21): 
				//    from virtual. mothers should be hardest incoming particles -- //
				// -- Mother1->PID == 23: from Z -- //
				if( (Status_Mother1 == 21 && Status_Mother2 == 21) || ID_Mother1 == 23 )
				{
					vec_Lepton_HardProcess.push_back( GenPar );
					if( fabs(GenPar->PID) == 13)
						nGenMuon++;
					else if( fabs(GenPar->PID) == 11 )
						nGenElectron++;
				}
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

GenParticle* Find_FinalStateLepton( GenParticle* GenPar, TClonesArray *Br_GenParticle )
{
	if( GenPar->Status == 1 )
	{
		cout << "It is already final state!" << endl;
		return GenPar;
	}

	GenParticle* Mother = GenPar;
	GenParticle* Daughter = (GenParticle*)Br_GenParticle->At(GenPar->D1);

	Int_t i=0;
	while( true )
	{
		// cout << "[" << i << "th iteration]" << endl; 
		// cout << "\t"; Print_GenParticle( Mother );
		// cout << "\t"; Print_GenParticle( Daughter );

		if( Daughter->Status == 1 ) // -- find the corresponding final state! -- //
			break;
		else
		{
			Mother = Daughter;
			Daughter = (GenParticle*)Br_GenParticle->At(Mother->D1);
		}

		if( i > 100 )
		{
			cout << "i becomes larger than 100!" << endl;
			cout << "Someting goes wrong!!! ... kill the loop" << endl;
			break;
		}

		i++;
	}

	return Daughter;
}

void Find_FinalStateLepton_All( vector< GenParticle* > vec_HP, TClonesArray *Br_GenParticle, vector< GenParticle* >& vec_FS )
{
	for( const auto& GenPar_HP : vec_HP )
	{
		if( GenPar_HP->Status == 1 )
			vec_FS.push_back( GenPar_HP );
		else
		{
			GenParticle* GenPar_FS = Find_FinalStateLepton( GenPar_HP, Br_GenParticle );
			vec_FS.push_back( GenPar_FS );
		}
	}
}

Bool_t Pass_CMSAcc( vector<GenParticle*> vec_Lepton_FinalState, Double_t PtMin = 10, Double_t EtaMax = 2.4 )
{
	Bool_t Flag_PassAcc = kTRUE;
	for( const auto& GenPar : vec_Lepton_FinalState )
	{
		if( !(GenPar->PT > PtMin && fabs(GenPar->Eta) < EtaMax) )
		{
			Flag_PassAcc = kFALSE;
			break;
		}
	}

	return Flag_PassAcc;
}

Bool_t IsSame_GenParticle( GenParticle* GenPar1, GenParticle *GenPar2 )
{
	Bool_t Flag = kFALSE;
	if( (GenPar1->Px == GenPar2->Px) &&
		(GenPar1->Py == GenPar2->Py) &&
		(GenPar1->Pz == GenPar2->Pz) &&
		(GenPar1->E == GenPar2->E) &&
		(GenPar1->Status == GenPar2->Status) &&
		(GenPar1->M1 == GenPar2->M1) &&
		(GenPar1->M2 == GenPar2->M2) ) Flag = kTRUE;

	return Flag;
}

Bool_t Matching_RECO_GENMuonHPFS( vector<GenParticle*> vec_Lepton_FinalState, TClonesArray* Br_Muon, vector< Muon* >& vec_MuonMatched )
{
	Bool_t Flag_Success = kFALSE;

	Int_t nMuon = Br_Muon->GetEntriesFast();

	for( const auto& GenMu_FS : vec_Lepton_FinalState )
	{
		// printf("[GenMu_FS] (Pt, Eta, Phi) = (%9.3lf, %9.3lf, %9.3lf)\n", GenMu_FS->PT, GenMu_FS->Eta, GenMu_FS->Phi);
		for(Int_t i_mu=0; i_mu<nMuon; i_mu++)
		{
			// cout << i_mu << "th reco muon" << endl;

			Muon* RecoMu = (Muon*)Br_Muon->At(i_mu);
			GenParticle* GenMu = (GenParticle*)RecoMu->Particle.GetObject();
			// printf("\t[GenMu matched to %02d muon] (Pt, Eta, Phi) = (%9.3lf, %9.3lf, %9.3lf)", i_mu, GenMu->PT, GenMu->Eta, GenMu->Phi);

			// Bool_t isMatched = kFALSE;
			// if( *(GenMu_FS) == *(GenMu) )
			if( IsSame_GenParticle(GenMu_FS, GenMu) )
			{
				// isMatched = kTRUE;
				vec_MuonMatched.push_back( RecoMu );
				break;
			}

			// cout << "-> isMatched = " << isMatched << endl;

		} // -- end of reco-lepton iteration -- //

	} // -- end of gen-lepton iteration -- //

	// -- check whether "ALL" of muons are matched to the final state gen-leptons from hard process -- //
	if( vec_MuonMatched.size() == vec_Lepton_FinalState.size() )
		Flag_Success = kTRUE;

	return Flag_Success;
}

Bool_t Matching_RECO_GENMuonHPFS( vector<GenParticle*> vec_Lepton_FinalState, vector< MyLepton* > vec_RECOLepton, vector< MyLepton* >& vec_LeptonMatched )
{
	Bool_t Flag_Success = kFALSE;

	Int_t nRECOLepton = (Int_t)vec_RECOLepton.size();

	for( const auto& GenMu_FS : vec_Lepton_FinalState )
	{
		// printf("[GenMu_FS] (Pt, Eta, Phi) = (%9.3lf, %9.3lf, %9.3lf)\n", GenMu_FS->PT, GenMu_FS->Eta, GenMu_FS->Phi);
		for(Int_t i_lep=0; i_lep<nRECOLepton; i_lep++)
		{
			// cout << i_lep << "th reco muon" << endl;

			MyLepton* RECOLepton = vec_RECOLepton[i_lep];
			GenParticle* GENLepton = RECOLepton->GenLepton;

			// Muon* RecoMu = (Muon*)Br_Muon->At(i_lep);
			// GenParticle* GenMu = (GenParticle*)RecoMu->Particle.GetObject();
			// printf("\t[GenMu matched to %02d muon] (Pt, Eta, Phi) = (%9.3lf, %9.3lf, %9.3lf)", i_mu, GenMu->PT, GenMu->Eta, GenMu->Phi);

			// Bool_t isMatched = kFALSE;
			// if( *(GenMu_FS) == *(GenMu) )
			if( IsSame_GenParticle(GenMu_FS, GENLepton) )
			{
				// isMatched = kTRUE;
				vec_LeptonMatched.push_back( RECOLepton );
				break;
			}

			// cout << "-> isMatched = " << isMatched << endl;

		} // -- end of reco-lepton iteration -- //

	} // -- end of gen-lepton iteration -- //

	// -- check whether "ALL" of muons are matched to the final state gen-leptons from hard process -- //
	if( vec_LeptonMatched.size() == vec_Lepton_FinalState.size() )
		Flag_Success = kTRUE;

	return Flag_Success;
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
