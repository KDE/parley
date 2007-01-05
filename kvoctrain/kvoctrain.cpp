/***************************************************************************

                      main part of kvoctrain

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
#include <QTimer>
#include <QPixmap>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QProgressBar>

#include <kstatusbar.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kconfig.h>
#include <kselectaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kprinter.h>
#include <kinputdialog.h>

#include <time.h>
#include <ctype.h>

#include "kvoctrain.h"
#include "entry-dialogs/EntryDlg.h"
#include "docprop-dialogs/DocPropDlg.h"
#include "docprop-dialogs/DocPropLangDlg.h"
#include "statistik-dialogs/StatistikDlg.h"
#include "common-dialogs/kvoctrainprefs.h"
#include "prefs.h"

#define MAX_LESSON       25
#define THRESH_LESSON    KV_MIN_GRADE

/*void KVocTrainApp::slotSaveOptions()
{
   saveOptions(true);
}
*/

KVocTrainApp::~KVocTrainApp()
{
   removeEntryDlg();
   delete header_m;
   delete btimer;
   delete m_doc;
}


void KVocTrainApp::slotCancelSelection ()
{
  m_tableView->clearSelection();
}


void KVocTrainApp::slotSelectAll ()
{
  m_tableView->selectAll();
}


void KVocTrainApp::slotEditRow()
{
  slotEditEntry2(m_tableView->currentIndex());
}


void KVocTrainApp::slotEditCallBack(int res)
{
//  cout << "secb\n";
  switch (res) {
    case EntryDlg::EditCancel:
      removeEntryDlg();
    break;

    case EntryDlg::EditApply:
      commitEntryDlg(true);
      if (Prefs::smartAppend())
      {
        int row = m_tableView->currentIndex().row();
        if (row == m_tableModel->rowCount(QModelIndex()) - 1)
        {
          int col = m_tableView->currentIndex().column();
          if (col < m_tableModel->columnCount(QModelIndex()) - 1 && col >= KV_COL_ORG )
          {
            int lesson = m_doc->entry(row)->lesson();
            if (lesson >= m_lessonsComboBox->count())
              lesson = qMax (0, m_lessonsComboBox->count()-1);
            slotChooseLesson(lesson);

            QString exp;
            exp = m_doc->entry(row)->translation(col+1-KV_COL_ORG);
            if (exp.isEmpty())
              m_tableView->setCurrentIndex(m_tableModel->index(row, col + 1)); /// @todo make sure entire row is selected
          }
          else
            slotAppendRow();
        }
      }
    break;

    case EntryDlg::EditUndo:
      int row, col;
      QList<Q3TableSelection> tabsel;
      entryDlg->getCell(row, col, tabsel);
      setDataEntryDlg(row, col);
    break;
  }
}


void KVocTrainApp::commitEntryDlg(bool force)
{
//  cout << "ced\n";
   if (entryDlg == 0) {
     kError() << "KVocTrainApp::commitEntryDlg: entryDlg == 0\n";
     return;
   }

   if (!force && entryDlg->isModified() && !Prefs::autoEntryApply()) {
     if( KMessageBox::No == KMessageBox::warningYesNo(this,
                   i18n("The entry dialog contains unsaved changes.\n"
                        "Do you want to apply or discard your changes?"),
                   i18n("Unsaved Changes"),
                   KStandardGuiItem::apply(), KStandardGuiItem::discard())) {
        return;
     }
   }

   int row, col;
   QList<Q3TableSelection> tabsel;
   entryDlg->getCell(row, col, tabsel);
   int hasSel = tabsel.size() > 1;
   if (tabsel.size() == 1)
     hasSel = (tabsel[0].bottomRow() - tabsel[0].topRow()) > 0;

   fillLessonBox();
   if (!hasSel) {
     KEduVocExpression *expr = m_doc->entry(row);
     if (col >= KV_COL_ORG) {
       col -= KV_EXTRA_COLS;
       if (col == 0)
         expr->setOriginal(entryDlg->getExpr());
       else
         expr->setTranslation(col, entryDlg->getExpr());

       expr->setRemark (col, entryDlg->getRemark());
       expr->setPronunciation(col, entryDlg->getPronounce());

       expr->setSynonym (col, entryDlg->getSynonym());
       expr->setAntonym (col, entryDlg->getAntonym());
       expr->setExample (col, entryDlg->getExample());
       expr->setUsageLabel (col, entryDlg->getUsageLabel());
       expr->setParaphrase (col, entryDlg->getParaphrase());
       expr->setConjugation (col, entryDlg->getConjugation());
       expr->setComparison(col, entryDlg->getComparison() );
       expr->setMultipleChoice(col, entryDlg->getMultipleChoice() );

       expr->setFauxAmi (col, entryDlg->getFromFauxAmi(), false);
       expr->setFauxAmi (col, entryDlg->getToFauxAmi(), true);
       expr->setGrade(col, entryDlg->getFromGrade(), false);
       expr->setGrade(col, entryDlg->getToGrade(), true);
       expr->setQueryCount(col, entryDlg->getFromQCount(), false);
       expr->setQueryCount(col, entryDlg->getToQCount(), true);
       expr->setBadCount(col, entryDlg->getFromBCount(), false);
       expr->setBadCount(col, entryDlg->getToBCount(), true);
       QDateTime dt;
       dt.setTime_t(entryDlg->getFromDate());
       expr->setQueryDate(col, dt, false);
       dt.setTime_t(entryDlg->getToDate());
       expr->setQueryDate(col, dt, true);
       expr->setType (col, entryDlg->getType());

       for (int j = 0; j <= expr->numTranslations(); j++)
         if (expr->type(j).isEmpty() )
           expr->setType(j, entryDlg->getType());

       for (int j = 0; j <= expr->numTranslations(); j++)
         if (QueryManager::getMainType(expr->type(j))
               !=
             QueryManager::getMainType(entryDlg->getType()) )
           expr->setType(j, entryDlg->getType());
     }
     expr->setLesson(entryDlg->getLesson());
     expr->setActive(entryDlg->getActive());

     entryDlg->setModified(false);
     m_doc->setModified(true);
     m_tableModel->reset();
     //view->getTable()->updateCell(row, col+KV_EXTRA_COLS);
     //view->getTable()->updateCell(row, KV_COL_LESS);
   }
   else {
     col -= KV_EXTRA_COLS;
     for (int ts = 0; ts < tabsel.size(); ++ts) {
       for (int er = tabsel[ts].topRow(); er <= tabsel[ts].bottomRow(); ++er) {
         KEduVocExpression *expr = m_doc->entry(er);

         if (col >= 0) {
           // only updated "common" props in multimode
           if (entryDlg->fromGradeDirty() )
             expr->setGrade(col, entryDlg->getFromGrade(), false);
           if (entryDlg->toGradeDirty() )
             expr->setGrade(col, entryDlg->getToGrade(), true);

           if (entryDlg->fromQCountDirty() )
             expr->setQueryCount(col, entryDlg->getFromQCount(), false);
           if (entryDlg->toQCountDirty() )
              expr->setQueryCount(col, entryDlg->getToQCount(), true);

           if (entryDlg->fromBCountDirty() )
             expr->setBadCount(col, entryDlg->getFromBCount(), false);
           if (entryDlg->toBCountDirty() )
             expr->setBadCount(col, entryDlg->getToBCount(), true);
           ///@todo port
           /*
           if (entryDlg->fromDateDirty() )
             expr->setQueryDate(col, entryDlg->getFromDate(), false);
           if (entryDlg->toDateDirty() )
             expr->setQueryDate(col, entryDlg->getToDate(), true);
            */
           if (entryDlg->usageDirty() ) {
             for (int j = 0; j <= expr->numTranslations(); j++)
               expr->setUsageLabel (j, entryDlg->getUsageLabel());
           }

           if (entryDlg->typeDirty() )
             for (int j = 0; j <= expr->numTranslations(); j++)
               expr->setType(j, entryDlg->getType());
         }

         if (entryDlg->activeDirty() )
           expr->setActive(entryDlg->getActive());

         if (entryDlg->lessonDirty() )
           expr->setLesson (entryDlg->getLesson());
       }
     }
     entryDlg->setModified(false);
     m_doc->setModified(true);
     m_tableModel->reset();
     /*for (uint ts = 0; ts < tabsel.size(); ++ts)
       for (int r = tabsel[ts].topRow(); r <= tabsel[ts].bottomRow(); ++r)
         for (int c = 0; c < view->getTable()->numCols(); ++c)
           view->getTable()->updateCell(r, c);*/

   }
}


