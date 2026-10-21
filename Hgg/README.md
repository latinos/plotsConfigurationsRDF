Hgg analysis
====

VH production, H > gluon gluon.

Same format as SSWW, with the EWK ssWW as a background.
Signal from:

    https://github.com/latinos/PlotsConfigurationsRun3/tree/amassiro-Hgg/Hgg

Where:

    <year>2L   2 leptons (ZH, Z>ll), cuts from 2018UL
    <year>1L   1 lepton  (WH, W>lv, and ZH with one lepton lost), cuts from 2018UL1l
    Run22L, Run21L, Run32L, Run31L   merge of the years

NB: signal only available for Run 2:

    2018: ZHgg (inclusive), ZHllHgg (qqZH), ggZHllHgg (ggZH), WminuslvHgg + WpluslvHgg (WH)
    2017: ZHgg (inclusive)
    2016: not available

NB: signal samples are missing the full post-processing: mcCommonWeight not applied, and signal excluded from mcALL.

NB: utils (macros, btag and fake rate maps) are taken from ../SSWW/utils

NB: Run 3 1L folders still use the 2 leptons trees (1 lepton skim not available yet)

Run (e.g. 2018, 2 leptons):

    cd 20182L
    source ../../../latinoRDF/setup.sh
    mkShapes.py  --pycfg=configuration.py

Full Run 2 merge:

    cd Run22L
    mkMergeYears.py
