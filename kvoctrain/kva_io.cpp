/***************************************************************************

    $Id$

                         i/o part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.11  2001/11/18 12:28:25  arnold
    provided menu entry for example files

    Revision 1.10  2001/11/17 17:58:22  arnold
    added inline editing of all columns

    Revision 1.9  2001/11/16 18:52:59  arnold
    added possibility to disable expressions

    Revision 1.8  2001/11/11 12:51:45  arnold
    fixed some strings for i18n purposes

    Revision 1.7  2001/11/10 22:27:08  arnold
    removed compatibility for kde1

    Revision 1.6  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/10/21 15:28:43  arnold
    removed all the 'charset' stuff

    Revision 1.4  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/12 19:28:13  arnold
    removed references to obsolete binary format

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

#include <unistd.h>

#include <qtimer.h>
#include <qprogressbar.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h>

#include <kfiledialog.h>
#include <kcombobox.h>
#include <kprogress.h>
#include <krecentdocument.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kstddirs.h>
#include <kdebug.h>

#include "kvoctrain.h"
#include "UsageManager.h"
#include "common-dialogs/ProgressDlg.h"

#define PATTERN_ALL  "*.*"            "|All files (*.*)\n"
#define PATTERN_ML   "*." KVTML_EXT   "|KVoctrain Markup (*.kvtml)\n"
#define PATTERN_LEX  "*." VT5_LEX_EXT "|Vocabulary Trainer 5.0 (*.lex)\n"
#define PATTERN_VL   "*." KVL_EXT     "|KVoclearn (*.vl)\n"
#define PATTERN_QVO  "*." QVOCAB_EXT  "|QVocab (*.qvo)\n"
#define PATTERN_CSV  "*." CSV_EXT     "|Text (*.csv)\n"
#define PATTERN_VCB  "*." VCB_EXT     "|Vocabbox (*.vocab)\n"

// we can read these
#define FILTER_RPATTERN  PATTERN_ML PATTERN_VCB PATTERN_CSV PATTERN_LEX PATTERN_ALL

// we can write these
#define FILTER_WPATTERN  PATTERN_ML PATTERN_VCB PATTERN_CSV PATTERN_LEX PATTERN_ALL

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
 i18n(FILTER_RPATTERN)
 i18n(FILTER_WPATTERN)
#endif


// helper that gets a file name (it only differs in the caption of the dialog)
static QString getFileName(const QString &caption,
			   const QString &dir = 0, const QString &filter = 0,
			   QWidget* parent = 0, const QString &name = 0)
{
    QString filename;
    KFileDialog dlg(dir, filter, parent, name.local8Bit(), true /*, false*/);

    dlg.setCaption(caption);

    if (dlg.exec() == QDialog::Accepted)
	filename = dlg.selectedFile();

    return filename;
}


