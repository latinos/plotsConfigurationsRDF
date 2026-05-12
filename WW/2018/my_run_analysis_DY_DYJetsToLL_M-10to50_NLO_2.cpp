
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDFHelpers.hxx"
#include "ROOT/RVec.hxx"

#include "TFile.h"
#include "TH1D.h"
#include "TDirectory.h"
#include "TChain.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <typeinfo>

//
// doFold
//   0 = no
//   1 = overflow
//   2 = underflow
//   3 = overflow and underflow
//

//                 TH1 works for TH1F, TH2F, ...
void FoldHistogram(TH1* h, int doFold) {

  // 1D historgram
  if (h->GetDimension() == 1) {

    // Get the number of visible bins
    int nBins = h->GetNbinsX();

    // overflow
    if (doFold == 1 || doFold == 3) {
      double content_last     = h->GetBinContent(nBins);
      double error_last       = h->GetBinError(nBins);
      double content_overflow = h->GetBinContent(nBins+1);
      double error_overflow   = h->GetBinError(nBins+1);

      h->SetBinContent(nBins, content_last + content_overflow);
      h->SetBinError(nBins, std::sqrt(error_last*error_last + error_overflow*error_overflow));

      h->SetBinContent(nBins + 1, 0);
      h->SetBinError(nBins + 1, 0);

    }

    // underflow
    if (doFold == 2 || doFold == 3) {

      double content_first     = h->GetBinContent(1);
      double error_first       = h->GetBinError(1);
      double content_underflow = h->GetBinContent(0);
      double error_underflow   = h->GetBinError(0);

      h->SetBinContent(1, content_first + content_underflow);
      h->SetBinError(1, std::sqrt(error_first*error_first + error_underflow*error_underflow));

      h->SetBinContent(0, 0);
      h->SetBinError(0, 0);

    }

  }
  // 2D histogram (com'on you don't need a 3D version ...)
  else {

    int nX = h->GetNbinsX();
    int nY = h->GetNbinsY();

    // overflow
    if (doFold == 1 || doFold == 3) {
      for (int iy = 0; iy <= nY + 1; ++iy) {
        h->SetBinContent(nX, iy, h->GetBinContent(nX, iy) + h->GetBinContent(nX + 1, iy));
        h->SetBinError(nX, iy, std::hypot(h->GetBinError(nX, iy), h->GetBinError(nX + 1, iy)));
        h->SetBinContent(nX + 1, iy, 0);
        h->SetBinError(nX + 1, iy, 0);
      }
      for (int ix = 0; ix <= nX + 1; ++ix) {
        h->SetBinContent(ix, nY, h->GetBinContent(ix, nY) + h->GetBinContent(ix, nY + 1));
        h->SetBinError(ix, nY, std::hypot(h->GetBinError(ix, nY), h->GetBinError(ix, nY + 1)));
        h->SetBinContent(ix, nY + 1, 0);
        h->SetBinError(ix, nY + 1, 0);
      }
    }

    // underflow
    if (doFold == 2 || doFold == 3) {
      for (int iy = 0; iy <= nY + 1; ++iy) {
        h->SetBinContent(1, iy, h->GetBinContent(1, iy) + h->GetBinContent(0, iy));
        h->SetBinError(1, iy, std::hypot(h->GetBinError(1, iy), h->GetBinError(0, iy)));
        h->SetBinContent(0, iy, 0);
        h->SetBinError(0, iy, 0);
      }
      for (int ix = 0; ix <= nX + 1; ++ix) {
        h->SetBinContent(ix, 1, h->GetBinContent(ix, 1) + h->GetBinContent(ix, 0));
        h->SetBinError(ix, 1, std::hypot(h->GetBinError(ix, 1), h->GetBinError(ix, 0)));
        h->SetBinContent(ix, 0, 0);
        h->SetBinError(ix, 0, 0);
      }
    }

  }


}


// --- transform 2D into 1D: unrolling
//
//      3    6    9
//      2    5    8
//      1    4    7
//


TH1D* UnrollHistogram(TH2D* h2) {

  int nBinsX = h2->GetNbinsX();
  int nBinsY = h2->GetNbinsY();
  int totalBins = nBinsX * nBinsY;

  // Create a 1D histogram with the same name (plus a suffix)
  TString name = h2->GetName();
  TString title = h2->GetTitle();
  h2->SetName(name + "_old"); // Rename original to avoid conflict

  TH1D* h1 = new TH1D(name, title, totalBins, 0.5, totalBins + 0.5);
  h1->Sumw2();

  int i1D = 1;
  for (int iy = 1; iy <= nBinsY; ++iy) {
      for (int ix = 1; ix <= nBinsX; ++ix) {
          h1->SetBinContent(i1D, h2->GetBinContent(ix, iy));
          h1->SetBinError(i1D, h2->GetBinError(ix, iy));
          i1D++;
      }
  }

  return h1;
}




