import urllib2
import urllib
import re
from sgmllib import SGMLParser

url = "http://translate.google.com/translate_dict"
param_lang_pair = ("langpair","en|fr")

class myParser(SGMLParser):
  #for every start_tagname function you add you have to make sure the tag is added to the self.tags_stack

  def reset(self):
    SGMLParser.reset(self)
    self.words = []         #translated words found in html
    self.langpairs = []     #language pairs found in html file
    self.tags_stack = []

  def unknown_starttag(self,tag,attrs):
        self.tags_stack.append(tag)
        #print "unknown : ", tag, " ", len(self.tags_stack)

  def start_span(self, attrs):
    #print "known : ", "span", " ", len(self.tags_stack)
    if ("class","definition") in attrs:
        self.tags_stack.append("<!translation!>")
    else:
        self.tags_stack.append("span")

  def start_option(self, attrs):
    for name,value in attrs:
      if name == "value":
        self.langpairs.append(value)
    self.tags_stack.append("option")

  def handle_data(self,data):
    if self.tags_stack[len(self.tags_stack)-1] == "<!translation!>":
        #print "data: ", data
        self.words.append(data)
  
  def unknown_endtag(self,tag):
    myParser.remove_not_closed_tags(self,tag)
    if len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] == tag:
        #print "end_tag : ", tag, " ", len(self.tags_stack)
        self.tags_stack.pop()

  #removes all the tags from the stack that have no closed tags (don't modify)
  def remove_not_closed_tags(self,tag):
    while len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] != tag:
      self.tags_stack.pop()

#parses data and returns the parser object (that contains the translations/langpairs found)
def parseData(data):
  p = myParser()
  p.feed(data)
  p.close()
  return p

def fetchData(word):
  param_word_trn = ("q",word)       #set query parameter
  request_url = url + "?" + urllib.urlencode([param_word_trn,param_lang_pair])
  #print request_url
  results = urllib2.urlopen(request_url)
  return results.read()
  
def fetchTranslation(word):
  data = fetchData(word)
  parser = parseData(data)
  return parser.words

def getLanguagePairs():
  data = fetchData("ignorethis")
  parser = parseData(data)
  return parser.langpairs

print fetchTranslation("love")
print getLanguagePairs()