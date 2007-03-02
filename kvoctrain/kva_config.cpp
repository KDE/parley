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

#include <kapplication.h>
#include <krecentfilesaction.h>
#include <kglobal.h>

#include "kvoctrain.h"
#include "common-dialogs/ProgressDlg.h"
#include "prefs.h"
#include "languagesettings.h"
#include "presettings.h"

void KVocTrainApp::saveOptions()
{
  fileOpenRecent->saveEntries(KGlobal::config()->group( "Recent Files") );

  if (m_tableView)
  {
    Prefs::setCurrentRow(m_tableView->currentIndex().row());
    Prefs::setCurrentCol(m_tableView->currentIndex().column());
  }
  saveLanguages();
  Prefs::writeConfig();
}

void KVocTrainApp::saveLanguages()
{
  Prefs::setNumLangSet(m_languages.count());
  for (int i = 0 ; i < m_languages.count(); i++)
  {
    LanguageSettings languageSettings(QString::number(i));
    languageSettings.setShortId(m_languages.shortId(i));
    languageSettings.setShort2Id(m_languages.shortId2(i));
    languageSettings.setLongId(m_languages.longId(i));
    languageSettings.setPixmapFile(m_languages.pixmapFile(i));
    languageSettings.setKeyboardLayout(m_languages.keyboardLayout(i));
    languageSettings.writeConfig();
  }
}

void KVocTrainApp::readOptions()
{
  fileOpenRecent->loadEntries(KGlobal::config()->group( "Recent Files") );
  readLanguages();
}

void KVocTrainApp::readLanguages()
{
  m_languages.clear();
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

    m_languages.addLanguage(shortId, longId, languageSettings.pixmapFile(), languageSettings.short2Id(),    languageSettings.keyboardLayout());
  }
}
void KVocTrainApp::saveProperties(KConfigGroup &config )
{
  saveOptions();
  if (m_doc) {
    config.writeEntry("Filename", m_doc->URL().path());
    config.writeEntry("Title", m_doc->title());
    config.writeEntry("Modified", m_doc->isModified());

    config.writeEntry("QueryMode", querymode);

    QString filename=m_doc->URL().path();
    QString tempname = kapp->tempSaveName(filename);
    saveDocProps(m_doc);
    m_doc->saveAs(this, KUrl(tempname), KEduVocDocument::automatic, "KVocTrain");
  }
}


void KVocTrainApp::readProperties(const KConfigGroup &config)
{
  querymode = config.readEntry("QueryMode", 0);

  QString filename = config.readEntry("Filename");
  QString title = config.readEntry("Title");
  bool modified = config.readEntry("Modified", false);
  if (modified){
    bool b_canRecover;
    QString tempname = kapp->checkRecoverFile(filename,b_canRecover);

    if (b_canRecover){
      pdlg = new ProgressDlg(QString(), QString(), QString());
      pdlg->show();
      m_doc = new KEduVocDocument(this);
      m_doc->setURL(KUrl(tempname));
      removeProgressBar();
      m_doc->setModified();
      m_doc->setTitle(title);
      m_doc->setURL(KUrl(filename));
      setCaption(m_doc->title(), m_doc->isModified());
      QFile::remove(tempname);
    }
  }
  else if (!filename.isEmpty()){
    pdlg = new ProgressDlg (QString(), QString(), "");
    pdlg->show();
    m_doc = new KEduVocDocument(this);
    m_doc->setURL(KUrl(filename));
    removeProgressBar();
    setCaption(m_doc->title(), m_doc->isModified());
  }

  show();
}

