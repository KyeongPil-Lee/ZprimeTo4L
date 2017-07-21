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
		cout << "[" << i << "th iteration]" << endl; 
		cout << "\t"; Print_GenParticle( Mother );
		cout << "\t"; Print_GenParticle( Daughter );

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