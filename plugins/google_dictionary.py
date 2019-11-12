#!/usr/bin/env kf5kross

import socket
import urllib2
import urllib
from sgmllib import SGMLParser
import Parley
import re

# timeout of search (important for slow connections, not to freeze Parley by waiting for a result)
timeout = 1.0
socket.setdefaulttimeout(timeout)

# fetches the html document for the given word and language pair
def fetchData(word,from_lang,to_lang):
  url = "https://translate.google.com/translate_dict"
  param_word_trn = ("q",word)       #set query parameter
  param_lang_pair = ("langpair",from_lang+"|"+to_lang)
  request_url = url + "?" + urllib.urlencode([param_word_trn,param_lang_pair])
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
  if lang == "zh_TW":
    return "zh-TW"
  if lang == "zh_HK":
    return "zh-HK"
  if lang == "zh_CN":
    return "zh-CN"

  return lang

# called by Parley to translate the word
def translateWord(word,from_lang,to_lang):
  print "google_dictionary.py - Translating",word,from_lang,to_lang
  data = fetchData(word,locale(from_lang),locale(to_lang))
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
    self.words = []         #translated words found in html
    self.langpairs = []     #language pairs found in html file
    self.tags_stack = []
    self.stop = False

  def unknown_starttag(self,tag,attrs):
    self.tags_stack.append(tag)
    #print "unknown : ", tag, " ", len(self.tags_stack)

  def start_span(self, attrs):
    #print "known : ", "span", " ", len(self.tags_stack)
    if ("class","definition") in attrs:
        self.tags_stack.append("<!translation!>") #marks tag with <!translation!> to get its data in handle_data
    else:
        self.tags_stack.append("span")

  def start_option(self, attrs):
    for name,value in attrs:
      if name == "value":
        self.langpairs.append(value)
    self.tags_stack.append("option")

  def handle_data(self,data):
    if data == "Web definitions": self.stop = True #to make it stop after the web definitions
    if len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] == "<!translation!>":
        #print "data: ", data
        self.words.append(data.strip())
        #print self.word, self.from_lang, self.to_lang
        if self.stop == False:
          w = self.clearWord(data)
          Parley.addTranslation(self.word,self.from_lang,self.to_lang,w)
  
  def unknown_endtag(self,tag):
    myParser.remove_not_closed_tags(self,tag)
    if len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] == tag:
        #print "end_tag : ", tag, " ", len(self.tags_stack)
        self.tags_stack.pop()

  #removes all the tags from the stack that have no closed tags (don't modify)
  def remove_not_closed_tags(self,tag):
    while len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] != tag:
      self.tags_stack.pop()

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
