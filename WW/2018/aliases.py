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
    'expr': '(Lepton_pdgId.size() > 0 ? Lepton_pdgId[0] : -99) * (Lepton_pdgId.size() > 1 ? Lepton_pdgId[1]: -99) == 13*13',
    'samples': ALL
}




# -------- lepton WP
aliases['LepWPCut'] = {
    'expr': 'LepCut3l__ele_'+eleWP+'__mu_'+muWP ,
    'samples': ALL
}




# aliases['luminosity'] = {
#     'expr': ''
#     # 'expr': '(Sum(Tau_pt > 18 && abs(Tau_eta)<2.3 && Tau_decayMode &&sqrt( pow(Tau_eta - Lepton_eta[0], 2) + pow(abs(abs(Tau_phi - Lepton_phi[0])-3.1415)-3.1415, 2) ) >= 0.4 && sqrt( pow(Tau_eta - Lepton_eta[1], 2) + pow(abs(abs(Tau_phi - Lepton_phi[1])-3.1415)-3.1415, 2) ) >= 0.4) == 0)'
# }


#
# def CombineBaseW(samples, proc, samplelist):
#     _filtFiles = list(filter(lambda k: k[0] in samplelist, samples[proc]['name']))
#     _files = list(map(lambda k: k[1], _filtFiles))
#     _l = list(map(lambda k: len(k), _files))
#     leastFiles = _files[_l.index(min(_l))]
#     dfSmall = ROOT.RDataFrame("Runs", leastFiles)
#     s = dfSmall.Sum('genEventSumw').GetValue()
#     f = ROOT.TFile(leastFiles[0])
#     t = f.Get("Events")
#     t.GetEntry(1)
#     xs = t.baseW * s
#
#     __files = []
#     for f in _files:
#         __files += f
#     df = ROOT.RDataFrame("Runs", __files)
#     s = df.Sum('genEventSumw').GetValue()
#     newbaseW = str(xs / s)
#     weight = newbaseW + '/baseW'
#
#     for iSample in samplelist:
#         addSampleWeight(samples, proc, iSample, weight)




