#include "TMVA/RReader.hxx"
#include "TMVA/RInferenceUtils.hxx"



ROOT::RDF::RNode SafeDefine(ROOT::RDF::RNode df, std::string name, std::string expr) {
    auto colNames = df.GetColumnNames();
    if (std::find(colNames.begin(), colNames.end(), name) == colNames.end()) {
        return df.Define(name, expr);
    }
    return df;
}


std::vector<std::string> getBranchNames(TTree* tree) {
  std::vector<std::string> names;
  TObjArray* branches = tree->GetListOfBranches();
  for (int i = 0; i < branches->GetEntries(); ++i)
    names.push_back(branches->At(i)->GetName());
  return names;
}



// --- Automatically generated: code to be added for additional functions ---


#include <cmath>
float calculate_mass(float pt1, float eta1, float phi1, float pt2, float eta2, float phi2) {
  return std::sqrt(2 * pt1 * pt2 * (std::cosh(eta1 - eta2) - std::cos(phi1-phi2)));
}






// ----------------------------------------




int main() {
    ROOT::EnableImplicitMT();

    // --- Automatically generated input root files ---

    //  Nominal input files
    auto* nominal = new TChain("Events");
    nominal->Add("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    nominal->Add("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    nominal->Add("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    nominal->Add("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    nominal->Add("root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    
    auto nominalBranches = getBranchNames(nominal);
    
//  Variations input files (if any)
    auto* friend_ElepTup = new TChain("Events");
    auto* friend_ElepTdo = new TChain("Events");
    friend_ElepTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_ElepTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_ElepTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_ElepTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_ElepTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_ElepTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_ElepTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_ElepTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_ElepTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_ElepTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__ElepTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_ElepTup, "ElepTup");
    nominal->AddFriend(friend_ElepTdo, "ElepTdo");
    auto varBranches_ElepTup = getBranchNames(friend_ElepTup);
    auto varBranches_ElepTdo = getBranchNames(friend_ElepTdo);
    auto* friend_MupTup = new TChain("Events");
    auto* friend_MupTdo = new TChain("Events");
    friend_MupTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_MupTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_MupTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_MupTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_MupTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_MupTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_MupTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_MupTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_MupTup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_MupTdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__MupTdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_MupTup, "MupTup");
    nominal->AddFriend(friend_MupTdo, "MupTdo");
    auto varBranches_MupTup = getBranchNames(friend_MupTup);
    auto varBranches_MupTdo = getBranchNames(friend_MupTdo);
    auto* friend_JESAbsoluteup = new TChain("Events");
    auto* friend_JESAbsolutedo = new TChain("Events");
    friend_JESAbsoluteup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESAbsolutedo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESAbsoluteup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESAbsolutedo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESAbsoluteup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESAbsolutedo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESAbsoluteup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESAbsolutedo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESAbsoluteup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESAbsolutedo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESAbsoluteup, "JESAbsoluteup");
    nominal->AddFriend(friend_JESAbsolutedo, "JESAbsolutedo");
    auto varBranches_JESAbsoluteup = getBranchNames(friend_JESAbsoluteup);
    auto varBranches_JESAbsolutedo = getBranchNames(friend_JESAbsolutedo);
    auto* friend_JESAbsolute_2018up = new TChain("Events");
    auto* friend_JESAbsolute_2018do = new TChain("Events");
    friend_JESAbsolute_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESAbsolute_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESAbsolute_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESAbsolute_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESAbsolute_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESAbsolute_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESAbsolute_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESAbsolute_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESAbsolute_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESAbsolute_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESAbsolute_2018up, "JESAbsolute_2018up");
    nominal->AddFriend(friend_JESAbsolute_2018do, "JESAbsolute_2018do");
    auto varBranches_JESAbsolute_2018up = getBranchNames(friend_JESAbsolute_2018up);
    auto varBranches_JESAbsolute_2018do = getBranchNames(friend_JESAbsolute_2018do);
    auto* friend_JESBBEC1up = new TChain("Events");
    auto* friend_JESBBEC1do = new TChain("Events");
    friend_JESBBEC1up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESBBEC1do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESBBEC1up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESBBEC1do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESBBEC1up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESBBEC1do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESBBEC1up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESBBEC1do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESBBEC1up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESBBEC1do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESBBEC1up, "JESBBEC1up");
    nominal->AddFriend(friend_JESBBEC1do, "JESBBEC1do");
    auto varBranches_JESBBEC1up = getBranchNames(friend_JESBBEC1up);
    auto varBranches_JESBBEC1do = getBranchNames(friend_JESBBEC1do);
    auto* friend_JESBBEC1_2018up = new TChain("Events");
    auto* friend_JESBBEC1_2018do = new TChain("Events");
    friend_JESBBEC1_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESBBEC1_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESBBEC1_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESBBEC1_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESBBEC1_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESBBEC1_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESBBEC1_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESBBEC1_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESBBEC1_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESBBEC1_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESBBEC1_2018up, "JESBBEC1_2018up");
    nominal->AddFriend(friend_JESBBEC1_2018do, "JESBBEC1_2018do");
    auto varBranches_JESBBEC1_2018up = getBranchNames(friend_JESBBEC1_2018up);
    auto varBranches_JESBBEC1_2018do = getBranchNames(friend_JESBBEC1_2018do);
    auto* friend_JESEC2up = new TChain("Events");
    auto* friend_JESEC2do = new TChain("Events");
    friend_JESEC2up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESEC2do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESEC2up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESEC2do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESEC2up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESEC2do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESEC2up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESEC2do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESEC2up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESEC2do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESEC2up, "JESEC2up");
    nominal->AddFriend(friend_JESEC2do, "JESEC2do");
    auto varBranches_JESEC2up = getBranchNames(friend_JESEC2up);
    auto varBranches_JESEC2do = getBranchNames(friend_JESEC2do);
    auto* friend_JESEC2_2018up = new TChain("Events");
    auto* friend_JESEC2_2018do = new TChain("Events");
    friend_JESEC2_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESEC2_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESEC2_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESEC2_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESEC2_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESEC2_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESEC2_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESEC2_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESEC2_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESEC2_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESEC2_2018up, "JESEC2_2018up");
    nominal->AddFriend(friend_JESEC2_2018do, "JESEC2_2018do");
    auto varBranches_JESEC2_2018up = getBranchNames(friend_JESEC2_2018up);
    auto varBranches_JESEC2_2018do = getBranchNames(friend_JESEC2_2018do);
    auto* friend_JESFlavorQCDup = new TChain("Events");
    auto* friend_JESFlavorQCDdo = new TChain("Events");
    friend_JESFlavorQCDup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESFlavorQCDdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESFlavorQCDup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESFlavorQCDdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESFlavorQCDup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESFlavorQCDdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESFlavorQCDup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESFlavorQCDdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESFlavorQCDup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESFlavorQCDdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESFlavorQCDup, "JESFlavorQCDup");
    nominal->AddFriend(friend_JESFlavorQCDdo, "JESFlavorQCDdo");
    auto varBranches_JESFlavorQCDup = getBranchNames(friend_JESFlavorQCDup);
    auto varBranches_JESFlavorQCDdo = getBranchNames(friend_JESFlavorQCDdo);
    auto* friend_JESHFup = new TChain("Events");
    auto* friend_JESHFdo = new TChain("Events");
    friend_JESHFup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESHFdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESHFup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESHFdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESHFup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESHFdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESHFup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESHFdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESHFup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESHFdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESHFup, "JESHFup");
    nominal->AddFriend(friend_JESHFdo, "JESHFdo");
    auto varBranches_JESHFup = getBranchNames(friend_JESHFup);
    auto varBranches_JESHFdo = getBranchNames(friend_JESHFdo);
    auto* friend_JESHF_2018up = new TChain("Events");
    auto* friend_JESHF_2018do = new TChain("Events");
    friend_JESHF_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESHF_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESHF_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESHF_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESHF_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESHF_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESHF_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESHF_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESHF_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESHF_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESHF_2018up, "JESHF_2018up");
    nominal->AddFriend(friend_JESHF_2018do, "JESHF_2018do");
    auto varBranches_JESHF_2018up = getBranchNames(friend_JESHF_2018up);
    auto varBranches_JESHF_2018do = getBranchNames(friend_JESHF_2018do);
    auto* friend_JESRelativeBalup = new TChain("Events");
    auto* friend_JESRelativeBaldo = new TChain("Events");
    friend_JESRelativeBalup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESRelativeBaldo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESRelativeBalup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESRelativeBaldo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESRelativeBalup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESRelativeBaldo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESRelativeBalup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESRelativeBaldo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESRelativeBalup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESRelativeBaldo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESRelativeBalup, "JESRelativeBalup");
    nominal->AddFriend(friend_JESRelativeBaldo, "JESRelativeBaldo");
    auto varBranches_JESRelativeBalup = getBranchNames(friend_JESRelativeBalup);
    auto varBranches_JESRelativeBaldo = getBranchNames(friend_JESRelativeBaldo);
    auto* friend_JESRelativeSample_2018up = new TChain("Events");
    auto* friend_JESRelativeSample_2018do = new TChain("Events");
    friend_JESRelativeSample_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESRelativeSample_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JESRelativeSample_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESRelativeSample_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JESRelativeSample_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESRelativeSample_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JESRelativeSample_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESRelativeSample_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JESRelativeSample_2018up->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JESRelativeSample_2018do->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__RDF__JESdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JESRelativeSample_2018up, "JESRelativeSample_2018up");
    nominal->AddFriend(friend_JESRelativeSample_2018do, "JESRelativeSample_2018do");
    auto varBranches_JESRelativeSample_2018up = getBranchNames(friend_JESRelativeSample_2018up);
    auto varBranches_JESRelativeSample_2018do = getBranchNames(friend_JESRelativeSample_2018do);
    auto* friend_METup = new TChain("Events");
    auto* friend_METdo = new TChain("Events");
    friend_METup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_METdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_METup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_METdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_METup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_METdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_METup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_METdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_METup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_METdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__METdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_METup, "METup");
    nominal->AddFriend(friend_METdo, "METdo");
    auto varBranches_METup = getBranchNames(friend_METup);
    auto varBranches_METdo = getBranchNames(friend_METdo);
    auto* friend_JERup = new TChain("Events");
    auto* friend_JERdo = new TChain("Events");
    friend_JERup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JERdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part10.root");
    friend_JERup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JERdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part11.root");
    friend_JERup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JERdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part12.root");
    friend_JERup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JERdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part13.root");
    friend_JERup->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERup_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    friend_JERdo->Add("/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Summer20UL18_106x_nAODv9_Full2018v9/MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9__JERdo_suffix/nanoLatino_DYJetsToLL_M-10to50_NLO__part14.root");
    nominal->AddFriend(friend_JERup, "JERup");
    nominal->AddFriend(friend_JERdo, "JERdo");
    auto varBranches_JERup = getBranchNames(friend_JERup);
    auto varBranches_JERdo = getBranchNames(friend_JERdo);
    ROOT::RDataFrame base_df(*nominal);
    auto varied_df = ROOT::RDF::RNode(base_df);


    // ----------------------------------------

    std::map<std::string, std::vector<ROOT::RDF::RResultPtr<TH1D>>> hist_map_1D;
    std::map<std::string, std::vector<ROOT::RDF::RResultPtr<TH2D>>> hist_map_2D;

    // --- Automatically generated define aliases ---

    //  aliases/define needed
    varied_df = SafeDefine(varied_df, "PromptGenLepMatch2l", "(Lepton_promptgenmatched.size() > 0 ? Lepton_promptgenmatched[0] : 0.) * (Lepton_promptgenmatched.size() > 1 ? Lepton_promptgenmatched[0] : 0.)");
    varied_df = SafeDefine(varied_df, "mumu", "(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == -13*13");
    varied_df = SafeDefine(varied_df, "ee", "(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == -11*11");
    varied_df = SafeDefine(varied_df, "LepWPCut", "LepCut2l__ele_mvaFall17V2Iso_WP90_SS_tthmva_70__mu_cut_Tight_HWWW_tthmva_80");
    varied_df = SafeDefine(varied_df, "bVeto", "ROOT::VecOps::Sum(Take(Jet_btagDeepB,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) == 0");
    varied_df = SafeDefine(varied_df, "bReq", "ROOT::VecOps::Sum(Take(Jet_btagDeepB,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) >= 1");
    varied_df = SafeDefine(varied_df, "bVetoSF", "ROOT::VecOps::Product(Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5])");
    varied_df = SafeDefine(varied_df, "bReqSF", "ROOT::VecOps::Product(Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)[CleanJet_pt > 30 && abs(CleanJet_eta) < 2.5])");
    varied_df = SafeDefine(varied_df, "btagSF", "bVeto*bVetoSF + bReq*bReqSF");
    varied_df = SafeDefine(varied_df, "Jet_PUIDSF", "ROOT::VecOps::Product(Jet_PUIDSF_loose[Jet_jetId>=2])");
    varied_df = SafeDefine(varied_df, "Jet_PUIDSF_up", "ROOT::VecOps::Product(Jet_PUIDSF_loose_up[Jet_jetId>=2])");
    varied_df = SafeDefine(varied_df, "Jet_PUIDSF_down", "ROOT::VecOps::Product(Jet_PUIDSF_loose_down[Jet_jetId>=2])");
    varied_df = SafeDefine(varied_df, "LepWPSF", "LepSF2l__ele_mvaFall17V2Iso_WP90_SS_tthmva_70__mu_cut_Tight_HWWW_tthmva_80");
    varied_df = SafeDefine(varied_df, "SFweightEleUp", "LepSF2l__ele_mvaFall17V2Iso_WP90_SS_tthmva_70__Up");
    varied_df = SafeDefine(varied_df, "SFweightEleDown", "LepSF2l__ele_mvaFall17V2Iso_WP90_SS_tthmva_70__Do");
    varied_df = SafeDefine(varied_df, "SFweightMuUp", "LepSF2l__mu_cut_Tight_HWWW_tthmva_80__Up");
    varied_df = SafeDefine(varied_df, "SFweightMuDown", "LepSF2l__mu_cut_Tight_HWWW_tthmva_80__Do");
    varied_df = SafeDefine(varied_df, "ptj1", "CleanJet_pt.size() > 0 ? CleanJet_pt[0] : -9999.");
    varied_df = SafeDefine(varied_df, "njet", "Sum(CleanJet_pt>30)");


    // ----------------------------------------

    // --- Automatically generated define aliases for functions, no JIT ---

    //  aliases of functions
    varied_df = varied_df
                     .Define("_var_myVariableMll_0", "Lepton_pt[0]")
                     .Define("_var_myVariableMll_1", "Lepton_eta[0]")
                     .Define("_var_myVariableMll_2", "Lepton_phi[0]")
                     .Define("_var_myVariableMll_3", "Lepton_pt[1]")
                     .Define("_var_myVariableMll_4", "Lepton_eta[1]")
                     .Define("_var_myVariableMll_5", "Lepton_phi[1]");

    varied_df = varied_df.Define("myVariableMll", calculate_mass, { "_var_myVariableMll_0", "_var_myVariableMll_1", "_var_myVariableMll_2", "_var_myVariableMll_3", "_var_myVariableMll_4", "_var_myVariableMll_5" } );

    varied_df = varied_df
                     .Define("_var_myVariableBDT_0_f", "(float)pt1")
                     .Define("_var_myVariableBDT_1_f", "(float)ptj1")
                     .Define("_var_myVariableBDT_2_f", "(float)mll")
                     .Define("_var_myVariableBDT_3_f", "(float)njet");

    TMVA::Experimental::RReader my_model_myVariableBDT("/afs/cern.ch/work/a/amassiro/Latinos/Framework/RDF/plotsConfigurationsRDF/WW/2018/code/TMVAClassification_BDTG.weights.xml");
    varied_df = varied_df.Define("myVariableBDT",
                     TMVA::Experimental::Compute<4, float>(my_model_myVariableBDT),
                     { "_var_myVariableBDT_0_f", "_var_myVariableBDT_1_f", "_var_myVariableBDT_2_f", "_var_myVariableBDT_3_f" });



    // ----------------------------------------

    // --- Automatically generated define weights ---

    //  weigths needed
    varied_df = SafeDefine(varied_df, "my_sample_weight", "(XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83");


    // ----------------------------------------


    // --- Automatically generated register nuisances variations ---

    //  Register the variations
    varied_df = varied_df.Vary(
      "my_sample_weight",
      "ROOT::RVecD{((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * TriggerSFWeight_2l_u/TriggerSFWeight_2l,((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * TriggerSFWeight_2l_d/TriggerSFWeight_2l}",
      {"up", "do"},
      "CMS_eff_hwwtrigger_2018"
      );
    varied_df = varied_df.Vary(
      "my_sample_weight",
      "ROOT::RVecD{((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * SFweightEleUp,((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * SFweightEleDown}",
      {"up", "do"},
      "CMS_eff_e_2018"
      );
    int suffix_size = 0;
    suffix_size = 8;
    for (const auto& branch : varBranches_ElepTup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "ElepTup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "scale_e_2018_UL"
                                    );
        };
      };
    };
    
    suffix_size = 8;
    for (const auto& branch : varBranches_ElepTdo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "ElepTdo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "scale_e_2018_UL"
                                    );
        };
      };
    };
    varied_df = varied_df.Vary(
      "my_sample_weight",
      "ROOT::RVecD{(XSWeight * SFweight2l * LepWPCut * LepWPSF*SFweightMuUp * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83,(XSWeight * SFweight2l * LepWPCut * LepWPSF*SFweightMuDown * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83}",
      {"up", "do"},
      "eff_m_2018"
      );
    suffix_size = 7;
    for (const auto& branch : varBranches_MupTup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "MupTup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_m_2018"
                                    );
        };
      };
    };
    
    suffix_size = 7;
    for (const auto& branch : varBranches_MupTdo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "MupTdo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_m_2018"
                                    );
        };
      };
    };
    varied_df = varied_df.Vary(
      "my_sample_weight",
      "ROOT::RVecD{((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * Jet_PUIDSF_up/Jet_PUIDSF,((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * Jet_PUIDSF_down/Jet_PUIDSF}",
      {"up", "do"},
      "CMS_PUID_2018"
      );
    suffix_size = 14;
    for (const auto& branch : varBranches_JESAbsoluteup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESAbsoluteup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESAbsolute"
                                    );
        };
      };
    };
    
    suffix_size = 14;
    for (const auto& branch : varBranches_JESAbsolutedo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESAbsolutedo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESAbsolute"
                                    );
        };
      };
    };
    suffix_size = 19;
    for (const auto& branch : varBranches_JESAbsolute_2018up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESAbsolute_2018up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESAbsolute_2018"
                                    );
        };
      };
    };
    
    suffix_size = 19;
    for (const auto& branch : varBranches_JESAbsolute_2018do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESAbsolute_2018do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESAbsolute_2018"
                                    );
        };
      };
    };
    suffix_size = 11;
    for (const auto& branch : varBranches_JESBBEC1up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESBBEC1up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESBBEC1"
                                    );
        };
      };
    };
    
    suffix_size = 11;
    for (const auto& branch : varBranches_JESBBEC1do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESBBEC1do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESBBEC1"
                                    );
        };
      };
    };
    suffix_size = 16;
    for (const auto& branch : varBranches_JESBBEC1_2018up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESBBEC1_2018up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESBBEC1_2018"
                                    );
        };
      };
    };
    
    suffix_size = 16;
    for (const auto& branch : varBranches_JESBBEC1_2018do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESBBEC1_2018do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESBBEC1_2018"
                                    );
        };
      };
    };
    suffix_size = 9;
    for (const auto& branch : varBranches_JESEC2up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESEC2up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESEC2"
                                    );
        };
      };
    };
    
    suffix_size = 9;
    for (const auto& branch : varBranches_JESEC2do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESEC2do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESEC2"
                                    );
        };
      };
    };
    suffix_size = 14;
    for (const auto& branch : varBranches_JESEC2_2018up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESEC2_2018up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESEC2_2018"
                                    );
        };
      };
    };
    
    suffix_size = 14;
    for (const auto& branch : varBranches_JESEC2_2018do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESEC2_2018do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESEC2_2018"
                                    );
        };
      };
    };
    suffix_size = 15;
    for (const auto& branch : varBranches_JESFlavorQCDup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESFlavorQCDup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESFlavorQCD"
                                    );
        };
      };
    };
    
    suffix_size = 15;
    for (const auto& branch : varBranches_JESFlavorQCDdo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESFlavorQCDdo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESFlavorQCD"
                                    );
        };
      };
    };
    suffix_size = 8;
    for (const auto& branch : varBranches_JESHFup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESHFup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESHF"
                                    );
        };
      };
    };
    
    suffix_size = 8;
    for (const auto& branch : varBranches_JESHFdo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESHFdo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESHF"
                                    );
        };
      };
    };
    suffix_size = 13;
    for (const auto& branch : varBranches_JESHF_2018up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESHF_2018up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESHF_2018"
                                    );
        };
      };
    };
    
    suffix_size = 13;
    for (const auto& branch : varBranches_JESHF_2018do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESHF_2018do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESHF_2018"
                                    );
        };
      };
    };
    suffix_size = 17;
    for (const auto& branch : varBranches_JESRelativeBalup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESRelativeBalup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESRelativeBal"
                                    );
        };
      };
    };
    
    suffix_size = 17;
    for (const auto& branch : varBranches_JESRelativeBaldo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESRelativeBaldo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESRelativeBal"
                                    );
        };
      };
    };
    suffix_size = 25;
    for (const auto& branch : varBranches_JESRelativeSample_2018up) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESRelativeSample_2018up") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_JESRelativeSample_2018"
                                    );
        };
      };
    };
    
    suffix_size = 25;
    for (const auto& branch : varBranches_JESRelativeSample_2018do) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JESRelativeSample_2018do") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_JESRelativeSample_2018"
                                    );
        };
      };
    };
    suffix_size = 6;
    for (const auto& branch : varBranches_METup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "METup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_scale_met_2018"
                                    );
        };
      };
    };
    
    suffix_size = 6;
    for (const auto& branch : varBranches_METdo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "METdo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_scale_met_2018"
                                    );
        };
      };
    };
    suffix_size = 6;
    for (const auto& branch : varBranches_JERup) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JERup") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"up"},
                                    "CMS_res_j_2018"
                                    );
        };
      };
    };
    
    suffix_size = 6;
    for (const auto& branch : varBranches_JERdo) {
      if (int(branch.size()) >= (suffix_size-1) && branch.compare(branch.size() - (suffix_size-1), (suffix_size-1), "JERdo") == 0 ) {
        std::string nomCol = branch.substr(0, branch.size() - suffix_size);
        if (std::find(nominalBranches.begin(), nominalBranches.end(), nomCol) != nominalBranches.end()) {
          std::string expression = "ROOT::RVec<" + varied_df.GetColumnType(nomCol) + ">{static_cast<" + varied_df.GetColumnType(nomCol) + ">(" + branch + ")}";
          varied_df = varied_df.Vary(
                                    nomCol,
                                    expression,
                                    {"do"},
                                    "CMS_res_j_2018"
                                    );
        };
      };
    };
    varied_df = varied_df.Vary(
      "my_sample_weight",
      "ROOT::RVecD{((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * PSWeight[2],((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * PSWeight[0]}",
      {"up", "do"},
      "ps_isr"
      );
    varied_df = varied_df.Vary(
      "my_sample_weight",
      "ROOT::RVecD{((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * PSWeight[3],((XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l) * 59.83) * PSWeight[1]}",
      {"up", "do"},
      "ps_fsr"
      );


    // ----------------------------------------

    // --- Automatically generated ... just the main node ---

    //  Initial ...
    auto current_node = ROOT::RDF::RNode(varied_df);


    //
    // from now on: current_node
    // ----------------------------------------

    // --- Automatically generated define cuts and histograms ---

    //  I need to define the RNode, otherwise SafeDefine will not work
    // Define variables 
    current_node = SafeDefine(current_node, "events", "1");
    current_node = SafeDefine(current_node, "ptj1", "CleanJet_pt.size() > 0 ? CleanJet_pt[0] : -9999.");
    current_node = SafeDefine(current_node, "mll", "mll");
    current_node = SafeDefine(current_node, "mll_mybins", "mll");
    current_node = SafeDefine(current_node, "njet", "Sum(CleanJet_pt>30)");
    current_node = SafeDefine(current_node, "BDT", "myVariableBDT");
    current_node = current_node.Filter("  (Lepton_pdgId.size() > 0 ? Lepton_pt[0]: -99) > 25            && (Lepton_pdgId.size() > 1 ? Lepton_pt[1]: -99) > 10            && abs(Lepton_pdgId.size() > 0 ? Lepton_eta[0]: -99) < 2.5            && abs(Lepton_pdgId.size() > 1 ? Lepton_eta[1]: -99) < 2.5 ", "supercut");
    auto node_lowmll_ee = current_node.Filter("mll>40 && mll<120 && ee", "lowmll_ee");
    hist_map_1D["lowmll_ee"].push_back(node_lowmll_ee.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["lowmll_ee"].push_back(node_lowmll_ee.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["lowmll_ee"].push_back(node_lowmll_ee.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["lowmll_ee"].push_back(node_lowmll_ee.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["lowmll_ee"].push_back(node_lowmll_ee.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["lowmll_ee"].push_back(node_lowmll_ee.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["lowmll_ee"].push_back(node_lowmll_ee.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    auto node_highmll_ee = current_node.Filter("mll>120 && ee", "highmll_ee");
    hist_map_1D["highmll_ee"].push_back(node_highmll_ee.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["highmll_ee"].push_back(node_highmll_ee.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["highmll_ee"].push_back(node_highmll_ee.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["highmll_ee"].push_back(node_highmll_ee.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["highmll_ee"].push_back(node_highmll_ee.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["highmll_ee"].push_back(node_highmll_ee.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["highmll_ee"].push_back(node_highmll_ee.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    auto node_lowmll_mumu = current_node.Filter("mll>40 && mll<120  && mumu", "lowmll_mumu");
    hist_map_1D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["lowmll_mumu"].push_back(node_lowmll_mumu.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    auto node_highmll_mumu = current_node.Filter("mll>120 && mumu", "highmll_mumu");
    hist_map_1D["highmll_mumu"].push_back(node_highmll_mumu.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["highmll_mumu"].push_back(node_highmll_mumu.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["highmll_mumu"].push_back(node_highmll_mumu.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["highmll_mumu"].push_back(node_highmll_mumu.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["highmll_mumu"].push_back(node_highmll_mumu.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["highmll_mumu"].push_back(node_highmll_mumu.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["highmll_mumu"].push_back(node_highmll_mumu.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    auto node_DY_cr = current_node.Filter("(mll>70 && mll<120)", "DY_cr");
    hist_map_1D["DY_cr"].push_back(node_DY_cr.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["DY_cr"].push_back(node_DY_cr.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["DY_cr"].push_back(node_DY_cr.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["DY_cr"].push_back(node_DY_cr.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["DY_cr"].push_back(node_DY_cr.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["DY_cr"].push_back(node_DY_cr.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["DY_cr"].push_back(node_DY_cr.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    auto node_DY_cr_eleele = node_DY_cr.Filter("(ee)", "DY_cr_eleele");
    hist_map_1D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["DY_cr_eleele"].push_back(node_DY_cr_eleele.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    auto node_DY_cr_mumu = node_DY_cr.Filter("(mumu)", "DY_cr_mumu");
    hist_map_1D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo1D({"h_events", "events", 1, 0, 2}, "events", "my_sample_weight"));
    hist_map_1D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo1D({"h_ptj1", "ptj1", 15, 0.0, 200}, "ptj1", "my_sample_weight"));
    hist_map_1D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo1D({"h_mll", "mll", 200, 10, 500}, "mll", "my_sample_weight"));
    hist_map_1D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo1D({"h_mll_mybins", "mll_mybins", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}}, "mll_mybins", "my_sample_weight"));
    hist_map_2D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo2D({"h_mllVSmth", "mllVSmth", 8, (const double[]){12, 17, 25, 30, 35, 40, 45, 65, 200}, 4, (const double[]){60, 95, 110, 135, 200}}, "mll", "mth", "my_sample_weight"));
    hist_map_1D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo1D({"h_njet", "njet", 8, 0, 8}, "njet", "my_sample_weight"));
    hist_map_1D["DY_cr_mumu"].push_back(node_DY_cr_mumu.Histo1D({"h_BDT", "BDT", 200, 0, 1}, "BDT", "my_sample_weight"));
    
    std::vector<std::string> list_of_variables_1D;
    std::vector<std::string> list_of_variables_2D;
    std::vector<int> list_of_variables_fold_1D;
    std::vector<int> list_of_variables_fold_2D;
    list_of_variables_1D.push_back("events");
    list_of_variables_fold_1D.push_back(3);
    list_of_variables_1D.push_back("ptj1");
    list_of_variables_fold_1D.push_back(3);
    list_of_variables_1D.push_back("mll");
    list_of_variables_fold_1D.push_back(3);
    list_of_variables_1D.push_back("mll_mybins");
    list_of_variables_fold_1D.push_back(3);
    list_of_variables_2D.push_back("mllVSmth");
    list_of_variables_fold_2D.push_back(3);
    list_of_variables_1D.push_back("njet");
    list_of_variables_fold_1D.push_back(2);
    list_of_variables_1D.push_back("BDT");
    list_of_variables_fold_1D.push_back(3);


    // ----------------------------------------


    //
    // In root file:    <cut>/<variable>/histo_<sample>
    //

    TFile out_file("rootFile/root_file___DY_DYJetsToLL_M-10to50_NLO_2.root", "RECREATE");
    for (auto& [cut_label, h_list] : hist_map_1D) {
        std::cout << " cut = " << cut_label << std::endl;
        // Create a folder for this cut
        TDirectory *dir = out_file.mkdir(cut_label.c_str());
        int ivar = 0;
        for (auto& h : h_list) {
          dir->cd();
          TDirectory *subdir = out_file.mkdir( (cut_label+"/"+list_of_variables_1D.at(ivar)).c_str() );
          subdir->cd();

          std::cout << "   >> variable = " << list_of_variables_1D.at(ivar) << std::endl;

          // get the nominal and the variations too
          auto all_histos = ROOT::RDF::Experimental::VariationsFor(h);

          for (auto& [name, histo] : all_histos) {
            std::cout << "     name histo = " << name << std::endl;
            std::string temp_name;
            if (name == "nominal") {
                temp_name = "histo_DY";
            }
            else {
              temp_name = name.c_str();
              // scale_e_2018_UL:up --> scale_e_2018_ULup
              temp_name.erase(std::remove(temp_name.begin(), temp_name.end(), ':'), temp_name.end());
              //size_t pos = temp_name.find(':');
              //if (pos != std::string::npos) {
              //  temp_name = temp_name.substr(0, pos);
              //}
              temp_name = ("histo_DY_" + temp_name);
            }
            gDirectory = subdir;
            histo->SetName(temp_name.c_str());

            if (list_of_variables_fold_1D.at(ivar) != 0) FoldHistogram(histo.get(), list_of_variables_fold_1D.at(ivar));

            histo->Write();

          }
          ivar++;
        }
        out_file.cd(); // Go back to root for the next directory
    }


    for (auto& [cut_label, h_list] : hist_map_2D) {
        // Create a folder for this cut
        TDirectory *dir = out_file.GetDirectory(cut_label.c_str());
        if (!dir) {
          // If it doesn't exist, create it
          dir = out_file.mkdir(cut_label.c_str());
        }
        int ivar = 0;
        for (auto& h : h_list) {
          dir->cd();
          TDirectory *subdir = out_file.GetDirectory( (cut_label+"/"+list_of_variables_2D.at(ivar)).c_str() );
          if (!subdir) {
            // If it doesn't exist, create it
            subdir = out_file.mkdir( (cut_label+"/"+list_of_variables_2D.at(ivar)).c_str() );
          }
          subdir->cd();

          // get the nominal and the variations too
          auto all_histos = ROOT::RDF::Experimental::VariationsFor(h);

          for (auto& [name, histo] : all_histos) {
            std::string temp_name;
            if (name == "nominal") {
                temp_name = "histo_DY";
            }
            else {
              temp_name = name.c_str();
              // scale_e_2018_UL:up --> scale_e_2018_ULup
              temp_name.erase(std::remove(temp_name.begin(), temp_name.end(), ':'), temp_name.end());
              //size_t pos = temp_name.find(':');
              //if (pos != std::string::npos) {
              //  temp_name = temp_name.substr(0, pos);
              //}
              temp_name = ("histo_DY_" + temp_name);
            }
            gDirectory = subdir;
            histo->SetName(temp_name.c_str());


            if (list_of_variables_fold_2D.at(ivar) != 0) FoldHistogram(histo.get(), list_of_variables_fold_2D.at(ivar));

            UnrollHistogram(dynamic_cast<TH2D*>(histo.get()))->Write();

          }
          ivar++;
        }
        out_file.cd(); // Go back to root for the next directory
    }



    out_file.Close();

    return 0;
}

        