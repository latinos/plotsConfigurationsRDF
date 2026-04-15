
#
# Alt(Lepton_pdgId,0,-9999))==11 --> ele
# Alt(Lepton_pdgId,0,-9999))==13 --> mu
#

cuts['lowmll']  = 'mll>40 && mll<120'
cuts['highmll'] = 'mll>120'



#
# cuts['DY_cr'] = {
#    'expr': 'mll>70 && mll<110',
#     # Define the sub-categorization of sr
#    'categories' : {
#       'eleele' : 'ee',   # "ee" is defined in aliases.py
#       'mumu'   : 'mm',   # "mm" is defined in aliases.py
#    }
# }



