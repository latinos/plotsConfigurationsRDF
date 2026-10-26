


# cuts['Sig_mllZ'] = {
    # }


cuts['Sig'] = {
    'expr' : '(Lepton_pt.size() > 2 ? Lepton_pt[2] : 0) < 15 && (abs(Lepton_pdgId[0])==abs(Lepton_pdgId[1])) && (CleanJet_pt.size() > 2 ? CleanJet_pt[2] : 0) < 30 && (CleanJet_pt.size() > 1 ? CleanJet_pt[1] : 0) > 30',
    'categories' : {
        'mllZ'               : 'mll>60 && mll< 120',
        # 'two'                : 'mll>60 && mll< 120',
      }
    }



# cuts['single'] = 'mll>60 && mll< 120'
