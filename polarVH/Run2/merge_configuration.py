
# tag: used to define root file name and more
tag = 'polarVH_full_Run2'

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
                    "tag"    : "polarVH",
                   },

                  "2016ULpostVFP" : {
                    "folder" : "../2016postVFP",
                    "tag"    : "polarVH",
                   },

                  "2017UL" : {
                    "folder" : "../2017",
                    "tag"    : "polarVH",
                   },

                  "2018UL" : {
                    "folder" : "../2018",
                    "tag"    : "polarVH",
                   }

                 }




