#!/usr/bin/env kross

import Parley

def init():
  print "init: myscript1"

def translate(word):
  return "czesc"

print "myscript1"
init()
Parley.callFromScript()
Parley.connect("translateWord(QString)",translate)
init()