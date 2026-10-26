
# tag: used to define root file name and more
tag = 'DMHMET'
tagger = '261021_Run3_2L'
tag = tag + "_" + tagger

# File with dict of variables
variablesFile = "variables.py"

# File with dict of cuts
cutsFile = "cuts.py"

# file with list of samples
samplesFile = "samples.py"


# structure file for datacard
structureFile = "structure.py"

# nuisances file for mkDatacards and for mkShape
nuisancesFile = "nuisances.py"

outputDir = "./rootFile/"+tag

plotDir   = "./plots/"+tag


# Folders to merge
foldersToMerge = {

                  "2022" : {
                    "folder" : "../20222L",
                    "tag"    : "261021_22_2L",
                  },

                  "2022EE" : {
                    "folder" : "../2022EE2L",
                    "tag"    : "261021_22EE_2L",
                  },

                  "2023" : {
                    "folder" : "../20232L",
                    "tag"    : "261021_23_2L",
                  },

                  "2023BPix" : {
                    "folder" : "../2023BPix2L",
                    "tag"    : "261021_23Bpix_2L",
                  },

                  "2024" : {
                    "folder" : "../20242L",
                    "tag"    : "261021_24_2L",
                   },

                  "2025" : {
                    'folder' : '../20252L',
                    'tag'    : '261021_25_2L',
                  },

                  "2026" : {
                    'folder' : '../20262L',
                    'tag'    : '261021_26_2L',
                  }
}



