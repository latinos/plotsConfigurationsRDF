# cuts

cuts = {}

supercut = 'mll>12  \
              && Lepton_pt[0]>25 \
              && Lepton_pt[1]>10 \
              && (abs(Lepton_pdgId[1])==13 || Lepton_pt[1]>13) \
              && abs(Lepton_eta[0])<2.5 && abs(Lepton_eta[1])<2.5 \
              && bVeto \
              '

              #&& bVeto \
              #&& PuppiMET_pt > 30 \
              #&& !hole_veto \


## Same-sign control region in the 0 jet bin: used in the WH3l category. Considering different flavor to avoid DY
#cuts['wh3l_13TeV_OS_CR'] = {
    #'expr' : '(Lepton_pt.size() > 2 ? Lepton_pt[2] : 0) < 15 && abs(Lepton_pdgId[0]*Lepton_pdgId[1]) == 11*13 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) < 30',
    #'categories' : {
        #'pt2ge20'  : 'Lepton_pdgId[0]*Lepton_pdgId[1]<0',
    #}
#}



cuts['DY'] = {
    'expr' : '(Lepton_pt.size() > 2 ? Lepton_pt[2] : 0) < 15 && (abs(Lepton_pdgId[0])==abs(Lepton_pdgId[1]))',
    'categories' : {
        '0j'  : '(CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) < 30',
        '1j'  : '(CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) < 30 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) > 30',
        '2j'  : '(CleanJet_pt.size() > 2 ? CleanJet_pt[2] : 0) < 30 && (CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) > 30',
    }
}
    


