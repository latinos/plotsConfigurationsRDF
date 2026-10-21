#
# Samples file
#

# import os

#
# How to get the list of files to be analysed
#

mcProduction = 'Summer20UL17_106x_nAODv9_Full2017v9'
dataReco     = 'Run2017_UL2017_nAODv9_Full2017v9'
mcSteps      = 'MCl1loose2017v9__MCCorr2017v9NoJERInHorn__l2tightOR2017v9'
fakeSteps    = 'DATAl1loose2017v9__l2loose__fakeW'
dataSteps    = 'DATAl1loose2017v9__l2loose__l2tightOR2017v9'

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

mcCommonWeight = 'XSWeight * SFweight2l * LepWPCut * LepWPSF * Jet_PUIDSF * btagSF * METFilter_MC * PromptGenLepMatch2l'






#
# The actual list of samples
#

############ Top ############


files = nanoGetSampleFiles(mcDirectory, 'TTTo2L2Nu') | \
        nanoGetSampleFiles(mcDirectory, 'ST_s-channel') | \
        nanoGetSampleFiles(mcDirectory, 'ST_t-channel_top') | \
        nanoGetSampleFiles(mcDirectory, 'ST_t-channel_antitop') | \
        nanoGetSampleFiles(mcDirectory, 'ST_tW_antitop') | \
        nanoGetSampleFiles(mcDirectory, 'ST_tW_top')

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





#
# Data
#


DataRun = [
    ['B','Run2017B-UL2017-v1'],
    ['C','Run2017C-UL2017-v1'],
    ['D','Run2017D-UL2017-v1'],
    ['E','Run2017E-UL2017-v1'],
    ['F','Run2017F-UL2017-v1'],
]

DataSets = [
  'MuonEG',
  'SingleMuon',
  'SingleElectron',
  'DoubleMuon',
  'DoubleEG'
  ]


DataTrig = {
    'MuonEG'         : ' Trigger_ElMu' ,
    'SingleMuon'     : '!Trigger_ElMu && Trigger_sngMu' ,
    'SingleElectron' : '!Trigger_ElMu && !Trigger_sngMu && Trigger_sngEl',
    'DoubleMuon'     : '!Trigger_ElMu && !Trigger_sngMu && !Trigger_sngEl && Trigger_dblMu',
    'DoubleEG'       : '!Trigger_ElMu && !Trigger_sngMu && !Trigger_sngEl && !Trigger_dblMu && Trigger_dblEl'
}


samples['DATA'] = {
  'name': {},
  'weight': 'LepWPCut*METFilter_DATA',
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

signalDirectory = '/eos/user/a/amassiro/HIG/ZHggPostProc/Summer20UL17_106x_nAODv9_Full2017v9/MCFull2017v9/'

# the string "ZHgg" here is very important, since the root files that will be selected must have the format "nanoLatino_ZHgg__part*.root"
files = nanoGetLocalSampleFiles(signalDirectory, 'ZHgg')

samples['Hgluglu'] = {
    'name': files,
    # 'weight': mcCommonWeight,  --> missing post processing
    'weight': 'baseW*genWeight*0.8839*0.08187*0.033658*3',
    'FilesPerJob': 1,
}

# FIXME: qqZH and ggZH split samples not yet available for 2017

signalsNoPostProc = ['Hgluglu']


#
# Useful later on, like aliases.py, nuisances.py, ...
#

mcALL     = [skey for skey in samples if skey not in ['DATA', 'Fake_lep'] + signalsNoPostProc]
ALL       = [skey for skey in samples]





