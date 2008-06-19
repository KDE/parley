#!/usr/bin/env kross

def init():
  print "init: myscript2"

def translate(word):
  print "myscript2: Translating Word ..."

print "myscript2"
#init()
#Parley.callFromScript()
Parley.connect("signalTranslateWord(QString)",translate)
#init()