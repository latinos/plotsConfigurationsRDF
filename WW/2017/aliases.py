#
# List of aliases to be "defined"
# This list will NOT be the actual "Alias" of RDataFrame (as useless)
# but "Define" of all this math will be performed
#

muWP  = 'cut_Tight_HWWW'
eleWP = 'mvaFall17V2Iso_WP90'


aliases['PromptGenLepMatch2l'] = {
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
    'expr': 'LepSF2l__mu_'+muWP+'__Up',
    'samples': mcALL
}
aliases['SFweightMuDown'] = {
    'expr': 'LepSF2l__mu_'+muWP+'__Do',
    'samples': mcALL
}


aliases['mumu'] = {
    'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == -13*13',
    'samples': ALL
}

aliases['ee'] = {
    'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == -11*11',
    'samples': ALL
}




# -------- lepton WP
aliases['LepWPCut'] = {
    'expr': 'LepCut2l__ele_'+eleWP+'__mu_'+muWP ,
    'samples': ALL
}



# ---------------------------- btagging (new)
#loose 0.1241
#medium 0.4184
#tight 0.7527
aliases['bVeto'] = {
    'expr' : 'ROOT::VecOps::Sum(Take(Jet_btagDeepB,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) == 0',
    'samples': ALL
}

aliases['bReq'] = {
    'expr' : 'ROOT::VecOps::Sum(Take(Jet_btagDeepB,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5] > 0.4184) >= 1',
    'samples': ALL
}

aliases['bVetoSF'] = {
    'expr' : 'ROOT::VecOps::Product(Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)[CleanJet_pt > 20 && abs(CleanJet_eta) < 2.5])',
    'samples': mcALL
}

aliases['bReqSF'] = {
    'expr' : 'ROOT::VecOps::Product(Take(Jet_btagSF_deepcsv_shape,CleanJet_jetIdx)[CleanJet_pt > 30 && abs(CleanJet_eta) < 2.5])',
    'samples': mcALL
}

aliases['btagSF'] = {
    'expr': 'bVeto*bVetoSF + bReq*bReqSF',
    'samples': mcALL
}





# --------------------------- PU weights
aliases['Jet_PUIDSF'] = {
  'expr' : 'ROOT::VecOps::Product(Jet_PUIDSF_loose[Jet_jetId>=2])',
  'samples': mcALL
}

aliases['Jet_PUIDSF_up'] = {
  'expr' : 'ROOT::VecOps::Product(Jet_PUIDSF_loose_up[Jet_jetId>=2])',
  'samples': mcALL
}

aliases['Jet_PUIDSF_down'] = {
  'expr' : 'ROOT::VecOps::Product(Jet_PUIDSF_loose_down[Jet_jetId>=2])',
  'samples': mcALL
}



# data/MC scale factors
aliases['SFweight'] = {
    #'expr': ' * '.join(['SFweight2l', 'LepWPCut', 'LepWPSF','Jet_PUIDSF', 'btagSF', 'LepWPttHMVASF']),
    # 'expr': ' * '.join(['SFweight2l', 'LepWPCut', 'LepWPSF','Jet_PUIDSF', 'btagSF']),
    # 'expr': ' * '.join(['SFweight2l', 'LepWPCut', 'Jet_PUIDSF', 'btagSF']),
    'expr': ' * '.join(['SFweight2l', 'LepWPCut']),
    'samples' : mcALL
}




#
# external defined variable: c++ code
#
# aliases['myVariable'] = {
#
# }
#

# aliases['myVariableMll'] = {
#   'external' : "code/mymll.c",
#   'variables': ["Lepton_pt[0]", "Lepton_eta[0]", "Lepton_phi[0]", "Lepton_pt[1]", "Lepton_eta[1]", "Lepton_phi[1]"],
#   'function' : 'calculate_mass',
#   'samples': mcALL
# }

