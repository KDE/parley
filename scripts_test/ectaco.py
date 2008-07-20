import urllib2
import urllib
from sgmllib import SGMLParser

#"http://online-old.ectaco.com/online/diction.php3?lang=7&q=2&refid=316&pagelang=23&word=test&direction=1"

# fetches the html document for the given word and language pair
def fetchData(word,from_lang,to_lang):
  url = "http://online-old.ectaco.com/online/diction.php3"
  params = [("lang",7),("q",2),("refid",31),("pagelang",23),("word",word),("direction",1)]
  #param_word_trn = ("q",word)       #set query parameter
  #param_lang_pair = ("langpair",from_lang+"|"+to_lang)
  request_url = url + "?" + urllib.urlencode(params)
  #print request_url
  results = urllib2.urlopen(request_url)
  #results.encoding = 'windows-1250'
  return results.read().decode("cp1250")
  #return unicode(results.read(),'iso-8859-1')


#parses data and returns the parser object (that contains the translations/langpairs found)
def parseData(data):
  p = myParser()
  p.feed(data)
  p.close()
  return p

# called by Parley to translate the word
def fetchTranslation(word,from_lang,to_lang):
  data = fetchData(word,from_lang,to_lang)
  parser = parseData(data)
  return parser.words


# called by Parley to retrieve the language pairs provided by this script
# should return: [("en","fr"),("en","de")] for translation from english to french and english to german
#def getLanguagePairs():
  #data = fetchData("ignorethis","en","fr")
  #parser = parseData(data)
  #return map(split_langpair,parser.langpairs)

# function to split a language pair string into a tuple
#def split_langpair(s):
  #[f,t] = s.split("|",1)
  #return (f,t)

# ------------ HTML Parser ----------- #

class myParser(SGMLParser):
  #for every start_tagname function you add you have to make sure the tag is added to the self.tags_stack

  def reset(self):
    SGMLParser.reset(self)
    self.words = []         #translated words found in html
    self.langpairs = []     #language pairs found in html file
    self.tags_stack = []
    self.next_table_contains_translations = False
    self.inside_translation_table = False
    self.td_count = 0

  def unknown_starttag(self,tag,attrs):
        self.tags_stack.append(tag)
        #print "unknown : ", tag, " ", len(self.tags_stack)

  def start_table(self,attrs):
    if self.next_table_contains_translations == True:
      #print "next is translation table"
      self.inside_translation_table = True
      self.next_table_contains_translations = False
      self.td_count = 0
    self.tags_stack.append("table")
      
  def start_td(self,attrs):
    if self.inside_translation_table == True:
      #print "inside translation table (tdcount = ", self.td_count, ")"
      if self.td_count % 2 == 1:
        self.tags_stack.append("<!translation!>")
      else:
        self.tags_stack.append("td")
      self.td_count = self.td_count + 1
    else:
        self.tags_stack.append("td")
    
  def end_table(self):
    if self.inside_translation_table == True:
      self.inside_translation_table = False

  def handle_data(self,data):
    if len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] == "b":
      if data.startswith("Results:"):
        #print "-", data, "-","hi"
        self.next_table_contains_translations = True

    if len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] == "<!translation!>":
        #print "data: ", data
        Words = data.split(",")
        for w in Words:
          #print w.strip()
          self.words.append(w.strip())
  
  def unknown_endtag(self,tag):
    myParser.remove_not_closed_tags(self,tag)
    if len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] == tag:
        #print "end_tag : ", tag, " ", len(self.tags_stack)
        self.tags_stack.pop()

  #removes all the tags from the stack that have no closed tags (don't modify)
  def remove_not_closed_tags(self,tag):
    while len(self.tags_stack) > 0 and self.tags_stack[len(self.tags_stack)-1] != tag:
      self.tags_stack.pop()