void KVocTrainApp::createEntryDlg(int row, int col)
{
  QString title, text, lang;

  int lesson = m_doc->entry(row)->lesson();
  if (lesson >= m_lessonsComboBox->count())
    lesson = qMax (0, m_lessonsComboBox->count()-1);

  bool hasSelection = (m_tableView->selectionModel()->selectedRows().count() > 1);
   if (col < KV_EXTRA_COLS) {
     title = i18n("Edit General Properties");
     col -= KV_EXTRA_COLS;
     entryDlg = new EntryDlg (
                    this,
                    m_doc,
                    hasSelection,
                    0,
                    0,
                    0,
                    0,
                    0,
                    0,
                    QDateTime(),
                    QDateTime(),
                    QString(),
                    QString(),
                    QString(),
                    lesson,
                    QString(),
                    m_doc->entry(row)->type(col),
                    QString(),
                    QString(),
                    QString(),
                    QString(),
                    QString(),
                    QString(),
                    m_doc->conjugation(0),
                    KEduVocConjugation(),
                    KEduVocComparison(),
                    KEduVocMultipleChoice(),
                    querymanager,
                    title,
                    m_doc->entry(row)->isActive());
   }
   else {
     col -= KV_EXTRA_COLS;

     if (col == 0) {
       lang = m_doc->originalIdentifier();
       text = m_doc->entry(row)->original();
       title = i18n("Edit Properties for Original");
     }
     else {
       lang = m_doc->identifier(col);
       text = m_doc->entry(row)->translation(col);
       title = i18n("Edit Properties of a Translation");
     }

     bool hasSelection = (m_tableView->selectionModel()->selectedRows().count() > 1);
     entryDlg = new EntryDlg (
                    this,
                    m_doc,
                    hasSelection,
                    m_doc->entry(row)->grade(col, false),
                    m_doc->entry(row)->grade(col, true),
                    m_doc->entry(row)->queryCount(col, false),
                    m_doc->entry(row)->queryCount(col, true),
                    m_doc->entry(row)->badCount(col, false),
                    m_doc->entry(row)->badCount(col, true),
                    m_doc->entry(row)->queryDate(col, false),
                    m_doc->entry(row)->queryDate(col, true),
                    m_doc->entry(row)->fauxAmi(col, false),
                    m_doc->entry(row)->fauxAmi(col, true),
                    text,
                    lesson,
                    m_doc->entry(row)->remark(col),
                    m_doc->entry(row)->type(col),
                    m_doc->entry(row)->pronunciation(col),
                    m_doc->entry(row)->synonym(col),
                    m_doc->entry(row)->antonym(col),
                    m_doc->entry(row)->example(col),
                    m_doc->entry(row)->usageLabel(col),
                    m_doc->entry(row)->paraphrase(col),
                    m_doc->conjugation(col),
                    m_doc->entry(row)->conjugation(col),
                    m_doc->entry(row)->comparison(col),
                    m_doc->entry(row)->multipleChoice(col),
                    querymanager,
                    title,
                    m_doc->entry(row)->isActive());
   }
   connect(entryDlg, SIGNAL(sigEditChoice(int)), this, SLOT(slotEditCallBack(int)));

   m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

   if (col == 0)
     entryDlg->setEnabled(EntryDlg::EnableOnlyOriginal);
   else
     entryDlg->setEnabled(EntryDlg::EnableAll);

   QList<Q3TableSelection> tabsel;
   entryDlg->setCell(row, col+KV_EXTRA_COLS, tabsel);
   entryDlg->show();
}


