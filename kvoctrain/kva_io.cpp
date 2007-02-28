/***************************************************************************

                         i/o part of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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
#include <qprogressbar.h>
#include <krecentdocument.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <krecentfilesaction.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <knewstuff/downloaddialog.h>

#include "kvoctrain.h"
#include "UsageManager.h"
#include "common-dialogs/ProgressDlg.h"
#include <prefs.h>

#define PATTERN_ML   I18N_NOOP("*.kvtml|KDE Vocabulary Document (*.kvtml)\n")
#define PATTERN_WQL  I18N_NOOP("*.wql|KWordQuiz Document (*.wql)\n")
#define PATTERN_PAU  I18N_NOOP("*.xml.qz *.pau.gz|Pauker Lesson (*.xml.gz *.pau.gz)\n")
#define PATTERN_VOC  I18N_NOOP("*.voc|Vokabeltrainer (*.voc)\n")
#define PATTERN_CSV  I18N_NOOP("*.csv|Text (*.csv)\n")
#define PATTERN_ALL  I18N_NOOP("*|All Files (*)")

// we can read these
#define FILTER_RPATTERN  i18n(PATTERN_ML)+i18n(PATTERN_WQL)+i18n(PATTERN_PAU)+i18n(PATTERN_VOC)+i18n(PATTERN_CSV)+i18n(PATTERN_ALL)

// we can write these
#define FILTER_WPATTERN  i18n(PATTERN_ML)+i18n(PATTERN_CSV)+i18n(PATTERN_ALL)


void KVocTrainApp::slotTimeOutBackup()
{
  if (Prefs::autoBackup() && m_doc && m_doc->isModified() ) {
    slotStatusMsg(i18n("Autobackup in progress"));
    slotFileSave();
  }
  if (Prefs::backupTime() > 0)
    btimer->start(Prefs::backupTime() * 60 * 1000);
  slotStatusMsg(IDS_DEFAULT);
}


bool KVocTrainApp::queryClose()
{
   bool erg = queryExit();
   if (erg)
     m_doc->setModified(false);  // avoid double query on exit via system menu
   return erg;
}


bool KVocTrainApp::queryExit()
{
  saveOptions();
  if (!m_doc || !m_doc->isModified() ) return true;

  bool save = (Prefs::autoSave()); //save without asking

  if (!save)
  {
     int exit = KMessageBox::warningYesNoCancel(this,
               i18n("Vocabulary is modified.\n\nSave file before exit?\n"),
               "",
               KStandardGuiItem::save(), KStandardGuiItem::discard());
     if (exit==KMessageBox::Yes) {
       save = true;   // save and exit
     }
     else if (exit == KMessageBox::No) {
       save = false;  // don't save but exit
     }
     else {
       return false;  // continue work
     }
  }

  if (save) {
    if (!m_doc->URL().isEmpty())
      slotFileSave();       // save and exit
    if (m_doc->isModified())
    {
      // Error while saving or no name
      slotFileSaveAs();
    }
  }
  return true;
}


void KVocTrainApp::slotFileQuit()
{
  ///////////////////////////////////////////////////////////////////
  // exits the Application
  if(queryExit())
    {
      m_doc->setModified(false);  // Make sure not to bother about saving again.
      kapp->quit();
    }
  else
    slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotProgress(KEduVocDocument *curr_doc, int percent)
{
  if (pdlg != 0)
    pdlg->setValue (curr_doc, percent);
  else if (pbar != 0)
    pbar->setValue (percent);
  kapp->processEvents();
}

void KVocTrainApp::slotFileNew()
{
  slotStatusMsg(i18n("Creating new file..."));

  if (queryExit() )
    createNewDocument();

  slotStatusMsg(IDS_DEFAULT);
}

void KVocTrainApp::slotFileOpen()
{
  slotStatusMsg(i18n("Opening file..."));

  if (queryExit()) {
    KUrl url = KFileDialog::getOpenUrl(QString(), FILTER_RPATTERN, this, i18n("Open Vocabulary Document"));
    loadFileFromPath(url, true);
  }

  slotStatusMsg(IDS_DEFAULT);
}

void KVocTrainApp::slotFileOpenRecent(const KUrl& url)
{
  slotStatusMsg(i18n("Opening file..."));
  if (queryExit() && fileOpenRecent->items().count() > 0)
  {
    fileOpenRecent->setCurrentItem(-1);
    loadFileFromPath(url);
  }

  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::loadFileFromPath(const KUrl & url, bool addRecent)
{
  if (!url.path().isEmpty())
  {
    delete m_doc;
    m_doc = 0;

    slotStatusMsg(i18n("Loading %1", url.path()));
    //prepareProgressBar();
    m_doc = new KEduVocDocument(this);
    m_doc->open(url, false);
    m_tableModel->setDocument(m_doc);
    m_lessonModel->setDocument(m_doc);

    removeProgressBar();
    loadDocProps();
    if (addRecent)
      fileOpenRecent->addUrl(url);
    connect (m_doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    m_doc->setModified(false);
    m_tableModel->reset();

    if (m_tableView)
      m_tableView->adjustContent();
  }
}


void KVocTrainApp::slotFileOpenExample()
{
  slotStatusMsg(i18n("Opening example file..."));

  if (queryExit() ) {
    QString s = KStandardDirs::locate("data", "kvoctrain/examples/");
    KUrl url = KFileDialog::getOpenUrl(s, FILTER_RPATTERN, this, i18n("Open Example Vocabulary Document"));
    loadFileFromPath(url, false);
    if (m_doc)
      m_doc->URL().setFileName(QString());
  }

  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotGHNS()
{
  if (!m_newStuff)
    m_newStuff = new KVTNewStuff(this);
  m_newStuff->download();
}


void KVocTrainApp::slotFileMerge()
{
  slotStatusMsg(i18n("Merging file..."));

  QString s;
  KUrl url = KFileDialog::getOpenUrl(QString(), FILTER_RPATTERN, parentWidget(), i18n("Merge Vocabulary File"));

  if (!url.isEmpty() ) {

    QString msg = i18n("Loading %1", url.path());

    slotStatusMsg(msg);
    prepareProgressBar();
    KEduVocDocument *new_doc = new KEduVocDocument(this);
    connect (new_doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    m_doc->setModified(false);
    removeProgressBar();

    QStringList old_names = m_doc->lessonDescriptions();
    QStringList new_names = new_doc->lessonDescriptions();

    QStringList old_types = m_doc->typeDescriptions();
    QStringList new_types = new_doc->typeDescriptions();

    QStringList old_tenses = m_doc->tenseDescriptions();
    QStringList new_tenses = new_doc->tenseDescriptions();

    QList<int> old_in_query = m_doc->lessonsInQuery();
    QList<int> new_in_query = new_doc->lessonsInQuery();

    QStringList old_usages = m_doc->usageDescriptions();
    QStringList new_usages = new_doc->usageDescriptions();

    msg = i18n("Merging %1", url.path());
    slotStatusMsg(msg);

    QApplication::setOverrideCursor( Qt::WaitCursor );

    int lesson_offset = m_lessonsComboBox->count()-1;
    for (int i = 0; i < (int) new_names.size(); i++) {
      m_lessonsComboBox->addItem (new_names[i]);
      old_names.push_back(new_names[i]);
    }
    m_doc->setLessonDescriptions(old_names);

    for (int i = 0; i < (int) new_in_query.size(); i++)
      old_in_query.push_back(new_in_query[i]+lesson_offset);
    m_doc->setLessonsInQuery(old_in_query);
    querymanager.setLessonItems(old_in_query);

    int types_offset = old_types.size();
    for (int i = 0; i < (int) new_types.size(); i++) {
      old_types.push_back(new_types[i]);
    }
    m_doc->setTypeDescriptions(old_types);
    QueryManager::setTypeNames(old_types);


    int tenses_offset = old_tenses.size();
    for (int i = 0; i < (int) new_tenses.size(); i++) {
      old_tenses.push_back(new_tenses[i]);
    }
    m_doc->setTenseDescriptions(old_tenses);
    KEduVocConjugation::setTenseNames(old_tenses);


    int usages_offset = old_usages.size();
    for (int i = 0; i < (int) new_usages.size(); i++) {
      old_usages.push_back(new_usages[i]);
    }
    m_doc->setUsageDescriptions(old_usages);
    UsageManager::setUsageNames(old_usages);


    bool equal = true;
    if (m_doc->originalIdentifier() != new_doc->originalIdentifier())
      equal = false;
    for (int i = 1; i < m_doc->identifierCount(); i++)
      if (m_doc->identifier(i) != new_doc->identifier(i))
        equal = false;

    if (equal) {   // easy way: same language codes, just append

      for (int i = 0; i < new_doc->entryCount(); i++) {
        KEduVocExpression *expr = new_doc->entry(i);

        expr->setLesson(expr->lesson()+lesson_offset);

        for (int lang = 0; lang <= (int) expr->translationCount(); lang++) {
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

        m_doc->appendEntry (expr);
      }
      m_doc->setModified();
    }
    else { // hard way: move entries around, most attributes get lost
      QList<int> move_matrix;
      QList<bool> cs_equal;

      for (int i = 0; i < qMax (m_doc->identifierCount(), new_doc->identifierCount()); i++)
        cs_equal.push_back (false);

      move_matrix.push_back(new_doc->findIdentifier(m_doc->originalIdentifier()));
      for (int i = 1; i < m_doc->identifierCount(); i++)
        move_matrix.push_back(new_doc->findIdentifier(m_doc->identifier(i)));

      for (int j = 0; j < new_doc->entryCount(); j++) {
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
              QString id = lpos == 0 ? m_doc->originalIdentifier() : m_doc->identifier(lpos);
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
        for (int i = 1; i < (int) m_doc->identifierCount(); i++)
          if (!new_expr.translation(i).isEmpty())
            used = true;

        if (used) {
          m_doc->appendEntry(&new_expr);
          m_doc->setModified();
        }
      }
    }
    delete (new_doc);
    fileOpenRecent->addUrl(url); // addRecentFile (url.path());
  }
  m_tableModel->reset();
  m_tableView->adjustContent();
  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotFileSave()
{
  if (entryDlg != 0)
    commitEntryDlg(false);

  if (m_doc->URL().fileName() == i18n("Untitled") ) {
    slotFileSaveAs();
    return;
  }

  QString msg = i18n("Saving %1", m_doc->URL().path());
  slotStatusMsg(msg);

  // remove previous backup
  QFile::remove(QFile::encodeName(m_doc->URL().path()+'~'));
  ::rename (QFile::encodeName(m_doc->URL().path()),
            QFile::encodeName(m_doc->URL().path()+'~'));

  prepareProgressBar();
  saveDocProps(m_doc);
  m_doc->saveAs(this, m_doc->URL(), KEduVocDocument::automatic, "KVocTrain");
  fileOpenRecent->addUrl(m_doc->URL());
  removeProgressBar();

  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotFileSaveAs()
{
  slotStatusMsg(i18n("Saving file under new filename..."));

  if (entryDlg != 0)
    commitEntryDlg(false);

  KUrl url = KFileDialog::getSaveUrl(QString(), FILTER_WPATTERN, parentWidget(), i18n("Save Vocabulary As"));

  if (!url.isEmpty() ) {
    QFileInfo fileinfo(url.path());
    if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
       i18n("<qt>The file<br><b>%1</b><br>already exists. Do you want to overwrite it?</qt>",
        url.path()),QString(),KStandardGuiItem::overwrite()) == KMessageBox::Cancel)
    {
    // do nothing
    }
    else

    if (m_doc) {
      QString msg = i18n("Saving %1", url.path());
      slotStatusMsg(msg);

      QFile::remove(QFile::encodeName(url.path()+'~')); // remove previous backup
      ::rename (QFile::encodeName(url.path()), QFile::encodeName(QString(url.path()+'~')));
      saveDocProps(m_doc);

      prepareProgressBar();
      m_doc->saveAs(this, url, KEduVocDocument::automatic, "KVocTrain");
      fileOpenRecent->addUrl(m_doc->URL());
      removeProgressBar();
    }
  }
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::fillLessonBox()
{
  m_lessonsComboBox->clear();
  m_lessonsComboBox->addItems(m_tableModel->data(m_tableModel->index(0, 0), KVTTableModel::LessonsRole).toStringList());
  m_currentLesson = m_doc->currentLesson();
  if (m_currentLesson > m_lessonsComboBox->count() ) {
    m_currentLesson = 0;
    m_doc->setCurrentLesson(m_currentLesson);
  }
  m_lessonsComboBox->setCurrentIndex(m_currentLesson);
}


void KVocTrainApp::loadDocProps()
{
  if (m_tableView)
    m_tableView->setSortingEnabled(m_doc->isSortingEnabled());
  fillLessonBox();

  random_expr1.clear();
  random_expr2.clear();
  queryList.clear();
  m_doc->queryIdentifier(act_query_org, act_query_trans);
  if (!act_query_org.isEmpty() && !act_query_trans.isEmpty() ) {
    for (int i = 0; i < m_doc->entryCount(); i++)
    {
      KEduVocExpression *entry = m_doc->entry(i);
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

  QueryManager::setTypeNames(m_doc->typeDescriptions());
  UsageManager::setUsageNames(m_doc->usageDescriptions());
  KEduVocConjugation::setTenseNames(m_doc->tenseDescriptions());

  querymanager.setLessonItems(m_doc->lessonsInQuery() );

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


void KVocTrainApp::saveDocProps(KEduVocDocument *the_doc)
{
  the_doc->setQueryIdentifier(act_query_org, act_query_trans);
}


void KVocTrainApp::slotSaveSelection ()
{
  if (entryDlg != 0)
    commitEntryDlg(false);

  slotStatusMsg(i18n("Saving selected area under new filename..."));
  QString save_separator = Prefs::separator();
  Prefs::setSeparator("\t");
  KEduVocDocument seldoc(this);
  // transfer most important parts
  seldoc.appendIdentifier(m_doc->originalIdentifier());
  for (int i = 1; i < m_doc->identifierCount(); i++)
    seldoc.appendIdentifier(m_doc->identifier(i));
  seldoc.setAuthor(m_doc->author());
  seldoc.setLessonDescriptions(m_doc->lessonDescriptions());
  seldoc.setTypeDescriptions(m_doc->typeDescriptions());

  for (int i = m_doc->entryCount()-1; i >= 0; i--)
    if (m_doc->entry(i)->isInQuery() )
      seldoc.appendEntry(m_doc->entry(i));

  KUrl url = KFileDialog::getSaveUrl(QString(), FILTER_WPATTERN, parentWidget(), i18n("Save Vocabulary As"));

  if (!url.isEmpty() )
  {
    QFileInfo fileinfo(url.path());
    if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
       i18n("<qt>The file<br><b>%1</b><br>already exists. Do you want to overwrite it?</qt>",
        url.path()),QString(),KStandardGuiItem::overwrite()) == KMessageBox::Cancel)
    {
    // do nothing
    }
    else
    {
      QString msg = i18n("Saving %1", url.path());
      slotStatusMsg(msg);

      QFile::remove(url.path()+'~'); // remove previous backup
      // FIXME: check error
      ::rename (QFile::encodeName(url.path()), QFile::encodeName(url.path()+'~'));
      saveDocProps(&seldoc);

      prepareProgressBar();
      seldoc.saveAs(this, url, KEduVocDocument::automatic, "KVocTrain");
      removeProgressBar();
    }
  }
  Prefs::setSeparator(save_separator);
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::prepareProgressBar()
{
  statusBar()->clearMessage();
  pbar = new QProgressBar(statusBar());
  statusBar()->addPermanentWidget(pbar, 150);
  pbar->show();
}


void KVocTrainApp::removeProgressBar()
{
  statusBar()->clearMessage();
  statusBar()->removeWidget(pbar);
  delete pbar;
  pbar = 0;
  delete pdlg;
  pdlg = 0;
}


void KVocTrainApp::createNewDocument()
{
  if (m_doc)
    delete m_doc;
  m_doc = 0;
  m_doc = new KEduVocDocument(this);
  m_doc->appendIdentifier(i18n("Original"));
  m_doc->appendIdentifier(i18n("Translation"));
  for (int i=0; i<20; i++)
    m_doc->appendEntry(new KEduVocExpression());
  connect (m_doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
  m_tableModel->setDocument(m_doc);
  m_lessonModel->setDocument(m_doc);
  loadDocProps();
  m_tableModel->reset();
  if (m_tableView)
    m_tableView->adjustContent();
  m_doc->setModified(false);
}
