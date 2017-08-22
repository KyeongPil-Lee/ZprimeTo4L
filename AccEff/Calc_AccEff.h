#include "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/Include/PlotTools.h"

class SampleInfo
{
public:
	TString TStr_Version;
	Bool_t IsSignal;

	Int_t MZp;
	Int_t Msn3;

	TString SampleName;

	Double_t Acc[5];
	Double_t AccErr[5];
	Double_t Eff[5];
	Double_t EffErr[5];

	SampleInfo( TString _TStr_Version, Bool_t _IsSignal, Int_t _MZp = 0, Int_t _Msn3 = 0)
	{
		this->TStr_Version = _TStr_Version;
		this->IsSignal = _IsSignal;
		this->MZp = _MZp;
		this->Msn3 = _Msn3;

		this->Set();
	}

	Double_t GetValue( TString ValueType, TString TStr_Channel )
	{
		if( ValueType == "Acc" )
		{
			if( TStr_Channel == "4e" ) return Acc[0];
			if( TStr_Channel == "3e1m" ) return Acc[1];
			if( TStr_Channel == "2e2m" ) return Acc[2];
			if( TStr_Channel == "1e3m" ) return Acc[3];
			if( TStr_Channel == "4m" ) return Acc[4];
		}
		if( ValueType == "AccErr" )
		{
			if( TStr_Channel == "4e" ) return AccErr[0];
			if( TStr_Channel == "3e1m" ) return AccErr[1];
			if( TStr_Channel == "2e2m" ) return AccErr[2];
			if( TStr_Channel == "1e3m" ) return AccErr[3];
			if( TStr_Channel == "4m" ) return AccErr[4];
		}
		if( ValueType == "Eff" )
		{
			if( TStr_Channel == "4e" ) return Eff[0];
			if( TStr_Channel == "3e1m" ) return Eff[1];
			if( TStr_Channel == "2e2m" ) return Eff[2];
			if( TStr_Channel == "1e3m" ) return Eff[3];
			if( TStr_Channel == "4m" ) return Eff[4];
		}
		if( ValueType == "EffErr" )
		{
			if( TStr_Channel == "4e" ) return EffErr[0];
			if( TStr_Channel == "3e1m" ) return EffErr[1];
			if( TStr_Channel == "2e2m" ) return EffErr[2];
			if( TStr_Channel == "1e3m" ) return EffErr[3];
			if( TStr_Channel == "4m" ) return EffErr[4];
		}

		return 0;
	}

protected:
	void Set()
	{
		if( this->IsSignal )
			this->SampleName = TString::Format("MZp_%d_Msn3_%d_200k", this->MZp, Msn3);
		else
			this->SampleName = "ZZto4L0j_1M";

		this->AccEff( "4e", Acc[0], AccErr[0], Eff[0], EffErr[0] );
		this->AccEff( "3e1m", Acc[1], AccErr[1], Eff[1], EffErr[1] );
		this->AccEff( "2e2m", Acc[2], AccErr[2], Eff[2], EffErr[2] );
		this->AccEff( "1e3m", Acc[3], AccErr[3], Eff[3], EffErr[3] );
		this->AccEff( "4m", Acc[4], AccErr[4], Eff[4], EffErr[4] );
	}

	void AccEff( TString TStr_Channel, Double_t& Acc, Double_t &AccErr, Double_t &Eff, Double_t &EffErr)
	{
		TString FileName = TString::Format("Local/%s/%s/ROOTFile_AccEff_%s.root", this->TStr_Version.Data(), SampleName.Data(), SampleName.Data());
		TString HistName_Base = TString::Format("h_%s", TStr_Channel.Data());
		Double_t nAccTotal = GetContent_OneBinHist( FileName, HistName_Base+"_AccTotal");
		Double_t nAccPass = GetContent_OneBinHist( FileName, HistName_Base+"_AccPass");
		Double_t nEffPass = GetContent_OneBinHist( FileName, HistName_Base+"_EffPass");

		if( nAccTotal != 0 )
		{
			Acc = nAccPass / nAccTotal;
			AccErr = Acc*this->Calc_RelUnc( nAccPass, nAccTotal ); // -- abs error -- //
		}
		else
		{
			Acc = 0;
			AccErr = 0;
		}

		if( nAccPass != 0 )
		{
			Eff = nEffPass / nAccPass;
			EffErr = Eff*this->Calc_RelUnc( nEffPass, nAccPass ); // -- abs error -- //
		}
		else
		{
			Eff = 0;
			EffErr = 0;
		}
	}

