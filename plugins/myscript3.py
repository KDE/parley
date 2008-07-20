#!/usr/bin/env kross

def init():
  print "init: myscript3"

def translate(word):
  print "myscript3: Translating Word ..."

print "myscript3"
#init()
#Parley.callFromScript()
Parley.connect("signalTranslateWord(QString)",translate)
#init()