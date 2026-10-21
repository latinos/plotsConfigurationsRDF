
# tag: used to define root file name and more
tag = 'Hgg2L_full_Run2'

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




# Folders to merge
foldersToMerge = {

                  "2016ULpreVFP" : {
                    "folder" : "../2016preVFP2L",
                    "tag"    : "Hgg2L",
                   },

                  "2016ULpostVFP" : {
                    "folder" : "../2016postVFP2L",
                    "tag"    : "Hgg2L",
                   },

                  "2017UL" : {
                    "folder" : "../20172L",
                    "tag"    : "Hgg2L",
                   },

                  "2018UL" : {
                    "folder" : "../20182L",
                    "tag"    : "Hgg2L",
                   }

                 }




