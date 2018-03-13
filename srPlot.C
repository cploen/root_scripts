#include <iostream> 
#include <fstream>
#include <cmath> 
#include <math.h> 
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TMinuit.h>
#include <TPaveText.h>
#include <TText.h>
#include <TSystem.h>
#include <TArc.h>

using namespace std;

void srPlot(string input_filename){
    
    // gROOT->Reset();
    gStyle->SetPalette(1);
    gStyle->SetOptStat(1);
    
    
    char the_filename[200];
    sprintf(the_filename, "%s",input_filename.substr(0,input_filename.rfind(".")).c_str());
    
    char output_filename[200];
    sprintf(output_filename, "%s_output.root",the_filename);
    TFile *outputfile=new TFile(output_filename, "recreate");
    

    //create and fill 1D histogram
    TH1F *hp1   = new TH1F("hp1","p for photon",1000,0.,1.);
    



    TFile *file=new TFile(input_filename.c_str());
    if (file->IsZombie()) {
	cout << "Error opening file" << input_filename << endl;
	exit(-1);
    }
    else cout << "open file " << input_filename << endl;
    
    TTree *flux = (TTree*)file->Get("flux");
    const double DEG=180./3.1415926;
    const double weight =1.0;
    vector<int> *id=0;
    vector<double> *pid=0,*mpid=0,*tid=0,*mtid=0,*otid=0,*trackE=0,*totEdep=0,*avg_x=0,*avg_y=0,*avg_z=0,*avg_lx=0,*avg_ly=0,*avg_lz=0,*px=0,*py=0,*pz=0,*vx=0,*vy=0,*vz=0,*mvx=0,*mvy=0,*mvz=0,*avg_t=0;
    
    flux->SetBranchAddress("id",&id);
    flux->SetBranchAddress("pid",&pid);
    flux->SetBranchAddress("mpid",&mpid);
    flux->SetBranchAddress("tid",&tid);
    flux->SetBranchAddress("mtid",&mtid);
    flux->SetBranchAddress("otid",&otid);
    flux->SetBranchAddress("trackE",&trackE);
    flux->SetBranchAddress("totEdep",&totEdep);
    flux->SetBranchAddress("avg_x",&avg_x);
    flux->SetBranchAddress("avg_y",&avg_y);
    flux->SetBranchAddress("avg_z",&avg_z);
    flux->SetBranchAddress("avg_lx",&avg_lx);
    flux->SetBranchAddress("avg_ly",&avg_ly);
    flux->SetBranchAddress("avg_lz",&avg_lz);
    flux->SetBranchAddress("px",&px);
    flux->SetBranchAddress("py",&py);
    flux->SetBranchAddress("pz",&pz);
    flux->SetBranchAddress("vx",&vx);
    flux->SetBranchAddress("vy",&vy);
    flux->SetBranchAddress("vz",&vz);
    flux->SetBranchAddress("mvx",&mvx);
    flux->SetBranchAddress("mvy",&mvy);
    flux->SetBranchAddress("mvz",&mvz);
    flux->SetBranchAddress("avg_t",&avg_t);
    
    int nevent = (int)flux->GetEntries();
    //int counter=0;
    
    //read all entries and fill the histograms
    
    for (Long64_t i=0;i<nevent;i++) {
	int pid_flux;
	double px_flux,py_flux,pz_flux,p_flux,theta_flux,phi_flux;    
	
	flux->GetEntry(i);

	for (int j=0;j<pid->size();j++){
	    pid_flux=pid->at(j);
	    px_flux=px->at(j)*1e3;  //in MeV, convert to keV
	    py_flux=py->at(j)*1e3;  //in MeV, convert to keV
	    pz_flux=pz->at(j)*1e3;  //in MeV, convert to keV
	    p_flux=sqrt(px_flux*px_flux+py_flux*py_flux+pz_flux*pz_flux); 
	    theta_flux=acos(pz_flux/p_flux)*DEG;  //in deg
	    phi_flux=atan2(py_flux,px_flux)*DEG;  //in deg
	    if(pid_flux==22){
		hp1->Fill(p_flux,weight);   
	    }
	}
    }
   
     //create and fill 2D histogram
     //should set ymax to largest nentries 
    int nbins = hp1->GetSize();
    double xmax = hp1->GetXaxis()->GetXmax();
    double xmin = hp1->GetXaxis()->GetXmin(); 
    
 
    TH2F *hp2   = new TH2F("hp2","pN for photon",nbins,xmin,xmax,1000, 0., 100000.);
    hp2->SetTitle("N*P vs P for PID=22; avg_p (kEv); N*avg_p (kEv)");
    for (int k=1; k<nbins-1; k++){
	Long64_t nentries = hp1->GetBinContent(k);

    //still need to add offset to p_bin
	double p_bin = hp1->GetBinCenter(k); 
        hp2->Fill(nentries*p_bin,p_bin);
        }
     
  
    file->Close();
    

//    TCanvas *c1 = new TCanvas("c1","hp1",600,600);
//  c1->Divide(1,1);
//   c1->cd(1);
//   hp1->Draw();
    
    TCanvas *c2 = new TCanvas("c2","hp2",600,600);
    c2->Divide(1,1);
    c2->cd(2);
    hp2->Draw();
 

    outputfile->Write();
    outputfile->Flush();
}

