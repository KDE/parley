/***************************************************************************

    $Id$

                     configuration part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.14  2002/01/04 17:28:51  arnold
    fixed resuming a query

    Revision 1.13  2001/12/13 18:39:09  arnold
    added phonetic alphabet stuff

    Revision 1.12  2001/11/25 11:11:02  arnold
    switch for inline edit, splitted kv_resource.h

    Revision 1.11  2001/11/11 12:51:45  arnold
    fixed some strings for i18n purposes

    Revision 1.10  2001/11/10 22:27:08  arnold
    removed compatibility for kde1

    Revision 1.9  2001/11/10 21:11:57  arnold
    removed icons and compatibilty for kde1

    Revision 1.8  2001/11/09 15:52:00  arnold
    fixed error messages for correct i18n use

    Revision 1.7  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.6  2001/11/02 10:17:48  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.5  2001/10/21 16:22:07  arnold
    removed all the 'charset' stuff

    Revision 1.4  2001/10/21 15:28:43  arnold
    removed all the 'charset' stuff

    Revision 1.3  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:36:34  arnold
    import of version 0.7.0pre8 to kde-edu


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

#include "common-dialogs/ProgressDlg.h"

void kvoctrainApp::saveOptions(bool all)
{
  KConfig *config = KApplication::kApplication()->config();

  config->setGroup(CFG_GENERAL);
  config->writeEntry(CFG_RECENT, recent_files);
  config->writeEntry(CFG_AUTOSAVEOPT, autosaveopts);
  config->writeEntry(CFG_ENTRYAUTOAPPLY, autoentryApply);

  config->setGroup(CFG_APPEARANCE);
  config->writeEntry(CFG_SHOW_TOOLBAR,toolBar()->isVisible());
  config->writeEntry(CFG_SHOW_STATUSBAR,statusBar()->isVisible());
  config->writeEntry(CFG_TOOLBAR_POS, (int)toolBar()->barPos());
  config->writeEntry(CFG_INLINE_EDIT, inline_edit);

  if (view) {
    config->setGroup(CFG_GENERAL);
    config->writeEntry(CFG_CUR_ROW, view->getTable()->currentRow());
    config->writeEntry(CFG_CUR_COL, view->getTable()->currentColumn());
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
    config->writeEntry(CFG_GC_USE, gradecols.use);
    config->writeEntry(CFG_GCOL0, gradecols.col0);
    config->writeEntry(CFG_GCOL1, gradecols.col1);
    config->writeEntry(CFG_GCOL2, gradecols.col2);
    config->writeEntry(CFG_GCOL3, gradecols.col3);
    config->writeEntry(CFG_GCOL4, gradecols.col4);
    config->writeEntry(CFG_GCOL5, gradecols.col5);
    config->writeEntry(CFG_GCOL6, gradecols.col6);
    config->writeEntry(CFG_GCOL7, gradecols.col7);
  
    config->setGroup(CFG_GENERAL);
    config->writeEntry(CFG_SMART_APPEND, (bool) smartAppend);
    config->writeEntry(CFG_HEADER_RESIZER, (int) header_resizer);

    s = separator;
    if (separator == "\t")
      s = TAB_REP;
    if (separator == "  ")
      s = SPC_REP;
    config->writeEntry(CFG_SEPARATOR, s);
    config->writeEntry(CFG_PASTEORDER, paste_order);
    config->writeEntry(CFG_USECURRENT, useCurrent);
    config->writeEntry(CFG_ENTRIESLESSON, entriesPerLesson);
    config->writeEntry(CFG_BACKTIME, backupTime);
  
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
    config->writeEntry(CFG_MAXTIME_PER, maxqueryTime);
    config->writeEntry(CFG_SHOWCOUNTER, showcounter);
    config->writeEntry(CFG_SWAP_DIR, swap_querydir);
    config->writeEntry(CFG_BLOCK, block);
    config->writeEntry(CFG_EXPIRE, expire);
    config->writeEntry(CFG_QUERYTIMEOUT, type_querytimeout);
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
    QString name = config->readEntry(QString(CFG_QP_NAME)+num, "");
    QString query = config->readEntry(QString(CFG_QP_QUERY)+num, "");
    QString thresh = config->readEntry(QString(CFG_QP_THRESH)+num, "");
    QString blockset = config->readEntry(QString(CFG_QP_BLOCK)+num, "");
    presettings.push_back(PreSetting(name, query, thresh, blockset));
  }

  config->setGroup(CFG_APPEARANCE);
  bViewToolbar = config->readBoolEntry(CFG_SHOW_TOOLBAR, true);
  bViewStatusbar = config->readBoolEntry(CFG_SHOW_STATUSBAR, true);
  inline_edit = config->readBoolEntry(CFG_INLINE_EDIT, false);
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

  gradecols.use = config->readBoolEntry(CFG_GC_USE, true);
  QColor qc = KV_NORM_COLOR;
  gradecols.col0 = config->readColorEntry(CFG_GCOL0, &qc);
  qc = KV_LEV1_COLOR;
  gradecols.col1 = config->readColorEntry(CFG_GCOL1, &qc);
  qc = KV_LEV2_COLOR;
  gradecols.col2 = config->readColorEntry(CFG_GCOL2, &qc);
  qc = KV_LEV3_COLOR;
  gradecols.col3 = config->readColorEntry(CFG_GCOL3, &qc);
  qc = KV_LEV4_COLOR;
  gradecols.col4 = config->readColorEntry(CFG_GCOL4, &qc);
  qc = KV_LEV5_COLOR;
  gradecols.col5 = config->readColorEntry(CFG_GCOL5, &qc);
  qc = KV_LEV6_COLOR;
  gradecols.col6 = config->readColorEntry(CFG_GCOL6, &qc);
  qc = KV_LEV7_COLOR;
  gradecols.col7 = config->readColorEntry(CFG_GCOL7, &qc);

  config->setGroup(CFG_GENERAL);
  smartAppend = config->readNumEntry(CFG_SMART_APPEND, 0);
  header_resizer = (kvoctrainView::Resizer) config->readNumEntry(CFG_HEADER_RESIZER, (int) kvoctrainView::Automatic);
  // initialize the recent file list
  recent_files = config->readListEntry(CFG_RECENT);
  autosaveopts = config->readBoolEntry(CFG_AUTOSAVEOPT, true);
  autoentryApply = config->readBoolEntry(CFG_ENTRYAUTOAPPLY, false);

  paste_order = config->readListEntry(CFG_PASTEORDER);
  useCurrent = config->readBoolEntry(CFG_USECURRENT, true);
  entriesPerLesson = config->readNumEntry(CFG_ENTRIESLESSON, 50);
  separator = config->readEntry(CFG_SEPARATOR, "\t");
  if (separator == TAB_REP)
    separator = "\t";
  else if (separator == SPC_REP)
    separator = "  ";
  backupTime = config->readNumEntry(CFG_BACKTIME, 15*60*1000);

  ls = config->readNumEntry(CFG_NUMLANGSET, 0);
  QString s;
  for (int i = 0; i < ls; i++) {
    s.setNum (i);
    s.insert (0, CFG_L_SHORT);
    QString shortId = config->readEntry(s, "");
    if (shortId.stripWhiteSpace().length() == 0) {
      shortId.setNum (i);
      shortId.insert (0, "id");
    }

    s.setNum (i);
    s.insert (0, CFG_L_SHORT2);
    QString shortId2 = config->readEntry(s, "");

    s.setNum (i);
    s.insert (0, CFG_L_LANG);
    QString longId = config->readEntry(s, "");
    if (longId.stripWhiteSpace().length() == 0) {
      longId.setNum (i);
      longId.insert (0, "ident");
    }

    s.setNum (i);
    s.insert (0, CFG_L_PIXMAP);
    QString PixMapFile = config->readEntry(s, "");

    langset.addSet (shortId, shortId2, longId, PixMapFile);
  }

  config->setGroup(CFG_QUERYPROP);
  type_querytimeout = (kvq_timeout_t) config->readNumEntry(CFG_QUERYTIMEOUT, (int) kvq_notimeout);
  maxqueryTime = config->readNumEntry(CFG_MAXTIME_PER, 20*1000);
  showcounter = config->readNumEntry(CFG_SHOWCOUNTER, false);
  swap_querydir= config->readBoolEntry(CFG_SWAP_DIR, false);
  block = config->readBoolEntry(CFG_BLOCK, true);
  expire = config->readBoolEntry(CFG_EXPIRE, true);

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

  QString filename = config->readEntry(CFG_FILENAME,"");
  QString title = config->readEntry(CFG_TITLE,"");
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

