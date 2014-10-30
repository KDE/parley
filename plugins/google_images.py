#!/usr/bin/env kf5kross
# -*- coding: utf-8 -*-
import Parley
import socket
from PyQt4 import QtGui
from PyQt4 import QtCore
from PyQt4 import uic
# for localization (i18n)
import Kross
from PyKDE4.kdeui import KIcon
from PyKDE4.kdeui import KDialog
from PyKDE4.kdeui import KMessageBox

from os import path
from os import mkdir
import urllib2
import urllib
import simplejson

T = Kross.module("kdetranslation")

# TODO
# + Fetch more images (get the next results page)
# + Resize images before saving (to take less space)
# + comment out the code!
# + improve getIdentifier function (maybe by adding a new C++ function)
# + add "Alert" message box when no translation was selected


#GUI
uiFile = "google_images.ui"

#dialog class (loads google_images.ui)
class ImageDialog(KDialog):
  def __init__(self, translations):
    KDialog.__init__(self)

    self.w = None
    #look for the uiFile in plugin dirs and get the first occurence of it
    for plugindir in Parley.pluginDirs():
      if path.exists(plugindir+uiFile):
        (ImageWidget, widget) = uic.loadUiType(plugindir+uiFile)
        self.w = ImageWidget()
        self.w.setupUi(self.mainWidget())
        break

    if self.w is None:
        KMessageBox.error(None, T.i18n("Error: ui file not found.\nCheck your installation."), T.i18n("Fetch Image"))
        return

    self.translation = translations[0]

    #member variables
    self.word = self.translation.text
    self.locale = locale(getIdentifier(self.translation))
    
    self.w.nextButton.setIcon(KIcon("go-next"))
    self.w.previousButton.setIcon(KIcon("go-previous"))
    self.w.searchButton.setIcon(KIcon("edit-find"))

    # Connect signals
    self.connect(self.w.nextButton, QtCore.SIGNAL("clicked()"),self.nextImage)
    self.connect(self.w.previousButton, QtCore.SIGNAL("clicked()"),self.previousImage)
    self.connect(self.w.searchButton, QtCore.SIGNAL("clicked()"),self.searchBtnClicked)
    self.connect(self.w.freeImageCheckBox, QtCore.SIGNAL("clicked()"),self.searchBtnClicked)
    self.connect(self, QtCore.SIGNAL("okClicked()"),self.okBtnClicked)
    self.connect(self, QtCore.SIGNAL("cancelClicked()"),self.cancelBtnClicked)
    
    #set search text
    self.w.searchEdit.setText(self.word)
    
    #display image
    self.searchBtnClicked()

    
  def showImage(self,url):
    #pixmap = self.getPixmap("http://www.tuning-blog.net/wp-content/uploads/2007/05/audittclubsportquattro1.jpg")
    pixmap = self.getPixmap(url)
    if pixmap:
        p = pixmap.scaledToWidth(self.width()-10)
        if p.height() > self.height()-100:
            p = p.scaledToHeight(self.height()-100)
        self.w.imageLabel.setPixmap(p)
        return True
    else:
      return False


  #Returns a QPixmap from the given http url or None if url is invalid
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
    data = self.fetchData()
    self.images = getImageUrls(data)
    #display the first image (a small hack)
    self.img_index = -1
    self.nextImage()

  def okBtnClicked(self):
    filesdir = getFilesDir()
    if not path.exists(filesdir):
        mkdir(filesdir)
    url = self.images[self.img_index]
    [u,img_name] = url.rsplit("/",1)
    [name,ext] = img_name.rsplit(".",1)
    
    #get a unique file name
    if path.exists(filesdir+"/"+name+"."+ext):
        c = 1
        while path.exists(filesdir+"/"+name+"-"+str(c)+"."+ext):
            c += 1
        fn = filesdir+"/"+name+"-"+str(c)+"."+ext
    else:
        fn = filesdir+"/"+name+"."+ext
        
    print fn
      
    self.w.imageLabel.pixmap().save(fn)
    self.translation.imageUrl = fn
    self.close()

  def cancelBtnClicked(self):
    self.close()


  # fetches the json document for the given word and language pair
  def fetchData(self):
    timeout = 10.0
    socket.setdefaulttimeout(timeout)

    url = "https://ajax.googleapis.com/ajax/services/search/images"
    user_agent = 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008072820 Firefox/3.0.1'
    #params = [("gbv","2"),("hl",lang),("safe","active"),("q",word),("sa","2"),("btnG","Bilder-Suche")]
    params = {"v":"1.0", "start":"0", "q":self.w.searchEdit.text(), "hl":self.locale, "safe":"active"}
    if self.w.freeImageCheckBox.isChecked():
      params["as_rights"]="(cc_publicdomain|cc_attribute|cc_sharealike|cc_noncommercial|cc_nonderived)"

    headrs = { 'User-Agent' : user_agent }
    request_url = url + "?" + urllib.urlencode(params)


    req = urllib2.Request(url = request_url, headers=headrs)
    
    try:
      response = urllib2.urlopen(req)
      #print response.read()
      return response
    except:
      #in case of error not to return incompleted results
      print "error on fetching online data"
      return ""


#FUNCTIONS


#parses the response (json) and returns all the links to images
def getImageUrls(response):
  print "Parsing data"
  search_res = simplejson.load(response)
 # print data
  print(search_res)
  results = []
  for result in search_res['responseData']['results']:
        href = result['originalContextUrl']
        title = result['title']
        if not result['url']:
            continue
        results.append(result['url'])
  return results


#downloads image from the url given and returns the data (array of chars)
def downloadImage(url):
  img = urllib2.urlopen(url)
  imgdata = img.read()
  return imgdata

#get the identifier of the given translation (see Scripting::Translation)
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

#Get the string that describes the _files directory next to the Parley document
def getFilesDir():
    filepath = Parley.doc.url
    #print path.dirname(filepath)
    p,f = path.split(filepath)
    [name,ext] = f.rsplit(".",1)
    #print f.rsplit(".",1)
    filesdir = p+"/"+name+"_files"
    return filesdir


#ACTION FUNCTION

#method called by clicking "Fetch image" in the Scripts menu
def fetchImage():
  print "fetching image"
  translations = Parley.selectedTranslations()
  if len(translations) > 0:
    m = ImageDialog(translations)
    m.exec_()

  else:
    KMessageBox.error(None, T.i18n("No Selection:\nSelect a word to fetch an image for it"), T.i18n("Fetch Image"))


#SCRIPT MENU

#create a new action for the Scripts menu (action1)
action1 = Parley.newAction("example_action1", T.i18n("Fetch Image"))
action1.statusTip = T.i18n("Fetches an image for the selected translation")
Parley.connect(action1,"triggered()",fetchImage)

