#!/usr/bin/env kross
import Parley

def init():
  print "init: myscript1"

def translateWord(word,fromLang,toLang):
  print "myscript1: Translating Word ...", word, fromLang, toLang
  Parley.addTranslation("Hello","en_US","fr","bonbon")
  
def test():
  print "Test from myscript1.py"
  #Parley.open('/home/kde-devel/My\ Vocabularies/test2.kvtml')
  print Parley.activeLesson.name
  Parley.activeLesson.name = "Hi!!"
  myentries = Parley.activeLesson.getEntries()
  #print myentries[0].getTranslation()
  print myentries

print Parley.languageCodes()
print Parley.languageCodeToName("en_US")

#doc = Parley.document()
#doc.callFromScriptTest()
Parley.document.callFromScriptTest()

doc = Parley.document
doc.name = "Hi"
doc.printName()

#print "OPENING DOCUMENT"
##Parley.open('/home/kde-devel/My\ Vocabularies/test.kvtml')

#print "myscript1"
#init()
#Parley.callFromScript()
#Parley.connect("signalTranslateWord(QString)",translate)
#init()