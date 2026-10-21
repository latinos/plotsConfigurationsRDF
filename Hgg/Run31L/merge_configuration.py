
# tag: used to define root file name and more
tag = 'Hgg'
tagger = '261021_Run3_1L'
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
                    "folder" : "../20221L",
                    "tag"    : "261021_22_1L",
                  },

                  "2022EE" : {
                    "folder" : "../2022EE1L",
                    "tag"    : "261021_22EE_1L",
                  },

                  "2023" : {
                    "folder" : "../20231L",
                    "tag"    : "261021_23_1L",
                  },

                  "2023BPix" : {
                    "folder" : "../2023BPix1L",
                    "tag"    : "261021_23Bpix_1L",
                  },

                  "2024" : {
                    "folder" : "../20241L",
                    "tag"    : "261021_24_1L",
                   },

                  "2025" : {
                    'folder' : '../20251L',
                    'tag'    : '261021_25_1L',
                  },

                  "2026" : {
                    'folder' : '../20261L',
                    'tag'    : '261021_26_1L',
                  }
}