	Double_t Calc_RelUnc( Double_t nNUM, Double_t nDEN )
	{
		Double_t RelUnc_NUM = sqrt( nNUM ) / nNUM;
		Double_t RelUnc_DEN = sqrt( nDEN ) / nDEN;

		return sqrt( RelUnc_NUM*RelUnc_NUM + RelUnc_DEN*RelUnc_DEN );
	}

};

class Tool_CalcAccEff
{
public:
	TString TStr_Version;

	vector< Int_t > vec_MZp;

	Tool_CalcAccEff( TString _TStr_Version )
	{
		this->TStr_Version = _TStr_Version;
		this->Init();
	}

	void Draw_All()
	{
		for(const auto& MZp : vec_MZp )
		{
			this->DrawPlot_vsMsn3( MZp, "Acc" );
			this->DrawPlot_vsMsn3( MZp, "Eff" );
		}

		this->DrawPlot_vsMZp( 50, "Acc" );
		this->DrawPlot_vsMZp( 50, "Eff" );
	}

protected:
	void DrawPlot_vsMZp( Int_t Msn3, TString ValueType )
	{
		TH1D* h_4e = this->MakeHist_vsMZp( Msn3, "4e", ValueType );
		TH1D* h_3e1m = this->MakeHist_vsMZp( Msn3, "3e1m", ValueType );
		TH1D* h_2e2m = this->MakeHist_vsMZp( Msn3, "2e2m", ValueType );
		TH1D* h_1e3m = this->MakeHist_vsMZp( Msn3, "1e3m", ValueType );
		TH1D* h_4m = this->MakeHist_vsMZp( Msn3, "4m", ValueType );

		HistInfo *Hist_4e = new HistInfo( kRed, "4e", h_4e );
		Hist_4e->h->SetMarkerStyle( 22 );

		HistInfo *Hist_3e1m = new HistInfo( kMagenta, "3e1#mu", h_3e1m );
		Hist_3e1m->h->SetMarkerStyle( 24 );

		HistInfo *Hist_2e2m = new HistInfo( kBlue, "2e2#mu", h_2e2m );
		Hist_2e2m->h->SetMarkerStyle( 23 );

		HistInfo *Hist_1e3m = new HistInfo( kGreen, "1e3#mu", h_1e3m );
		Hist_1e3m->h->SetMarkerStyle( 25 );

		HistInfo *Hist_4m = new HistInfo( kBlack, "4#mu", h_4m );

		TCanvas *c;
		TString CanvasName = TString::Format("Local/%s_vs_MZp_EachChannel_Msn3_%d", ValueType.Data(), Msn3 );
		SetCanvas_Square( c, CanvasName, 0, 0 );

		c->cd();
		Hist_4e->Draw("HISTLPSAME");
		Hist_3e1m->Draw("HISTLPSAME");
		Hist_2e2m->Draw("HISTLPSAME");
		Hist_1e3m->Draw("HISTLPSAME");
		Hist_4m->Draw("HISTLPSAME");

		// -- draw error bar -- //
		gStyle->SetErrorX(0.); // -- remove horizental errors -- //
		Hist_4e->Draw("EPSAME");
		Hist_3e1m->Draw("EPSAME");
		Hist_2e2m->Draw("EPSAME");
		Hist_1e3m->Draw("EPSAME");
		Hist_4m->Draw("EPSAME");

		TString YTitle = "";
		if( ValueType == "Acc" ) YTitle = "Acceptance";
		if( ValueType == "Eff" ) YTitle = "Efficiency";
		SetHistFormat_SinglePad( Hist_4e->h, "M(Z')", YTitle );
		Hist_4e->h->GetYaxis()->SetRangeUser(0, 1.01);
		// if( ValueType == "Acc" ) Hist_4e->h->GetYaxis()->SetRangeUser(0.5, 1.1);
		// if( ValueType == "Acc" && MZp == 200 ) Hist_4e->h->GetYaxis()->SetRangeUser(0, 0.2);
		// if( ValueType == "Eff" ) Hist_4e->h->GetYaxis()->SetRangeUser(0, 0.8);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.90, 0.97, 0.95 );
		legend->SetNColumns(5);
		Hist_4e->AddToLegend( legend );
		Hist_3e1m->AddToLegend( legend );
		Hist_2e2m->AddToLegend( legend );
		Hist_1e3m->AddToLegend( legend );
		Hist_4m->AddToLegend( legend );
		legend->Draw();

		TLatex latex;
		TString Latex_Scenario = TString::Format("M(#phi)=%d GeV", Msn3);
		latex.DrawLatexNDC(0.14, 0.96, "#scale[0.6]{#font[42]{"+Latex_Scenario+"}}");
		latex.DrawLatexNDC(0.85, 0.96, "#scale[0.6]{#font[42]{13 TeV}}");

		c->SaveAs(".pdf");
	}

