echo off
:while
del testdata.txt
del target.txt
del correct.txt
gentreedata 100 50 30 20>>testdata.txt
type testdata.txt|bbstzry.exe>>target.txt
type testdata.txt|bst.exe>>correct.txt
outcompare.exe target.txt  correct.txt
goto while