/***************************************************************************

                         i/o part of kvoctrain

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

#include <unistd.h>

#include <QTimer>
#include <QFrame>

#include <kfiledialog.h>
#include <kcombobox.h>
#include <kprogressbar.h>
#include <krecentdocument.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <knewstuff/downloaddialog.h>

#include "kvoctrain.h"
#include "UsageManager.h"
#include "common-dialogs/ProgressDlg.h"
#include <prefs.h>

#define PATTERN_ALL  I18N_NOOP("*|All Files (*)\n")
#define PATTERN_ML   I18N_NOOP("*.kvtml|KVocTrain Markup (*.kvtml)\n")
#define PATTERN_LEX  I18N_NOOP("*.lex|Vocabulary Trainer 5.0 (*.lex)\n")
#define PATTERN_VL   I18N_NOOP("*.vl|KVoclearn (*.vl)\n")
#define PATTERN_QVO  I18N_NOOP("*.qvo|QVocab (*.qvo)\n")
#define PATTERN_VOC  I18N_NOOP("*.voc|Vokabeltrainer (*.voc)\n")
#define PATTERN_CSV  I18N_NOOP("*.csv|Text (*.csv)\n")
#define PATTERN_VCB  I18N_NOOP("*.vocab|Vocabbox (*.vocab)\n")

// we can read these
#define FILTER_RPATTERN  i18n(PATTERN_ML)+i18n(PATTERN_VCB)+i18n(PATTERN_VOC)+i18n(PATTERN_CSV)+i18n(PATTERN_LEX)+i18n(PATTERN_ALL)

// we can write these
#define FILTER_WPATTERN  i18n(PATTERN_ML)+i18n(PATTERN_VCB)+i18n(PATTERN_CSV)+i18n(PATTERN_LEX)+i18n(PATTERN_ALL)


void kvoctrainApp::slotTimeOutBackup()
{
  if (Prefs::autoBackup() && doc && doc->isModified() ) {
    slotStatusMsg(i18n("Autobackup in progress"));
    slotFileSave();
  }
  if (Prefs::backupTime() > 0)
    btimer->start(Prefs::backupTime() * 60 * 1000, TRUE);
  slotStatusMsg(IDS_DEFAULT);
}


bool kvoctrainApp::queryClose()
{
   bool erg = queryExit();
   if (erg)
     doc->setModified(false);  // avoid double query on exit via system menu
   return erg;
}


bool kvoctrainApp::queryExit()
{
  saveOptions();
  if (!doc || !doc->isModified() ) return true;

  bool save = (Prefs::autoSave()); //save without asking

  if (!save)
  {
     int exit = KMessageBox::warningYesNoCancel(this,
               i18n("Vocabulary is modified.\n\nSave file before exit?\n"),
               kapp->makeStdCaption(""),
               KStdGuiItem::save(), KStdGuiItem::discard());
     if (exit==KMessageBox::Yes) {
       save = true;   // save and exit
     }
     else if (exit == KMessageBox::No) {
       save = false;  // dont save but exit
     }
     else {
       return false;  // continue work
     }
  }

  if (save) {
    if (!doc->URL().isEmpty())
      slotFileSave();       // save and exit
    if (doc->isModified())
    {
      // Error while saving or no name
      slotFileSaveAs();
    }
  }
  return true;
}


void kvoctrainApp::slotFileQuit()
{
  ///////////////////////////////////////////////////////////////////
  // exits the Application
  if(queryExit())
    {
      doc->setModified(false);  // Make sure not to bother about saving again.
      kapp->quit();
    }
  else
    slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotProgress(KEduVocDocument *curr_doc, int percent)
{
  if (pdlg != 0)
    pdlg->setValue (curr_doc, percent);
  else if (pbar != 0)
    pbar->setValue (percent);
  kapp->processEvents();
}


void kvoctrainApp::slotFileOpenRecent(const KUrl& url)
{
  slotStatusMsg(i18n("Opening file..."));
  if (queryExit() && fileOpenRecent->items().count() > 0)
  {
    fileOpenRecent->setCurrentItem(-1);
    loadfileFromPath(url);
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotFileNew()
{
  slotStatusMsg(i18n("Creating new file..."));

  if (queryExit() ) {
    view->setView (0, langset);
    delete doc;
    QString name = "";
    doc = new KEduVocDocument(this);
    loadDocProps(doc);
    if (doc->numIdentifiers() == 0) {
      QString l = "en";
      doc->appendIdentifier(l);
    }
    view->setView(doc, langset);
    view->getTable()->setFont(Prefs::tableFont());
    view->adjustContent();
    connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    doc->setModified(false);
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotFileOpen()
{
  slotStatusMsg(i18n("Opening file..."));

  if (queryExit() ) {
    QString s;
    KUrl url = KFileDialog::getOpenURL(QString(), FILTER_RPATTERN, parentWidget(), i18n("Open Vocabulary File"));
    loadfileFromPath(url, true);
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::loadfileFromPath(const KUrl & url, bool addRecent)
{
  if (!url.path().isEmpty())
  {
    view->setView(0, langset);
    delete doc;
    doc = 0;

    slotStatusMsg(i18n("Loading %1", url.path()));
    prepareProgressBar();
    doc = new KEduVocDocument(this);
    doc->open(url, false);
    removeProgressBar();
    loadDocProps(doc);
    view->setView(doc, langset);
    view->getTable()->setFont(Prefs::tableFont());
    view->adjustContent();
    if (addRecent)
      fileOpenRecent->addUrl(url);
    connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    doc->setModified(false);
  }
}

void kvoctrainApp::slotFileOpenExample()
{
  slotStatusMsg(i18n("Opening example file..."));

  if (queryExit() ) {
    QString s;
    s = locate("data",  "kvoctrain/examples/");
    KUrl url = KFileDialog::getOpenURL(s, FILTER_RPATTERN, parentWidget(), i18n("Open Example Vocabulary File"));
    loadfileFromPath(url, false);
    if (doc)
       doc->URL().setFileName(QString());
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotGHNS()
{
  if (!m_newStuff)
    m_newStuff = new KVTNewStuff(this);
  m_newStuff->download();
}


void kvoctrainApp::slotFileMerge()
{
  slotStatusMsg(i18n("Merging file..."));

  QString s;
  KUrl url = KFileDialog::getOpenURL(QString(), FILTER_RPATTERN, parentWidget(), i18n("Merge Vocabulary File"));

  if (!url.isEmpty() ) {

    QString msg = i18n("Loading %1", url.path());

    slotStatusMsg(msg);
    prepareProgressBar();
    KEduVocDocument *new_doc = new KEduVocDocument(this);
    connect (new_doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    doc->setModified(false);
    removeProgressBar();

    QStringList old_names = doc->lessonDescriptions();
    QStringList new_names = new_doc->lessonDescriptions();

    QStringList old_types = doc->typeDescriptions();
    QStringList new_types = new_doc->typeDescriptions();

    QStringList old_tenses = doc->tenseDescriptions();
    QStringList new_tenses = new_doc->tenseDescriptions();

    QList<int> old_in_query = doc->lessonsInQuery();
    QList<int> new_in_query = new_doc->lessonsInQuery();

    QStringList old_usages = doc->usageDescriptions();
    QStringList new_usages = new_doc->usageDescriptions();

    msg = i18n("Merging %1", url.path());
    slotStatusMsg(msg);

    QApplication::setOverrideCursor( Qt::WaitCursor );

    int lesson_offset = lessons->count()-1;
    for (int i = 0; i < (int) new_names.size(); i++) {
      lessons->insertItem (new_names[i]);
      old_names.push_back(new_names[i]);
    }
    doc->setLessonDescriptions(old_names);

    for (int i = 0; i < (int) new_in_query.size(); i++)
      old_in_query.push_back(new_in_query[i]+lesson_offset);
    doc->setLessonsInQuery(old_in_query);
    querymanager.setLessonItems(old_in_query);

    int types_offset = old_types.size();
    for (int i = 0; i < (int) new_types.size(); i++) {
      old_types.push_back(new_types[i]);
    }
    doc->setTypeDescriptions(old_types);
    QueryManager::setTypeNames(old_types);


    int tenses_offset = old_tenses.size();
    for (int i = 0; i < (int) new_tenses.size(); i++) {
      old_tenses.push_back(new_tenses[i]);
    }
    doc->setTenseDescriptions(old_tenses);
    KEduVocConjugation::setTenseNames(old_tenses);


    int usages_offset = old_usages.size();
    for (int i = 0; i < (int) new_usages.size(); i++) {
      old_usages.push_back(new_usages[i]);
    }
    doc->setUsageDescriptions(old_usages);
    UsageManager::setUsageNames(old_usages);


    bool equal = true;
    if (doc->originalIdentifier() != new_doc->originalIdentifier())
      equal = false;
    for (int i = 1; i < doc->numIdentifiers(); i++)
      if (doc->identifier(i) != new_doc->identifier(i))
        equal = false;

    if (equal) {   // easy way: same language codes, just append

      for (int i = 0; i < new_doc->numEntries(); i++) {
        KEduVocExpression *expr = new_doc->entry(i);

        expr->setLesson(expr->lesson()+lesson_offset);

        for (int lang = 0; lang <= (int) expr->numTranslations(); lang++) {
          QString t = expr->type (lang);
          // adjust type offset
          if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
            QString t2;
            t.remove (0, 1);
            t2.setNum (t.toInt()+types_offset);
            t2.insert (0, QM_USER_TYPE);
            expr->setType (lang, t2);
          }

          t = expr->usageLabel (lang);
          // adjust usage offset
          QString tg;
          if (!t.isEmpty() ) {
            QString t2;
            while (t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
              QString n;
              t.remove (0, 1);
              int next;
              if ((next = t.indexOf(UL_USAGE_DIV)) >= 0) {
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

          KEduVocConjugation conj = expr->conjugation(lang);
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
      QList<int> move_matrix;
      QList<bool> cs_equal;

      for (int i = 0; i < QMAX (doc->numIdentifiers(), new_doc->numIdentifiers()); i++)
        cs_equal.push_back (false);

      move_matrix.push_back(new_doc->findIdentifier(doc->originalIdentifier()));
      for (int i = 1; i < doc->numIdentifiers(); i++)
        move_matrix.push_back(new_doc->findIdentifier(doc->identifier(i)));

      for (int j = 0; j < new_doc->numEntries(); j++) {
        KEduVocExpression new_expr;
        KEduVocExpression *expr = new_doc->entry(j);
        new_expr.setLesson(expr->lesson()+lesson_offset);

        for (int i = 0; i < (int) move_matrix.size(); i++) {
          int lpos = move_matrix[i];
          if (lpos >= 0) {

            if (lpos == 0)
              s = expr->original();
            else
              s = expr->translation(lpos);

            if (!cs_equal[lpos]) {
              cs_equal[lpos] = true;
              QString id = lpos == 0 ? doc->originalIdentifier() : doc->identifier(lpos);
            }

            if (i == 0)
              new_expr.setOriginal(s);
            else
              new_expr.setTranslation(i, s);
            QString r = expr->remark(lpos);
            new_expr.setRemark (i, r);

            QString t = expr->type(lpos);
            if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
              QString t2;
              t.remove (0, 1);
              t2.setNum (t.toInt()+types_offset);
              t2.insert (0, QM_USER_TYPE);
              new_expr.setType (i, t2);
            }

            t = expr->usageLabel(lpos);
            if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
              QString t2;
              t.remove (0, 1);
              t2.setNum (t.toInt()+usages_offset);
              t2.insert (0, QM_USER_TYPE);
              new_expr.setUsageLabel (i, t2);
            }

            KEduVocConjugation conj = expr->conjugation(lpos);
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
        bool used = !new_expr.original().isEmpty();
        for (int i = 1; i < (int) doc->numIdentifiers(); i++)
          if (!new_expr.translation(i).isEmpty())
            used = true;

        if (used) {
          doc->appendEntry(&new_expr);
          doc->setModified();
        }
      }
    }
    delete (new_doc);
    fileOpenRecent->addUrl(url); // addRecentFile (url.path());
  }

  view->setView(doc, langset);
  view->getTable()->setFont(Prefs::tableFont());
  view->adjustContent();
  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotFileSave()
{
  if (entryDlg != 0)
    commitEntryDlg(false);

  if (doc->URL().fileName() == i18n("Untitled") ) {
    slotFileSaveAs();
    return;
  }

  QString msg = i18n("Saving %1", doc->URL().path());
  slotStatusMsg(msg);

  // remove previous backup
  QFile::remove(QFile::encodeName(doc->URL().path()+"~"));
  ::rename (QFile::encodeName(doc->URL().path()),
            QFile::encodeName(doc->URL().path()+"~"));

  prepareProgressBar();
  saveDocProps(doc);
  doc->saveAs(this, doc->URL(), KEduVocDocument::automatic, "KVocTrain");
  fileOpenRecent->addUrl(doc->URL());
  removeProgressBar();

  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::fillLessonBox(KEduVocDocument *the_doc)
{
  lessons->clear();
  lessons->insertItem (the_doc->lessonDescription(0));
  QStringList names = the_doc->lessonDescriptions();
  for (int i = 0; i < (int) names.size(); i++)
    lessons->insertItem (names[i]);
  act_lesson = the_doc->currentLesson();
  if (act_lesson > lessons->count() ) {
    act_lesson = 0;
    the_doc->setCurrentLesson(act_lesson);
  }
  lessons->setCurrentItem (act_lesson);
}


void kvoctrainApp::loadDocProps(KEduVocDocument *the_doc)
{
  fillLessonBox(the_doc);

  random_expr1.clear();
  random_expr2.clear();
  queryList.clear();
  the_doc->queryIdentifier(act_query_org, act_query_trans);
  if (!act_query_org.isEmpty() && !act_query_trans.isEmpty() ) {
    for (int i = 0; i < the_doc->numEntries(); i++)
    {
      KEduVocExpression *entry = the_doc->entry(i);
      if (entry->isInQuery())
      {
         int less = entry->lesson();
         for (int l = (int) queryList.size(); l <= less; l++) {
            QList<QueryEntryRef> ref_vec;
            queryList.push_back(ref_vec);
         }
         QueryEntryRef ref(entry, i);
         queryList[less].push_back(ref);
      }
    }
  }

  QueryManager::setTypeNames(doc->typeDescriptions());
  UsageManager::setUsageNames(doc->usageDescriptions());
  KEduVocConjugation::setTenseNames(doc->tenseDescriptions());

  querymanager.setLessonItems(doc->lessonsInQuery() );

  // remove empty elements
  for (int i = (int) queryList.size()-1; i >= 0; i--)
    if (queryList[i].size() == 0) {
      queryList.erase(queryList.begin() + i);
    }

  query_cycle = 1;
  query_startnum = 0;
  for (int i = 0; i < (int) queryList.size(); i++)
    query_startnum += queryList[i].size();
  query_num = query_startnum;
}


void kvoctrainApp::saveDocProps(KEduVocDocument *the_doc)
{
  the_doc->setQueryIdentifier(act_query_org, act_query_trans);
}


void kvoctrainApp::slotFileSaveAs()
{
  slotStatusMsg(i18n("Saving file under new filename..."));

  if (entryDlg != 0)
    commitEntryDlg(false);

  KUrl url = KFileDialog::getSaveURL(QString(), FILTER_WPATTERN, parentWidget(), i18n("Save Vocabulary As"));

  if (!url.isEmpty() ) {
    QFileInfo fileinfo(url.path());
    if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
       i18n("<qt>The file<br><b>%1</b><br>already exists. Do you want to overwrite it?</qt>",
        url.path()),QString(),i18n("Overwrite")) == KMessageBox::Cancel)
    {
    // do nothing
    }
    else

    if (doc) {
      QString msg = i18n("Saving %1", url.path());
      slotStatusMsg(msg);

      QFile::remove(QFile::encodeName(url.path()+"~"));         // remove previous backup
      ::rename (QFile::encodeName(url.path()), QFile::encodeName(QString(url.path()+"~")));
      saveDocProps(doc);

      prepareProgressBar();
      doc->saveAs(this, url, KEduVocDocument::automatic, "KVocTrain");
      fileOpenRecent->addUrl(doc->URL());
      removeProgressBar();
    }
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotSaveSelection ()
{
  if (entryDlg != 0)
    commitEntryDlg(false);

  slotStatusMsg(i18n("Saving selected area under new filename..."));
  QString save_separator = Prefs::separator();
  Prefs::setSeparator("\t");
  KEduVocDocument seldoc(this);
  // transfer most important parts
  seldoc.appendIdentifier(doc->originalIdentifier());
  for (int i = 1; i < doc->numIdentifiers(); i++)
    seldoc.appendIdentifier(doc->identifier(i));
  seldoc.setAuthor(doc->author());
  seldoc.setLessonDescriptions(doc->lessonDescriptions());
  seldoc.setTypeDescriptions(doc->typeDescriptions());

  for (int i = doc->numEntries()-1; i >= 0; i--)
    if (doc->entry(i)->isInQuery() )
      seldoc.appendEntry(doc->entry(i));

  KUrl url = KFileDialog::getSaveURL(QString(), FILTER_WPATTERN, parentWidget(), i18n("Save Vocabulary As"));

  if (!url.isEmpty() )
  {
    QFileInfo fileinfo(url.path());
    if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
       i18n("<qt>The file<br><b>%1</b><br>already exists. Do you want to overwrite it?</qt>",
        url.path()),QString(),i18n("Overwrite")) == KMessageBox::Cancel)
    {
    // do nothing
    }
    else
    {
      QString msg = i18n("Saving %1", url.path());
      slotStatusMsg(msg);

      QFile::remove(url.path()+"~");         // remove previous backup
      // FIXME: check error
      ::rename (QFile::encodeName(url.path()), QFile::encodeName(url.path()+"~"));
      saveDocProps(&seldoc);

      prepareProgressBar();
      seldoc.saveAs(this, url, KEduVocDocument::automatic, "KVocTrain");
      removeProgressBar();
    }
  }
  Prefs::setSeparator(save_separator);
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::prepareProgressBar ()
{
    statusBar()->clearMessage ();
    pbar = new KProgressBar (statusBar());
    //pbar->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    statusBar()->addWidget(pbar, 150, true);
    pbar->show();
}


void kvoctrainApp::removeProgressBar ()
{
    statusBar()->clearMessage ();
    statusBar()->removeWidget(pbar);
    delete pbar;
    pbar = 0;
    delete pdlg;
    pdlg = 0;
}
