#!/usr/bin/env kross

import Parley

def init():
  print "init: myscript1"

def translate(word):
  print "myscript1: Translating Word ..."

print "myscript1"
#init()
#Parley.callFromScript()
Parley.connect("signalTranslateWord(QString)",translate)
#init()