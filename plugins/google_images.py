#!/usr/bin/env kross
import Parley
import socket
from PyQt4 import QtGui
from PyQt4 import QtCore
from PyQt4 import uic
from os import path
import urllib2
import urllib
from sgmllib import SGMLParser

#GUI
uiFile = "google_images.ui"


#look for the uiFile in plugin dirs and get the first occurence of it
for plugindir in Parley.pluginDirs():
  if path.exists(plugindir+uiFile):
    (MyWidget, baseClass) = uic.loadUiType(plugindir+uiFile)
    break

class MyDialog(QtGui.QDialog, MyWidget):
  def __init__(self,images,word,locale):
    QtGui.QDialog.__init__(self)

    self.images = images
    self.word = word
    self.locale = locale
    self.img_index = 0

    # Set up the user interface from Designer.
    self.setupUi(self)

    print "connecting"
    
    #self.connect(self.pushButton, QtCore.SIGNAL("clicked()"),self.edittext)
    self.connect(self.nextButton, QtCore.SIGNAL("clicked()"),self.nextImage)
    self.connect(self.previousButton, QtCore.SIGNAL("clicked()"),self.previousImage)
    self.connect(self.searchButton, QtCore.SIGNAL("clicked()"),self.searchBtnClicked)
    self.connect(self.okButton, QtCore.SIGNAL("clicked()"),self.okBtnClicked)
    self.connect(self.cancelButton, QtCore.SIGNAL("clicked()"),self.cancelBtnClicked)
    
    self.searchEdit.setText(self.word)
    
    self.showImage(self.images[self.img_index])
    
  def showImage(self,url):
    #pixmap = self.getPixmap("http://www.tuning-blog.net/wp-content/uploads/2007/05/audittclubsportquattro1.jpg")
    pixmap = self.getPixmap(url)
    if pixmap:
        p = pixmap.scaledToWidth(self.width()-10)
        if p.height() > self.height()-100:
            p = p.scaledToHeight(self.height()-100)
        self.imageLabel.setPixmap(p)
        return True
    else:
      return False


  def getPixmap(self,url):
    pixmap = QtGui.QPixmap()
    
    try:
      img = urllib2.urlopen(url)
      imgdata = img.read()
      pixmap.loadFromData(imgdata);
      return pixmap

    except:
      print "Image not found"
      return None
    

  def nextImage(self):
    self.img_index += 1
    if self.img_index < len(self.images):
      print self.images[self.img_index]
      if not self.showImage(self.images[self.img_index]):
        self.nextImage()

  def previousImage(self):
    self.img_index -= 1
    if self.img_index >= 0:
      print self.images[self.img_index]
      if not self.showImage(self.images[self.img_index]):
        self.previousImage()

  def searchBtnClicked(self):
    data = fetchData(self.searchEdit.text(),self.locale)
    self.images = getImageUrls(data)
    self.img_index = -1
    self.nextImage()

  def okBtnClicked(self):
    filesdir = getFilesDir()
    if not path.exists(filesdir):
        os.mkdir(filesdir)
    url = self.images[self.img_index]
    [u,img_name] = url.rsplit("/",1)
    [name,ext] = img_name.rsplit(".",1)
    
    if path.exists(filesdir+"/"+name+"."+ext):
        c = 1
        while path.exists(filesdir+"/"+name+"-"+str(c)+"."+ext):
            c += 1
        fn = filesdir+"/"+name+"-"+str(c)+"."+ext
    else:
        fn = filesdir+"/"+name+"."+ext
        
    print fn
      
    self.imageLabel.pixmap().save(fn)
    self.translation.imageUrl = fn
    self.close()

  def cancelBtnClicked(self):
    self.close()


#URL : http://images.google.com/images?hl=en&safe=active&q=pies


#FUNCTIONS

# timeout of search (important for slow connections, not to freeze Parley by waiting for a result)

  #params = [("gbv","2"),("hl",lang),("safe","active"),("q",word),("sa","2"),("btnG","Bilder-Suche")]

# fetches the html document for the given word and language pair
def fetchData(word,lang):
  timeout = 10.0
  socket.setdefaulttimeout(timeout)

  url = "http://images.google.com/images"
  user_agent = 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008072820 Firefox/3.0.1'
  params = {'q':word, "hl":lang, "safe":"active"}
  headrs = { 'User-Agent' : user_agent }
  request_url = url + "?" + urllib.urlencode(params)
  req = urllib2.Request(url = request_url, headers=headrs)
  
  try:
    response = urllib2.urlopen(req)
    #print response.read()
    return response.read()
  except:
    #in case of error not to return incompleted results
    print "error on fetching online data"
    return ""

def getImageUrls(data):
  print "Parsing data"
  imageurls = []
  imageextensions=[".jpg",".png",".bmp",".jif"]
  print data
  for d in data.split('"'):
    if d.startswith('http://'):
      
      if (d[-4:]).lower() in imageextensions:
        print d
        imageurls.append(d)
  return imageurls
  

def downloadImage(url):
  img = urllib2.urlopen(url)
  imgdata = img.read()
  return imgdata
        
def getIdentifier(trans):
  sel_entries = Parley.selectedEntries()
  for entry in sel_entries:
    #print "entry",entry
    for i in entry.translationIndices():
      #print "index",i
      if entry.translation(i).text == trans.text:
        return Parley.doc.identifier(i).locale

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


def fetchImage():
  print "fetching image"
  tr = Parley.selectedTranslations()
  if len(tr) > 0:
    #print getIdentifier(tr[0])
    data = fetchData(tr[0].text,locale(getIdentifier(tr[0])))
    images = getImageUrls(data)
    m = MyDialog(images,tr[0].text,locale(getIdentifier(tr[0])))
    #print m.images
    m.translation = tr[0]
    m.exec_()

  else:
    print "No translation selected"


def getFilesDir():
    filepath = Parley.doc.url
    #print path.dirname(filepath)
    p,f = path.split(filepath)
    [name,ext] = f.rsplit(".",1)
    #print f.rsplit(".",1)
    filesdir = p+"/"+name+"_files"
    return filesdir

  
#class myParser(SGMLParser):
  #def reset(self):
    #SGMLParser.reset(self)
    #self.images = []
    
  #def start_img(self,attrs):
    #print "Image:",attrs

#SCRIPT MENU

#create a new action for the Scripts menu (action1)
action1 = Parley.newAction("example_action1","Fetch Image")
action1.statusTip="Fetches an image for the selected translation"
Parley.connect(action1,"triggered()",fetchImage)

