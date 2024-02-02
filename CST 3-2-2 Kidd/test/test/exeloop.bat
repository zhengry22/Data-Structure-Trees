echo off
:while
del testdata.txt
del target.txt
del correct.txt
geninput>>testdata.txt
type testdata.txt|kiddtree.exe>>target.txt
type testdata.txt|kiddcorrect.exe>>correct.txt
outcompare.exe target.txt  correct.txt
goto while