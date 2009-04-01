#!/usr/bin/env kross
# -*- coding: utf-8 -*-
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
# Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
# Copyright 2009 Thomas Eitzenberger <eth@gmx.at>
# Parts of this file are based on the google_images.py script by Avgoustinos Kadis

import Parley
import mwclient
import sys
import os
from os import path
import string

# Connected to the action menu
def fetchSound():
    print "fetching sound"
    tr = Parley.selectedTranslations()
    for word in tr:
        fetchSoundForTranslation(word)
    else:
        print "No translation selected"

def fetchSoundForTranslation(word):
    locale = getLocale(word)
    wikiLocale = getWikiLocale(locale)
    print 'Language:', wikiLocale, ' Word: ', word.text
    soundfile = getWikiObject(wikiLocale, word.text)
    if soundfile:
        url = downloadFromWiki(soundfile)
        word.soundUrl = url
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
def getWikiLocale(lang):
    if (string.lower(lang[:2]) == 'en'):
        lang = 'En-us'
    lang.replace('_', '-')
    return lang

def getWikiObject(lang, word):
    commons = mwclient.Site('commons.wikimedia.org')
    # TODO: figure out if some utf-8 magic is needed here
    query = lang + '-' + word + '.ogg'
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
    remote = wikiObject.download()
    saveas.write(remote.read())
    saveas.close()
    return filename

# directory the files are saved to (subdirectory of the document)
def getTargetDirectory():
    filepath = Parley.doc.url
    p,f = path.split(filepath)
    [name,ext] = f.rsplit(".", 1)
    filesdir = p + "/" + name + "_files/"
    return filesdir

#create a new action in Parley's script menu
action = Parley.newAction("fetch_sound","Fetch Sound")
action.statusTip="Fetches a sound file from commons.wikimedia.org for the selected word"
Parley.connect(action,"triggered()",fetchSound)
