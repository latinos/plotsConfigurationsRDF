#
# Samples file
#

# import os

#
# How to get the list of files to be analysed
#

mcProduction = 'Summer20UL18_106x_nAODv9_Full2018v9'
dataReco = 'Run2018_UL2018_nAODv9_Full2018v9'
mcSteps = 'MCl1loose2018v9__MCCorr2018v9NoJERInHorn'
fakeSteps = 'DATAl1loose2018v9'
dataSteps = 'DATAl1loose2018v9'

treeBaseDir = '/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano'


def makeMCDirectory(var=""):
    _treeBaseDir = treeBaseDir + ""
    if var == "":
        return "/".join([_treeBaseDir, mcProduction, mcSteps])
    else:
        return "/".join([_treeBaseDir, mcProduction, mcSteps + "__" + var])

mcDirectory = makeMCDirectory()
fakeDirectory = os.path.join(treeBaseDir, dataReco, fakeSteps)
dataDirectory = os.path.join(treeBaseDir, dataReco, dataSteps)


from search_files import SearchFiles
searchFiles = SearchFiles()

useXROOTD = False
redirector = 'root://eoscms.cern.ch/'

def nanoGetSampleFiles(path, name):
  _files = searchFiles.searchFiles(path, name, redirector=redirector)
  return  {name : _files}

# local files (e.g. /eos/user/...), no redirector
def nanoGetLocalSampleFiles(path, name):
  _files = searchFiles.searchFiles(path, name, redirector='')
  return  {name : _files}


def addSubSampleWeights(samples, sampleName, subSampleName, weight):
  if 'weights' not in samples[sampleName].keys():
    samples[sampleName]['weights'] = {}
  if subSampleName in samples[sampleName]['weights'].keys():
    samples[sampleName]['weights'][subSampleName] = "(" + samples[sampleName]['weights'][subSampleName] + ") * " +  weight
  else :
    samples[sampleName]['weights'][subSampleName] = weight



#
# The weights
#

#
# 1 lepton weights
#
# see https://github.com/latinos/PlotsConfigurationsRun3/blob/amassiro-Hgg/Hgg/2018UL1l/samples.py
#

XSWeight = 'baseW*genWeight'

trigg_MC = '( (abs(Lepton_pdgId[0])==13) * (HLT_IsoMu24 > 0.5) || (abs(Lepton_pdgId[0])==11) * (HLT_Ele32_WPTight_Gsf > 0.5) )'

#
# TriggerSFWeight_1l --> check the values but ok for now
# see https://github.com/latinos/mkShapesRDF/blob/master/mkShapesRDF/processor/data/TrigMaker_cfg.py
#
# Jet_PUIDSF defined in aliases.py
# btagSF     defined in aliases.py
#
SFweight1l = ['puWeight', 'Lepton_RecoSF[0]', 'LepWPWeight_1l', 'LepWPCut_1l', 'btagSF', 'Jet_PUIDSF', 'TriggerSFWeight_1l', trigg_MC]

SFweight = ' * '.join(SFweight1l)

#
# METFilter_MC not filled??
#

mcCommonWeightNoMatch = XSWeight + '*' + SFweight
mcCommonWeight        = XSWeight + '*' + SFweight + '*Lepton_genmatched[0]'

# mcCommonWeight = 'XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l'




#
# The actual list of samples
#



############ DY ############

files = nanoGetSampleFiles(mcDirectory, 'DYJetsToLL_M-10to50_NLO') | \
        nanoGetSampleFiles(mcDirectory, 'DYJetsToLL_M-50')

#print (" list of files DY = ", files)

samples['DY'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 3,
    'subsamples': {
      #            definition          specific weight
      'Low' : { 'cut' : 'mll<70',    'weight' :  '1.00'},
      'High': { 'cut' : 'mll>70',    'weight' :  '1.00'}
    }
}


############ Top ############


