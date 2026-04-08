#
# List of aliases to be "defined"
# This list will NOT be the actual "Alias" of RDataFrame (as useless)
# but "Define" of all this math will be performed
#

muWP  = 'cut_Tight_HWWW_tthmva_80'
eleWP = 'mvaFall17V2Iso_WP90_SS_tthmva_70'


aliases['PromptGenLepMatch2l'] = {
    # 'expr': 'Alt(Lepton_promptgenmatched,0,0)*Alt(Lepton_promptgenmatched,1,0)',
    # (Lepton_promptgenmatched.size() > 0 ? Lepton_promptgenmatched[0] : 0.)
    # (Lepton_promptgenmatched.size() > 1 ? Lepton_promptgenmatched[0] : 0.)
    'expr': '(Lepton_promptgenmatched.size() > 0 ? Lepton_promptgenmatched[0] : 0.) * (Lepton_promptgenmatched.size() > 1 ? Lepton_promptgenmatched[0] : 0.)',
    'samples': mcALL
}


aliases['SFweightEle'] = {
    'expr': '1',
    # 'expr': 'LepSF3l__ele_'+muWP,
    'samples': mcALL
}


aliases['SFweightMu'] = {
    'expr': '1',
    # 'expr': 'LepSF3l__mu_'+muWP,
    'samples': mcALL
}
aliases['SFweightMuUp'] = {
    'expr': 'LepSF3l__mu_'+muWP+'__Up',
    'samples': mcALL
}
aliases['SFweightMuDown'] = {
    'expr': 'LepSF3l__mu_'+muWP+'__Do',
    'samples': mcALL
}


aliases['mumu'] = {
    # 'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0]==13 : 0) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]==13 : 0)',
    'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == -13*13',
    'samples': ALL
}

aliases['ee'] = {
    # 'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0]==13 : 0) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]==13 : 0)',
    'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == -11*11',
    'samples': ALL
}




# -------- lepton WP
aliases['LepWPCut'] = {
    'expr': 'LepCut3l__ele_'+eleWP+'__mu_'+muWP ,
    'samples': ALL
}



# ---------------------------- btagging (new)
#loose 0.1241
#medium 0.4184
#tight 0.7527
aliases['bVeto'] = {
    # 'expr': 'ROOT::VecOps::Sum(CleanJet_pt > 20. && abs(CleanJet_eta) < 2.5 && Take(Jet_btagDeepB,CleanJet_jetIdx) > 0.4184) == 0'
    # 'expr' : 'ROOT::VecOps::Sum(Jet_btagDeepB[CleanJet_jetIdx][CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) == 0',
    'expr' : 'ROOT::VecOps::Sum(Take(Jet_btagDeepB,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) == 0',
    'samples': ALL
}

aliases['bReq'] = {
    # 'expr' : 'ROOT::VecOps::Sum(Jet_btagDeepB[CleanJet_jetIdx][CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) >= 1',
    'expr' : 'ROOT::VecOps::Sum(Take(Jet_btagDeepB,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) >= 1',
    'samples': ALL
}

aliases['bVetoSF'] = {
   # 'expr': 'TMath::Exp(ROOT::VecOps::Sum(LogVec((CleanJet_pt>20 && abs(CleanJet_eta)<2.5)*Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)+1*(CleanJet_pt<=20 || abs(CleanJet_eta)>=2.5))))',
    # 'expr' : 'ROOT::VecOps::Product(Jet_btagSF_deepcsv_shape[CleanJet_jetIdx][CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5])',
    'expr' : 'ROOT::VecOps::Product(Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5])',
    'samples': mcALL
}

