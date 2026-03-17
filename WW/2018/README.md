WW analysis 2018
====

Setup:

    source setup.sh
    source ../../../latinoRDF/setup.sh


Histogram production

    mkShapes.py  --pycfg=configuration.py



Readable:

    easyDescription.py --inputFileSamples samples.py   --outputFileSamples extended_samples.py
    easyDescription.py --inputFileCuts    cuts.py      --outputFileCuts    extended_cuts.py




