#include "HiForestAnalysis/hiForest.h"
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <fstream>
#include <iostream>
#include <math.h>


void randomcone(string filename, int centmin = 0, int centmax = 4);
float getdR(float eta1, float eta2, float phi1, float phi2);

void randomcone(int condor_iter, string flist, string tag, int centmin, int centmax)
{
  using namespace std;
  
  string buffer;
  vector<string> listoffiles;
  int nlines = 0;
  ifstream infile(flist.data());
  if (!infile.is_open()) {
    cout << "Error opening file. Exiting." << endl;
    return;
  } else {
    while (!infile.eof()) {
      infile >> buffer;
      listoffiles.push_back(buffer);
      nlines++;
    }
  }
  
  HiForest * c = new HiForest(listoffiles[condor_iter].data(),"forest",cPPb,0);
  
  // TH2D * etaphi = new TH2D();
  int nevents = c->GetEntries();
  const int netabins = 48;
  
  TRandom3 r;
  float rConeEta = 0;
  float rConePhi = 0;
  float dR = 0.3;
  float thisdR = 0;
  float pfPtSum;
  float pfVsPtSum;
  float pfVsPtInitialSum;
  
  TFile * outf = new TFile(Form("randcone_%s_centmin%d_centmax%d_%d.root",tag.data(),centmin,centmax,condor_iter),"recreate");
  TH1D * hpfPtSum = new TH1D("hpfPtSum","pfPtSum;#Sigma p_{T}",240,0,120);
  TH1D * hpfVsPtSum = new TH1D("hpfVsPtSum","pfVsPtSum;#Sigma p_{T}",200,-50,50);
  TH1D * hpfVsPtInitialSum = new TH1D("hpfVsPtInitialSum","hpfVsPtInitialSum;#Sigma p_{T}",200,-50,50);
  
  TH1D * hpfPtSumEta[netabins];
  TH1D * hpfVsPtSumEta[netabins];
  TH1D * hpfVsPtInitialSumEta[netabins];
  for(int i = 0 ; i < netabins ; ++i)
  {
    hpfPtSumEta[i] = new TH1D(Form("hpfPtSumEta_%d",i),Form("pfPtSum_%d;#Sigma p_{T}",i),200,-100,100);
    hpfVsPtSumEta[i] = new TH1D(Form("hpfVsPtSumEta_%d",i),Form("pfVsPtSum_%d;#Sigma p_{T}",i),200,-100,100);
    hpfVsPtInitialSumEta[i] = new TH1D(Form("hpfVsPtInitialSumEta_%d",i),Form("hpfVsPtInitialSum_%d;#Sigma p_{T}",i),200,-100,100);
  }
  
  TH1D * hConeEta = new TH1D("hConeEta","hConeEta;#eta",netabins,-2.4,2.4);
  
  for(int i = 0 ; i < nevents ; ++i)
  {
    if(i%1000==0) cout<<i<<"/"<<nevents<<endl;
    c->GetEntry(i);
    if(c->evt.hiBin < centmin || c->evt.hiBin >= centmax) continue;
    rConeEta = 4.8*(r.Rndm()-0.5); // uniform dist. +- 2.4 eta
    rConePhi = 2*TMath::Pi()*(r.Rndm()-0.5); // uniform dist. +- pi
    hConeEta->Fill(rConeEta);
    int whichbin = hConeEta->FindBin(rConeEta);
    pfPtSum = 0.0;
    pfVsPtSum = 0.0;
    pfVsPtInitialSum = 0.0;
    for(int j = 0 ; j < c->pf.nPFpart ; ++j)
    {
      thisdR = getdR(rConeEta,c->pf.pfEta[j],rConePhi,c->pf.pfPhi[j]);
      if(thisdR < dR)
      {
        pfPtSum+=c->pf.pfPt[j];
        pfVsPtSum+=c->pf.pfVsPt[j];
        pfVsPtInitialSum+=c->pf.pfVsPtInitial[j];
      }
    }
    hpfPtSum->Fill(pfPtSum);
    hpfVsPtSum->Fill(pfVsPtSum);
    hpfVsPtInitialSum->Fill(pfVsPtInitialSum);
    hpfPtSumEta[whichbin-1]->Fill(pfPtSum);
    hpfVsPtSumEta[whichbin-1]->Fill(pfVsPtSum);
    hpfVsPtInitialSumEta[whichbin-1]->Fill(pfVsPtInitialSum);
  }
  TH1D * hmeanpfPtSumEta = new TH1D("hmeanpfPtSumEta","hmeanpfPtSumEta;#eta;mean #Sigma p_{T}",netabins,-2.4,2.4);
  TH1D * hmeanpfVsPtSumEta = new TH1D("hmeanpfVsPtSumEta","hmeanpfVsPtSumEta;#eta;mean #Sigma p_{T}",netabins,-2.4,2.4);
  TH1D * hmeanpfVsPtInitialSumEta = new TH1D("hmeanpfVsPtInitialSumEta","hmeanpfVsPtInitialSumEta;#eta;mean #Sigma p_{T}",netabins,-2.4,2.4);
  for(int i = 1 ; i <= netabins ; ++i)
  {
    hmeanpfPtSumEta->SetBinContent(i,hpfPtSumEta[i-1]->GetMean());
    hmeanpfPtSumEta->SetBinError(i,hpfPtSumEta[i-1]->GetMeanError());
    hmeanpfVsPtSumEta->SetBinContent(i,hpfVsPtSumEta[i-1]->GetMean());
    hmeanpfVsPtSumEta->SetBinError(i,hpfVsPtSumEta[i-1]->GetMeanError());
    hmeanpfVsPtInitialSumEta->SetBinContent(i,hpfVsPtInitialSumEta[i-1]->GetMean());
    hmeanpfVsPtInitialSumEta->SetBinError(i,hpfVsPtInitialSumEta[i-1]->GetMeanError());
  }
  TH1D * one = new TH1D("one","",1,0,1);
  one->Fill(0.5);
  outf->Write();
  outf->Close();
}

float getdR(float eta1, float eta2, float phi1, float phi2)
{
  float detasq = (eta1-eta2)*(eta1-eta2);
  float dphi = fabs(phi1 - phi2);
  if(dphi > TMath::Pi())
    dphi = 2*TMath::Pi() - dphi;
  float dphisq = dphi*dphi;
  return sqrt( dphisq + detasq );
}

int main(int argc, char *argv[])
{
  if(argc != 6)
  {
    cout<<"usage ./randomcone.exe <filenum> <tag> <filelist> <centmin> <centmax>"<<endl;
    return 1;
  }
  randomcone(atoi(argv[1]),argv[2],argv[3],atof(argv[4]),atof(argv[5]));
  return 0;
}
