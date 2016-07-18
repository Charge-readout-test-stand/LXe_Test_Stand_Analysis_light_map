#include <iostream>
using namespace std;

void test()
{
	TFile *data = TFile::Open("~/software/LXe_Test_Stand_Analysis_light_map/utilities/data/data.root"); //Opens existing TFile data.root
//loop over events in tree, calc ave photon_nr, loop over fill, every time photon_nr changes, calc ave photon_nr

	TFile f("data2.root", "recreate"); //creates a Tree file data2.root
	TCanvas c1("c1", "");
	TH2D *hist = new TH2D("hist", "light map", 50, 0, 10, 38, 0, 7.6); //Creates new TH2D histogram
  	TTree *tree = data->Get("data"); //get TTree data from data.root
 	cout << tree->GetEntries() << endl; 
	
	//fill hist with info from data.root
	Int_t event;
	Double_t radius;
	Double_t zdir;
	Double_t photon_nr;
	tree->SetBranchAddress("event",&event);
	tree->SetBranchAddress("radius",&radius);
	tree->SetBranchAddress("zdir",&zdir);
	tree->SetBranchAddress("photon_nr",&photon_nr);

	for (Int_t i=1; i<=190000; i++) {
		tree->GetEntry(i); 
		
			Double_t photon_nr_ave = photon_nr/100;
			Double_t Radius = radius + 0.001;
			Double_t Zdir = zdir + 0.001;	
			hist->Fill(Radius,Zdir,photon_nr_ave);
			cout << photon_nr_ave << endl;		
		
	  }
        
//	for (Int_t i=0; i<=190000; i+=100) {  
//		hist->Fill(zdir,radius,photon_nr_ave);
//		cout << hist->GetEntries() << endl;
//          }	
		gStyle->SetOptStat(0);   
		hist->Draw("colz");    
		c1.Update();
		c1.Print("hist.png");
		hist->Write();
		Int_t pause;
		cin >> pause;
  	    	
}
