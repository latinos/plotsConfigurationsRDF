#
# Samples file
#

# import os

#
# How to get the list of files to be analysed
#

mcProduction = 'Summer20UL16_106x_nAODv9_noHIPM_Full2016v9'
dataReco     = 'Run2016_UL2016_nAODv9_noHIPM_Full2016v9'
mcSteps      = 'MCl1loose2016v9__MCCorr2016v9NoJERInHorn'
fakeSteps    = 'DATAl1loose2016v9'
dataSteps    = 'DATAl1loose2016v9'

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
  # return  [(name, _files)]


#
# The weights
#

#
# 1 lepton weights
#
# see https://github.com/latinos/PlotsConfigurationsRun3/blob/amassiro-Hgg/Hgg/2018UL1l/samples.py
#

XSWeight = 'baseW*genWeight'

# FIXME: single lepton triggers for 2016 to be checked
trigg_MC = '( (abs(Lepton_pdgId[0])==13) * (HLT_IsoMu24 > 0.5 || HLT_IsoTkMu24 > 0.5) || (abs(Lepton_pdgId[0])==11) * (HLT_Ele27_WPTight_Gsf > 0.5) )'

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
    # 'weight': mcCommonWeight + " * Top_pTrw ",
    'FilesPerJob': 3,
}


############ DY ############

files = nanoGetSampleFiles(mcDirectory, 'DYJetsToLL_M-10to50_NLO') | \
        nanoGetSampleFiles(mcDirectory, 'DYJetsToLL_M-50')

#print (" list of files DY = ", files)

samples['DY'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 5,
}






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
# FIXME: HT stiching corrections for 2016 (2018 ones in ../20181L/samples.py), and check sample names


############ VV ############

# FIXME: check sample names for 2016
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
    ['F','Run2016F-UL2016-v1'],
    ['G','Run2016G_UL2016-v1'],
    ['H','Run2016H_UL2016-v1']
]

DataSets = [
  'SingleMuon',
  'SingleElectron'
  ]


DataTrig = {
    'SingleMuon'     : 'Trigger_sngMu' ,
    'SingleElectron' : '!Trigger_sngMu && Trigger_sngEl' ,
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

    # get the files
    files = nanoGetSampleFiles(dataDirectory, datatag)
    samples['DATA']['name'].update(files)

    # add the weight that is different pd by pd, to take into account orthogonality of triggers
    samples['DATA']['weights'].update( {datatag : DataTrig[pd] })






#
# Useful later on, like aliases.py, nuisances.py, ...
#

mcALL     = [skey for skey in samples if skey not in ['DATA', 'Fake_lep']]
ALL       = [skey for skey in samples]





