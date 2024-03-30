# ETLBeamTestReco

## Setup
This initial setup must be done once only
```
cd <WorkingArea>
git clone git@github.com:CMS-MTD/ETLBeamTestReco.git
cd ETLBeamTestReco/test
source setup.sh
./configure
make clean
make -j4
```

Later, everytime you make a change, must run
```
cd <WorkingArea>/ETLBeamTestReco/test
source setup.sh
make -j4
```

Example of running MyAnalysis interactively
```
cd <WorkingArea>/ETLBeamTestReco/test
source setup.sh
./MyAnalysis -A Analyze -D ETL_Test -E 100001
cd ../macros
python Plot_TimeDiffVsXY.py -D ETL_Test
```