	TH1D* MakeHist_vsMZp( Int_t Msn3, TString TStr_Channel, TString ValueType )
	{
		Int_t nScenario = (Int_t)this->vec_MZp.size();
		TH1D* h = new TH1D("h", "", nScenario+1, 0, nScenario+1 );

		// -- 1st bin: background -- //
		SampleInfo *Info_Bkg = new SampleInfo( this->TStr_Version, 0 );
		Double_t value_bkg = Info_Bkg->GetValue( ValueType, TStr_Channel );
		Double_t valueErr_bkg = Info_Bkg->GetValue( ValueType+"Err", TStr_Channel );

		h->GetXaxis()->SetBinLabel( 1, "Bkg.(ZZ #rightarrow 4l)" );
		h->SetBinContent( 1, value_bkg );
		h->SetBinError( 1, valueErr_bkg );

		for(Int_t i=1; i<nScenario+1; i++)
		{
			Int_t i_bin = i+1;

			// -- bin label -- //
			TString BinLabel = TString::Format("%d GeV", this->vec_MZp[i-1]);
			h->GetXaxis()->SetBinLabel( i_bin, BinLabel );

			SampleInfo *Info = new SampleInfo( this->TStr_Version, 1, this->vec_MZp[i-1], Msn3 );
			Double_t value = Info->GetValue( ValueType, TStr_Channel );
			Double_t valueErr = Info->GetValue( ValueType+"Err", TStr_Channel );

			h->SetBinContent( i_bin, value );
			h->SetBinError( i_bin, valueErr );
		}

		return h;
	}

	void DrawPlot_vsMsn3(Int_t MZp, TString ValueType)
	{
		TH1D* h_4e = this->MakeHist_vsMsn3( MZp, "4e", ValueType );
		TH1D* h_3e1m = this->MakeHist_vsMsn3( MZp, "3e1m", ValueType );
		TH1D* h_2e2m = this->MakeHist_vsMsn3( MZp, "2e2m", ValueType );
		TH1D* h_1e3m = this->MakeHist_vsMsn3( MZp, "1e3m", ValueType );
		TH1D* h_4m = this->MakeHist_vsMsn3( MZp, "4m", ValueType );

		HistInfo *Hist_4e = new HistInfo( kRed, "4e", h_4e );
		Hist_4e->h->SetMarkerStyle( 22 );

		HistInfo *Hist_3e1m = new HistInfo( kMagenta, "3e1#mu", h_3e1m );
		Hist_3e1m->h->SetMarkerStyle( 24 );

		HistInfo *Hist_2e2m = new HistInfo( kBlue, "2e2#mu", h_2e2m );
		Hist_2e2m->h->SetMarkerStyle( 23 );

		HistInfo *Hist_1e3m = new HistInfo( kGreen, "1e3#mu", h_1e3m );
		Hist_1e3m->h->SetMarkerStyle( 25 );

		HistInfo *Hist_4m = new HistInfo( kBlack, "4#mu", h_4m );

		TCanvas *c;
		TString CanvasName = TString::Format("Local/%s_vs_Msn3_EachChannel_MZp_%d", ValueType.Data(), MZp );
		SetCanvas_Square( c, CanvasName, 0, 0 );

		c->cd();
		Hist_4e->Draw("HISTLPSAME");
		Hist_3e1m->Draw("HISTLPSAME");
		Hist_2e2m->Draw("HISTLPSAME");
		Hist_1e3m->Draw("HISTLPSAME");
		Hist_4m->Draw("HISTLPSAME");

		// -- draw error bar -- //
		gStyle->SetErrorX(0.); // -- remove horizental errors -- //
		Hist_4e->Draw("EPSAME");
		Hist_3e1m->Draw("EPSAME");
		Hist_2e2m->Draw("EPSAME");
		Hist_1e3m->Draw("EPSAME");
		Hist_4m->Draw("EPSAME");

		TString YTitle = "";
		if( ValueType == "Acc" ) YTitle = "Acceptance";
		if( ValueType == "Eff" ) YTitle = "Efficiency";
		SetHistFormat_SinglePad( Hist_4e->h, "M(#phi)", YTitle );
		Hist_4e->h->GetYaxis()->SetRangeUser(0, 1.01);
		if( ValueType == "Acc" ) Hist_4e->h->GetYaxis()->SetRangeUser(0.5, 1.1);
		if( ValueType == "Acc" && MZp == 200 ) Hist_4e->h->GetYaxis()->SetRangeUser(0, 0.2);
		if( ValueType == "Eff" ) Hist_4e->h->GetYaxis()->SetRangeUser(0, 0.8);

		TLegend *legend;
		SetLegend( legend, 0.15, 0.90, 0.97, 0.95 );
		legend->SetNColumns(5);
		Hist_4e->AddToLegend( legend );
		Hist_3e1m->AddToLegend( legend );
		Hist_2e2m->AddToLegend( legend );
		Hist_1e3m->AddToLegend( legend );
		Hist_4m->AddToLegend( legend );
		legend->Draw();

		TLatex latex;
		TString Latex_Scenario = TString::Format("M(Z')=%d GeV", MZp);
		latex.DrawLatexNDC(0.14, 0.96, "#scale[0.6]{#font[42]{"+Latex_Scenario+"}}");
		latex.DrawLatexNDC(0.85, 0.96, "#scale[0.6]{#font[42]{13 TeV}}");

		c->SaveAs(".pdf");
	}

