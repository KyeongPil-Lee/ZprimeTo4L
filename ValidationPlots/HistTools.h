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

	for(Int_t i_gen=0; i_gen<nGenParticle; i_gen++)
	{
		GenParticle* GenPar = (GenParticle*)Br_GenParticle->At(i_gen);
		if( fabs(GenPar->PID) == 11 || fabs(GenPar->PID) == 13 )
		{
			Int_t ID_Mother1 = GetMotherID( GenPar, Br_GenParticle );
			if( (GenPar->Status == 23 || GenPar->Status == 1) && fabs(ID_Mother1) == 1000600 )
			{
				vec_Lepton_HardProcess.push_back( GenPar );
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