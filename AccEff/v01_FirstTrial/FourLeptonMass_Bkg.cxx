#include "/Users/KyeongPil_Lee/Physics/DYAnalysis_76X_LumiUpdate/Include/PlotTools.h"

void FourLeptonMass_Bkg( Int_t nRebin )
{
	TString FileName = "Local/ROOTFile_4LeptonMass_ZZto4L0j_1M.root";

	TH1D* h_4e = Get_Hist( FileName, "h_4e_All" );
	// TH1D* h_3e1m = Get_Hist( FileName_Base+"_3e1m.root", HistName );
	TH1D* h_2e2m = Get_Hist( FileName, "h_2e2m_All" );
	// TH1D* h_1e3m = Get_Hist( FileName_Base+"_1e3m.root", HistName );
	TH1D* h_4m = Get_Hist( FileName, "h_4m_All" );

	h_4e->Rebin( nRebin );
	// h_3e1m->Rebin( nRebin );
	h_2e2m->Rebin( nRebin );
	// h_1e3m->Rebin( nRebin );
	h_4m->Rebin( nRebin );

	HistInfo *Hist_4e = new HistInfo( kRed, "4e", h_4e );
	Hist_4e->h->SetMarkerStyle( 22 );

	// HistInfo *Hist_3e1m = new HistInfo( kMagenta, "3e1#mu", h_3e1m );
	// Hist_3e1m->h->SetMarkerStyle( 24 );

	HistInfo *Hist_2e2m = new HistInfo( kBlue, "2e2#mu", h_2e2m );
	Hist_2e2m->h->SetMarkerStyle( 23 );

	// HistInfo *Hist_1e3m = new HistInfo( kGreen, "1e3#mu", h_1e3m );
	// Hist_1e3m->h->SetMarkerStyle( 25 );

	HistInfo *Hist_4m = new HistInfo( kBlack, "4#mu", h_4m );

	TCanvas *c;
	TString CanvasName = TString::Format("4LeptonMass_ZZto4l_%dGeV", nRebin);
	SetCanvas_Square( c, CanvasName, 0, 0 );

	c->cd();
	// Hist_4e->Draw("HISTLPSAME");
	// Hist_3e1m->Draw("HISTLPSAME");
	// Hist_2e2m->Draw("HISTLPSAME");
	// Hist_1e3m->Draw("HISTLPSAME");
	// Hist_4m->Draw("HISTLPSAME");

	// -- draw error bar -- //
	// gStyle->SetErrorX(0.); // -- remove horizental errors -- //
	Hist_4e->Draw("EPSAME");
	// Hist_3e1m->Draw("EPSAME");
	Hist_2e2m->Draw("EPSAME");
	// Hist_1e3m->Draw("EPSAME");
	Hist_4m->Draw("EPSAME");

	TString YTitle = TString::Format("Events / %d GeV", nRebin);
	SetHistFormat_SinglePad( Hist_4e->h, "M (4l) [GeV]", YTitle );
	Hist_4e->h->GetXaxis()->SetRangeUser(0, 5000);
	Hist_4e->h->GetYaxis()->SetRangeUser(0.1, 1e3);
	// if( ValueType == "Acc" ) Hist_4e->h->GetYaxis()->SetRangeUser(0.5, 1.1);
	// if( ValueType == "Acc" && MZp == 200 ) Hist_4e->h->GetYaxis()->SetRangeUser(0, 0.2);
	// if( ValueType == "Eff" ) Hist_4e->h->GetYaxis()->SetRangeUser(0, 0.8);

	TLegend *legend;
	SetLegend( legend, 0.15, 0.90, 0.97, 0.95 );
	legend->SetNColumns(5);
	Hist_4e->AddToLegend( legend );
	// Hist_3e1m->AddToLegend( legend );
	Hist_2e2m->AddToLegend( legend );
	// Hist_1e3m->AddToLegend( legend );
	Hist_4m->AddToLegend( legend );
	legend->Draw();

	TLatex latex;
	TString Latex_Scenario = "ZZ #rightarrow 4l (1M events)";
	latex.DrawLatexNDC(0.14, 0.96, "#scale[0.6]{#font[42]{"+Latex_Scenario+"}}");
	latex.DrawLatexNDC(0.85, 0.96, "#scale[0.6]{#font[42]{13 TeV}}");

	c->SetLogy();
	c->SaveAs(".pdf");
}

void FourLeptonMass_Bkg()
{
	FourLeptonMass_Bkg( 10 );
	FourLeptonMass_Bkg( 50 );
}
