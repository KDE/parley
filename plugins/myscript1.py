#!/usr/bin/env kross
import Parley

def init():
  print "init: myscript1"

def translateWord(word,fromLang,toLang):
  print "myscript1: Translating Word ...", word, fromLang, toLang
  Parley.addTranslation("Hello","en_US","fr","bonbon")
  
def addNewEntry():
  print "Adding new entry"
  newEntry = Parley.activeLesson.newEntry()
  newEntry.translation(0).text = "boy"
  newEntry.translation(1).text = "hijo"
  Parley.activeLesson.appendEntry(newEntry)
  
def addNewLesson():
  newlesson = Parley.document.newLesson("Hello There!!");
  lesson.appendChildLesson(newlesson)
  newEntry = newlesson.newEntry()
  newEntry.translation(0).text = "Hello"
  newEntry.translation(1).text = "Hola"
  newlesson.appendEntry(newEntry)
  
def testKUrl():
  l = Parley.activeLesson
  print l.name
  print l.imageUrl
  l.imageUrl = "/home/kde-devel/"
  print l.imageUrl
  #print l.imageUrl()
  #l.setImageUrl("/home/kde-devel/image.jpg");
  #print l.imageUrl()
  
def registerActions():
  newaction = Parley.newAction("my_script1")
  print newaction
  newaction.text="My script 1"
  Parley.connect(newaction,"triggered()",actionFunction)
  
  newaction2 = Parley.newAction("my_script1")
  newaction2.text = "My script 2"
  Parley.connect(newaction2,"triggered()",actionFunction2)
  return

def appendChildrenToAllLessons():
  children = Parley.activeLesson.childLessons()
  print children
  i = 1
  for child in children:
    print dir(child)
    newlesson = Parley.document.newLesson("Testa!! " + str(i))
    child.appendChildLesson(newlesson)
    i = i + 1
  return
  
def testcode():
  for entry in Parley.document.rootLesson.entries(True):
    for translation in entry.translations():
      print translation.text
      print translation.practiceDate
      print translation.practiceDate.isValid()


def actionFunction():
  print "Action called!!"
  testcode()
  #appendChildrenToAllLessons()
  
  #for lesson in Parley.document.allLessons():
    #print lesson.name
  
  #children = Parley.activeLesson.childLessons(True)
  #for child in children:
    #print child.name
    

def actionFunction_old():
  print "Action called!!"
  entries = Parley.activeLesson.entries(True)
  
  for entry in entries:
    s = ""
    for tr in entry.translations:
      print tr
      if tr.text:
        s += tr.text + " "
    print s;
    
  print "sublessons"
  for sublesson in Parley.activeLesson.childLessons:
    print sublesson.name

  return

def actionFunction2():
  print "Action2 called!!"
  return


def test():
    print "Test from myscript1.py"
    testAction()
  
def test_old():
  print "Test from myscript1.py"
  print Parley.document.rootLesson.name
  #addNewEntry()
  testKUrl()
  #Parley.open('/home/kde-devel/My\ Vocabularies/test2.kvtml')
  #print Parley.activeLesson.name
  #Parley.activeLesson.name = "Hi!!"
  lesson = Parley.activeLesson
  #print lesson.getEntries()
  
  print lesson.name
  print "Child lessons"
  print lesson.childLessonCount()
  for i in range(0,lesson.childLessonCount()):
    print " ",lesson.childLesson(i).name
    
  print lesson.entry(0,True).translationIndices
  print lesson.entry(0,True).translation(0).text
  print lesson.entry(0,True).translation(1).text
  print lesson.entry(0,True).translation(1).practiceDate
  print "Listing"
  print dir(lesson.entry(0,True).translation(1))
  
  print "Translation attributes"
  print len(dir(lesson.entry(0,True).translation(1)))
  for attr in dir(lesson.entry(0,True).translation(1)):
    a = getattr(lesson.entry(0,True).translation(1),attr)
    print attr, a

  for entry in lesson.entries(true):
     print entry.translation(0).text,lesson.entry(i,True).translation(1).grade
     
  for i in range(0,lesson.entryCount(True)):
    entry = lesson.entry(i,True)
    if entry.translation(0).text == "girl":
      entry.translation(0).text = "boy"

  print lesson.entryCount()
  lesson.removeEntry(0)
  print lesson.entryCount()

#print Parley.languageCodes()
#print Parley.languageCodeToName("en_US")

#doc = Parley.document()
#doc.callFromScriptTest()

#Parley.document.callFromScriptTest()

#doc = Parley.document
#doc.name = "Hi"
#doc.printName()

print "TEST ACTION"
registerActions()

#print "OPENING DOCUMENT"
##Parley.open('/home/kde-devel/My\ Vocabularies/test.kvtml')

#print "myscript1"
#init()
#Parley.callFromScript()
#Parley.connect("signalTranslateWord(QString)",translate)
#init()