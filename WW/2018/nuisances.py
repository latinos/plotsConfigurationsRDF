


def makeMCDirectory(var=""):
    treeBaseDir = '/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano'
    mcProduction = 'Summer20UL18_106x_nAODv9_Full2018v9'
    mcSteps = 'MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9'
    _treeBaseDir = treeBaseDir + ""
    if var == "":
        return "/".join([_treeBaseDir, mcProduction, mcSteps])
    else:
        return "/".join([_treeBaseDir, mcProduction, mcSteps + "__" + var])


# mcALL     = [skey for skey in samples if skey not in ('DATA', 'Fake_lep')]








nuisances['lumi_Uncorrelated'] = {
    'name': 'lumi_13TeV_2018',
    'type': 'lnN',
    'samples': dict((skey, '1.015') for skey in mcALL if skey not in ['top', 'dytt'])
}

nuisances['lumi_correlated'] = {
    'name': 'lumi_13TeV_correlated',
    'type': 'lnN',
    'samples': dict((skey, '1.009') for skey in mcALL if skey not in ['top', 'dytt'])
}

nuisances['lumi_correlated_1718'] = {
    'name': 'lumi_13TeV_correlated_1718',
    'type': 'lnN',
    'samples': dict((skey, '1.006') for skey in mcALL if skey not in ['top', 'dytt'])
}



nuisances['fake_syst_e'] = {
    'name': 'CMS_fake_syst_e',
    'skipCMS' : 1,
    'type': 'lnN',
    'samples': {
        'Fake_e': '1.3'
    },
}

nuisances['fake_syst_m'] = {
    'name': 'CMS_fake_syst_m',
    'skipCMS' : 1,
    'type': 'lnN',
    'samples': {
        'Fake_m': '1.3'
    },
}

nuisances['fake_ele'] = {
    'name': 'CMS_fake_e_2018',
    'skipCMS' : 1,
    'kind': 'weight',
    'type': 'shape',
    'samples': {
        'Fake_e': ['fakeWEleUp', 'fakeWEleDown'],
        'Fake_m': ['fakeWEleUp', 'fakeWEleDown'],
    }
}


nuisances['fake_ele_stat'] = {
    'name': 'CMS_fake_stat_e_2018',
    'skipCMS' : 1,
    'kind': 'weight',
    'type': 'shape',
    'samples': {
        'Fake_e': ['fakeWStatEleUp', 'fakeWStatEleDown'],
        'Fake_m': ['fakeWStatEleUp', 'fakeWStatEleDown']
    }
}

nuisances['fake_mu'] = {
    'name': 'CMS_fake_m_2018',
    'skipCMS' : 1,
    'kind': 'weight',
    'type': 'shape',
    'samples': {
        'Fake_e': ['fakeWMuUp', 'fakeWMuDown'],
        'Fake_m': ['fakeWMuUp', 'fakeWMuDown']
    }
}

nuisances['fake_mu_stat'] = {
    'name': 'CMS_fake_stat_m_2018',
    'skipCMS' : 1,
    'kind': 'weight',
    'type': 'shape',
    'samples': {
        'Fake_e': ['fakeWStatMuUp', 'fakeWStatMuDown'],
        'Fake_m': ['fakeWStatMuUp', 'fakeWStatMuDown']
    }
}







### B-tagger
# Fixed BTV SF variations
for flavour in ['bc', 'light']:
    for corr in ['uncorrelated', 'correlated']:
        btag_syst = [f'btagSF{flavour}_up_{corr}/btagSF{flavour}', f'btagSF{flavour}_down_{corr}/btagSF{flavour}']
        if corr == 'correlated':
            name = f'CMS_btagSF{flavour}_{corr}'
        else:
            name = f'CMS_btagSF{flavour}_2018'
        nuisances[f'btagSF{flavour}{corr}'] = {
            'name': name,
            'skipCMS' : 1,
            'kind': 'weight',
            'type': 'shape',
            'samples': dict((skey, btag_syst) for skey in mcALL),
        }




##### Trigger Efficiency

trig_syst = ['TriggerSFWeight_2l_u/TriggerSFWeight_2l', 'TriggerSFWeight_2l_d/TriggerSFWeight_2l']

nuisances['trigg'] = {
    'name': 'CMS_eff_hwwtrigger_2018',
    'skipCMS' : 1,
    'kind': 'weight',
    'type': 'shape',
    'samples': dict((skey, trig_syst) for skey in mcALL)
}













nuisances['QCDscale_CRSR_accept_dytt']  = {
               'name'  : 'QCDscale_CRSR_accept_dytt',
               'type'  : 'lnN',
               'samples'  : {
                   'DY' : '1.02',
                   },
               'cuts'  : [
                 'hww2l2v_13TeV_dytt_of0j',
                 'hww2l2v_13TeV_dytt_of1j',
                 'hww2l2v_13TeV_dytt_of2j',
                 'hww2l2v_13TeV_dytt_of2j_vbf',
                 'hww2l2v_13TeV_dytt_of2j_vh2j'
                ]
              }



nuisances['electronpt'] = {
    'name': 'scale_e_2018_UL',
    'kind': 'suffix',
    'type': 'shape',
    'mapUp': 'ElepTup',
    'mapDown': 'ElepTdo',
    'samples': dict((skey, ['1', '1']) for skey in mcALL),
    'folderUp': makeMCDirectory('ElepTup_suffix'),
    'folderDown': makeMCDirectory('ElepTdo_suffix'),
}



# ------------------- muon efficiency and energy scale
nuisances['eff_m'] = {
    'name': 'eff_m_2018',
    'kind': 'weight',
    'type': 'shape',
    #                        nominal          up                     down
    'samples': dict((skey, ['LepWPSF',   'LepWPSF*SFweightMuUp', 'LepWPSF*SFweightMuDown']) for skey in mcALL)
}






# nuisances['eff_m'] = {
#     'name': 'eff_m_2018_UL',
#     'kind': 'weight',
#     'type': 'shape',
#     #                        nominal          up               down
#     'samples': dict((skey, ['SFweightMu','SFweightMuUp', 'SFweightMuDown']) for skey in mcALL)
# }


# aliases['SFweightMuUp'] = {
#     'expr': 'LepSF3l__mu_'+muWP+'__Up',
#     'samples': mcALL
# }
# aliases['SFweightMuDown'] = {
#     'expr': 'LepSF3l__mu_'+muWP+'__Do',
#     'samples': mcALL
# }












