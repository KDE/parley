/***************************************************************************

                     configuration part of kvoctrain

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004, 2005 Peter Hedlund <peter@peterandlinda.com>

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

#include <qfile.h>

#include "kvoctrain.h"
#include <kvoctraincore.h>
#include "common-dialogs/ProgressDlg.h"
#include "prefs.h"
#include "languagesettings.h"
#include "presettings.h"

void kvoctrainApp::saveOptions()
{
  KConfig *config = KApplication::kApplication()->config();
  fileOpenRecent->saveEntries(config, "Recent Files");

  if (view)
  {
    Prefs::setCurrentRow(view->getTable()->currentRow());
    Prefs::setCurrentCol(view->getTable()->currentColumn());
  }
  /*
  Prefs::setNumPreSetting(presettings.size());
  for (int i = 0 ; i < (int) presettings.size(); i++)
  {
    PreSettings preSettings(QString::number(i));
    preSettings.setName(presettings[i].name);
    preSettings.setQuery(presettings[i].query_set);
    preSettings.setThreshold(presettings[i].thresh_set);
    preSettings.setBlocking(presettings[i].block_set);
    preSettings.writeConfig();
  }
  */
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
  KConfig *config = KApplication::kApplication()->config();
  fileOpenRecent->loadEntries(config, "Recent Files");
  /*
  int ls = Prefs::numPreSetting();
  for (int i = 0 ; i < ls; i++)
  {
    PreSettings preSettings(QString::number(i));
    preSettings.readConfig();
    presettings.push_back(PreSetting(preSettings.name(), preSettings.query(),
      preSettings.threshold(), preSettings.blocking()));
  }
  */
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
    if (shortId.stripWhiteSpace().length() == 0) {
      shortId.setNum (i);
      shortId.insert (0, "id");
    }

    QString longId = languageSettings.longId();
    if (longId.stripWhiteSpace().length() == 0) {
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
    config->writeEntry(CFG_FILENAME,doc->URL().path());
    config->writeEntry(CFG_TITLE,doc->getTitle());
    config->writeEntry(CFG_MODIFIED,doc->isModified());

    config->writeEntry(CFG_QUERYMODE, querymode);

    QString filename=doc->URL().path();
    QString tempname = kapp->tempSaveName(filename);
    saveDocProps(doc);
    doc->saveAs(this, KURL(tempname), doc->getTitle(), kvoctrainDoc::automatic);
  }
}


void kvoctrainApp::readProperties(KConfig *config)
{
  querymode = config->readBoolEntry(CFG_QUERYMODE, 0);

  QString filename = config->readEntry(CFG_FILENAME);
  QString title = config->readEntry(CFG_TITLE);
  bool modified = config->readBoolEntry(CFG_MODIFIED,false);
  if( modified ){
    bool b_canRecover;
    QString tempname = kapp->checkRecoverFile(filename,b_canRecover);

    if(b_canRecover){
      pdlg = new ProgressDlg(QString(), QString(), kapp->makeStdCaption(""));
      pdlg->show();
      doc = new kvoctrainDoc(this, KURL(tempname));
      removeProgressBar();
      doc->setModified();
      doc->setTitle(title);
      doc->setURL(KURL(filename));
      setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
      QFile::remove(tempname);
    }
  }
  else if(!filename.isEmpty()){
    pdlg = new ProgressDlg (QString(), QString(),
                            kapp->makeStdCaption(""));
    pdlg->show();
    doc = new kvoctrainDoc(this, KURL(filename));
    removeProgressBar();
    setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
  }

  show();
  kapp->setTopWidget(this);
  kapp->setMainWidget( this );
}