void kvoctrainApp::slotTimeOutBackup() /*FOLD00*/
{
  if (backupTime != 0 && doc && doc->isModified() ) {
    slotStatusMsg(i18n("Autobackup in progress"));
    slotFileSave();
  }
  btimer->start(backupTime, TRUE);
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::aboutToShowFile() /*FOLD00*/
{
  file_menu->setItemEnabled(ID_FILE_SAVE, doc->isModified());
}


bool kvoctrainApp::queryClose() /*FOLD00*/
{
   bool erg = queryExit();
   if (erg)
     doc->setModified(false);  // avoid double query on exit via system menu
   return erg;
}


bool kvoctrainApp::queryExit() /*FOLD00*/
{
  saveOptions(false);
  if (!doc || !doc->isModified() ) return true;

  if (backupTime != 0) {  // autobackup on: save without asking
    slotFileSave();       // save and exit
    return true;
  }

  int exit = KMessageBox::warningYesNoCancel(this, 
            i18n("Vocabulary is modified\n\nSave file before exit ?\n"),
            kapp->makeStdCaption(""));
  if(exit==KMessageBox::Yes) {
    slotFileSave();   // save and exit
    return true;
  }
  else if (exit == 2) {
    return true;     // dont save but exit
  }

  return false;      // continue work
}


void kvoctrainApp::slotFileQuit() /*FOLD00*/
{ 
  ///////////////////////////////////////////////////////////////////
  // exits the Application
  if(this->queryExit())
    {
      kapp->quit();
    }
  else
    slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotProgress(kvoctrainDoc *curr_doc, int percent) /*FOLD00*/
{
  if (pdlg != 0)
    pdlg->setValue (curr_doc, percent);
  else if (pbar != 0)
    pbar->setValue (percent);
  kapp->processEvents();
}


void kvoctrainApp::slotFileOpenRecent(int id_) /*FOLD00*/
{
  slotStatusMsg(i18n("Opening file..."));

  id_ >>= 16;
  if (queryExit() && recent_files.count() != 0) {
    QString name = recent_files[id_];
    if (!name.isEmpty() ) {
      view->setView(0, langset, gradecols);
      delete doc;
      doc = 0;

      QString format = i18n("Loading %1");
      QString msg = format.arg(name);

      slotStatusMsg(msg);
      prepareProgressBar();
      doc = new kvoctrainDoc (this, name, separator, &paste_order);
      connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
      slotModifiedDoc(false);
      removeProgressBar();
      loadDocProps(doc);
      if (!doc->getTitle().isEmpty() )
        setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
      else
        setCaption(kapp->makeStdCaption("", false, doc->isModified()));
      addRecentFile(name);
      view->setView(doc, langset, gradecols);
    }
  }
  slotStatusMsg(IDS_DEFAULT);

  for (int i = 0; i < 10; i++)
    kapp->processEvents();
}


void kvoctrainApp::addRecentFile(const QString &rel_file) /*FOLD00*/
{
  if (rel_file.stripWhiteSpace().length() == 0)
    return;

  QString file = QFileInfo (rel_file).absFilePath();

  int pos = recent_files.findIndex(file);
  if((pos == -1)){
    if( recent_files.count() < MAX_RECENTFILES) {
      recent_files.insert(recent_files.begin(), file);
      KRecentDocument::add (file, false);
    }
    else{
      recent_files.remove(--recent_files.end());
      recent_files.insert(recent_files.begin(), file);
      KRecentDocument::add (file, false);
    }
  }
  else {
    recent_files.remove(recent_files.at(pos));
    recent_files.insert(recent_files.begin(), file);  // make most recent the first entry
    KRecentDocument::add (file, false);
  }

  // create the file_open_popup for the toolbar and the menu
  recent_files_menu->clear();
  file_open_popup->clear();
  QString accel;
  for ( int i =0 ; i < (int)recent_files.count(); i++){
    accel.setNum (i);
    accel.insert (0, "&");
    accel += "  ";
    accel += recent_files[i];
    recent_files_menu->insertItem(accel, (i << 16) | ID_FILE_OPEN_RECENT);
    file_open_popup->insertItem(accel, (i << 16) | ID_FILE_OPEN_RECENT);
  }
}


void kvoctrainApp::slotFileNew() /*FOLD00*/
{
  slotStatusMsg(i18n("Creating new file..."));

  if (queryExit() ) {
    view->setView (0, langset, gradecols);
    delete doc;
    doc = new kvoctrainDoc (this, "", separator, &paste_order);
    connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    slotModifiedDoc(false);
    loadDocProps(doc);

    if (doc->numLangs() == 0)
      doc->appendLang("en");
    setCaption(kapp->makeStdCaption("", false, doc->isModified()));
    view->setView(doc, langset, gradecols);
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotFileOpen() /*FOLD00*/
{
  slotStatusMsg(i18n("Opening file..."));

  if (queryExit() ) {
    QString s;
    if (recent_files.count() > 0)
      s = recent_files[0];
    QString name = getFileName(kapp->makeStdCaption(i18n("Open vocabulary file")),
                               s, FILTER_RPATTERN, parentWidget());
    loadfileFromPath(name);
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::loadfileFromPath(QString &name)
{
    if (!name.isEmpty() ) {
      view->setView(0, langset, gradecols);
      delete doc;
      doc = 0;

      QString format = i18n("Loading %1");
      QString msg = format.arg(name);

      slotStatusMsg(msg);
      prepareProgressBar();
      doc = new kvoctrainDoc (this, name, separator, &paste_order);
      connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
      slotModifiedDoc(false);
      removeProgressBar();
      loadDocProps(doc);
      if (!doc->getTitle().isEmpty() )
        setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
      else
        setCaption(kapp->makeStdCaption("", false, doc->isModified()));
      view->setView(doc, langset, gradecols);
      addRecentFile (name);
    }
}

void kvoctrainApp::slotFileOpenExample() /*FOLD00*/
{
  slotStatusMsg(i18n("Opening example file..."));

  if (queryExit() ) {
    QString s;
    s = locate("data",  "kvoctrain/examples/");
    QString name = getFileName(kapp->makeStdCaption(i18n("Open example vocabulary file")),
                               s, FILTER_RPATTERN, parentWidget());
    loadfileFromPath(name);
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotFileMerge() /*FOLD00*/
{
  slotStatusMsg(i18n("Merging file..."));

  QString s;
  if (recent_files.count() > 0)
    s = recent_files[0];
  QString name = getFileName(kapp->makeStdCaption(i18n("Merge vocabulary file")),
                             s, FILTER_RPATTERN, parentWidget());
  if (!name.isEmpty() ) {

    QString format = i18n("Loading %1");
    QString msg = format.arg(name);

    slotStatusMsg(msg);
    prepareProgressBar();
    kvoctrainDoc *new_doc = new kvoctrainDoc (this, name, separator, &paste_order);
    connect (new_doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    slotModifiedDoc(false);
    removeProgressBar();

    vector<QString> old_names = doc->getLessonDescr();
    vector<QString> new_names = new_doc->getLessonDescr();

    vector<QString> old_types = doc->getTypeDescr();
    vector<QString> new_types = new_doc->getTypeDescr();

    vector<QString> old_tenses = doc->getTenseDescr();
    vector<QString> new_tenses = new_doc->getTenseDescr();

    vector<int> old_in_query = doc->getLessonsInQuery();
    vector<int> new_in_query = new_doc->getLessonsInQuery();

    vector<QString> old_usages = doc->getUsageDescr();
    vector<QString> new_usages = new_doc->getUsageDescr();

    format = i18n("Mergeing %1");
    msg = format.arg(name);
    slotStatusMsg(msg);

    QApplication::setOverrideCursor( waitCursor );

    int lesson_offset = lessons->count()-1;
    for (int i = 0; i < (int) new_names.size(); i++) {
      lessons->insertItem (new_names[i]);
      old_names.push_back(new_names[i]);
    }
    doc->setLessonDescr(old_names);

    for (int i = 0; i < (int) new_in_query.size(); i++)
      old_in_query.push_back(new_in_query[i]+lesson_offset);
    doc->setLessonsInQuery(old_in_query);
    querymanager.setLessonItems(old_in_query);

    int types_offset = old_types.size();
    for (int i = 0; i < (int) new_types.size(); i++) {
      old_types.push_back(new_types[i]);
    }
    doc->setTypeDescr(old_types);
    QueryManager::setTypeNames(old_types);


    int tenses_offset = old_tenses.size();
    for (int i = 0; i < (int) new_tenses.size(); i++) {
      old_tenses.push_back(new_tenses[i]);
    }
    doc->setTenseDescr(old_tenses);
    Conjugation::setTenseNames(old_tenses);


    int usages_offset = old_usages.size();
    for (int i = 0; i < (int) new_usages.size(); i++) {
      old_usages.push_back(new_usages[i]);
    }
    doc->setUsageDescr(old_usages);
    UsageManager::setUsageNames(old_usages);


    bool equal = true;
    if (doc->getOriginalIdent() != new_doc->getOriginalIdent())
      equal = false;
    for (int i = 1; i < doc->numLangs(); i++)
      if (doc->getIdent(i) != new_doc->getIdent(i))
        equal = false;

    if (equal) {   // easy way: same language codes, just append

      for (int i = 0; i < new_doc->numEntries(); i++) {
        kvoctrainExpr *expr = new_doc->getEntry(i);

        expr->setLesson(expr->getLesson()+lesson_offset);

        for (int lang = 0; lang <= (int) expr->numTranslations(); lang++) {
          QString t = expr->getType (lang);
          // adjust type offset
          if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
            QString t2;
            t.remove (0, 1);
            t2.setNum (t.toInt()+types_offset);
            t2.insert (0, QM_USER_TYPE);
            expr->setType (lang, t2);
          }

          t = expr->getUsageLabel (lang);
          // adjust usage offset
          QString tg;
          if (!t.isEmpty() ) {
            QString t2;
            while (t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
              QString n;
              t.remove (0, 1);
              int next;
              if ((next = t.find(UL_USAGE_DIV)) >= 0) {
                n = t.left(next);
                t.remove (0, next+1);
              }
              else {
                n = t;
                t = "";
              }

              t2.setNum (n.toInt()+usages_offset);
              t2.insert (0, UL_USER_USAGE);
              if (tg.length() == 0)
                tg = t2;
              else
                tg += UL_USAGE_DIV + t2;
            }

            if (tg.length() == 0)
              tg = t;
            else if (t.length() != 0)
              tg += UL_USAGE_DIV + t;

            expr->setUsageLabel (lang, tg);
          }

          Conjugation conj = expr->getConjugation(lang);
          bool condirty = false;
          for (int ci = 0; ci < conj.numEntries(); ci++) {
            t = conj.getType(ci);
            if (!t.isEmpty() && t.left(1) == UL_USER_TENSE) {
              t.remove (0, strlen(UL_USER_TENSE));
              QString t2;
              t2.setNum (t.toInt()+tenses_offset);
              t2.insert (0, UL_USER_TENSE);
              conj.setType(ci, t2);
              condirty = true;
            }
            if (condirty)
              expr->setConjugation(lang, conj);
          }
        }

        doc->appendEntry (expr);
      }
      doc->setModified();
    }
    else { // hard way: move entries around, most attributes get lost
      vector<int> move_matrix;

      vector <bool> cs_equal;
      for (int i = 0; i < QMAX (doc->numLangs(), new_doc->numLangs()); i++)
        cs_equal.push_back (false);

      move_matrix.push_back(new_doc->findIdent(doc->getOriginalIdent()));
      for (int i = 1; i < doc->numLangs(); i++)
        move_matrix.push_back(new_doc->findIdent(doc->getIdent(i)));

      for (int j = 0; j < new_doc->numEntries(); j++) {
        kvoctrainExpr new_expr;
        kvoctrainExpr *expr = new_doc->getEntry(j);
        new_expr.setLesson(expr->getLesson()+lesson_offset);

        for (int i = 0; i < (int) move_matrix.size(); i++) {
          int lpos = move_matrix[i];
          if (lpos >= 0) {

            if (lpos == 0)
              s = expr->getOriginal();
            else
              s = expr->getTranslation(lpos);

            if (!cs_equal[lpos]) {
              cs_equal[lpos] = true;
              QString id = lpos == 0 ? doc->getOriginalIdent()
                                     : doc->getIdent(lpos);
            }

            if (i == 0)
              new_expr.setOriginal(s);
            else
              new_expr.setTranslation(i, s);
            new_expr.setRemark (i, expr->getRemark(lpos));

            QString t = expr->getType(lpos);
            if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
              QString t2;
              t.remove (0, 1);
              t2.setNum (t.toInt()+types_offset);
              t2.insert (0, QM_USER_TYPE);
              new_expr.setType (i, t2);
            }

            t = expr->getUsageLabel(lpos);
            if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
              QString t2;
              t.remove (0, 1);
              t2.setNum (t.toInt()+usages_offset);
              t2.insert (0, QM_USER_TYPE);
              new_expr.setUsageLabel (i, t2);
            }

            Conjugation conj = expr->getConjugation(lpos);
            for (int ci = 0; ci < conj.numEntries(); ci++) {
              t = conj.getType(ci);
              if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                t.remove (0, strlen(QM_USER_TYPE));
                QString t2;
                t2.setNum (t.toInt()+tenses_offset);
                t2.insert (0, QM_USER_TYPE);
                conj.setType(ci, t2);
              }
            }

          }
        }
        // only append if entries are used
        bool used = !new_expr.getOriginal().isEmpty();
        for (int i = 1; i < (int) doc->numLangs(); i++)
          if (!new_expr.getTranslation(i).isEmpty())
            used = true;

        if (used) {
          doc->appendEntry(&new_expr);
          doc->setModified();
        }
      }
    }
    delete (new_doc);
    addRecentFile (name);
  }

  view->setView(doc, langset, gradecols);
  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotFileSave() /*FOLD00*/
{
  if (doc->getFileName().isEmpty() ) {
    slotFileSaveAs();
    return;
  }

  QString format = i18n("Saving %1");
  QString msg = format.arg(doc->getFileName());
  slotStatusMsg(msg);

  // remove previous backup
  QFile::remove(QFile::encodeName(doc->getFileName()+"~"));
  ::rename (QFile::encodeName(doc->getFileName()),
            QFile::encodeName(doc->getFileName()+"~"));

  prepareProgressBar();
  saveDocProps(doc);
  doc->saveAs(this, doc->getFileName(), doc->getTitle(),
              kvoctrainDoc::automatic, separator, &paste_order);
  removeProgressBar();

  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::fillLessonBox(kvoctrainDoc *the_doc)
{
  lessons->clear();
  lessons->insertItem (the_doc->getLessonDescr(0));
  vector<QString> names = the_doc->getLessonDescr();
  for (int i = 0; i < (int) names.size(); i++)
    lessons->insertItem (names[i]);
  act_lesson = the_doc->getCurrentLesson();
  if (act_lesson > lessons->count() ) {
    act_lesson = 0;
    the_doc->setCurrentLesson(act_lesson);
  }
  lessons->setCurrentItem (act_lesson);
}


void kvoctrainApp::loadDocProps(kvoctrainDoc *the_doc) /*FOLD00*/
{
  fillLessonBox(the_doc);

  random_expr1.clear();
  random_expr2.clear();
  queryList.clear();
  the_doc->getQueryLang (act_query_org, act_query_trans);
  if (!act_query_org.isEmpty() && !act_query_trans.isEmpty() ) {
    for (int i = 0; i < the_doc->numEntries(); i++)
    {
      kvoctrainExpr *entry = the_doc->getEntry(i);
      if (entry->isInQuery())
      {
         int less = entry->getLesson();
         for (int l = (int) queryList.size(); l <= less; l++) {
            vector<QueryEntryRef> ref_vec;
            queryList.push_back(ref_vec);
         }
         QueryEntryRef ref(entry, i);
         queryList[less].push_back(ref);
      }
    }
  }

  QueryManager::setTypeNames (doc->getTypeDescr() );
  UsageManager::setUsageNames (doc->getUsageDescr() );
  Conjugation::setTenseNames (doc->getTenseDescr() );

  querymanager.setLessonItems(doc->getLessonsInQuery() );

  // remove empty elements
  for (int i = (int) queryList.size()-1; i >= 0; i--)
    if (queryList[i].size() == 0) {
      queryList.erase(&queryList[i], &queryList[i+1]);
    }

  query_cycle = 1;
  query_startnum = 0;
  for (int i = 0; i < (int) queryList.size(); i++)
    query_startnum += queryList[i].size();
  query_num = query_startnum;

}


void kvoctrainApp::saveDocProps(kvoctrainDoc *the_doc) /*FOLD00*/
{
  the_doc->setQueryLang (act_query_org, act_query_trans);
}


void kvoctrainApp::slotFileSaveAs() /*FOLD00*/
{
  slotStatusMsg(i18n("Saving file under new filename..."));

  QString s;
  if (recent_files.count() > 0)
    s = recent_files[0];

  QString name = getFileName(kapp->makeStdCaption(i18n("Save vocabulary as")),
                             s, FILTER_WPATTERN, parentWidget());
  if (!name.isEmpty() ) {

    QFile file (name);
    if (file.exists() ) {
      QString format = i18n("Your selected file exists already.\n\n"
                            "Do you want to overwrite \"%1\"?");
      QString msg = format.arg(name);

      int exit = KMessageBox::warningYesNo(this,
                    msg,
                    kapp->makeStdCaption(i18n("File exists")));
      if(exit!=KMessageBox::Yes)
        return;
    }

    if (doc) {
      addRecentFile(name);

      QString format = i18n("Saving %1");
      QString msg = format.arg(name);
      slotStatusMsg(msg);

      QFile::remove(QFile::encodeName(name+"~"));         // remove previous backup
      ::rename (QFile::encodeName(name), QFile::encodeName(QString(name+"~")));
      saveDocProps(doc);

      prepareProgressBar();
      doc->saveAs(this, name, doc->getTitle(),
                  kvoctrainDoc::automatic, separator, &paste_order);
      removeProgressBar();
    }
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotSaveSelection () /*FOLD00*/
{
  slotStatusMsg(i18n("Saving selected area under new filename..."));

  kvoctrainDoc seldoc(this, "");
  // transfer most important parts
  seldoc.appendLang(doc->getOriginalIdent());
  for (int i = 1; i < doc->numLangs(); i++)
    seldoc.appendLang(doc->getIdent(i));
  seldoc.setAuthor(doc->getAuthor());
  seldoc.setLessonDescr(doc->getLessonDescr());
  seldoc.setTypeDescr(doc->getTypeDescr());

  for (int i = doc->numEntries()-1; i >= 0; i--)
    if (doc->getEntry(i)->isInQuery() )
      seldoc.appendEntry(doc->getEntry(i));

  QString s;
  QString name = getFileName(kapp->makeStdCaption(i18n("Save vocabulary block as")),
                             s, FILTER_WPATTERN, parentWidget());
  if (!name.isEmpty() ) {

    QFile file (name);
    if (file.exists() ) {
      QString format = i18n("Your selected file exists already.\n\n"
                            "Do you want to overwrite \"%1\"?");
      QString msg = format.arg(name);

      int exit = KMessageBox::warningYesNo(this, msg,
                 kapp->makeStdCaption(i18n("File exists")));
      if(exit!=KMessageBox::Yes)
        return;
    }

    QString format = i18n("Saving %1");
    QString msg = format.arg(name);
    slotStatusMsg(msg);

    QFile::remove(name+"~");         // remove previous backup
    // FIXME: check error
    ::rename (QFile::encodeName(name), QFile::encodeName(name+"~"));
    saveDocProps(&seldoc);
 
    prepareProgressBar();
    seldoc.saveAs(this, name, QString (i18n ("Part of: ")) +doc->getTitle(),
                kvoctrainDoc::automatic, separator, &paste_order);
    removeProgressBar();
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::prepareProgressBar () /*FOLD00*/
{
    statusBar()->clear ();
    pbar = new KProgress (statusBar());
    pbar->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    statusBar()->addWidget(pbar, 150, true);
    pbar->show();
}


void kvoctrainApp::removeProgressBar () /*FOLD00*/
{
    statusBar()->clear ();
    statusBar()->removeWidget(pbar);
    delete pbar;
    pbar = 0;
    delete pdlg;
    pdlg = 0;
}