void KVocTrainApp::removeEntryDlg()
{
  if (entryDlg != 0) {
    commitEntryDlg(false);
    delete entryDlg;
    entryDlg = 0;
  }

  m_tableView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
}


void KVocTrainApp::slotEditEntry(int row, int col)
{
   if (entryDlg == 0) {
     createEntryDlg(row, col);
     return;
   }

   if (entryDlg->isModified()) {
     commitEntryDlg(false);
   }

   setDataEntryDlg(row, col);
}


void KVocTrainApp::slotEditEntry2(const QModelIndex & index)
{
  if (index.isValid())
    slotEditEntry(index.row(), index.column());
}


void KVocTrainApp::setDataEntryDlg (int row, int col)
{
//  cout << "sded\n";
   if (entryDlg == 0) {
     kError() << "KVocTrainApp::setDataEntryDlg: entryDlg == 0\n";
     return;
   }

   if ((row < 0) || (col < 0) || (m_tableModel->rowCount(QModelIndex()) <= 0))
     return;

   QString text, lang, title;

   KEduVocExpression *expr = m_doc->entry(row);

   if (expr == 0)
     return; // entry delete in the meantime

   int lesson = expr->lesson();
   if (lesson >= m_lessonsComboBox->count())
     lesson = qMax (0, m_lessonsComboBox->count() - 1);

   bool hasSelection = (m_tableView->selectionModel()->selectedRows().count() > 1);

   if (col < KV_EXTRA_COLS) {
     title = i18n("Edit General Properties");
     col -= KV_EXTRA_COLS;
     entryDlg->setData(m_doc,
                       hasSelection,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       QDateTime(),
                       QDateTime(),
                       QString(),
                       QString(),
                       QString(),
                       lesson,
                       QString(),
                       m_doc->entry(row)->type(0),
                       QString(),
                       QString(),
                       QString(),
                       QString(),
                       QString(),
                       QString(),
                       m_doc->conjugation(0),
                       KEduVocConjugation(),
                       KEduVocComparison(),
                       KEduVocMultipleChoice(),
                       querymanager,
                       title,
                       m_doc->entry(row)->isActive());
   }
   else {
     col -= KV_EXTRA_COLS;

     if (col == 0) {
       title = i18n("Edit Properties for Original");
       lang = m_doc->originalIdentifier();
       text = m_doc->entry(row)->original();
     }
     else {
       lang = m_doc->identifier(col);
       text = m_doc->entry(row)->translation(col);
       title = i18n("Edit Properties of a Translation");
     }

     entryDlg->setData(m_doc,
                       hasSelection,
                       m_doc->entry(row)->grade(col, false),
                       m_doc->entry(row)->grade(col, true),
                       m_doc->entry(row)->queryCount(col, false),
                       m_doc->entry(row)->queryCount(col, true),
                       m_doc->entry(row)->badCount(col, false),
                       m_doc->entry(row)->badCount(col, true),
                       m_doc->entry(row)->queryDate(col, false),
                       m_doc->entry(row)->queryDate(col, true),
                       m_doc->entry(row)->fauxAmi(col, false),
                       m_doc->entry(row)->fauxAmi(col, true),
                       text,
                       lesson,
                       m_doc->entry(row)->remark(col),
                       m_doc->entry(row)->type(col),
                       m_doc->entry(row)->pronunciation(col),
                       m_doc->entry(row)->synonym(col),
                       m_doc->entry(row)->antonym(col),
                       m_doc->entry(row)->example(col),
                       m_doc->entry(row)->usageLabel(col),
                       m_doc->entry(row)->paraphrase(col),
                       m_doc->conjugation(col),
                       m_doc->entry(row)->conjugation(col),
                       m_doc->entry(row)->comparison(col),
                       m_doc->entry(row)->multipleChoice(col),
                       querymanager,
                       title,
                       m_doc->entry(row)->isActive());
   }
   m_tableModel->reset();
   //view->getTable()->updateCell(row, col);
   //view->getTable()->updateCell(row, KV_COL_LESS);
   ///@todo port commented section below
   /*QList<Q3TableSelection> tabsel;
   if (hasSel) {
     for (int i = 0; i < view->getTable()->numSelections(); ++i)
       tabsel.push_back(view->getTable()->selection(i));
   }
   entryDlg->setCell(row, col+KV_EXTRA_COLS, tabsel);*/
}


