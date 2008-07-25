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
  
def setWordType():
  for entry in Parley.doc.rootLesson.entries(True):
    for tr in entry.translations():
        Parley.doc.setWordType(tr,"Noun")
        print tr.wordType()
  
def conjugations():
  Parley.doc.setTenses(["present","past"])
  print Parley.doc.tenses()
  for entry in Parley.doc.rootLesson.entries(True):
    for tr in entry.translations():
        if tr.wordType() == "Verb":
          print "setting conjugation"
          print tr.text
          #tr.setConjugation("first","present","Plural","First")
          tr.setConjugation("first","present",Parley.Plural,Parley.First)
          tr.setConjugation("third","present",Parley.Plural,Parley.Third)
          tr.setConjugation("male","present",Parley.Plural,Parley.ThirdMale)
          #tr.setConjugation("female","present","Plural","ThirdFemale")
          #tr.setConjugation("playing","past","plural","Third")
          #tr.setConjugation("se","present","Plural","Second")
          #tr.setConjugation("dualthird","present","Dual","Third")
          #print tr.conjugation("past","plural","Third")
          print tr.conjugation("present")
          print tr.conjugation("present",Parley.Plural)
  
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
  
  newaction2 = Parley.newAction("my_script1","My script 2a")
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
 new_entry = Parley.activeLesson.newEntry()
 new_entry.setTranslations(["good morning","bonjour"])
 print new_entry.active
 new_entry.active = False
 Parley.activeLesson.appendEntry(new_entry)
 return
 #Parley.doc.setTenseName(0,"present")
 #Parley.doc.setTenseName(1,"past simple")
 for entry in Parley.doc.rootLesson.entries(True):
    for tr in entry.translations():
        print tr.wordType()
        if tr.text == "play":
            Parley.doc.setWordType(tr,"Verb")
            tr.setConjugation("plays","present","singular","third")
            tr.setConjugation("played","past simple","singular","first")
 return
 for entry in Parley.document.rootLesson.entries(True):
    for translation in entry.translations():
      print translation.text
      print translation.practiceDate()
      print translation.practiceDate("dd/MM/yyyy")


def newDocument():
  doc = Parley.newDocument()
  doc.title = "New document"
  #set identifiers
  doc.appendNewIdentifier("English","en_US")
  doc.appendNewIdentifier("French","fr")
  #lessons
  l1 = doc.newLesson("Lesson1")
  doc.rootLesson.appendChildLesson(l1)
  #first way
  e = l1.newEntry()
  e.setTranslation(0,"dog")
  e.setTranslation(1,"chien")
  l1.appendEntry(e)
  #second way
  ee = l1.newEntry(["glass","verre"])
  l1.appendEntry(ee)
  #third way
  ee = l1.appendNewEntry(["book","livre"])
  
  #new lesson (fast way)
  l2 = doc.appendNewLesson("Lesson 2")
  l2.appendNewEntry(["I","je"]);
  
  #new lesson under Lesson 2
  l3 = doc.appendNewLesson("Lesson 3",l2)
  l3.appendNewEntry(["good morning","bonjour"])
  
  l5 = doc.newLesson("Lesson 5")
  doc.appendLesson(l5)
  
  #fl = doc.findLesson("Lesson 3")
  #if fl != None:
    #print "found"
    #print fl.name
  #else:
    #print "not found"
  
  #save document
  doc.saveAs("/home/kde-devel/test_new_document.kvtml")
  
def tryArticle():
   #add new one
  newid = Parley.doc.newIdentifier()
  newid.name = "English"
  newid.locale = "en_US"
  newid.setArticle("o",Parley.Singular,Parley.Definite,Parley.Masculine)
  newid.setArticle("h",Parley.Singular,Parley.Definite,Parley.Feminine)
  newid.setArticle("to",Parley.Singular,Parley.Definite,Parley.Neutral)
  newid.setPersonalPronoun("oi",Parley.Plural,Parley.First)
  print newid.personalPronoun(Parley.Plural,Parley.First)
  print newid.article(Parley.Singular,Parley.Definite,Parley.Neutral)
  index = Parley.doc.appendIdentifier(newid)
  
def GermanArticles():
    newid = Parley.doc.newIdentifier()
    newid.name = "German"
    newid.locale = "fr"
    newid.setArticle("der",Parley.Singular,Parley.Definite,Parley.Masculine)
    newid.setArticle("die",Parley.Singular,Parley.Definite,Parley.Feminine)
    newid.setArticle("das",Parley.Singular,Parley.Definite,Parley.Neutral)
    newid.setArticle("ein",Parley.Singular,Parley.Indefinite,Parley.Masculine)
    newid.setArticle("eine",Parley.Singular,Parley.Indefinite,Parley.Feminine)
    newid.setArticle("ein",Parley.Singular,Parley.Indefinite,Parley.Neutral)
    newid.setPersonalPronoun("ich",Parley.Singular,Parley.First)
    newid.setPersonalPronoun("du",Parley.Singular,Parley.Second)
    newid.setPersonalPronoun("er",Parley.Singular,Parley.ThirdMale)
    newid.setPersonalPronoun("sie",Parley.Singular,Parley.ThirdFemale)
    newid.setPersonalPronoun("es",Parley.Singular,Parley.Third)
    newid.setPersonalPronoun("wir",Parley.Plural,Parley.First)
    newid.setPersonalPronoun("ihr",Parley.Plural,Parley.Second)
    newid.setPersonalPronoun("sie",Parley.Plural,Parley.Third)
    print newid.personalPronouns()
    index = Parley.doc.appendIdentifier(newid)
  
def testEnums():
  for entry in Parley.doc.rootLesson.entries(True):
    tr = entry.translation(0)
    print tr
    tr.setDeclension("Paizontas",Parley.Singular,Parley.Genitive)
    tr.setDeclension("Paizomenos",Parley.Singular,Parley.Accusative)
    print tr.declension(Parley.Singular,Parley.Genitive).text
    print tr.declension(Parley.Singular,Parley.Accusative).text
    return
  
def testSetEntries():
  l = Parley.activeLesson
  entries = []
  #entries.append(l.newEntry(["good morning","bonjoura"]))
  entries.append(l.newEntry(["ela","paenne"]))
  print entries
  l.setEntries(entries)
  
def actionFunction():
  print "Action called!!"
  #for entry in Parley.doc.rootLesson.entries(True):
    #print entry.translationTexts()
  testSetEntries()
  #testEnums()
  #tryArticle()
  #GermanArticles()
  #newDocument()
  #conjugations()
  #print Parley.doc.wordTypes()
  #setWordType()
  #testcode()
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
  tenses = Parley.doc.tenses()
  print tenses
  tenses.append("Present Perfect")
  tenses.append("Past Simple")
  Parley.doc.setTenses(tenses)
  print Parley.doc.tenses()
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

#print Parley.locales()
#print Parley.localeName("en_US")

#doc = Parley.document()
#doc.callFromScriptTest()

#Parley.document.callFromScriptTest()

#doc = Parley.document
#doc.name = "Hi"
#doc.printName()

def translateFromInternet(word,fromLang,toLang):
    print "Translating from Internet!!.."

Parley.connect("translateWord(const QString &,const QString &,const QString &)",translateFromInternet)

print "TEST ACTION"
registerActions()

#print "OPENING DOCUMENT"
##Parley.open('/home/kde-devel/My\ Vocabularies/test.kvtml')

#print "myscript1"
#init()
#Parley.callFromScript()
#Parley.connect("signalTranslateWord(QString)",translate)
#init()

