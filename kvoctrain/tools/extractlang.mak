g++ -I $QTDIR/include -L $QTDIR/lib extractlang.cpp -o extractlang  -lqt
cat englangn.html | ./extractlang > languages.txt