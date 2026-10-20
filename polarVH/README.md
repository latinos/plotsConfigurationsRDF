polarVH analysis
====

Run 2 configurations (UL): 2016preVFP, 2016postVFP, 2017, 2018, and the merged Run2.

NB: for now samples, cuts and nuisances are the same as in SSWW (Run 2 folders).

Run (e.g. 2018):

    cd 2018
    source ../../../latinoRDF/setup.sh
    mkShapes.py  --pycfg=configuration.py

Full Run 2 merge:

    cd Run2
    mkMergeYears.py
