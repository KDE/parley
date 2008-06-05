import urllib2
import urllib
import re

url = "http://translate.google.com/translate_dict"
param_lang_pair = ("langpair","en|fr")

def fetchTranslation(word):
  param_word_trn = ("q",word)
  request_url = url + "?" + urllib.urlencode([param_word_trn,param_lang_pair])
  print request_url
  results = urllib2.urlopen(request_url)
  data = results.read()
  #p = re.compile('<span\ class="definition">(?:<span class="comment">.*</span>)*([^<>]*)<span class="comment">',re.IGNORECASE)
  p = re.compile('<span\ class="definition">([^<>]*)<span class="comment">',re.IGNORECASE)
  #l = p.search(data).group(0)
  #l = p.search(data).group(1)
  l = p.findall(data)
  print l
  return "hallo"

def returnlist():
    L = [1,2,3,4,5]
    return L

fetchTranslation("love")