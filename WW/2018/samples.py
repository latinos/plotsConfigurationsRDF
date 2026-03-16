#
# Samples file
#

#
# How to get the list of files to be analysed
#

mcProduction = 'Summer20UL18_106x_nAODv9_Full2018v9'
dataReco = 'Run2018_UL2018_nAODv9_Full2018v9'
mcSteps = 'MCl1loose2018v9__MCCorr2018v9NoJERInHorn__l2tightOR2018v9'
fakeSteps = 'DATAl1loose2018v9__l2loose__fakeW'
dataSteps = 'DATAl1loose2018v9__l2loose__l2tightOR2018v9'

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
  # return  [(name, _files)]




# def addSampleWeight(samples, sampleName, sampleNameType, weight):
#     obj = list(filter(lambda k: k[0] == sampleNameType, samples[sampleName]['name']))[0]
#     samples[sampleName]['name'] = list(filter(lambda k: k[0] != sampleNameType, samples[sampleName]['name']))
#     if len(obj) > 2:
#         samples[sampleName]['name'].append((obj[0], obj[1], obj[2] + '*(' + weight + ')'))
#     else:
#         samples[sampleName]['name'].append((obj[0], obj[1], '(' + weight + ')' ))





#
# The weights
#



mcCommonWeight = '3.14'


#
# The actual list of samples
#

files = nanoGetSampleFiles(mcDirectory, 'WpWpJJ_EWK_UL')
samples['SSWW'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 4
}

files = nanoGetSampleFiles(mcDirectory, 'WpWpJJ_QCD_UL')
samples['WpWp_QCD'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 4
}

files = nanoGetSampleFiles(mcDirectory, 'WZTo3LNu')
samples['WZ_QCD'] = {
    'name': files,
    'weight': mcCommonWeight,
    'FilesPerJob': 4
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

DataSets = ['MuonEG','SingleMuon','EGamma','DoubleMuon']

DataTrig = {
    'MuonEG'         : ' Trigger_ElMu' ,
    'DoubleMuon'     : '!Trigger_ElMu &&  Trigger_dblMu' ,
    'SingleMuon'     : '!Trigger_ElMu && !Trigger_dblMu &&  Trigger_sngMu' ,
    'EGamma'         : '!Trigger_ElMu && !Trigger_dblMu && !Trigger_sngMu && (Trigger_sngEl || Trigger_dblEl)' ,
}

samples['DATA'] = {
  'name': {},
  'weight': 'LepWPCut*METFilter_DATA',
  'weights': {},
  'isData': ['all'],
  'FilesPerJob': 50
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
    # samples['DATA']['name'].extend(files)

    # add the weight that is different pd by pd, to take into account orthogonality of triggers
    samples['DATA']['weights'].update( {datatag : DataTrig[pd] })
    # samples['DATA']['weights'].append( [datatag , DataTrig[pd] ])
    # addSampleWeight(samples, 'DATA', datatag, DataTrig[pd])





