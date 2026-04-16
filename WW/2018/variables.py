

#'fold' : # 0 = not fold (default), 1 = fold underflowbin, 2 = fold overflow bin, 3 = fold underflow and overflow


variables['events']  = {   'name': '1',
                           'range' : (1,0,2),
                           'xaxis' : 'events',
                           'fold' : 3
}

variables['ptj1']  = {
                           'name': 'CleanJet_pt.size() > 0 ? CleanJet_pt[0] : -9999.',
                           # 'name': 'Alt(CleanJet_pt,0,-9999.)',
                           'range' : (15,0.,200),
                           'xaxis' : 'p_{T} 1st jet',
                           'fold'  : 3
                           }



variables['ptj2']  = {
                           'name': 'CleanJet_pt.size() > 1 ? CleanJet_pt[1] : -9999.',
                           # 'name': 'Alt(CleanJet_pt,1,-9999.)',
                           'range' : (15,0.,150),
                           'xaxis' : 'p_{T} 2nd jet',
                           'fold'  : 3
                           }
variables['ptl1']  = {
                           'name': 'Lepton_pt.size() > 0 ? Lepton_pt[0] : -9999.',
                           # 'name': 'Alt(Lepton_pt,0,-9999.)',
                           'range' : (15,0.,200),
                           'xaxis' : 'p_{T} 1st jet',
                           'fold'  : 3
                           }

variables['mll']  = {   'name': 'mll',
                        'range' : (200,10,500),
                        'xaxis' : 'mll [GeV]',
                        'fold' : 3,
                        }

variables['njet']  = {
                       'name': 'Sum(CleanJet_pt>30)',
                       'range' : (8,0,8),
                       'xaxis' : 'Number of jets',
                       'fold' : 2   # 0 = not fold (default), 1 = fold underflowbin, 2 = fold overflow bin, 3 = fold underflow and overflow
                       }

#
# variables['ptll']  = {   'name': 'ptll',
#                         'range' : (200,0,500),
#                         'xaxis' : 'pTll [GeV]',
#                         'fold' : 3,
#                         }
#
#

