#! /bin/sh
# invoke the extractrc script on all .ui, .rc, and .kcfg files in the sources
# the results are stored in a pseudo .cpp file to be picked up by xgettext.
$EXTRACTRC `find . -name \*.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
# call preparetips as well.
(cd tipofday && $PREPARETIPS > ../tips.cpp)
# call xgettext on all source files. If your sources have other filename
# extensions besides .cc, .cpp, and .h, just add them in the find call.
$XGETTEXT `find . -name \*.cpp -o -name \*.h` -o $podir/parley.pot
$XGETTEXT -L python `find . -name \*.py` -o $podir/parley.pot -j
# remove the generated files
rm -f tips.cpp rc.cpp python_parley.pot

