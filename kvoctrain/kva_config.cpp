/***************************************************************************

    $Id$

                     configuration part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

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

#include "kvoctrain.h"
#include <kvoctraincore.h>

#include <qmessagebox.h>
#include <qfile.h>

#include <kconfig.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kdebug.h>

#include "common-dialogs/ProgressDlg.h"
#include "prefs.h"

void kvoctrainApp::saveOptions(bool all)
{
  KConfig *config = KApplication::kApplication()->config();

  config->setGroup(CFG_GENERAL);
  Prefs::setRecentFiles(recent_files);
  config->writeEntry(CFG_AUTOSAVEOPT, autosaveopts);
  config->writeEntry(CFG_ENTRYAUTOAPPLY, autoentryApply);

  config->setGroup(CFG_APPEARANCE);
  config->writeEntry(CFG_SHOW_TOOLBAR,toolBar()->isVisible());
  config->writeEntry(CFG_SHOW_STATUSBAR,statusBar()->isVisible());
  config->writeEntry(CFG_TOOLBAR_POS, (int)toolBar()->barPos());
  Prefs::setEnableInlineEdit(inline_edit);

  if (view) {
    kdDebug() << "Rows: " << view->getTable()->currentRow() << endl;
    Prefs::setCurrentRow(view->getTable()->currentRow());
    Prefs::setCurrentCol(view->getTable()->currentColumn());
    Prefs::writeConfig();
  }

  if (all || autosaveopts) {
    config->setGroup(CFG_QUERY_PRESETTING);
    config->writeEntry(CFG_QP_NUM, presettings.size() );
    QString s;
    for (int i = 0; i < (int) presettings.size(); i++) {
      s.setNum (i);
      config->writeEntry(QString(CFG_QP_NAME)+s, presettings[i].name);
      config->writeEntry(QString(CFG_QP_QUERY)+s, presettings[i].query_set);
      config->writeEntry(QString(CFG_QP_THRESH)+s, presettings[i].thresh_set);
      config->writeEntry(QString(CFG_QP_BLOCK)+s, presettings[i].block_set);
    }
  
    config->setGroup(CFG_APPEARANCE);
    config->writeEntry(CFG_IPA_FFAMILY, ipafont.family());
    config->writeEntry(CFG_IPA_FSIZE, ipafont.pointSize());
    config->writeEntry(CFG_FFAMILY, tablefont.family());
    config->writeEntry(CFG_FSIZE, tablefont.pointSize());
    Prefs::setUseGradeCol(gradecols.use);
    Prefs::setGradeCol0(gradecols.col0);
    Prefs::setGradeCol1(gradecols.col1);
    Prefs::setGradeCol2(gradecols.col2);
    Prefs::setGradeCol3(gradecols.col3);
    Prefs::setGradeCol4(gradecols.col4);
    Prefs::setGradeCol5(gradecols.col5);
    Prefs::setGradeCol6(gradecols.col6);
    Prefs::setGradeCol7(gradecols.col7);
  
    config->setGroup(CFG_GENERAL);
    Prefs::setSmartAppend((bool) smartAppend);
    config->writeEntry(CFG_HEADER_RESIZER, (int) header_resizer);

    s = separator;
    if (separator == "\t")
      s = TAB_REP;
    if (separator == "  ")
      s = SPC_REP;
    Prefs::setSeparator(s);
    config->writeEntry(CFG_PASTEORDER, paste_order);
    Prefs::setUseCurrent(useCurrent);
    Prefs::setEntriesPerLesson(entriesPerLesson);
    Prefs::setBackupTime(backupTime);
  
    config->writeEntry(CFG_NUMLANGSET, langset.size() );
    for (int i = 0; i < (int) langset.size(); i++) {
      s.setNum (i);
      s.insert (0, CFG_L_SHORT);
      config->writeEntry(s, langset.shortId(i));

      s.setNum (i);
      s.insert (0, CFG_L_SHORT2);
      config->writeEntry(s, langset.shortId2(i));

      s.setNum (i);
      s.insert (0, CFG_L_LANG);
      config->writeEntry(s, langset.longId(i));

      s.setNum (i);
      s.insert (0, CFG_L_PIXMAP);
      config->writeEntry(s, langset.PixMapFile(i));
    }

    config->setGroup(CFG_QUERYPROP);
    Prefs::setMaxTimePer(maxqueryTime);
    Prefs::setShowcounter(showcounter);
    Prefs::setSwapDir(swap_querydir);
    Prefs::setAltLearn(alt_learn);
    Prefs::setBlock(block);
    Prefs::setExpire(expire);
    Prefs::setQueryTimeout(type_querytimeout);
    Prefs::writeConfig();
   
    config->setGroup(CFG_QUERYMANAG);
    querymanager.saveConfig (config);
  }
}


void kvoctrainApp::readOptions()
{
  ///////////////////////////////////////////////////////////////////
  // read the config file entries
  KConfig *config = KApplication::kApplication()->config();

  config->setGroup(CFG_QUERY_PRESETTING);
  int ls = config->readNumEntry(CFG_QP_NUM, 0);
  QString num;

  for (int i = 0; i < ls; i++) {
    num.setNum(i);
    QString name = config->readEntry(QString(CFG_QP_NAME)+num);
    QString query = config->readEntry(QString(CFG_QP_QUERY)+num);
    QString thresh = config->readEntry(QString(CFG_QP_THRESH)+num);
    QString blockset = config->readEntry(QString(CFG_QP_BLOCK)+num);
    presettings.push_back(PreSetting(name, query, thresh, blockset));
  }

  config->setGroup(CFG_APPEARANCE);
  bViewToolbar = config->readBoolEntry(CFG_SHOW_TOOLBAR, true);
  bViewStatusbar = config->readBoolEntry(CFG_SHOW_STATUSBAR, true);
  inline_edit = Prefs::enableInlineEdit();;
  tool_bar_pos = (KToolBar::BarPosition)config->readNumEntry(CFG_TOOLBAR_POS, KToolBar::Top);

  QFont fdefault;
  QString family = config->readEntry(CFG_FFAMILY, fdefault.family());
  int size = config->readNumEntry(CFG_FSIZE, 12);
  tablefont.setPointSize(size);
  tablefont.setFamily(family);
  tablefont.setWeight(QFont::Normal);

  family = config->readEntry(CFG_IPA_FFAMILY, fdefault.family());
  size = config->readNumEntry(CFG_IPA_FSIZE, 14);
  ipafont.setPointSize(size);
  ipafont.setFamily(family);
  ipafont.setWeight(QFont::Normal);
  if (pron_label != 0)
    pron_label->setFont(ipafont);

  gradecols.use = Prefs::useGradeCol();
  gradecols.col0 = Prefs::gradeCol0();
  gradecols.col1 = Prefs::gradeCol1();
  gradecols.col2 = Prefs::gradeCol2();
  gradecols.col3 = Prefs::gradeCol3();  
  gradecols.col4 = Prefs::gradeCol4();
  gradecols.col5 = Prefs::gradeCol5();
  gradecols.col6 = Prefs::gradeCol6();
  gradecols.col7 = Prefs::gradeCol7();

  config->setGroup(CFG_GENERAL);
  smartAppend = Prefs::smartAppend();
  header_resizer = (kvoctrainView::Resizer) config->readNumEntry(CFG_HEADER_RESIZER, (int) kvoctrainView::Automatic);
  // initialize the recent file list
  recent_files = Prefs::recentFiles();//config->readPathListEntry(CFG_RECENT);
  autosaveopts = config->readBoolEntry(CFG_AUTOSAVEOPT, true);
  autoentryApply = config->readBoolEntry(CFG_ENTRYAUTOAPPLY, false);

  paste_order = config->readListEntry(CFG_PASTEORDER);
  useCurrent = Prefs::useCurrent();
  entriesPerLesson =Prefs::entriesPerLesson();
  separator = Prefs::separator();
  if (separator == TAB_REP)
    separator = "\t";
  else if (separator == SPC_REP)
    separator = "  ";
  backupTime = Prefs::backupTime();

  ls = config->readNumEntry(CFG_NUMLANGSET, 0);
  QString s;
  for (int i = 0; i < ls; i++) {
    s.setNum (i);
    s.insert (0, CFG_L_SHORT);
    QString shortId = config->readEntry(s);
    if (shortId.stripWhiteSpace().length() == 0) {
      shortId.setNum (i);
      shortId.insert (0, "id");
    }

    s.setNum (i);
    s.insert (0, CFG_L_SHORT2);
    QString shortId2 = config->readEntry(s);

    s.setNum (i);
    s.insert (0, CFG_L_LANG);
    QString longId = config->readEntry(s);
    if (longId.stripWhiteSpace().length() == 0) {
      longId.setNum (i);
      longId.insert (0, "ident");
    }

    s.setNum (i);
    s.insert (0, CFG_L_PIXMAP);
    QString PixMapFile = config->readEntry(s);

    langset.addSet (shortId, shortId2, longId, PixMapFile);
  }

  config->setGroup(CFG_QUERYPROP);
  type_querytimeout = (kvq_timeout_t) Prefs::queryTimeout();
  maxqueryTime = Prefs::maxTimePer();
  showcounter = Prefs::showcounter();
  swap_querydir= Prefs::swapDir();
  alt_learn= Prefs::altLearn();
  block = Prefs::block();
  expire = Prefs::expire();
  config->setGroup(CFG_QUERYMANAG);
  querymanager.loadConfig (config);
}


void kvoctrainApp::saveProperties(KConfig *config )
{
  saveOptions(false);
  if (doc) {
    config->writeEntry(CFG_FILENAME,doc->getFileName());
    config->writeEntry(CFG_TITLE,doc->getTitle());
    config->writeEntry(CFG_MODIFIED,doc->isModified());

    config->writeEntry(CFG_QUERYMODE, querymode);

    QString filename=doc->getFileName();
    QString tempname = kapp->tempSaveName(filename);
    saveDocProps(doc);
    doc->saveAs(this, tempname, doc->getTitle(), kvoctrainDoc::automatic, separator, &paste_order);
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
      pdlg = new ProgressDlg (QString(), QString(),
                              kapp->makeStdCaption(""));
      pdlg->show();
      doc = new kvoctrainDoc (this, tempname, separator, &paste_order);
      removeProgressBar();
      doc->setModified();
      doc->setTitle(title);
      doc->setFileName(filename);
      setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
      QFile::remove(tempname);
    }
  }
  else if(!filename.isEmpty()){
    pdlg = new ProgressDlg (QString(), QString(),
                            kapp->makeStdCaption(""));
    pdlg->show();
    doc = new kvoctrainDoc (this, filename, separator, &paste_order);
    removeProgressBar();
    setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
  }

  show();
  kapp->setTopWidget(this);
  kapp->setMainWidget( this );
}		

