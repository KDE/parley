#!/usr/bin/env kross
# -*- coding: utf-8 -*-
#
#  SPDX-License-Identifier: GPL-2.0-or-later
#
# SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
# SPDX-FileCopyrightText: 2009 Thomas Eitzenberger <eth@gmx.at>
# Parts of this file are based on the google_images.py script by Avgoustinos Kadis

import Parley
import mwclient
import sys
import os
from os import path
import string
import Kross
import urllib
from urlparse import urlparse

T = Kross.module("kdetranslation")

# fetch http_content
def fetch_http_content(url, params, method):
    params = urllib.urlencode(params)
    if method=='POST':
        f = urllib.urlopen(url, params)
    else:
        f = urllib.urlopen(url+'?'+params)
    return (f.read(), f.code)

# Connected to the action menu
def fetchSound():
    print "fetching sound"
    tr = Parley.selectedTranslations()
    for word in tr:
        print "checking sound for " + word.text
        fetchSoundForTranslation(word)

def fetchSoundForTranslation(word):
    locale = getLocale(word)
    wikiLocales = getWikiLocales(locale)
    bFound = 0
    for wikiLocale in wikiLocales:
        if (bFound == 0):
            print 'Language:', wikiLocale, ' Word: ', word.text
            soundfile = getWikiObject(wikiLocale, word.text, None)
            # check for specific pronunciation for nouns
            if soundfile == None:
                soundfile = getWikiObject(wikiLocale, word.text, 'noun')
            # check for specific pronunciation for verbs
            if soundfile == None:
                soundfile = getWikiObject(wikiLocale, word.text, 'verb')
            if soundfile:
                url = downloadFromWiki(soundfile)
                word.soundUrl = url
                bFound = 1
                print "setting url for word " + word.text + ":" + word.soundUrl
            else:
                print "No translation found for: ", word.text

# locale of the given translation
def getLocale(trans):
    sel_entries = Parley.selectedEntries()
    for entry in sel_entries:
        for i in entry.translationIndices():
            if entry.translation(i).text == trans.text:
                return Parley.doc.identifier(i).locale

# go from iso codes to wiktionary
def getWikiLocales(lang):
    langs = []
    if (string.lower(lang[:2]) == 'en'):
        langs.append('En-us')
        langs.append('En-uk')
        langs.append('En-ca')
    elif (string.lower(lang[:2]) == 'de'):
        langs.append('De')
        langs.append('De-at')
    elif (string.lower(lang[:2]) == 'fr'):
        langs.append('Fr')
    else:
        langs.append(lang.capitalize())
    return langs

def getWikiObject(lang, word, appendix):
    commons = mwclient.Site('commons.wikimedia.org')
    # TODO: figure out if some utf-8 magic is needed here
    query = lang + '-' + word
    if (appendix != None):
        query += '-' + appendix

    query += '.ogg'
    soundfile = commons.Images[query]
    print 'Query: ', query, 'File: ', soundfile.name.encode('utf-8'), ' Exists: ', soundfile.exists
    if soundfile.exists:
        return soundfile
    return None

def downloadFromWiki(wikiObject):
    filesdir = getTargetDirectory()
    if not path.exists(filesdir):
        os.mkdir(filesdir)
    filename = filesdir + wikiObject.name.encode('utf-8')
    print filename
    saveas = open(filename,'w')
#   remote = wikiObject.download()
#   workaround: because download method results with http response code 404
    content, response_code = fetch_http_content(
                              wikiObject.imageinfo['url'],
                              {},
                              'GET'
                         )
    print 'HTTP Response code: ', response_code
    if(response_code == 200):
	    saveas.write(content)
    saveas.close()
    return filename

# directory the files are saved to (subdirectory of the document)
def getTargetDirectory():
    filepath = Parley.doc.url
    if filepath.startswith('file://'):
        url = urlparse(filepath)
        filepath = path.abspath(path.join(url.netloc, url.path))
    p,f = path.split(filepath)
    [name,ext] = f.rsplit(".", 1)
    filesdir = p + "/" + name + "_files/"
    return filesdir

#create a new action in Parley's script menu
action = Parley.newAction("fetch_sound",T.i18n("Fetch Sound"))
action.statusTip=T.i18n("Fetches a sound file from commons.wikimedia.org for the selected word")
Parley.connect(action,"triggered()",fetchSound)
