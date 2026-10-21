#
# Used by mkDatacards.py
#

structure['DY']  = {
    'isSignal' : 0,
    'isData'   : 0,
}

structure['top'] = {
    'isSignal' : 0,
    'isData'   : 0,
}

# Signal: ZH, H>gluglu
structure['Hgluglu'] = {
    'isSignal' : 1,
    'isData'   : 0,
    #'scaleSampleForDatacard' : 10 # scaling signal to have sensitivity
}

# qqZH + ggZH is an alternative to the inclusive Hgluglu: do not use them together (double counting)
structure['qqZHgluglu'] = {
    'isSignal' : 2,
    'isData'   : 0,
}

structure['ggZHgluglu'] = {
    'isSignal' : 2,
    'isData'   : 0,
}

# Data
structure['DATA']  = {
    'isSignal' : 0,
    'isData'   : 1
}