aliases['bReqSF'] = {
    # 'expr': 'TMath::Exp(ROOT::VecOps::Sum(LogVec((CleanJet_pt>30 && abs(CleanJet_eta)<2.5)*Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)+1*(CleanJet_pt<=30 || abs(CleanJet_eta)>=2.5))))',
    # 'expr' : 'ROOT::VecOps::Product(Jet_btagSF_deepcsv_shape[CleanJet_jetIdx][CleanJet_pt > 30 && abs(CleanJet_eta) < 2.5])',
    'expr' : 'ROOT::VecOps::Product(Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)[CleanJet_pt > 30 && abs(CleanJet_eta) < 2.5])',
    'samples': mcALL
}

aliases['btagSF'] = {
    'expr': 'bVeto*bVetoSF + bReq*bReqSF',
    'samples': mcALL
}





# --------------------------- PU weights
aliases['Jet_PUIDSF'] = {
  # 'expr' : 'TMath::Exp(ROOT::VecOps::Sum((Jet_jetId>=2)*LogVec(Jet_PUIDSF_loose)))',
  'expr' : 'ROOT::VecOps::Product(Jet_PUIDSF_loose[Jet_jetId>=2])',
  'samples': mcALL
}

aliases['Jet_PUIDSF_up'] = {
  # 'expr' : 'TMath::Exp(ROOT::VecOps::Sum((Jet_jetId>=2)*LogVec(Jet_PUIDSF_loose_up)))',
  'expr' : 'ROOT::VecOps::Product(Jet_PUIDSF_loose_up[Jet_jetId>=2])',
  'samples': mcALL
}

aliases['Jet_PUIDSF_down'] = {
  # 'expr' : 'TMath::Exp(ROOT::VecOps::Sum((Jet_jetId>=2)*LogVec(Jet_PUIDSF_loose_down)))',
  'expr' : 'ROOT::VecOps::Product(Jet_PUIDSF_loose_down[Jet_jetId>=2])',
  'samples': mcALL
}



# data/MC scale factors
aliases['SFweight'] = {
    #'expr': ' * '.join(['SFweight2l', 'LepWPCut', 'LepWPSF','Jet_PUIDSF', 'btagSF', 'LepWPttHMVASF']),
    # 'expr': ' * '.join(['SFweight2l', 'LepWPCut', 'LepWPSF','Jet_PUIDSF', 'btagSF']),
    'expr': ' * '.join(['SFweight2l', 'LepWPCut', 'Jet_PUIDSF', 'btagSF']),
    'samples' : mcALL
}






# aliases['luminosity'] = {
#     'expr': ''
#     # 'expr': '(ROOT::VecOps::Sum(Tau_pt > 18 && abs(Tau_eta)<2.3 && Tau_decayMode &&sqrt( pow(Tau_eta - Lepton_eta[0], 2) + pow(abs(abs(Tau_phi - Lepton_phi[0])-3.1415)-3.1415, 2) ) >= 0.4 && sqrt( pow(Tau_eta - Lepton_eta[1], 2) + pow(abs(abs(Tau_phi - Lepton_phi[1])-3.1415)-3.1415, 2) ) >= 0.4) == 0)'
# }


#
# def CombineBaseW(samples, proc, samplelist):
#     _filtFiles = list(filter(lambda k: k[0] in samplelist, samples[proc]['name']))
#     _files = list(map(lambda k: k[1], _filtFiles))
#     _l = list(map(lambda k: len(k), _files))
#     leastFiles = _files[_l.index(min(_l))]
#     dfSmall = ROOT.RDataFrame("Runs", leastFiles)
#     s = dfSmall.ROOT::VecOps::Sum('genEventSumw').GetValue()
#     f = ROOT.TFile(leastFiles[0])
#     t = f.Get("Events")
#     t.GetEntry(1)
#     xs = t.baseW * s
#
#     __files = []
#     for f in _files:
#         __files += f
#     df = ROOT.RDataFrame("Runs", __files)
#     s = df.ROOT::VecOps::Sum('genEventSumw').GetValue()
#     newbaseW = str(xs / s)
#     weight = newbaseW + '/baseW'
#
#     for iSample in samplelist:
#         addSampleWeight(samples, proc, iSample, weight)




















