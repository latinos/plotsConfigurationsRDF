
# tag: used to define root file name and more
tag = 'DMHMET_full_Run2'

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
                    "folder" : "../2016preVFP",
                    "tag"    : "DMHMET",
                   },

                  "2016ULpostVFP" : {
                    "folder" : "../2016postVFP",
                    "tag"    : "DMHMET",
                   },

                  "2017UL" : {
                    "folder" : "../2017",
                    "tag"    : "DMHMET",
                   },

                  "2018UL" : {
                    "folder" : "../2018",
                    "tag"    : "DMHMET",
                   }

                 }




