#!/usr/bin/env kross

import Parley

def init():
  print "init: myscript1"

print "myscript1"
init()
Parley.callFromScript()
init()