files = nanoGetSampleFiles(mcDirectory, 'TTTo2L2Nu') | \
        nanoGetSampleFiles(mcDirectory, 'ST_s-channel') | \
        nanoGetSampleFiles(mcDirectory, 'ST_t-channel_top') | \
        nanoGetSampleFiles(mcDirectory, 'ST_t-channel_antitop') | \
        nanoGetSampleFiles(mcDirectory, 'ST_tW_antitop') | \
        nanoGetSampleFiles(mcDirectory, 'ST_tW_top') | \
        nanoGetSampleFiles(mcDirectory, 'TTToSemiLeptonic')

samples['top'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 3,
}

addSubSampleWeights (samples, 'top', 'TTTo2L2Nu', 'Top_pTrw')
addSubSampleWeights (samples, 'top', 'TTToSemiLeptonic', 'Top_pTrw')





############ Wjets ############

files = nanoGetSampleFiles(mcDirectory, 'WJetsToLNu-LO')            | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT70To100')     | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT100To200')    | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT200To400')    | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT400To600')    | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT600To800')    | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT800To1200')   | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT1200To2500')  | \
        nanoGetSampleFiles(mcDirectory, 'WJetsToLNu_HT2500ToInf')

samples['Wjets'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 4,
}

# Fix Wjets binned + LO
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu-LO', '(LHE_HT < 70)')
# HT stiching corrections 2018
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT70To100',    '1.21 * 0.95148')  # adding also k-factor
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT100To200',   '0.9471')
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT200To400',   '0.9515')
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT400To600',   '0.9581')
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT600To800',   '1.0582')
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT800To1200',  '1.1285')
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT1200To2500', '1.3268')
addSubSampleWeights (samples, 'Wjets', 'WJetsToLNu_HT2500ToInf',  '2.7948')


############ VV ############

files = nanoGetSampleFiles(mcDirectory, 'WmToLNu_WmTo2J_QCD')  | \
        nanoGetSampleFiles(mcDirectory, 'WmToLNu_ZTo2J_QCD')   | \
        nanoGetSampleFiles(mcDirectory, 'WmTo2J_ZTo2L_QCD')    | \
        nanoGetSampleFiles(mcDirectory, 'WpTo2J_WmToLNu_QCD')  | \
        nanoGetSampleFiles(mcDirectory, 'WpTo2J_ZTo2L_QCD')    | \
        nanoGetSampleFiles(mcDirectory, 'WpToLNu_WpTo2J_QCD')  | \
        nanoGetSampleFiles(mcDirectory, 'WpToLNu_WmTo2J_QCD')  | \
        nanoGetSampleFiles(mcDirectory, 'WpToLNu_ZTo2J_QCD')   | \
        nanoGetSampleFiles(mcDirectory, 'ZTo2L_ZTo2J_QCD')

samples['VV'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 4,
}



#
# Data
#

DataRun = [
    ['A','Run2018A-UL2018-v1'],
    ['B','Run2018B-UL2018-v1'],
    ['C','Run2018C-UL2018-v1'],
    ['D','Run2018D-UL2018-v1'],
]

DataSets = [
  'SingleMuon',
  'EGamma'
  ]

DataTrig = {
    'SingleMuon'     : 'Trigger_sngMu' ,
    'EGamma'         : '!Trigger_sngMu && Trigger_sngEl' ,
}

samples['DATA'] = {
  'name': {},
  'weight': 'LepWPCut_1l*METFilter_DATA',
  'weights': {},
  'isData': ['all'],
  'FilesPerJob': 10
}

for era, era_name in DataRun:
  for pd in DataSets:
    datatag = pd + '_' + era_name

    # fix 2018 peculiarity in versioning
    if (   ('DoubleMuon' in pd and 'Run2018B' in era_name)
        or ('DoubleMuon' in pd and 'Run2018D' in era_name)
        or ('SingleMuon' in pd and 'Run2018A' in era_name)
        or ('SingleMuon' in pd and 'Run2018B' in era_name)
        or ('SingleMuon' in pd and 'Run2018C' in era_name)):
        print("era_name = {}".format(era_name))
        print("pd       = {}".format(pd))
        print("Old datatag = {}".format(datatag))
        datatag = datatag.replace('v1','v2')
        print("New datatag = {}".format(datatag))

    # get the files
    files = nanoGetSampleFiles(dataDirectory, datatag)
    samples['DATA']['name'].update(files)

    # add the weight that is different pd by pd, to take into account orthogonality of triggers
    samples['DATA']['weights'].update( {datatag : DataTrig[pd] })




