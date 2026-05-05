

variables['mll_mybins']  = {   'name': 'mll',
                        'range' : ([12, 17, 25, 30, 35, 40, 45, 65, 200]),
                        'xaxis' : 'mll [GeV]',
                        'fold' : 3,
                        }

#
# NB: both x and y variables must be defined somewhere or already in the variables in the ttree
#
variables['mllVSmth'] = {
                        'name'  : 'mll:mth',
                        'range' : ([12, 17, 25, 30, 35, 40, 45, 65, 200],[60, 95, 110, 135, 200],),
                        'xaxis' : 'm_{ll} : m_{T}^{H}',
                        'fold'  : 3,
                        'is2d'  : 1
}


variables['njet']  = {
                       'name': 'Sum(CleanJet_pt>30)',
                       'range' : (8,0,8),
                       'xaxis' : 'Number of jets',
                       'fold' : 2   # 0 = not fold (default), 1 = fold underflowbin, 2 = fold overflow bin, 3 = fold underflow and overflow
                       }