cuts['Sig'] = {
    'expr' : '(Lepton_pt.size() > 2 ? Lepton_pt[2] : 0) < 15 && (abs(Lepton_pdgId[0])==abs(Lepton_pdgId[1])) && (CleanJet_pt.size() > 2 ? CleanJet_pt[2] : 0) < 30 && (CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) > 30',
    'categories' : {
        'mllZ'               : 'mll>60 && mll< 120',
        'mllZpt1'            : 'mll>80 && mll< 100 && Lepton_pt[0]>40',
        'mllZpt1qgl'         : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5',
        'mllZpt1qglmet'      : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && PuppiMET_pt<60',
        'mllZpt1qglmetbVeto' : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && PuppiMET_pt<60 && bVeto',
        'opt1'               : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && PuppiMET_pt<60 && bVeto && detajj<3',
        'opt2'               : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && PuppiMET_pt<60 && bVeto && detajj<3 && mjj<160 && mjj>60 && dphilljetjet>1',
        'opt3'               : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && PuppiMET_pt<60 && bVeto && detajj<3 && mjj<160 && mjj>60 && dphilljetjet>1 && (CleanJet_jetIdx.size() > 0 && Jet_btagDeepB.size() > CleanJet_jetIdx[0] ? Jet_btagDeepB[CleanJet_jetIdx[0]] : 2)<0.2 && (CleanJet_jetIdx.size() > 1 && Jet_btagDeepB.size() > CleanJet_jetIdx[1] ? Jet_btagDeepB[CleanJet_jetIdx[1]] : 2) < 0.2 && (CleanJet_jetIdx.size() > 0 && Jet_btagCSVV2.size() > CleanJet_jetIdx[0] ? Jet_btagCSVV2[CleanJet_jetIdx[0]] : 2)<0.7 && (CleanJet_jetIdx.size() > 1 && Jet_btagCSVV2.size() > CleanJet_jetIdx[1] ? Jet_btagCSVV2[CleanJet_jetIdx[1]] : 2)<0.7',
        'opt4'               : 'mll>80 && mll< 100 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.5 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.5 && PuppiMET_pt<60 && bVeto && detajj<2 && mjj<150 && mjj>60 && dphilljetjet>1 && (CleanJet_jetIdx.size() > 0 && Jet_btagDeepB.size() > CleanJet_jetIdx[0] ? Jet_btagDeepB[CleanJet_jetIdx[0]] : 2)<0.2 && (CleanJet_jetIdx.size() > 1 && Jet_btagDeepB.size() > CleanJet_jetIdx[1] ? Jet_btagDeepB[CleanJet_jetIdx[1]] : 2) < 0.2 && (CleanJet_jetIdx.size() > 0 && Jet_btagCSVV2.size() > CleanJet_jetIdx[0] ? Jet_btagCSVV2[CleanJet_jetIdx[0]] : 2)<0.7 && (CleanJet_jetIdx.size() > 1 && Jet_btagCSVV2.size() > CleanJet_jetIdx[1] ? Jet_btagCSVV2[CleanJet_jetIdx[1]] : 2)<0.7 && ptll>20 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) > 50',
        'opt5future'         : 'mll>85 && mll< 95 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.1 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.1 && PuppiMET_pt<60 && bVeto && detajj<2 && mjj<150 && mjj>60 && dphilljetjet>1 && (CleanJet_jetIdx.size() > 0 && Jet_btagDeepB.size() > CleanJet_jetIdx[0] ? Jet_btagDeepB[CleanJet_jetIdx[0]] : 2)<0.2 && (CleanJet_jetIdx.size() > 1 && Jet_btagDeepB.size() > CleanJet_jetIdx[1] ? Jet_btagDeepB[CleanJet_jetIdx[1]] : 2) < 0.2 && (CleanJet_jetIdx.size() > 0 && Jet_btagCSVV2.size() > CleanJet_jetIdx[0] ? Jet_btagCSVV2[CleanJet_jetIdx[0]] : 2)<0.7 && (CleanJet_jetIdx.size() > 1 && Jet_btagCSVV2.size() > CleanJet_jetIdx[1] ? Jet_btagCSVV2[CleanJet_jetIdx[1]] : 2)<0.7 && ptll>20 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) > 50 && (CleanJet_eta.size() > 1 ? CleanJet_eta[1] : 0) < 2.5 && (CleanJet_eta.size() > 1 ? CleanJet_eta[1] : 0) > -2.5 && (CleanJet_eta.size() > 0 ? CleanJet_eta[0] : 0) < 2.5 && (CleanJet_eta.size() > 0 ? CleanJet_eta[0] : 0) > -2.5 && (Lepton_pt.size() > 0 ? Lepton_pt[0] : 0)>50',
        'opt6future'         : 'mll>85 && mll< 95 && Lepton_pt[0]>40 && (CleanJet_jetIdx.size() > 0 && Jet_qgl.size() > CleanJet_jetIdx[0] ? Jet_qgl[CleanJet_jetIdx[0]] : 2)<0.04 && (CleanJet_jetIdx.size() > 1 && Jet_qgl.size() > CleanJet_jetIdx[1] ? Jet_qgl[CleanJet_jetIdx[1]] : 2)<0.04 && PuppiMET_pt<60 && bVeto && detajj<2 && mjj<150 && mjj>60 && dphilljetjet>1 && (CleanJet_jetIdx.size() > 0 && Jet_btagDeepB.size() > CleanJet_jetIdx[0] ? Jet_btagDeepB[CleanJet_jetIdx[0]] : 2)<0.2 && (CleanJet_jetIdx.size() > 1 && Jet_btagDeepB.size() > CleanJet_jetIdx[1] ? Jet_btagDeepB[CleanJet_jetIdx[1]] : 2) < 0.2 && (CleanJet_jetIdx.size() > 0 && Jet_btagCSVV2.size() > CleanJet_jetIdx[0] ? Jet_btagCSVV2[CleanJet_jetIdx[0]] : 2)<0.7 && (CleanJet_jetIdx.size() > 1 && Jet_btagCSVV2.size() > CleanJet_jetIdx[1] ? Jet_btagCSVV2[CleanJet_jetIdx[1]] : 2)<0.7 && ptll>20 && (CleanJet_pt.size() > 0 ? CleanJet_pt[0] : 0) > 75 && (CleanJet_eta.size() > 1 ? CleanJet_eta[1] : 0) < 2.0 && (CleanJet_eta.size() > 1 ? CleanJet_eta[1] : 0) > -2.0 && (CleanJet_eta.size() > 0 ? CleanJet_eta[0] : 0) < 2.0 && (CleanJet_eta.size() > 0 ? CleanJet_eta[0] : 0) > -2.0 && (Lepton_pt.size() > 0 ? Lepton_pt[0] : 0)>50 && ptll>100 && Lepton_pt[1]>20',
        
    }
}
    