#
# Signal: ZH, H>gluglu
# from https://github.com/latinos/PlotsConfigurationsRun3/tree/amassiro-Hgg/Hgg
#
# during post-processing, baseW, xsec = 1
# xs_db["ZHgg"] = ["xsec=1.000", "kfact=1.000", "ref=X"]
# Units in pb
# From https://twiki.cern.ch/twiki/pub/LHCPhysics/HiggsXSBR/Higgs_XSBR_YR4_update.xlsx
#   ZH:   8.839E-01 pb
#   qqZH: 7.612E-01 pb
#   ggZH: 1.227E-01 pb
# and the branching ratio:
# https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR
#   8.187E-02
# BR Z>ll:  3.3658% x 3
# From https://pdg.lbl.gov/2018/listings/rpp2018-list-z-boson.pdf
#
# ZH_HToGluGlu_ZToLL_13TeV_powheg_pythia8
#
# NB: signal samples are missing the full post-processing (no LepCut2l, LepSF2l, ...)
#     --> mcCommonWeight not applied, and signal excluded from mcALL
#

signalDirectory = '/eos/user/a/amassiro/HIG/ZHggPostProc/Summer20UL18_106x_nAODv9_Full2018v9/MCFull2018v9/'

# the string "ZHgg" here is very important, since the root files that will be selected must have the format "nanoLatino_ZHgg__part*.root"
# files = nanoGetLocalSampleFiles(signalDirectory, 'ZHgg')
#
# samples['Hgluglu'] = {
#     'name': files,
#     # 'weight': mcCommonWeight,  --> missing post processing
#     'weight': 'baseW*genWeight*0.8839*0.08187*0.033658*3',
#     'FilesPerJob': 1,
# }


files = nanoGetLocalSampleFiles(signalDirectory, 'ZHllHgg')

samples['qqZHgluglu'] = {
    'name': files,
    # 'weight': mcCommonWeight,  --> missing post processing
    'weight': 'baseW*genWeight*0.7612*0.08187*0.033658*3',
    'FilesPerJob': 200,
}


files = nanoGetLocalSampleFiles(signalDirectory, 'ggZHllHgg')

samples['ggZHgluglu'] = {
    'name': files,
    # 'weight': mcCommonWeight,  --> missing post processing
    'weight': 'baseW*genWeight*0.1227*0.08187*0.033658*3',
    'FilesPerJob': 200,
}


#
# WH, W>lv
#   W+ : 8.400E-01  pb
#   W- : 5.328E-01  pb
# BR W>lv: 10.86% x 3
# https://pdg.lbl.gov/2018/listings/rpp2018-list-w-boson.pdf
#

files = nanoGetLocalSampleFiles(signalDirectory, 'WminuslvHgg') | \
        nanoGetLocalSampleFiles(signalDirectory, 'WpluslvHgg')

samples['WHgluglu'] = {
    'name': files,
    # 'weight': mcCommonWeight,  --> missing post processing
    'weight': 'baseW*genWeight*1.0*0.08187*0.1086*3',
    'FilesPerJob': 200,
}

addSubSampleWeights (samples, 'WHgluglu', 'WminuslvHgg', '0.5328') # xsec pb (above I put 1.0)
addSubSampleWeights (samples, 'WHgluglu', 'WpluslvHgg',  '0.8400') # xsec pb (above I put 1.0)

signalsNoPostProc = ['qqZHgluglu', 'ggZHgluglu', 'WHgluglu']


#
# Useful later on, like aliases.py, nuisances.py, ...
#

mcALL     = [skey for skey in samples if skey not in ['DATA', 'Fake_lep'] + signalsNoPostProc]
ALL       = [skey for skey in samples]