void KVocTrainApp::slotDocProps ()
{
   int old_lessons = (int) m_lessonsComboBox->count();
   int old_types = (int) m_doc->typeDescriptions().size();
   int old_tenses = (int) m_doc->tenseDescriptions().size();
   int old_usages = (int) m_doc->usageDescriptions().size();
   QList<int> old_lessoninquery = m_doc->lessonsInQuery();

   DocPropsDlg ddlg (m_doc,
                     0,
                     m_lessonsComboBox,
                     m_doc->title(),
                     m_doc->author(),
                     m_doc->license(),
                     m_doc->docRemark(),
                     m_doc->typeDescriptions(),
                     m_doc->tenseDescriptions(),
                     m_doc->usageDescriptions());

   int res = ddlg.exec();

   if (res == QDialog::Accepted) {
      QList<int> typeIndex;
      QList<int> tenseIndex;
      QList<int> usageIndex;
      QList<int> lessonIndex;
      QStringList new_typeStr;
      QStringList new_tenseStr;
      QStringList new_usageStr;
      QStringList new_lessonStr;
      QList<int> new_lessoninquery;

      m_doc->enableSorting(ddlg.getSorting());

      m_doc->setTitle(ddlg.getTitle() );
      m_doc->setAuthor(ddlg.getAuthor() );
      m_doc->setLicense(ddlg.getLicense() );
      m_doc->setDocRemark(ddlg.getDocRemark() );

      slotStatusMsg(i18n("Updating lesson indices..."));
      QApplication::setOverrideCursor( Qt::WaitCursor );

      ddlg.getLesson(m_lessonsComboBox, lessonIndex);
      ddlg.getTypeNames(new_typeStr, typeIndex);
      ddlg.getTenseNames(new_tenseStr, tenseIndex);
      ddlg.getUsageLabels(new_usageStr, usageIndex);

      new_lessoninquery = old_lessoninquery;
      LessOptPage::cleanUnused(m_doc, m_lessonsComboBox, lessonIndex, old_lessons, new_lessoninquery);
      for (int i = 1; i < m_lessonsComboBox->count(); i++)
        new_lessonStr.push_back(m_lessonsComboBox->itemText(i));

      slotStatusMsg(i18n("Updating type indices..."));
      TypeOptPage::cleanUnused(m_doc, typeIndex, old_types);
      QueryManager::setTypeNames (new_typeStr);

      slotStatusMsg(i18n("Updating tense indices..."));
      TenseOptPage::cleanUnused(m_doc, tenseIndex, old_tenses);
      ///@todo port KEduVocConjugation::setTenseNames (new_tenseStr);

      slotStatusMsg(i18nc("usage (area) of an expression",
                         "Updating usage label indices..."));
      UsageOptPage::cleanUnused(m_doc, usageIndex, old_usages);
      UsageManager::setUsageNames (new_usageStr);

      m_doc->setTypeDescriptions(new_typeStr);
      m_doc->setTenseDescriptions(new_tenseStr);
      m_doc->setUsageDescriptions(new_usageStr);
      m_doc->setLessonDescriptions(new_lessonStr);
      m_doc->setLessonsInQuery(new_lessoninquery);
      querymanager.setLessonItems(new_lessoninquery);

      m_doc->setModified();
      m_tableModel->reset();
      setCaption(m_doc->title(), m_doc->isModified());
      QApplication::restoreOverrideCursor();
      slotStatusMsg(IDS_DEFAULT);
   }
}


void KVocTrainApp::slotDocPropsLang ()
{
   DocPropsLangDlg ldlg (m_doc, &m_languages);
   int res = ldlg.exec();

   if (res == QDialog::Accepted) {
      for (int i = 0; i < m_doc->numIdentifiers(); i++) {
        m_doc->setArticle(i, ldlg.getArticle(i) );
        m_doc->setConjugation(i, ldlg.getConjugation(i) );
      }

      m_doc->setModified();
      m_tableModel->reset();
      setCaption(m_doc->title(), m_doc->isModified()); 
      slotStatusMsg(IDS_DEFAULT);
   }
}


