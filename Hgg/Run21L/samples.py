#
# Samples file
#

samples['top'] = {
}


samples['DY'] = {
}


samples['Wjets'] = {
}


samples['VV'] = {
}


# Signal: VH, H>gluglu
# NB: not available for 2016 yet, ZH inclusive (Hgluglu) for 2017, qqZH + ggZH + WH for 2018
samples['Hgluglu'] = {
}

samples['qqZHgluglu'] = {
}

samples['ggZHgluglu'] = {
}

samples['WHgluglu'] = {
}

signalsNoPostProc = ['Hgluglu', 'qqZHgluglu', 'ggZHgluglu', 'WHgluglu']


samples['DATA'] = {
}


#
# Useful later on, like aliases.py, nuisances.py, ...
#

mcALL     = [skey for skey in samples if skey not in ['DATA', 'Fake_lep'] + signalsNoPostProc]
ALL       = [skey for skey in samples]





