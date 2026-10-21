# FIXME: check Jet_qgl is available in Run 3 nanoAOD (otherwise use e.g. Jet_btagUParTAK4QvG)

# cuts

cuts = {}

supercut = 'Lepton_pt[0]>25 \
              && (Lepton_pt.size() > 1 ? Lepton_pt[1] : 0)<10  \
              && abs(Lepton_eta[0])<2.5 \
              && bVeto \
              '


cuts['WjetsCR'] = {
    'expr' : '1',
    'categories' : {
        '0j'  : '(CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) < 30',
        '1j'  : '(CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) < 30 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) > 30',
        '2j'  : '(CleanJet_pt.size() > 2 ? CleanJet_pt[2] : 0) < 30 && (CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) > 30',
    }
}
    

cuts['Sig1l'] = {
    'expr' : '   (Lepton_pt.size() > 1 ? Lepton_pt[1] : 0)<10 \
              && (CleanJet_pt.size() > 2 ? CleanJet_pt[2] : 0) < 30 && (CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) > 30',
    'categories' : {
        'inclusive'               : '1',
        'opt1'                    : 'PuppiMET_pt>20 && detajj<3 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && abs(Lepton_eta[0])<1.5',
        # FIXME: Jet_btagDeepB and Jet_btagCSVV2 not available in Run 3 nanoAOD
        # 'opt2'                    : 'PuppiMET_pt>20 && detajj<3 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && abs(Lepton_eta[0])<1.5 && (CleanJet_jetIdx.size() > 0 && Jet_btagCSVV2.size() > CleanJet_jetIdx[0] ? Jet_btagCSVV2[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_btagCSVV2.size() > CleanJet_jetIdx[1] ? Jet_btagCSVV2[CleanJet_jetIdx[1]] : 2) < 0.5 && (CleanJet_jetIdx.size() > 0 && Jet_btagDeepB.size() > CleanJet_jetIdx[0] ? Jet_btagDeepB[CleanJet_jetIdx[0]] : 2)<0.2 && (CleanJet_jetIdx.size() > 1 && Jet_btagDeepB.size() > CleanJet_jetIdx[1] ? Jet_btagDeepB[CleanJet_jetIdx[1]] : 2)<0.2',
        # FIXME: Jet_btagDeepB and Jet_btagCSVV2 not available in Run 3 nanoAOD
        # 'opt3'                    : 'PuppiMET_pt>20 && detajj<2 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && abs(Lepton_eta[0])<1.5 && (CleanJet_jetIdx.size() > 0 && Jet_btagCSVV2.size() > CleanJet_jetIdx[0] ? Jet_btagCSVV2[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_btagCSVV2.size() > CleanJet_jetIdx[1] ? Jet_btagCSVV2[CleanJet_jetIdx[1]] : 2) < 0.5 && (CleanJet_jetIdx.size() > 0 && Jet_btagDeepB.size() > CleanJet_jetIdx[0] ? Jet_btagDeepB[CleanJet_jetIdx[0]] : 2)<0.2 && (CleanJet_jetIdx.size() > 1 && Jet_btagDeepB.size() > CleanJet_jetIdx[1] ? Jet_btagDeepB[CleanJet_jetIdx[1]] : 2)<0.2 && mjj<180 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) > 50 && abs((CleanJet_eta.size() > 0 ? CleanJet_eta[0] : 0))<2.0 && abs((CleanJet_eta.size() > 1 ? CleanJet_eta[1] : 0))<2.0',

    }
}