void KVocTrainApp::slotModifiedDoc(bool /*mod*/)
{
  setCaption(m_doc->title(), m_doc->isModified());
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotRemoveRow()
{
  if (m_tableView->selectionModel()->selectedRows().count() == 1) {
    if (KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18n("Do you really want to delete the selected entry?"), "", KStandardGuiItem::del()))
    {
      int currentRow = m_tableView->currentIndex().row();
      int currentColumn = m_tableView->currentIndex().column();
      m_tableModel->removeRows(m_tableView->currentIndex().row(), 1, QModelIndex());
      m_tableView->selectionModel()->setCurrentIndex(m_tableModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
  }
  else {
    if(KMessageBox::Continue == KMessageBox::warningContinueCancel(this, i18n("Do you really want to delete the selected entries?"), "", KStandardGuiItem::del()))
    {
      int currentRow = m_tableView->currentIndex().row();
      int currentColumn = m_tableView->currentIndex().column();
      int rowCount = m_tableModel->rowCount(QModelIndex());
      // Must count backwards otherwise entry-numbering goes wrong when
      // deleting.
      for (int i = rowCount - 1; i >= 0; i--)
        if (m_tableView->selectionModel()->isRowSelected(i, QModelIndex()))
          m_tableModel->removeRows(i, 1, QModelIndex());
      m_tableView->selectionModel()->setCurrentIndex(m_tableModel->index(currentRow, currentColumn), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
  }
  editDelete->setEnabled(m_tableModel->rowCount(QModelIndex()) > 0);
}


void KVocTrainApp::slotAppendRow ()
{
  m_tableModel->insertRows(m_tableModel->rowCount(QModelIndex()), 1, QModelIndex());
  m_tableView->selectionModel()->setCurrentIndex(m_tableModel->index(m_tableModel->rowCount(QModelIndex()) - 1, KV_COL_ORG), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  m_tableModel->setData(m_tableView->currentIndex(), m_currentLesson, KVTTableModel::LessonRole);
  editDelete->setEnabled(m_tableModel->rowCount(QModelIndex()) > 0);
}


void KVocTrainApp::keyReleaseEvent(QKeyEvent *e)
{
  switch(e->key()) {
    case Qt::Key_Control:  
      controlActive = false;
      break;
  }
}


void KVocTrainApp::keyPressEvent(QKeyEvent *e)
{
  controlActive = (e->modifiers() & Qt::ControlModifier) !=0;

  switch( e->key() ) {
    case Qt::Key_Plus:
      if (controlActive) {
        int less = m_lessonsComboBox->currentIndex();
        if (less == m_lessonsComboBox->count() - 1)
          m_lessonsComboBox->setCurrentIndex(0);
        else
          m_lessonsComboBox->setCurrentIndex(less+1);
        slotChooseLesson(m_lessonsComboBox->currentIndex());
      }
    break;

    case Qt::Key_Minus:
      if (controlActive) {
        int less = m_lessonsComboBox->currentIndex();
        if (less == 0)
          m_lessonsComboBox->setCurrentIndex(m_lessonsComboBox->count() - 1);
        else
          m_lessonsComboBox->setCurrentIndex(less - 1);
        slotChooseLesson(m_lessonsComboBox->currentIndex());
      }
    break;

    case Qt::Key_Control:
      controlActive = true;
      break;

    case Qt::Key_Tab:
      if (m_tableView->hasFocus() )  {
        searchLine->setFocus();
        searchLine->selectAll();
      }
      else
        m_tableView->setFocus();
    break;

    case Qt::Key_Backtab:
      if (searchLine->hasFocus() )
        m_tableView->setFocus();
      else {
        searchLine->setFocus();
        searchLine->selectAll();
      }
    break;

    default:
      bool found = false;
      if (!found)
        e->ignore();
  }
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotChooseLesson(int idx)
{
  m_currentLesson = idx;
  m_doc->setCurrentLesson(idx);
  m_doc->setModified(true);
}


void KVocTrainApp::slotCreateLesson(int header)
{
  QList<int> sel;
  m_doc->setModified();
  for (int i = 0; i < m_tableModel->rowCount(QModelIndex()); i++) {
    KEduVocExpression *kv = m_doc->entry(i);
    kv->setLesson(0);
    if (kv->grade(header) > THRESH_LESSON && !kv->translation(header).isEmpty() )
      sel.push_back(i);
  }

  int cnt = 0;
  while (cnt < MAX_LESSON && sel.size() != 0) {
    int nr = random.getLong(sel.size());
    KEduVocExpression *kv = m_doc->entry(sel[nr]);
    // include non-lesson and non-empty string
    if (kv->lesson() == 0) {
      kv->setLesson(1);
      sel.erase (sel.begin() + nr);
      cnt++;
    }
  }
  m_tableModel->reset();
}


void KVocTrainApp::slotShowStatist()
{
   StatistikDlg sdlg (m_languages, m_doc);
   sdlg.exec();
}


void KVocTrainApp::slotCleanVocabulary ()
{
   prepareProgressBar();
   QApplication::setOverrideCursor( Qt::WaitCursor );
   int num = m_doc->cleanUp();
   QApplication::restoreOverrideCursor();
   removeProgressBar();

   slotStatusMsg(IDS_DEFAULT);

   if (num != 0) {
     ///@todo port view->setView(m_doc, m_languages);
     QString s =
        i18np("1 entry with the same content has been found and removed.",
             "%n entries with the same content have been found and removed.", num);

     KMessageBox::information(this,
       s,
       i18n("Clean Up"));
   }
}


void KVocTrainApp::slotCreateRandom()
{
   bool ok = false;
   int res = KInputDialog::getInteger(i18n( "Entries in Lesson" ),
                i18n( "Enter number of entries in lesson:" ), Prefs::entriesPerLesson(), 1, 1000, 1, &ok, this );
   if (!ok)
     return;

   Prefs::setEntriesPerLesson(res);

   slotStatusMsg(i18n("Creating random lessons..."));
   QApplication::setOverrideCursor( Qt::WaitCursor );

   QList<KEduVocExpression*> randomList;
   for (int i = 0; i < m_doc->numEntries(); i++) {
     KEduVocExpression *expr = m_doc->entry(i);
     if (expr->lesson() == 0)
       randomList.push_back(expr);
   }

   if (randomList.size () != 0) {
     int less_no = m_lessonsComboBox->count() /* +1 anyway */ ;
     QString s;
     s.setNum (less_no);
     s.insert (0, "- ");
     m_lessonsComboBox->addItem (s);
     int less_cnt = Prefs::entriesPerLesson();
     while (randomList.size () != 0) {
       if (--less_cnt <= 0) {
         less_cnt = Prefs::entriesPerLesson();
         less_no++;
         s.setNum (less_no);
         s.insert (0, "- ");
         m_lessonsComboBox->addItem (s);
       }
       int nr = random.getLong(randomList.size());
       randomList[nr]->setLesson(less_no);
       randomList.erase(randomList.begin() + nr);
     }

     QStringList new_lessonStr;
     for (int i = 1; i < m_lessonsComboBox->count(); i++)
       new_lessonStr.push_back(m_lessonsComboBox->itemText(i));
     m_doc->setLessonDescriptions(new_lessonStr);
     m_tableModel->reset();
     m_doc->setModified ();
   }
   QApplication::restoreOverrideCursor();
   slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotGeneralOptions()
{
  slotGeneralOptionsPage(-1);
}


void KVocTrainApp::slotGeneralOptionsPage(int index)
{
  KVocTrainPrefs* dialog = new KVocTrainPrefs(m_languages, m_doc, m_lessonsComboBox, &querymanager, this, "settings",  Prefs::self() );
  connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
  if (index >= 0)
    dialog->selectPage(index);
  dialog->show();
}


void KVocTrainApp::slotApplyPreferences()
{
  if (pron_label)
    pron_label->setFont(Prefs::iPAFont());

  m_tableView->setFont(Prefs::tableFont());
  m_tableView->reset();

  readLanguages();
  m_tableModel->setLanguages(m_languages);
  m_tableModel->reset();
}


void KVocTrainApp::slotAppendLang(int header_and_cmd)
{
   int lang_id = (header_and_cmd >> 16) & 0xFF;

   if (lang_id == 0xFF) {
     QString msg = i18n("To append a new language which is not listed in "
                        "the submenu, you must first add its data in the "
                        "general options dialog.\n"
                        "Should this dialog be invoked now?");
    if( KMessageBox::Yes == KMessageBox::questionYesNo(this,
                  msg,
                  "",KGuiItem(i18n("Invoke Dialog")), KGuiItem(i18n("Do Not Invoke"))))
     {
       slotGeneralOptionsPage(1);
     }
     return;
   }

   if (lang_id >= (int) m_languages.size())
     return;
   m_doc->appendIdentifier("");
   int num = m_doc->numEntries()-1;
   for (int i = 0; i < (int) num; i++) {
      KEduVocExpression *expr = m_doc->entry(i);
      expr->setType (num, expr->type(0));
   }

   m_doc->setIdentifier(m_doc->numIdentifiers()-1, m_languages.shortId(lang_id));
   m_tableModel->reset();
   m_doc->setModified();
}


void KVocTrainApp::slotInitSearch()
{
  searchpos = 0;
  m_textToFind = "";
}


void KVocTrainApp::slotSearchNext()
{
  slotResumeSearch(m_textToFind);
}


void KVocTrainApp::slotResumeSearch(const QString& s)
{
  if (s.length() == 0) {
    slotInitSearch();
    return;
  }

  slotStatusMsg(i18n("Searching expression..."));
  QApplication::setOverrideCursor( Qt::WaitCursor );

  // new word or shortend word
  if (s.length() < m_textToFind.length() )
    searchpos = 0;

  // search in current col from current row till end
  // CTRL means start search from beginning of word
  bool word_beg = controlActive;
  int currentColumn = m_tableView->currentIndex().column();
  int idx = m_doc->search(s, currentColumn - KV_EXTRA_COLS, searchpos, -1, word_beg);
  if (idx >= 0) {
    m_tableView->selectionModel()->setCurrentIndex(m_tableModel->index(idx, currentColumn), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
    searchpos = idx + 1;
  }
  else { // try again from beginning up to current pos
    int idx = m_doc->search(s, currentColumn - KV_EXTRA_COLS, 0, searchpos, word_beg);
    if (idx >= 0) {
      m_tableView->selectionModel()->setCurrentIndex(m_tableModel->index(idx, currentColumn), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
      searchpos = idx+1;
    }
    else
      searchpos = 0;
  }

  m_textToFind = s;
  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotStatusMsg(const QString &/*text*/)
{
//
}


void KVocTrainApp::aboutToShowLearn()
{
  learn_menu->clear();

  QList<QString> set_names;
  for (int i = 0; i < (int) m_languages.size(); i++) {
    if(m_languages.longId(i).isEmpty() )
      set_names.push_back(m_languages.shortId(i));
    else
      set_names.push_back(m_languages.longId(i));
  }

  QList<QString> main_names;
  for (int j = 0; j < (int) m_doc->numIdentifiers(); j++) {
   int i;
   QString did = j == 0 ? m_doc->originalIdentifier() : m_doc->identifier(j);
   if ((i = m_languages.indexShortId(did)) >= 0)
     main_names.push_back(m_languages.longId(i));
   else
     main_names.push_back(did);
  }

  for (int header = 0; header < (int) m_doc->numIdentifiers(); header++) {
    // show pixmap and long name if available
    int j;
    header_m = new QMenu();
    if (header != 0 ) {
      header_m->insertItem(SmallIconSet("run_query"), i18n("Create Random &Query"), (header << 16) | IDH_START_QUERY);
      header_m->insertItem(SmallIconSet("run_multi"), i18n("Create &Multiple Choice"), (header << 16) | IDH_START_MULTIPLE);

      header_m->setItemEnabled((header << 16) | IDH_START_MULTIPLE, m_doc->numIdentifiers() > 1);
      header_m->setItemEnabled((header << 16) | IDH_START_QUERY,  m_doc->numIdentifiers() > 1);
      header_m->insertSeparator();

      header_m->insertItem(i18n("&Verbs"), (header << 16) | IDH_START_VERB);
      header_m->insertItem(i18n("&Articles"), (header << 16) | IDH_START_ARTICLE);
      header_m->insertItem(i18n("&Comparison Forms"), (header << 16) | IDH_START_ADJECTIVE);
      header_m->insertSeparator();
      header_m->insertItem(i18n("S&ynonyms"), (header << 16) | IDH_START_SYNONYM);
      header_m->insertItem(i18n("A&ntonyms"), (header << 16) | IDH_START_ANTONYM);
      header_m->insertItem(i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
      header_m->insertItem(i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);
    }
    else {
      QMenu *query_m = new QMenu();
      QMenu *multiple_m = new QMenu();

      for (int i = 1; i < (int) m_doc->numIdentifiers(); i++) {
        // show pixmap and long name if available
        int j;
        if((j = m_languages.indexShortId(m_doc->identifier(i))) >= 0
           && !m_languages.pixmapFile(j).isEmpty()
           && !m_languages.longId(j).isEmpty() ) {
          query_m->insertItem(QPixmap(m_languages.pixmapFile(j)), i18n("From %1", main_names[i]),
              (i << (16+8)) |  IDH_START_QUERY);  // hack: IDs => header-ids + cmd
        }
        else {
          query_m->insertItem(i18n("From %1", m_doc->identifier(i)), (i << (16+8)) |  IDH_START_QUERY);
        }
      }

      header_m->insertItem(SmallIconSet("run_query"), i18n("Create Random &Query"), query_m, (3 << 16) | IDH_NULL);
      connect (query_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
      connect (query_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

      for (int i = 1; i < (int) m_doc->numIdentifiers(); i++) {
        // show pixmap and long name if available
        int j;
        if((j = m_languages.indexShortId(m_doc->identifier(i))) >= 0
           && !m_languages.pixmapFile(j).isEmpty()
           && !m_languages.longId(j).isEmpty() ) {
          multiple_m->insertItem(QPixmap(m_languages.pixmapFile(j)), i18n("From %1", main_names[i]),
              (i << (16+8)) |  IDH_START_MULTIPLE);  // hack: IDs => header-ids + cmd
        }
        else {
          multiple_m->insertItem(i18n("From %1", m_doc->identifier(i)), (i << (16+8)) |  IDH_START_MULTIPLE);
        }
      }
      header_m->insertItem(SmallIconSet("run_multi"), i18n("Create &Multiple Choice"), multiple_m, (4 << 16) | IDH_NULL);
      header_m->insertSeparator();

      header_m->insertItem(i18n("Train &Verbs"), (header << 16) | IDH_START_VERB);
      header_m->insertItem(i18n("&Article Training"), (header << 16) | IDH_START_ARTICLE);
      header_m->insertItem(i18n("&Comparison Training"), (header << 16) | IDH_START_ADJECTIVE);
      header_m->insertSeparator();
      header_m->insertItem(i18n("&Synonyms"), (header << 16) | IDH_START_SYNONYM);
      header_m->insertItem(i18n("&Antonyms"), (header << 16) | IDH_START_ANTONYM);
      header_m->insertItem(i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
      header_m->insertItem(i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);

      connect (multiple_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
      connect (multiple_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
    }

    connect (header_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (header_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    QString id = header == 0 ? m_doc->originalIdentifier() : m_doc->identifier(header);
    if((j = m_languages.indexShortId(id)) >= 0
       && !m_languages.pixmapFile(j).isEmpty()
       && !m_languages.longId(j).isEmpty() ) {
      learn_menu->insertItem(QPixmap(m_languages.pixmapFile(j)), main_names[header], header_m, IDH_NULL);
    }
    else {
      learn_menu->insertItem(id, header_m, IDH_NULL);
    }
  }

  learn_menu->insertSeparator();
  learn_menu->insertItem(SmallIconSet("run_query"), i18n("Resume &Query"), ID_RESUME_QUERY );
  learn_menu->insertItem(SmallIconSet("run_multi"), i18n("Resume &Multiple Choice"), ID_RESUME_MULTIPLE );

  learn_menu->setItemEnabled(ID_RESUME_QUERY,  query_num != 0);
  learn_menu->setItemEnabled(ID_RESUME_MULTIPLE,  query_num != 0);

}

void KVocTrainApp::aboutToShowVocabAppendLanguage()
{
  if (m_doc != 0)
  {
    vocabAppendLanguage->clear();
    QMenu * add_m = vocabAppendLanguage->menu();

    QStringList names;
    for (int i = 0; i < (int) m_languages.size(); i++)
    {
      if(m_languages.longId(i).isEmpty() )
        names.append(m_languages.shortId(i));
      else
        names.append(m_languages.longId(i));
    }

    for (int i = 0; i < (int) m_languages.size(); i++)
    {
      if(!m_languages.pixmapFile(i).isEmpty() && !m_languages.longId(i).isEmpty())
        add_m->insertItem(QPixmap(m_languages.pixmapFile(i)), names[i], (i << 16) | IDH_APPEND);
      else
        add_m->insertItem(names[i], (i << 16) | IDH_APPEND);
    }

    add_m->insertItem(i18n("Another Language..."), (0xFF << 16) | IDH_APPEND);
  }
}


void KVocTrainApp::aboutToShowVocabSetLanguage()
{
  if (m_doc != 0)
  {
    vocabSetLanguage->clear();
    QMenu * set_m = vocabSetLanguage->menu();

    QStringList names;
    for (int i = 0; i < (int) m_languages.size(); i++)
    {
      if(m_languages.longId(i).isEmpty() )
        names.append(m_languages.shortId(i));
      else
        names.append(m_languages.longId(i));
    }

    for (int header = 0; header < m_doc->numIdentifiers(); ++header )
    {
      // select one of the available languages for the column
      QMenu *langs_m = new QMenu();
        // hack: ID => header-id + language

      for (int i = 0; i < (int) m_languages.size(); i++) {
        if(!m_languages.pixmapFile(i).isEmpty() && !m_languages.longId(i).isEmpty())
          langs_m->insertItem(QPixmap(m_languages.pixmapFile(i)), names[i], (header << 16) | (i << (16+8)) | IDH_SET_LANG);
        else
          langs_m->insertItem(names[i], (header << 16) | (i << (16+8)) | IDH_SET_LANG);
      }

      connect (langs_m, SIGNAL(activated(int)), this, SLOT(slotSetHeaderProp(int)));
      connect (langs_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

      if (header == 0)
        set_m->insertItem(i18n("&Original"), langs_m, (2 << 16) | IDH_NULL);
      else {
        if (m_doc->numIdentifiers() <= 2)
          set_m->insertItem(i18n("&Translation"), langs_m, (2 << 16) | IDH_NULL);
        else
          set_m->insertItem(i18n("&%1. Translation", header), langs_m, (2 << 16) | IDH_NULL);
      }
    }
  }
}


void KVocTrainApp::aboutToShowVocabRemoveLanguage()
{
  if (m_doc != 0)
  {
    vocabRemoveLanguage->clear();
    QMenu * remove_m = vocabRemoveLanguage->menu();

    QStringList names;
    for (int j = 1; j < (int) m_doc->numIdentifiers(); j++)
    {
      int i;
      if ((i = m_languages.indexShortId(m_doc->identifier(j))) >= 0)
        names.append(m_languages.longId(i));
      else
        names.append(m_doc->identifier(j));
    }

    for (int i = 1; i < (int) m_doc->numIdentifiers(); i++)
    {
      // show pixmap and long name if available
      int j;
      if((j = m_languages.indexShortId(m_doc->identifier(i))) >= 0
          && !m_languages.pixmapFile(j).isEmpty()
          && !m_languages.longId(j).isEmpty() )
        remove_m->insertItem(QPixmap(m_languages.pixmapFile(j)), names[i-1], (i << 16) |  IDH_REMOVE);  // hack: IDs => header-ids + cmd
      else
        remove_m->insertItem(m_doc->identifier(i), (i << 16) | IDH_REMOVE);
    }
  }
}


void KVocTrainApp::slotStatusHelpMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  if (pbar == 0 || !pbar->isVisible() )
    statusBar()->showMessage(text, 3000);
}

void KVocTrainApp::slotFilePrint()
{
  slotStatusMsg(i18n("Printing..."));
  KPrinter printer;
  printer.setFullPage(true);
  if (printer.setup(this))
    m_tableView->print(&printer);
  slotStatusMsg(i18n("Ready"));
}

void KVocTrainApp::slotSmartSearchClip()
{
  QString textToPaste = QApplication::clipboard()->text();

  QTextStream ts;
  ts.setString(&textToPaste, QIODevice::Text);

  if (!ts.atEnd()) {
    searchpos = 0; // search from beginning
    searchLine->setFocus();
    searchLine->setText(ts.readLine());
  }
  else
    searchLine->setFocus();
}


QList<int> KVocTrainApp::csvOrder()
{
  QList<int> csv_order;
  QStringList languageList = Prefs::pasteOrder();

  if (!Prefs::useCurrent())
  {
    foreach(QString language, languageList)
    {
      int j = m_doc->findIdentifier(language);
      if (j >= 0)
        csv_order.append(j);
      else
        csv_order.append(-1);
    }
  }
/*
  for (int i = 0; i < csv_order.size(); i++)
    cout << csv_order[i] << " ";
  cout << endl;
*/

/*  perhaps skip missing ??
  for (int i = csv_order.size()-1; i >= 0; i--)
    if (csv_order[i] == -1)
      csv_order.erase(csv_order.begin() + i);
*/
  // append indices from doc if no order given
  for (int i = 0; i < m_doc->numIdentifiers(); i++)
    if (qFind (csv_order.begin(), csv_order.end(), i) == csv_order.end())
       csv_order.append(i);
/*
  if (csv_order.size() > doc->numIdentifiers() )
    csv_order.erase(csv_order.begin() + doc->numIdentifiers(), csv_order.end());
*/

  // remove trailing garbage
  for (int i = csv_order.size()-1; i >= 0; i--) {
    if (csv_order[i] != -1)
      break;
    else
      csv_order.erase(csv_order.begin() + i);
  }

  return csv_order;
}


void KVocTrainApp::slotEditCopy()
{
  slotStatusMsg(i18n("Copying selection to clipboard..."));

  QApplication::setOverrideCursor(Qt::WaitCursor);

  QString textToCopy;
  QList<int> csv_order = csvOrder();
  QModelIndexList selectedRows = m_tableView->selectionModel()->selectedRows(0);

  foreach(QModelIndex idx, selectedRows)
  {
    bool sep = false;
    foreach(int i, csv_order)
    {
      if (!sep)
        sep = true;
      else
        textToCopy += Prefs::separator();

      if (i >= 0)
        textToCopy += m_tableModel->data(m_tableModel->index(idx.row(), i + KV_COL_ORG), Qt::DisplayRole).toString();
    }
    if (!textToCopy.isEmpty())
      textToCopy += '\n';
  }

  if (!textToCopy.isEmpty())
    QApplication::clipboard()->setText(textToCopy);

  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotEditPaste()
{
  slotStatusMsg(i18n("Inserting clipboard contents..."));

  QApplication::setOverrideCursor( Qt::WaitCursor );
  QString s;
  QString textToPaste = QApplication::clipboard()->text();

  QTextStream ts;
  ts.setString(&textToPaste, QIODevice::Text);
  QList<int> csv_order = csvOrder();

  QString num;

  while (!ts.atEnd()) {
    s = ts.readLine();
    kDebug() << s << endl;
    // similar block in kvd_csv.cpp::loadFromCsv()

    if (!s.isEmpty()) {
      m_tableModel->insertRows(m_tableModel->rowCount(QModelIndex()), 1, QModelIndex());
      QStringList sl = s.split(Prefs::separator(), QString::KeepEmptyParts);
      ///@todo check this function
      if (csv_order.count() > 0) {
        // now move columns according to paste-order
        int j = 0;
        foreach(int i, csv_order)
        {
          kDebug() << "i= " << i << " j= " << j << endl;
          if (j < sl.count()) {
            m_tableModel->setData(m_tableModel->index(m_tableModel->rowCount(QModelIndex()) - 1, i + KV_COL_ORG), sl[j], Qt::EditRole);
            m_tableModel->setData(m_tableModel->index(m_tableModel->rowCount(QModelIndex()) - 1, i + KV_COL_ORG), m_currentLesson, KVTTableModel::LessonRole);
          }
          j++;
        }
      }
      else {
        for (int i = 0; i < sl.count(); ++i)
        {
          m_tableModel->setData(m_tableModel->index(m_tableModel->rowCount(QModelIndex()) - 1, i + KV_COL_ORG), sl[i], Qt::EditRole);
          m_tableModel->setData(m_tableModel->index(m_tableModel->rowCount(QModelIndex()) - 1, i + KV_COL_ORG), m_currentLesson, KVTTableModel::LessonRole);
        }
      }
    }
  }

  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}

void KVocTrainApp::slotCurrentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  Q_UNUSED(previous);
  if (!current.isValid())
    return;

  int column = current.column() - KV_EXTRA_COLS;
  int row = current.row();
  bool noData = false;
  KEduVocExpression *expr = 0;

  if (m_doc->numEntries() <= row || m_doc->numIdentifiers() <= column || row < 0 || column < 0)
    noData = true;
  else
    expr = m_doc->entry(row);

  if (rem_label != 0)
    rem_label->setText(i18nc("Abbreviation for R)emark","R: %1", noData ? QString() : expr->remark(column)));
  if (pron_label != 0)
    pron_label->setText(i18nc("Abbreviation for P)ronouncation","P: %1", noData ? QString() : expr->pronunciation(column)));
  if (type_label != 0)
    type_label->setText(i18nc("Abbreviation for T)ype of word", "T: %1", noData ? QString() : QueryManager::typeStr(expr->type(column))));

  if (entryDlg != 0) {
    if (noData)
      entryDlg->setEnabled(EntryDlg::EnableOnlyCommon);
    else {
      if (column == 0)
        entryDlg->setEnabled(EntryDlg::EnableOnlyOriginal);
      else
        entryDlg->setEnabled(EntryDlg::EnableAll);
    }
    slotEditEntry2(current);
  }
}

#include "kvoctrain.moc"
