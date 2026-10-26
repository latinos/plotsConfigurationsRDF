
# tag: used to define root file name and more
tag = 'DMHMET1L_full_Run2'

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
                    "folder" : "../2016preVFP1L",
                    "tag"    : "DMHMET1L",
                   },

                  "2016ULpostVFP" : {
                    "folder" : "../2016postVFP1L",
                    "tag"    : "DMHMET1L",
                   },

                  "2017UL" : {
                    "folder" : "../20171L",
                    "tag"    : "DMHMET1L",
                   },

                  "2018UL" : {
                    "folder" : "../20181L",
                    "tag"    : "DMHMET1L",
                   }

                 }




