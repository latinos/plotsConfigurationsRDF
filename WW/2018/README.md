WW analysis 2018
====

Setup:

    source setup.sh
    source ../../../latinoRDF/setup.sh


Histogram production


Prepare the scripts:

    mkShapes.py  --pycfg=configuration.py

Submit on batch system:

    mkShapes.py  --pycfg=configuration.py --submitBatch
    mkShapes.py  --pycfg=configuration.py --submit

Hadd the root files:

    mkShapes.py  --pycfg=configuration.py --hadd



Readable:

    easyDescription.py --inputFileSamples      samples.py     --outputFileSamples      extended_samples.py
    easyDescription.py --inputFileCuts         cuts.py        --outputFileCuts         extended_cuts.py
    easyDescription.py --inputFileNuisances    nuisances.py   --outputFileNuisances    extended_nuisances.py   --inputFileSamples      samples.py




    g++ -O2 -Wall -pthread -std=c++17 -m64 -fsized-deallocation -I/usr/include/root  my_run_analysis_WpWp_QCD_WpWpJJ_QCD_UL_3.cpp -o my_run_analysis_WpWp_QCD_WpWpJJ_QCD_UL_3 -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lROOTNTuple -lROOTNTupleUtil -lMultiProc -lROOTDataFrame -Wl,-rpath,/usr/lib64/root -pthread -lm -ldl -rdynamic


    g++ -O2 -Wall -pthread -std=c++20 -m64 -fsized-deallocation -I/usr/include/root  my_run_analysis_WpWp_QCD_WpWpJJ_QCD_UL_3.cpp -o my_run_analysis_WpWp_QCD_WpWpJJ_QCD_UL_3 -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lROOTNTuple -lROOTNTupleUtil -lMultiProc -lROOTDataFrame -Wl,-rpath,/usr/lib64/root -pthread -lm -ldl -rdynamic


    g++ -O2 -Wall -pthread -std=c++17 -m64 -fsized-deallocation -I/usr/include/root -std=c++20 -Wcpp  scripts_run/WpWp_QCD/WpWpJJ_QCD_UL/my_run_analysis_WpWp_QCD_WpWpJJ_QCD_UL_10.cpp -o scripts_run/WpWp_QCD/WpWpJJ_QCD_UL/my_run_analysis_WpWp_QCD_WpWpJJ_QCD_UL_10 -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lROOTNTuple -lROOTNTupleUtil -lMultiProc -lROOTDataFrame -Wl,-rpath,/usr/lib64/root -pthread -lm -ldl -rdynamic

    g++ -O2 -Wall -pthread -std=c++17 -m64 -fsized-deallocation -I/usr/include/root scripts_run/SSWW/WpWpJJ_EWK_UL/my_run_analysis_SSWW_WpWpJJ_EWK_UL_24.cpp -o scripts_run/SSWW/WpWpJJ_EWK_UL/my_run_analysis_SSWW_WpWpJJ_EWK_UL_24 -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lROOTNTuple -lROOTNTupleUtil -lMultiProc -lROOTDataFrame -Wl,-rpath,/usr/lib64/root -pthread -lm -ldl -rdynamic

