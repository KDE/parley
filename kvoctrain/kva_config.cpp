/***************************************************************************

                     configuration part of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2004-2006 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QFile>

#include "kvoctrain.h"
#include "common-dialogs/ProgressDlg.h"
#include "prefs.h"
#include "languagesettings.h"
#include "presettings.h"

void kvoctrainApp::saveOptions()
{
  fileOpenRecent->saveEntries(KGlobal::config(), "Recent Files");

  if (view)
  {
    Prefs::setCurrentRow(view->getTable()->currentRow());
    Prefs::setCurrentCol(view->getTable()->currentColumn());
  }
  saveLanguages();
  Prefs::writeConfig();
}

void kvoctrainApp::saveLanguages()
{
  Prefs::setNumLangSet(langset.size());
  for (int i = 0 ; i < (int) langset.size(); i++)
  {
    LanguageSettings languageSettings(QString::number(i));
    languageSettings.setShortId(langset.shortId(i));
    languageSettings.setShort2Id(langset.shortId2(i));
    languageSettings.setLongId(langset.longId(i));
    languageSettings.setPixmapFile(langset.PixMapFile(i));
    languageSettings.setKeyboardLayout(langset.keyboardLayout(i));
    languageSettings.writeConfig();
  }
}

void kvoctrainApp::readOptions()
{
  fileOpenRecent->loadEntries(KGlobal::config(), "Recent Files");
  readLanguages();
}

void kvoctrainApp::readLanguages()
{
  langset.clear();
  int ls = Prefs::numLangSet();
  for (int i = 0 ; i < ls; i++)
  {
    LanguageSettings languageSettings(QString::number(i));
    languageSettings.readConfig();

    QString shortId = languageSettings.shortId();
    if (shortId.simplified().length() == 0) {
      shortId.setNum (i);
      shortId.insert (0, "id");
    }

    QString longId = languageSettings.longId();
    if (longId.simplified().length() == 0) {
      longId.setNum (i);
      longId.insert (0, "ident");
    }

    langset.addSet (shortId, longId, languageSettings.pixmapFile(), languageSettings.short2Id(),
      languageSettings.keyboardLayout());
  }
}
void kvoctrainApp::saveProperties(KConfig *config )
{
  saveOptions();
  if (doc) {
    config->writeEntry("Filename", doc->URL().path());
    config->writeEntry("Title", doc->title());
    config->writeEntry("Modified", doc->isModified());

    config->writeEntry("QueryMode", querymode);

    QString filename=doc->URL().path();
    QString tempname = kapp->tempSaveName(filename);
    saveDocProps(doc);
    doc->saveAs(this, KURL(tempname), KEduVocDocument::automatic, "KVocTrain");
  }
}


void kvoctrainApp::readProperties(KConfig *config)
{
  querymode = config->readEntry("QueryMode", 0);

  QString filename = config->readEntry("Filename");
  QString title = config->readEntry("Title");
  bool modified = config->readEntry("Modified", false);
  if( modified ){
    bool b_canRecover;
    QString tempname = kapp->checkRecoverFile(filename,b_canRecover);

    if(b_canRecover){
      pdlg = new ProgressDlg(QString(), QString(), kapp->makeStdCaption(""));
      pdlg->show();
      doc = new KEduVocDocument(this);
      doc->setURL(KURL(tempname));
      removeProgressBar();
      doc->setModified();
      doc->setTitle(title);
      doc->setURL(KURL(filename));
      setCaption(kapp->makeStdCaption(doc->title(), false, doc->isModified()));
      QFile::remove(tempname);
    }
  }
  else if(!filename.isEmpty()){
    pdlg = new ProgressDlg (QString(), QString(),
                            kapp->makeStdCaption(""));
    pdlg->show();
    doc = new KEduVocDocument(this);
    doc->setURL(KURL(filename));
    removeProgressBar();
    setCaption(kapp->makeStdCaption(doc->title(), false, doc->isModified()));
  }

  show();
  kapp->setTopWidget(this);
  kapp->setMainWidget( this );
}

