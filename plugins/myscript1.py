#!/usr/bin/env kross

import Parley

def init():
  print "init: myscript1"

def translateWord(word,fromLang,toLang):
  print "myscript1: Translating Word ...", word, fromLang, toLang
  Parley.addTranslation("Hello","en_US","fr","bonbon")

print "myscript1"
#init()
#Parley.callFromScript()
#Parley.connect("signalTranslateWord(QString)",translate)
#init()