	TH1D* MakeHist_vsMsn3( Int_t MZp, TString TStr_Channel, TString ValueType )
	{
		vector<Int_t> vec_Msn3 = this->GetVector_Msn3( MZp );
		Int_t nScenario = (Int_t)vec_Msn3.size();
		TH1D* h = new TH1D("h", "", nScenario, 0, nScenario );

		for(Int_t i=0; i<nScenario; i++)
		{
			Int_t i_bin = i+1;

			// -- bin label -- //
			TString BinLabel = TString::Format("%d GeV", vec_Msn3[i]);
			h->GetXaxis()->SetBinLabel( i_bin, BinLabel );

			SampleInfo *Info = new SampleInfo( this->TStr_Version, 1, MZp, vec_Msn3[i] );
			Double_t value = Info->GetValue( ValueType, TStr_Channel );
			Double_t valueErr = Info->GetValue( ValueType+"Err", TStr_Channel );

			h->SetBinContent( i_bin, value );
			h->SetBinError( i_bin, valueErr );
		}

		return h;
	}

	void AccEff( TString SampleName, TString TStr_Channel, Double_t& Acc, Double_t &Eff )
	{
		TString FileName = TString::Format("Local/%s/%s/ROOTFile_%s.root", this->TStr_Version.Data(), SampleName.Data(), SampleName.Data());
		TString HistName_Base = TString::Format("h_%s", TStr_Channel.Data());
		Double_t nAccTotal = GetContent_OneBinHist( FileName, HistName_Base+"_AccTotal");
		Double_t nAccPass = GetContent_OneBinHist( FileName, HistName_Base+"_AccPass");
		Double_t nEffPass = GetContent_OneBinHist( FileName, HistName_Base+"_EffPass");

		if( nAccTotal != 0 )
			Acc = nAccPass / nAccTotal;
		else
			Acc = 0;

		if( nAccPass != 0 )
			Eff = nEffPass / nAccPass;
		else
			Eff = 0;
	}

	vector< Int_t > GetVector_Msn3( Int_t MZp )
	{
		vector< Int_t > vec_Msn3;
		if( MZp == 200 || MZp == 800 )
		{
			vec_Msn3.push_back( 1 );
			vec_Msn3.push_back( 50 );
		}
		else if( MZp == 1000 )
		{
			vec_Msn3.push_back( 10 );
			vec_Msn3.push_back( 50 );
			vec_Msn3.push_back( 300 );
		}
		else if( MZp == 2000 )
		{
			vec_Msn3.push_back( 1 );
			vec_Msn3.push_back( 20 );
			vec_Msn3.push_back( 50 );
			vec_Msn3.push_back( 600 );
		}
		else if( MZp == 3000 )
		{
			vec_Msn3.push_back( 1 );
			vec_Msn3.push_back( 30 );
			vec_Msn3.push_back( 50 );
			vec_Msn3.push_back( 900 );
		}
		else if( MZp == 4000 )
		{
			vec_Msn3.push_back( 1 );
			vec_Msn3.push_back( 40 );
			vec_Msn3.push_back( 50 );
			vec_Msn3.push_back( 1200 );
		}

		return vec_Msn3;
	}

