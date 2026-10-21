#
# Samples file
#

samples['top'] = {
}


samples['DY'] = {
}


# Signal: ZH, H>gluglu
# NB: not available for 2016 yet
samples['Hgluglu'] = {
}

signalsNoPostProc = ['Hgluglu']


samples['DATA'] = {
}


#
# Useful later on, like aliases.py, nuisances.py, ...
#

mcALL     = [skey for skey in samples if skey not in ['DATA', 'Fake_lep'] + signalsNoPostProc]
ALL       = [skey for skey in samples]





