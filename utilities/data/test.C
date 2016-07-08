void test()
{
	TFile *data = TFile::Open("~/software/LXe_Test_Stand_Analysis_light_map/utilities/data/data.root"); //Opens existing TFile data.root
//loop over events in tree, calc ave photon_nr, loop over fill, every time photon_nr changes, calc ave photon_nr

	TFile f("data2.root", "recreate"); //creates a Tree file data2.root
	TCanvas c1("c1", "");
	TH2D *hist2 = new TH2D("hist2", "light map", 40, 0, 8, 40, 0, 8); //Creates new TH2D histogram
	for (Int_t i=0; i<190000; i++) {
  	TTree *tree = data->Get("data"); //get TTree data from data.root
 	//cout << tree->GetEntries() << endl; 

	}
	//fill hist2 with info from data.root
	Int_t event;
	Double_t radius;
	Double_t zdir;
	Double_t photon_nr;
	tree->SetBranchAddress("event",&event);
	tree->SetBranchAddress("radius",&radius);
	tree->SetBranchAddress("zdir",&zdir);
	tree->SetBranchAddress("photon_nr",&photon_nr);
	for (Int_t i=0; i<=100; 
	hist2->Fill(zdir,radius,photon_nr);
	cout << hist2->GetEntries() << endl;
	hist2->Draw("colz");
	c1.Update();
	c1.Print("hist2.pdf");
	hist2->Write();
	Int_t pause;
	cin >> pause;

}
