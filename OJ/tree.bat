del testdata.txt
del AVL.txt
del RB_Tree.txt
gentreedata 1000000 60 40 0 >> testdata.txt
type testdata.txt|AVL.exe >> AVL.txt
type testdata.txt|RB_Tree.exe >> RB_Tree.txt