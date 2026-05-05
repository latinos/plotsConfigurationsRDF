WW analysis 2017
====

Setup:

    source setup.sh
    source ../../../latinoRDF/setup.sh

Histogram production


Prepare the scripts and compile them:

    mkShapes.py  --pycfg=configuration.py

Submit on batch system:

    mkShapes.py  --pycfg=configuration.py --submitBatch
    mkShapes.py  --pycfg=configuration.py --submit

Check if done:

    mkShapes.py  --pycfg=configuration.py --checkBatch

Hadd the root files:

    mkShapes.py  --pycfg=configuration.py --hadd


Plot:

    mkPlot.py  --pycfg=configuration.py
    mkPlot.py  --pycfg=configuration.py  --showIntegralLegend 1


Datacards:

    mkDatacards.py  --pycfg=configuration.py

Dump the trees:

    mkSnapshot.py   --pycfg=configuration.py
    mkSnapshot.py   --pycfg=configuration.py  --submitBatch
    mkSnapshot.py   --pycfg=configuration.py  --checkBatch