	void Init()
	{
		vec_MZp.push_back( 200 );
		vec_MZp.push_back( 800 );
		vec_MZp.push_back( 1000 );
		vec_MZp.push_back( 2000 );
		vec_MZp.push_back( 3000 );
		vec_MZp.push_back( 4000 );
		// vec_SampleName_Signal.push_back( "MZp_200_Msn3_1_200k" );
		// vec_SampleName_Signal.push_back( "MZp_200_Msn3_50_200k" );

		// vec_SampleName_Signal.push_back( "MZp_800_Msn3_1_200k" );
		// vec_SampleName_Signal.push_back( "MZp_800_Msn3_50_200k" );
		
		// vec_SampleName_Signal.push_back( "MZp_1000_Msn3_10_200k" );
		// vec_SampleName_Signal.push_back( "MZp_1000_Msn3_50_200k" );
		// vec_SampleName_Signal.push_back( "MZp_1000_Msn3_300_200k" );

		// vec_SampleName_Signal.push_back( "MZp_2000_Msn3_1_200k" );
		// vec_SampleName_Signal.push_back( "MZp_2000_Msn3_20_200k" );
		// vec_SampleName_Signal.push_back( "MZp_2000_Msn3_50_200k" );
		// vec_SampleName_Signal.push_back( "MZp_2000_Msn3_600_200k" );

		// vec_SampleName_Signal.push_back( "MZp_3000_Msn3_1_200k" );
		// vec_SampleName_Signal.push_back( "MZp_3000_Msn3_30_200k" );
		// vec_SampleName_Signal.push_back( "MZp_3000_Msn3_50_200k" );
		// vec_SampleName_Signal.push_back( "MZp_3000_Msn3_900_200k" );

		// vec_SampleName_Signal.push_back( "MZp_4000_Msn3_1_200k" );
		// vec_SampleName_Signal.push_back( "MZp_4000_Msn3_40_200k" );
		// vec_SampleName_Signal.push_back( "MZp_4000_Msn3_50_200k" );
		// vec_SampleName_Signal.push_back( "MZp_4000_Msn3_1200_200k" );

		// if( this->MZp == 200 || this->MZp == 800 )
		// {
		// 	vec_Msn3.push_back( 1 );
		// 	vec_Msn3.push_back( 50 );
		// }
		// else if( this->MZp == 1000 )
		// {
		// 	vec_Msn3.push_back( 10 );
		// 	vec_Msn3.push_back( 50 );
		// 	vec_Msn3.push_back( 300 );
		// }
		// else if( this->MZp == 2000 )
		// {
		// 	vec_Msn3.push_back( 1 );
		// 	vec_Msn3.push_back( 20 );
		// 	vec_Msn3.push_back( 50 );
		// 	vec_Msn3.push_back( 600 );
		// }
		// else if( this->MZp == 3000 )
		// {
		// 	vec_Msn3.push_back( 1 );
		// 	vec_Msn3.push_back( 30 );
		// 	vec_Msn3.push_back( 50 );
		// 	vec_Msn3.push_back( 900 );
		// }
		// else if( this->MZp == 4000 )
		// {
		// 	vec_Msn3.push_back( 1 );
		// 	vec_Msn3.push_back( 40 );
		// 	vec_Msn3.push_back( 50 );
		// 	vec_Msn3.push_back( 1200 );
		// }

		// for( const auto& Msn3 : vec_Msn3 )
		// {
		// 	TString FileName = TString::Format("MZp_%d_Msn3_%d_200k", this->MZp, Msn3);
		// 	vec_SampleName_Signal.push_back( FileName );
		// }

		// SampleName_Bkg = "ZZto4L0j_1M";

		// vec_ChannelType.push_back( "4e" );
		// vec_ChannelType.push_back( "3e1m" );
		// vec_ChannelType.push_back( "2e2m" );
		// vec_ChannelType.push_back( "1e3m" );
		// vec_ChannelType.push_back( "4m" );
	}
};