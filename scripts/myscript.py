import urllib2
import urllib
import re
from sgmllib import SGMLParser


url = "http://translate.google.com/translate_dict"
google_url = "http://ajax.googleapis.com/ajax/services/language/translate"
referer_url = "http://edu.kde.org/parley/"
referer = "Referer: "+referer_url
header_referer = ("Referer",referer_url)
param_lang_pair = ("langpair","en|fr")
param_version = ("v","1.0")

class myParser(SGMLParser):

  def reset(self):
    SGMLParser.reset(self)
    self.words = []
    self.good = False

  def start_span(self, attrs):
    for name, value in attrs:
      if name == "class" and value == "definition":
        self.good = True
        print self.get_starttag_text()

  def handle_data(self,data):
    if self.good == True:
        print "data: ", data
        self.good = False

def parserTest(data):
  p = myParser()
  p.feed(data)
  p.close()


def googleTranslation(word):
  param_word_trn = ("q",word)
  request_url = google_url + "?" + urllib.urlencode([param_version,param_word_trn,param_lang_pair])
  print request_url
  headers = urllib.urlencode([header_referer])
  results = urllib2.urlopen(request_url,headers)
  data = results.read()
  
def googleTranslation2(word):
  param_word_trn = ("q",word)
  request_url = google_url + "?" + urllib.urlencode([param_version,param_word_trn,param_lang_pair])
  print request_url
  r = urllib2.Request(request_url)
  r.add_header("Referer",referer_url)
  results = urllib2.urlopen(r)
  data = results.read()
  return data

def fetchTranslation(word):
  param_word_trn = ("q",word)
  request_url = url + "?" + urllib.urlencode([param_word_trn,param_lang_pair])
  print request_url
  results = urllib2.urlopen(request_url)
  data = results.read()
  #p = re.compile('<span\ class="definition">(?:<span class="comment">.*</span>)*([^<>]*)<span class="comment">',re.IGNORECASE)
  p = re.compile('<span\ class="definition">([^<>]*)<span class="comment">',re.IGNORECASE)
  l = p.findall(data)
  parserTest(data)
  return l

def returnlist():
    L = [1,2,3,4,5]
    return L

print fetchTranslation("love")