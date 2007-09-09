#! /bin/sh
$EXTRACTRC `find . -name "*.ui"` >> rc.cpp || exit 11
$XGETTEXT `find . -name "*.cpp"` -kI18N_NOOP2 -o $podir/kvoctrain.pot
