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
#include "compat_2x.h"
#include <qfile.h>

#include <X11/Xlib.h>

#include <qmessagebox.h>

#include <kconfig.h>

#include "common-dialogs/ProgressDlg.h"
#include "common-dialogs/CharSetDlg.h"

void kvoctrainApp::saveOptions(bool all)
{
  KConfig *config = EA_KappGetConfig;

  config->setGroup(CFG_GENERAL);
  config->writeEntry(CFG_RECENT, recent_files);
  config->writeEntry(CFG_AUTOSAVEOPT, autosaveopts);

  config->setGroup(CFG_APPEARANCE);
  config->writeEntry("ShowToolbar",toolBar()->isVisible());
  config->writeEntry("ShowStatusbar",statusBar()->isVisible());
  config->writeEntry("ToolBar_Pos", (int)toolBar()->barPos());

  if (view) {
    config->setGroup(CFG_GENERAL);
    config->writeEntry(CFG_CUR_ROW, view->getTable()->currentRow());
    config->writeEntry(CFG_CUR_COL, view->getTable()->currentCol());
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
    config->writeEntry(CFG_FFAMILY, tablefont.family());
    config->writeEntry(CFG_FCHARSET, kvoctrainDoc::charSet2String (tablefont.charSet()));
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
    config->writeEntry(CFG_HEADER_RESIZER, (int) header_resizer);
    config->writeEntry(CFG_SMART_APPEND, (bool) smartAppend);
  
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

      QFont font;
      bool stdfont;
      langset.Font(i, font, stdfont);

      s.setNum (i);
      s.insert (0, CFG_L_FONT);
      config->writeEntry(s, font);

      s.setNum (i);
      s.insert (0, CFG_L_STDFONT);
      config->writeEntry(s, stdfont);
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


static char **getXFontNames( const char *pattern, int *count )
// from qfont_x11.cpp
// Get an array of X font names that matches a pattern
{
    static int maxFonts = 256;
    char **list;
    while( 1 ) {
	list = XListFonts( QPaintDevice::x11AppDisplay(), (char*)pattern,
			   maxFonts, count );
	// I know precisely why 32768 is 32768.
	if ( *count != maxFonts || maxFonts >= 32768 )
	    return list;
	XFreeFontNames( list );
	maxFonts *= 2;
    }
}


bool kvoctrainApp::substituteFontInfo (QFont &font, QString &langname)
{
   char      **xFontNames;
   int 	       count;

   QString s = kvoctrainDoc::charSet2XName (font.charSet());

   QString pattern = "-*-*-*-*-*-*-*-*-*-*-*-*-"+s;
   xFontNames = getXFontNames( pattern.local8Bit(), &count );
   QStrList rawnames;
   for( int i = 0; i < count; i++ ) {
      rawnames.append (xFontNames[i]);
   }

   XFreeFontNames( xFontNames );

   if (count <= 0)
     return false;

   s = kvoctrainDoc::charSet2String (font.charSet() );
   QString fs = font.family();

   QString format =
     i18n("Could not find a font for \"%1\" with charset\n"
          "\"%3\" which matches font name \"%3\"\n"
          "\n"
          "Please select one of the following for temporary use\n"
          "and do not forget to adjust your settings.\n");
   QString msg = format.arg(langname).arg(s).arg(fs);

   QApplication::setOverrideCursor( arrowCursor, true );
   CharSetDlg sdlg (false, msg, rawnames);
   if (sdlg.exec() == QDialog::Accepted) {
     font = sdlg.getFont();
   }
   QApplication::restoreOverrideCursor();
   return true;
}


void kvoctrainApp::checkFontInfo (QFont &font, QString &langname)
{
    QFontInfo fi (font);

    if (font.charSet() != QFont::AnyCharSet && // exact font found ?
        font.charSet() != fi.charSet()) {

      if (kvoctrainApp::substituteFontInfo (font, langname)) // substitute charset
        return;

      QString s1 = kvoctrainDoc::charSet2String(font.charSet());
      QString s3 = kvoctrainDoc::charSet2String(fi.charSet(), true);
      QString format = i18n("Could not find a font with charset \"%1\" on your system\n"
                            "which is needed to display \"%2\".\n"
                            "\n"
                            "\"%3\" will be used instead, but please take into\n"
                            " account,that characters might be rendered wrong!\n");
      QString msg = format.arg(s1).arg(langname).arg(s3);

      QApplication::setOverrideCursor( arrowCursor, true );

      KMessageBox::information(0,
                 msg,
                 kvoctrainApp::generateCaption(i18n("Missing charset")),
                  i18n("&OK"));
     QApplication::restoreOverrideCursor();

    }
}


void kvoctrainApp::readOptions()
{
  ///////////////////////////////////////////////////////////////////
  // read the config file entries
  KConfig *config = EA_KappGetConfig;

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
  bViewToolbar = config->readBoolEntry("ShowToolbar", true);
  bViewStatusbar = config->readBoolEntry("ShowStatusbar", true);
  tool_bar_pos = (KToolBar::BarPosition)config->readNumEntry("ToolBar_Pos", KToolBar::Top);
  QFont fdefault;
  QString family = config->readEntry(CFG_FFAMILY, fdefault.family());
  QString charset = config->readEntry(CFG_FCHARSET, "");
  QFont::CharSet cs = kvoctrainDoc::string2CharSet (charset);
  int size = config->readNumEntry(CFG_FSIZE, fdefault.pointSize());
  tablefont.setPointSize(size);
  tablefont.setFamily(family);
  tablefont.setWeight(QFont::Normal);
  tablefont.setCharSet(cs);
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
  header_resizer = (kvoctrainView::Resizer) config->readNumEntry(CFG_HEADER_RESIZER, (int) kvoctrainView::Automatic);
  smartAppend = config->readNumEntry(CFG_SMART_APPEND, 0);
  // initialize the recent file list
  recent_files = config->readListEntry(CFG_RECENT);
  autosaveopts = config->readBoolEntry(CFG_AUTOSAVEOPT, true);

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

    s.setNum (i);
    s.insert (0, CFG_L_FONT);
    QFont font = config->readFontEntry(s);

    checkFontInfo (font, longId);

    s.setNum (i);
    s.insert (0, CFG_L_STDFONT);
    bool stdfont = config->readNumEntry(s);

    langset.addSet (shortId, shortId2, longId, PixMapFile, font, stdfont);
  }

  if (langset.size() == 0) {  // insert some language codes
    langset.addSet ("en", i18n("English"), EA_KDEDATADIR("", "share/locale/l10n/en/flag.png"), QFont(), false);
    langset.addSet ("de", i18n("German"),  EA_KDEDATADIR("", "share/locale/l10n/de/flag.png"), QFont(), false);
    langset.addSet ("fr", i18n("French"),  EA_KDEDATADIR("", "share/locale/l10n/fr/flag.png"), QFont(), false);
//    langset.addSet ("it", i18n("Italian"), EA_KDEDATADIR("", "share/locale/l10n/it/flag.png"), QFont(), false);
//    langset.addSet ("sv", i18n("Swedish"), EA_KDEDATADIR("", "share/locale/l10n/sv/flag.png"), QFont(), false);
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
                    kvoctrainApp::generateCaption(""));
      pdlg->show();
      doc = new kvoctrainDoc (this, tempname, separator, &paste_order);
      removeProgressBar();
      doc->setModified();
      doc->setTitle(title);
      doc->setFileName(filename);
      setCaption(kvoctrainApp::generateCaption(doc->getTitle()), doc->isModified());
      QFile::remove(tempname);
    }
  }
  else if(!filename.isEmpty()){
    pdlg = new ProgressDlg (QString(), QString(),
                  kvoctrainApp::generateCaption(""));
    pdlg->show();
    doc = new kvoctrainDoc (this, filename, separator, &paste_order);
    removeProgressBar();
    setCaption(kvoctrainApp::generateCaption(doc->getTitle()), doc->isModified());
  }

  if (isQueryMode()) {
    slotStartQuery(act_query_trans, act_query_org); // query was running at last exit
    if (!isQueryMode()) {                           // query mode could not start
      show();
      kapp->setTopWidget(this);
      kapp->setMainWidget( this );
    }
  }
  else {
    show();
    kapp->setTopWidget(this);
    kapp->setMainWidget( this );
  }
}		

