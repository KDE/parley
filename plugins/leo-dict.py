#!/usr/bin/env kf5kross
# -*- coding: utf-8 -*-

import socket
import urllib2
import urllib
from sgmllib import SGMLParser
import Parley
import re

#german to french (Glas) [links off]
#https://dict.leo.org/frde?lp=frde&lang=de&searchLoc=1&cmpType=relaxed&sectHdr=on&spellToler=on&chinese=both&pinyin=diacritic&search=Glas&relink=off

#german to spanish (Glas)
#https://dict.leo.org/esde?lp=esde&lang=de&searchLoc=1&cmpType=relaxed&sectHdr=on&spellToler=std&chinese=both&pinyin=diacritic&search=Glas&relink=on

#spanish to german (tarro)
#https://dict.leo.org/esde?lp=esde&lang=de&searchLoc=-1&cmpType=relaxed&sectHdr=on&spellToler=std&chinese=both&pinyin=diacritic&search=tarro&relink=on

#spanish to german (tarro) [links off]
#https://dict.leo.org/esde?lp=esde&lang=de&searchLoc=-1&cmpType=relaxed&sectHdr=on&spellToler=std&chinese=both&pinyin=diacritic&search=tarro&relink=off

# timeout of search (important for slow connections, not to freeze Parley by waiting for a result)
timeout = 1.0
socket.setdefaulttimeout(timeout)

def languageString(from_lang,to_lang):
  combined = from_lang + "-" + to_lang
  A = { 'es-de': 'esde', 'de-es':'esde', 'fr-de':'frde', 'de-fr':'frde', 'en-de':'ende', 'de-en':'ende', 'it-de':'itde', 'de-it':'itde', 'ch-de':'chde', 'de-ch':'chde' }
  if (A.has_key(combined)):
    return A[combined]
  else:
    return None

def searchLoc(from_lang,to_lang):
  if from_lang == "de":
    return 1
  return -1

# fetches the html document for the given word and language pair
def fetchData(word,from_lang,to_lang):
  locale = languageString(from_lang,to_lang)
  if locale == None:
    return #not supported languages
  url = "https://dict.leo.org/"+locale
  #esde?lp=esde&lang=de&searchLoc=-1&cmpType=relaxed&sectHdr=on&spellToler=std&chinese=both&pinyin=diacritic&search=tarro&relink=off
  params = [("lp",locale),("lang","de"),("searchLoc",searchLoc(from_lang,to_lang)),("cmpType","relaxed"),("sectHdr","on"),("spellToler","std"),("chinese","both"),("pinyin","diacritic"),("search",word),("relink","off")]

  #param_word_trn = ("q",word)       #set query parameter
  #param_lang_pair = ("langpair",from_lang+"|"+to_lang)
  request_url = url + "?" + urllib.urlencode(params)
  #print request_url
  try:
    results = urllib2.urlopen(request_url)
    return results.read()
  except:
    #in case of error not to return incompleted results
    return ""

#parses data and returns the parser object (that contains the translations/langpairs found)
def parseData(data,word,from_lang,to_lang):
  p = myParser()
  p.word = word
  p.from_lang = from_lang
  p.to_lang = to_lang
  p.feed(data)
  p.close()
  return p

#corrects the difference between the locale names of Parley and the google dictionary
def locale(lang):
  if lang == "en_US":
    return "en"
  if lang == "en_GB":
    return "en"
  if lang == "zh_TW":
    return "zh-TW"
  if lang == "zh_HK":
    return "zh-HK"
  if lang == "zh_CN":
    return "zh-CN"

  return lang

# called by Parley to translate the word
def translateWord(word,from_lang,to_lang):
  print "dict-leo.py - Translating:",word, "from", from_lang, "to", to_lang
  data = fetchData(word,locale(from_lang),locale(to_lang))
  if data != None:
    print "Data Fetched for:",word
    #print data
  #print data
  parser = parseData(data,word,from_lang,to_lang)
  #return parser.words

# called by Parley to retrieve the language pairs provided by this script
# should return: [("en","fr"),("en","de")] for translation from english to french and english to german
def getLanguagePairs():
  data = fetchData("ignorethis","en","fr")
  parser = parseData(data)
  return map(split_langpair,parser.langpairs)

# function to split a language pair string into a tuple
def split_langpair(s):
  [f,t] = s.split("|",1)
  return (f,t)

# ------------ HTML Parser ----------- #

class myParser(SGMLParser):
  #for every start_tagname function you add you have to make sure the tag is added to the self.tags_stack

  def reset(self):
    SGMLParser.reset(self)
    #self.tags_stack = []
    self.td_data_stack = []
    self.in_results_table = False
    self.td_data = ""
    self.td_after = 0
    self.keyword_found = False
    self.in_translation_td = False
    self.in_small = False

  def start_table(self,attrs):
    #mark the results table
    if ("id","results") in attrs:
      self.in_results_table = True
    
  #to ignore the <small> tags
  def start_small(self,attrs):
    self.in_small = True
    
  #def end_small(self):
    #self.in_small = False
    
  #checks if the word contained by this <td> (self.td_data) matches the keyword. If yes then the corresponding td_data (previous/next two) is added as a translation
  def end_td(self):
    #print "end of a table data"
    #print "-" ,self.td_data , "-"
    #print myParser.clearWord(self,self.td_data)

    self.td_data = myParser.clearWord(self,self.td_data)

    #matching on the second column (german) and getting the translation from 2 column's on the left
    if self.td_data.lower() == self.word.lower(): #found word
      #print self.td_data
      self.keyword_found = True
      if self.from_lang == "de": #then get the one that is two td_data behind (using the stack)
        #print "Translation: ", self.td_data_stack[0]
        Parley.addTranslation(self.word,self.from_lang,self.to_lang,self.td_data_stack[0].strip())

    #matching on the first column (not german) and getting the translation from 2 column's on the right
    if self.in_translation_td: #found translation
      if self.from_lang != "de":
        #print "Translation: ", self.td_data
        Parley.addTranslation(self.word,self.from_lang,self.to_lang,self.td_data.strip())
      self.in_translation_td = False
      self.td_after = 0
      self.keyword_found = False

    #append td_data on the stack that keeps the last two ones
    self.td_data_stack.append(self.td_data)
    
    #make it keep only the last 2 td_data (works since we append only one item at a time)
    if len(self.td_data_stack) > 2:
      self.td_data_stack.pop(0)

    self.td_data = ""
    
    self.in_small = False

  #marks in which <td> to be able to count 2 more <td>'s and find our translation
  def start_td(self,attrs):
    if self.keyword_found == True:
        self.td_after += 1
        if self.td_after == 2:
          self.in_translation_td = True

  def handle_data(self,data):
    if self.in_small:
      return
    self.td_data += data
     
  #cleans up the given word from parentheses etc
  def clearWord(self,word):
    #word = "b[lue] socks (and) red shoes"
    p = re.compile( '(jmdn\.|etw\.)')
    word = p.sub( '', word)
    p = re.compile( '(\(.*\))')
    word = p.sub( '', word)
    p = re.compile( '(\[.*\])')
    word = p.sub( '', word)
    p = re.compile( '(\W)',re.UNICODE)
    word = p.sub( ' ', word)
    #replace double spaces produced from the previous ones
    p = re.compile( '(\s\s)')
    word = p.sub( ' ', word)
    return word.strip()


