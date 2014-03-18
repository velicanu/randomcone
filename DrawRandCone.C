#include "HiForestAnalysis/hiForest.h"
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TRandom3.h>
#include <fstream>
#include <iostream>
#include <math.h>

void DrawRandCone(string tag, int centmin, int centmax)
{
  TFile* f = new TFile(Form("merged/randcone_%s_centmin%d_centmax%d.root",tag.data(),centmin,centmax));
  TH1D* one = (TH1D*) f->Get("one");
  cout<<one->GetBinContent(1)<<endl;
  double norm = one->GetBinContent(1);
  
  TH1D * hmeanpfPtSumEta = (TH1D*) f->Get("hmeanpfPtSumEta");
  TH1D * hmeanpfVsPtSumEta = (TH1D*) f->Get("hmeanpfVsPtSumEta");
  TH1D * hmeanpfVsPtInitialSumEta = (TH1D*) f->Get("hmeanpfVsPtInitialSumEta");
  hmeanpfPtSumEta->GetXaxis()->CenterTitle();
  hmeanpfPtSumEta->GetYaxis()->CenterTitle();
  hmeanpfVsPtSumEta->GetXaxis()->CenterTitle();
  hmeanpfVsPtSumEta->GetYaxis()->CenterTitle();
  hmeanpfVsPtInitialSumEta->GetXaxis()->CenterTitle();
  hmeanpfVsPtInitialSumEta->GetYaxis()->CenterTitle();
  
  
  hmeanpfPtSumEta->Scale(1.0/norm);
  hmeanpfVsPtSumEta->Scale(1.0/norm);
  hmeanpfVsPtInitialSumEta->Scale(1.0/norm);
  
  
  TCanvas * c1 = new TCanvas("c1");
  hmeanpfPtSumEta->Draw();
  // hmeanpfVsPtSumEta->Draw();
  // hmeanpfVsPtInitialSumEta->Draw();
  
  TLatex * lmult = new TLatex(0.53, 0.95, Form("CMS PbPb %2.0f-%2.0f%%", centmin*0.5, centmax*0.5));
  lmult->SetNDC(1);
  lmult->SetTextSize(lmult->GetTextSize()*1.5);
  lmult->Draw();
  
  c1->SaveAs(Form("hmeanpfPtSumEta-%d_%d.png",centmin,centmax));
  // c1->SaveAs(Form("hmeanpfVsPtSumEta-%d_%d.png",centmin,centmax));
  // c1->SaveAs(Form("hmeanpfVsPtInitialSumEta-%d_%d.png",centmin,centmax));
}