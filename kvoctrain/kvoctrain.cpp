/***************************************************************************

    $Id$

                      main part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.33  2002/01/07 17:54:37  arnold
    fixed typo

    Revision 1.32  2002/01/07 17:19:44  arnold
    fixed i18n() issues

    Revision 1.31  2001/12/30 18:41:38  arnold
    improved reaction upon 'empty' query selections

    Revision 1.30  2001/12/30 12:12:32  arnold
    fixed smart appending and editing

    Revision 1.29  2001/12/29 15:49:11  arnold
    fixed kmessagebox calls

    Revision 1.28  2001/12/29 10:40:03  arnold
    merged fixes from POST-branch

    Revision 1.27  2001/12/26 13:15:21  arnold
    fixed header buttons and kbd handling

    Revision 1.26  2001/12/16 16:51:25  arnold
    fixed keyboard handling in main view

    Revision 1.25  2001/12/13 18:39:09  arnold
    added phonetic alphabet stuff

    Revision 1.24  2001/12/01 11:28:13  arnold
    fixed flickering in query dialogs

    Revision 1.23  2001/11/25 11:11:02  arnold
    switch for inline edit, splitted kv_resource.h

    Revision 1.22  2001/11/20 01:38:03  jsinger
    Proofreading changes for grammar and spelling. No functional changes.

    Revision 1.21  2001/11/18 12:28:25  arnold
    provided menu entry for example files

    Revision 1.20  2001/11/17 17:58:22  arnold
    added inline editing of all columns

    Revision 1.19  2001/11/16 19:50:06  arnold
    added submenu to set language to main menu

    Revision 1.18  2001/11/16 18:52:59  arnold
    added possibility to disable expressions

    Revision 1.17  2001/11/11 12:51:45  arnold
    fixed some strings for i18n purposes

    Revision 1.16  2001/11/10 22:59:17  arnold
    fixed some bugs

    Revision 1.15  2001/11/10 22:27:08  arnold
    removed compatibility for kde1

    Revision 1.14  2001/11/10 21:11:57  arnold
    removed icons and compatibilty for kde1

    Revision 1.13  2001/11/09 14:18:00  arnold
    fixed and improved some dialog pages

    Revision 1.12  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.11  2001/11/02 10:17:48  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.10  2001/11/01 11:26:12  arnold
    fixed some editing actions

    Revision 1.9  2001/10/31 19:10:56  arnold
    fixed generation of i18n messages

    Revision 1.8  2001/10/30 14:09:46  arnold
    added property 'multiple choice'

    Revision 1.7  2001/10/21 15:28:43  arnold
    removed all the 'charset' stuff

    Revision 1.6  2001/10/20 12:08:37  arnold
    removed aboutdialog files

    Revision 1.5  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.4  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.3  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.2  2001/10/12 20:55:31  arnold
    added include file

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

#define S_RESUME_QUERY i18n("Resume &query")
#define S_RESUME_MULTI i18n("Resume &multiple choice")

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT

  S_RESUME_QUERY
  S_RESUME_MULTI

#else

#include <kstatusbar.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kprogress.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include <qcursor.h>
#include <qtimer.h>
#include <qfile.h>
#include <qtooltip.h>
#include <qprogressbar.h>

#include <time.h>
#include <ctype.h>

#include <kvoctraincore.h>
#include "kvoctraindoc.h"
#include "langset.h"
#include "kvoctrain.h"
#include "entry-dialogs/EntryDlg.h"
#include "option-dialogs/GeneralOptionsDlg.h"
#include "docprop-dialogs/DocPropDlg.h"
#include "docprop-dialogs/DocPropLangDlg.h"
#include "common-dialogs/ProgressDlg.h"
#include "statistik-dialogs/StatistikDlg.h"
#include "common-dialogs/NumLessonDlg.h"


void kvoctrainApp::slotSaveOptions()
{
   saveOptions(true);
}


kvoctrainApp::~kvoctrainApp()
{
   delete header_m;
   delete btimer;
   delete view;
   delete doc;
}


void kvoctrainApp::slotCancelSelection ()
{
  view->getTable()->clearSelection();
}


void kvoctrainApp::slotSelectAll ()
{
  QTableSelection ts;
  RowTable *table = view->getTable();
  table->clearSelection();
  ts.init(0,0);
  ts.expandTo(table->numRows()-1, table->numCols()-1);
  table->addSelection(ts);
}


void kvoctrainApp::slotCurrentCellChanged(int row, int col)
{
  if (col < KV_EXTRA_COLS) {
    if (rem_label != 0)
      rem_label->setText  (i18n (PREFIX_Remark));
    if (pron_label != 0)
      pron_label->setText (i18n (PREFIX_Pronunce));
    if (type_label != 0)
      type_label->setText (i18n (PREFIX_Type));
    return;
  }

  col -= KV_EXTRA_COLS;

  statusBar()->clear();
  if (  doc->numEntries() <= row
      ||doc->numLangs() <= col
      || row < 0
      || col < 0) {
    if (rem_label != 0)
      rem_label->setText  (i18n (PREFIX_Remark));
    if (pron_label != 0)
      pron_label->setText (i18n (PREFIX_Pronunce));
    if (type_label != 0)
      type_label->setText (i18n (PREFIX_Type));
    return;
  }

  kvoctrainExpr *expr = doc->getEntry(row);

  if (rem_label != 0)
    rem_label->setText (QString(i18n  (PREFIX_Remark))+expr->getRemark (col));
  if (pron_label != 0)
    pron_label->setText (QString(i18n (PREFIX_Pronunce))+expr->getPronunce (col));
  if (type_label != 0)
    type_label->setText (QString(i18n (PREFIX_Type))
            +QueryManager::typeStr(expr->getType (col)));
}


void kvoctrainApp::slotEditRow()
{
  slotEditEntry (view->getTable()->currentRow(),
                 view->getTable()->currentColumn());
}

/*
void kvoctrainApp::slotSelectEntry (int row, int col, int key_state)
{
   if (view->getTable()->numRows() <= 0
       || row < 0
       || col < 0)
     return;

   if (col == KV_COL_LESS) {
     if (key_state != 0) { // some shift/ctrl key
       if (doc->getEntry(row)->getLesson () == 0)
         doc->getEntry(row)->setLesson (act_lesson);
       else
         doc->getEntry(row)->setLesson (0);
     }
   }
   else if (col == KV_COL_MARK) {
     // Quick'N'dirty
     kvoctrainExpr *expr = doc->getEntry(row);
     if (expr->isActive() && !expr->isInQuery()) {
       expr->setInQuery();
     }
     else if (expr->isActive() && expr->isInQuery()) {
       expr->setInQuery(false);
       expr->setActive(false);
     }
     else if (!expr->isActive() && !expr->isInQuery()) {
       expr->setActive(true);
     }
   }

   doc->setModified(true);
   // update current row with all cols
   for (int i = 0; i < view->getTable()->numCols(); i++)
     view->getTable()->updateCell(row, i);
}
*/

bool kvoctrainApp::slotEditEntry (int row, int col)
{
   QString s1,
           s2,
           text,
           lang;

   if ((row < 0) ||
       (col < 0))
     return false;

   if (col < KV_EXTRA_COLS)
      return false;

   if (view->getTable()->numRows() <= 0)
     return false;

   col -= KV_EXTRA_COLS;

   s1.setNum(row);
   s2.setNum(col);

   if (col == 0) {
     lang = doc->getOriginalIdent();
     text = doc->getEntry(row)->getOriginal();
   }
   else {
     lang = doc->getIdent(col);
     text = doc->getEntry(row)->getTranslation(col);
   }

   int lesson = doc->getEntry(row)->getLesson();
   if (lesson >= lessons->count())
     lesson = QMAX (0, lessons->count()-1);

   QString title;
   if (col == 0)
     title = i18n("Edit properties for original");
   else
     title = i18n("Edit properties of translation");

   bool hasSel = hasSelection();

   int res;
   EntryDlg edlg (doc,
                  hasSel,
                  col==0,
                  doc->getEntry(row)->getGrade(col, false),
                  doc->getEntry(row)->getGrade(col, true),
                  doc->getEntry(row)->getQueryCount(col, false),
                  doc->getEntry(row)->getQueryCount(col, true),
                  doc->getEntry(row)->getBadCount(col, false),
                  doc->getEntry(row)->getBadCount(col, true),
                  doc->getEntry(row)->getQueryDate(col, false),
                  doc->getEntry(row)->getQueryDate(col, true),
                  doc->getEntry(row)->getFauxAmi(col, false),
                  doc->getEntry(row)->getFauxAmi(col, true),
                  text,
                  lesson,
                  lessons,
                  lang,
                  langset,
                  doc->getEntry(row)->getRemark(col),
                  doc->getEntry(row)->getType(col),
                  doc->getEntry(row)->getPronunce(col),
                  doc->getEntry(row)->getSynonym(col),
                  doc->getEntry(row)->getAntonym(col),
                  doc->getEntry(row)->getExample(col),
                  doc->getEntry(row)->getUsageLabel(col),
                  doc->getEntry(row)->getParaphrase(col),
                  doc->getConjugation(col),
                  doc->getEntry(row)->getConjugation(col),
                  doc->getArticle(col),
                  doc->getEntry(row)->getComparison(col),
                  doc->getEntry(row)->getMultipleChoice(col),
                  querymanager,
                  title,
                  doc->getEntry(row)->isActive(),
                  ipafont);

   edlg.initFocus();
   res = edlg.exec();

   if (res != QDialog::Accepted)
      return false;

   fillLessonBox(doc);

   if (!hasSel)
   {
     kvoctrainExpr *expr = doc->getEntry(row);
     if (col == 0)
       expr->setOriginal(edlg.getExpr());
     else
       expr->setTranslation(col, edlg.getExpr());

     // do the same for "append entry"

     expr->setRemark (col, edlg.getRemark());
     expr->setPronunce (col, edlg.getPronunce());

     expr->setSynonym (col, edlg.getSynonym());
     expr->setAntonym (col, edlg.getAntonym());
     expr->setExample (col, edlg.getExample());
     expr->setUsageLabel (col, edlg.getUsageLabel());
     expr->setParaphrase (col, edlg.getParaphrase());
     expr->setConjugation (col, edlg.getConjugation());
     expr->setComparison(col, edlg.getComparison() );
     expr->setMultipleChoice(col, edlg.getMultipleChoice() );

     expr->setFauxAmi (col, edlg.getFromFauxAmi(), false);
     expr->setFauxAmi (col, edlg.getToFauxAmi(), true);
     expr->setGrade(col, edlg.getFromGrade(), false);
     expr->setGrade(col, edlg.getToGrade(), true);
     expr->setQueryCount(col, edlg.getFromQCount(), false);
     expr->setQueryCount(col, edlg.getToQCount(), true);
     expr->setBadCount(col, edlg.getFromBCount(), false);
     expr->setBadCount(col, edlg.getToBCount(), true);
     expr->setQueryDate(col, edlg.getFromDate(), false);
     expr->setQueryDate(col, edlg.getToDate(), true);
     expr->setLesson (edlg.getLesson());
     expr->setActive(edlg.getActive());
     expr->setType (col, edlg.getType());

     for (int i = 0; i <= expr->numTranslations(); i++)
       if (expr->getType(i).isEmpty() )
         expr->setType(i, edlg.getType());

     for (int i = 0; i <= expr->numTranslations(); i++)
       if (QueryManager::getMainType(expr->getType(i))
             !=
           QueryManager::getMainType(edlg.getType()) )
         expr->setType(i, edlg.getType());
     view->getTable()->updateCell(row, col+KV_EXTRA_COLS);
   }
   else
   {
     RowTable *table = view->getTable();

     int numRows = table->numRows();
     for (int i = 0; i < numRows; i++)
     {
       if (table->isRowSelected(i))
       {
         kvoctrainExpr *expr = table->getRow(i);
         // only updated "common" props in multimode
         if (edlg.fromGradeDirty() )
           expr->setGrade(col, edlg.getFromGrade(), false);
         if (edlg.toGradeDirty() )
           expr->setGrade(col, edlg.getToGrade(), true);

         if (edlg.fromQCountDirty() )
           expr->setQueryCount(col, edlg.getFromQCount(), false);
         if (edlg.toQCountDirty() )
            expr->setQueryCount(col, edlg.getToQCount(), true);

         if (edlg.fromBCountDirty() )
           expr->setBadCount(col, edlg.getFromBCount(), false);
         if (edlg.toBCountDirty() )
           expr->setBadCount(col, edlg.getToBCount(), true);

         if (edlg.fromDateDirty() )
           expr->setQueryDate(col, edlg.getFromDate(), false);
         if (edlg.toDateDirty() )
           expr->setQueryDate(col, edlg.getToDate(), true);

         if (edlg.lessonDirty() )
           expr->setLesson (edlg.getLesson());

         if (edlg.typeDirty() )
         {
           for (int i = 0; i <= expr->numTranslations(); i++)
             expr->setType(i, edlg.getType());
         }

         if (edlg.activeDirty() )
           expr->setActive(edlg.getActive());

       }
     }
     table->updateContents();
   }

   doc->setModified(true);
   slotCurrentCellChanged(view->getTable()->currentRow(),
                          view->getTable()->currentColumn());

   return true;
}


void kvoctrainApp::slotDocProps ()
{
   int old_lessons = (int) lessons->count();
   int old_types = (int) doc->getTypeDescr().size();
   int old_tenses = (int) doc->getTenseDescr().size();
   int old_usages = (int) doc->getUsageDescr().size();
   vector<int> old_lessoninquery = doc->getLessonsInQuery();

   DocPropsDlg ddlg (doc,
                     0,
                     lessons,
                     doc->getTitle(),
                     doc->getAuthor(),
                     doc->getLicense(),
                     doc->getDocRemark(),
                     doc->getTypeDescr(),
                     doc->getTenseDescr(),
                     doc->getUsageDescr());

   int res = ddlg.exec();

   if (res == QDialog::Accepted) {
      vector<int> typeIndex;
      vector<int> tenseIndex;
      vector<int> usageIndex;
      vector<int> lessonIndex;
      vector<QString> new_typeStr;
      vector<QString> new_tenseStr;
      vector<QString> new_usageStr;
      vector<QString> new_lessonStr;
      vector<int> new_lessoninquery;

      doc->allowSorting(ddlg.getSorting() );

      doc->setTitle(ddlg.getTitle() );
      doc->setAuthor(ddlg.getAuthor() );
      doc->setLicense(ddlg.getLicense() );
      doc->setDocRemark(ddlg.getDocRemark() );

      slotStatusMsg(i18n("updating lesson indices..."));
      QApplication::setOverrideCursor( waitCursor );

      ddlg.getLesson(lessons, lessonIndex);
      ddlg.getTypeNames(new_typeStr, typeIndex);
      ddlg.getTenseNames(new_tenseStr, tenseIndex);
      ddlg.getUsageLabels(new_usageStr, usageIndex);

      new_lessoninquery = old_lessoninquery;
      LessOptPage::cleanUnused(doc, lessons, lessonIndex, old_lessons, new_lessoninquery);
      for (int i = 1; i < lessons->count(); i++)
        new_lessonStr.push_back(lessons->text(i));

      slotStatusMsg(i18n("updating type indices..."));
      TypeOptPage::cleanUnused(doc, typeIndex, old_types);
      QueryManager::setTypeNames (new_typeStr);

      slotStatusMsg(i18n("updating tense indices..."));
      TenseOptPage::cleanUnused(doc, tenseIndex, old_tenses);
      Conjugation::setTenseNames (new_tenseStr);

      slotStatusMsg(i18n("usage (area) of an expression",
                         "updating usage label indices..."));
      UsageOptPage::cleanUnused(doc, usageIndex, old_usages);
      UsageManager::setUsageNames (new_usageStr);

      doc->setTypeDescr (new_typeStr);
      doc->setTenseDescr (new_tenseStr);
      doc->setUsageDescr (new_usageStr);
      doc->setLessonDescr (new_lessonStr);
      doc->setLessonsInQuery(new_lessoninquery);
      querymanager.setLessonItems(new_lessoninquery);

      doc->setModified();
      view->getTable()->updateContents();

      setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));

      QApplication::restoreOverrideCursor();
      slotStatusMsg(IDS_DEFAULT);
   }
}


void kvoctrainApp::slotDocPropsLang ()
{
   DocPropsLangDlg ldlg (doc, &langset);
   int res = ldlg.exec();

   if (res == QDialog::Accepted) {
      for (int i = 0; i < doc->numLangs(); i++) {
        doc->setArticle(i, ldlg.getArticle(i) );
        doc->setConjugation(i, ldlg.getConjugation(i) );
      }

      doc->setModified();
      view->getTable()->updateContents();
      setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));

      slotStatusMsg(IDS_DEFAULT);
   }
}


void kvoctrainApp::slotModifiedDoc(bool mod)
{
  setCaption(kapp->makeStdCaption(doc->getTitle(), false, doc->isModified()));
  toolBar()->setItemEnabled(ID_FILE_SAVE, mod);
  slotStatusMsg(IDS_DEFAULT);
}


bool kvoctrainApp::hasSelection()
{
  int num = view->getTable()->numSelections();
  if (num < 1) return false;
  if (num > 1) return true;
  QTableSelection ts = view->getTable()->selection(0);
  return (ts.bottomRow() - ts.topRow()) > 0;
}


void kvoctrainApp::slotRemoveRow()
{
  view->setInlineEnabled(false);

  if (!hasSelection()) {
    if( KMessageBox::Yes == KMessageBox::questionYesNo(this,
                  i18n("Do you really want to delete the selected entry ?\n"),
                  kapp->makeStdCaption("")))
    {
      RowTable *table = view->getTable();
      doc->removeEntry(table->currentRow());
      doc->setModified();
      table->updateContents();
    }
  }
  else {
    if(KMessageBox::Yes == KMessageBox::questionYesNo(this,
                  i18n("Do you really want to delete the selected range ?\n"),
                  kapp->makeStdCaption("")));
    {
      RowTable *table = view->getTable();

      int numRows = table->numRows();
      // Must count backwards otherwise entry-numbering goes wrong when
      // deleting.
      for (int i = numRows-1; i >= 0; i--)
        if (table->isRowSelected(i) )
          doc->removeEntry(i);
      doc->setModified();
      table->updateContents();
    }
  }
  view->setInlineEnabled(inline_edit);
}


void kvoctrainApp::slotAppendRow ()
{
  if (inline_edit) {
    kvoctrainExpr expr;
    expr.setLesson(act_lesson);
    doc->appendEntry(&expr);
    doc->setModified();
    int row = doc->numEntries()-1;
    view->getTable()->updateContents(row, KV_COL_ORG);
    return;
  }


  int res;
  do {
    EntryDlg edlg (doc,
                   false, true,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   "",
                   "",
                   "",
                   act_lesson,
                   lessons,
                   doc->getOriginalIdent(),
                   langset,
                   "",
                   "",
                   "",
                   "",
                   "",
                   "",
                   "",
                   0,
                   doc->getConjugation(0),
                   Conjugation(),
                   doc->getArticle(0),
                   Comparison(),
                   MultipleChoice(),
                   querymanager,
                   i18n("Enter new original expression"),
                   true,
                   ipafont);
    edlg.initFocus();
    res = edlg.exec();

    if (res == QDialog::Accepted) {
      fillLessonBox(doc);
      kvoctrainExpr expr (edlg.getExpr(), edlg.getLesson());
      expr.setRemark (0, edlg.getRemark());
      expr.setType (0, edlg.getType());
      expr.setPronunce (0, edlg.getPronunce());
      expr.setSynonym (0, edlg.getSynonym());
      expr.setAntonym (0, edlg.getAntonym());
      expr.setExample (0, edlg.getExample());
      expr.setUsageLabel (0, edlg.getUsageLabel());
      expr.setParaphrase (0, edlg.getParaphrase());
      expr.setConjugation (0, edlg.getConjugation());
      expr.setComparison(0, edlg.getComparison() );
      expr.setMultipleChoice(0, edlg.getMultipleChoice() );
      expr.setLesson (edlg.getLesson());
      expr.setActive (edlg.getActive());
      expr.setType (0, edlg.getType());

      for (int i = 1; i <= doc->numLangs(); i++)
         expr.setType (i, edlg.getType());

      doc->appendEntry(&expr);
      doc->setModified();
      int row = doc->numEntries()-1;
      view->getTable()->updateContents(row, KV_COL_ORG);

      // enter all new translations
      if (smartAppend) {
        slotChooseLesson(edlg.getLesson());

        bool nextcol = smartAppend;
        for (int i = 2; nextcol && i <= doc->numLangs(); i++) {
          if ((nextcol = slotEditEntry (row, i-1+KV_EXTRA_COLS))) {
            int lesson = doc->getEntry(row)->getLesson();
            if (lesson >= lessons->count())
              lesson = QMAX (0, lessons->count()-1);
            slotChooseLesson(lesson);
          }
          else
            return;
         view->getTable()->updateContents(row, i+KV_EXTRA_COLS);
        }
      }
    }
  } while (res == QDialog::Accepted && smartAppend);

}


void kvoctrainApp::keyReleaseEvent( QKeyEvent *e )
{
  switch( e->key() ) {
    case Key_Shift:  shiftActive = false;
    break;

    case Key_Alt:  altActive = false;
    break;

    case Key_Control:  controlActive = false;
    break;
  }
}


void kvoctrainApp::keyPressEvent( QKeyEvent *e )
{
  controlActive = (e->state() & ControlButton) != 0;
  shiftActive = (e->state() & ShiftButton) != 0;
  altActive = (e->state() & AltButton) != 0;

  switch( e->key() ) {
    case Key_Plus:
      if (controlActive) {
        int less = lessons->currentItem();
        if (less == lessons->count()-1)
          lessons->setCurrentItem(0);
        else
          lessons->setCurrentItem(less+1);
        slotChooseLesson(lessons->currentItem());
      }
    break;

    case Key_Minus:
      if (controlActive) {
        int less = lessons->currentItem();
        if (less == 0)
          lessons->setCurrentItem(lessons->count()-1);
        else
          lessons->setCurrentItem(less-1);
        slotChooseLesson(lessons->currentItem());
      }
    break;

    case Key_Shift:  shiftActive = true;
    break;

    case Key_Alt:  altActive = true;
    break;

    case Key_Control:  controlActive = true;
    break;

    case Key_Tab:
      if (view->getTable()->hasFocus() )  {
        searchLine->setFocus();
        searchLine->selectAll();
      }
      else
        view->getTable()->setFocus();
    break;

    case Key_Backtab:
      if (searchLine->hasFocus() )
        view->getTable()->setFocus();
      else {
        searchLine->setFocus();
        searchLine->selectAll();
      }
    break;

    case Key_Delete:
      slotRemoveRow();
    break;

    case Key_Insert: {
      slotAppendRow();
    }
    break;

    default:
      bool found = false;
      if (!found)
        e->ignore();
  }
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotChooseLesson(int idx)
{
  act_lesson = idx;
  doc->setCurrentLesson(idx);
  doc->setModified(true);
}


void kvoctrainApp::slotCreateLesson(int header)
{
  vector <int> sel;
  doc->setModified();
  for (int i = 0; i < view->getTable()->count(); i++) {
    kvoctrainExpr *kv = view->getTable()->getRow(i);
    kv->setLesson(0);
    if (kv->getGrade(header) > THRESH_LESSON
        && !kv->getTranslation(header).isEmpty() )
      sel.push_back(i);
  }

  int cnt = 0;
  srand((unsigned int)time((time_t *)NULL));
  while (cnt < MAX_LESSON && sel.size() != 0) {
    int nr = (int) (sel.size() * ((1.0*rand())/RAND_MAX));
    kvoctrainExpr *kv = view->getTable()->getRow(sel[nr]);
    // include non-lesson and non-empty string
    if (kv->getLesson() == 0) {
      kv->setLesson(1);
      sel.erase (&sel[nr], &sel[nr+1]);
      cnt++;
    }
  }
  view->getTable()->updateContents();
}


void kvoctrainApp::slotShowStatist()
{
   StatistikDlg sdlg (langset, doc, &gradecols);
   sdlg.exec();
}


void kvoctrainApp::slotCleanVocabulary ()
{
   prepareProgressBar();
   QApplication::setOverrideCursor( waitCursor );
   int num = doc->cleanUp();
   QApplication::restoreOverrideCursor();
   removeProgressBar();

   slotStatusMsg(IDS_DEFAULT);

   if (num != 0) {
     view->setView(doc, langset, gradecols);
     QString s =
        i18n("%1 entries with same content have been found and removed").arg(num);

     KMessageBox::information(this,
       s,
       kapp->makeStdCaption(i18n("Clean up")));
   }
}


void kvoctrainApp::slotCreateRandom()
{
   NumLessonDlg numDlg (entriesPerLesson, this, "name");
   if (numDlg.exec() != QDialog::Accepted)
     return;

   entriesPerLesson = numDlg.getInput();

   slotStatusMsg(i18n("Creating random lessons..."));
   QApplication::setOverrideCursor( waitCursor );

   vector<kvoctrainExpr*> random;
   for (int i = 0; i < doc->numEntries(); i++) {
     kvoctrainExpr *expr = doc->getEntry(i);
     if (expr->getLesson() == 0)
       random.push_back(expr);
   }

   if (random.size () != 0) {
     srand((unsigned int)time((time_t *)NULL));
     int less_no = lessons->count() /* +1 anyway */ ;
     QString s;
     s.setNum (less_no);
     s.insert (0, "- ");
     lessons->insertItem (s);
     int less_cnt = entriesPerLesson;
     while (random.size () != 0) {
       if (--less_cnt <= 0) {
         less_cnt = entriesPerLesson;
         less_no++;
         s.setNum (less_no);
         s.insert (0, "- ");
         lessons->insertItem (s);
       }
       int nr = (int) (random.size() * ((1.0*rand())/RAND_MAX));
       random[nr]->setLesson (less_no);
       random.erase(&random[nr], &random[nr+1]);
     }

     vector<QString> new_lessonStr;
     for (int i = 1; i < lessons->count(); i++)
       new_lessonStr.push_back(lessons->text(i));
     doc->setLessonDescr (new_lessonStr);
     view->getTable()->updateContents();
     doc->setModified ();
   }
   QApplication::restoreOverrideCursor();
   slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotGeneralOptions()
{
  slotGeneralOptionsPage(-1);
}


void kvoctrainApp::slotGeneralOptionsPage(int index)
{
   QString defTrans;
   GeneralOptionsDlg godlg (defTrans,
                    separator,
                    backupTime /(60*1000),
                    langset,
                    lastPixName,
                    lessons,
                    paste_order,
                    useCurrent,
                    doc,
                    tablefont,
                    ipafont,
                    &querymanager,
                    gradecols,
                    header_resizer,
                    smartAppend,
                    autosaveopts);

   if (index >= 0)
     godlg.selectPage(index);

   int res = godlg.exec();
   if (res == QDialog::Accepted) {

      defTrans = godlg.getDefaultLang();
      backupTime = godlg.getBackupTime()*60*1000;
      smartAppend = godlg.getSmartAppend();
      autosaveopts = godlg.getAutoSaveOpts();
      separator = godlg.getSeparator();
      langset = godlg.getLangSet();
      paste_order = godlg.getPasteOrder();
      useCurrent = godlg.getUseCurrent(),
      tablefont = godlg.getFont();
      ipafont = godlg.getIPAFont();
      gradecols = godlg.getGradeCols();
      header_resizer = godlg.getResizer();

      // rather ugly hack to keep useCurrent "globally" up to date
      KConfig *config = KApplication::kApplication()->config();
      config->setGroup(CFG_GENERAL);
      config->writeEntry(CFG_USECURRENT, useCurrent);

      if (pron_label)
        pron_label->setFont(ipafont);
      view->getTable()->setFont(tablefont);
      view->setResizer (header_resizer);
      view->getTable()->updateContents();

      // update header buttons
      for (int i = 0; i < (int) doc->numLangs(); i++) {
        QString sid = i>0 ? doc->getIdent(i)
                          : doc->getOriginalIdent();

        int idx = langset.indexShortId(sid);
        QString pm = "";
        QString lid = sid;;
        if  (idx >= 0) {
          lid = langset.longId(idx);
          pm = langset.PixMapFile(idx);
        }
        view->setHeaderProp (i+KV_EXTRA_COLS, lid, pm);
      }

      slotStatusMsg(IDS_DEFAULT);
   }
}


void kvoctrainApp::slotAppendLang(int header_and_cmd)
{
   int lang_id = (header_and_cmd >> 16) & 0xFF;

   if (lang_id == 0xFF) {
     QString msg = i18n("To append a new language which is not listed in "
                        "the submenu, you must first add its data in the "
                        "general options dialog.\n"
                        "Should this dialog be invoked now ?");
    if( KMessageBox::Yes == KMessageBox::questionYesNo(this,
                  msg,
                  kapp->makeStdCaption("")));
     {
       slotGeneralOptionsPage(1);
     }
     return;
   }

   if (lang_id >= (int) langset.size())
     return;

   doc->appendLang("");
   int num = doc->numEntries()-1;
   for (int i = 0; i < (int) num; i++) {
      kvoctrainExpr *expr = doc->getEntry(i);
      expr->setType (num, expr->getType(0));
   }

   doc->setIdent(doc->numLangs()-1, langset.shortId(lang_id));
   view->setView(doc, langset, gradecols);
   doc->setModified();
}


void kvoctrainApp::slotInitSearch()
{
  searchpos = 0;
  searchstr = "";
}


void kvoctrainApp::slotSearchNext()
{
  slotResumeSearch(searchstr);
}


void kvoctrainApp::slotResumeSearch(const QString& s)
{
  if (s.length() == 0) {
    slotInitSearch();
    return;
  }

  slotStatusMsg(i18n("Searching expression..."));
  QApplication::setOverrideCursor( waitCursor );

  // new word or shortend word
  if (s.length() < searchstr.length() )
    searchpos = 0;

  // search in current col from current row till end
  // SHIFT means start search from beginning of word
  bool word_beg = controlActive;
  int idx = doc->search(s, view->getTable()->currentColumn()-KV_EXTRA_COLS, searchpos, -1, word_beg, false);
  if (idx >= 0) {
    view->getTable()->setCurrentRow(idx, view->getTable()->currentColumn());
    searchpos = idx+1;
  }
  else { // try again from beginning up to current pos
    int idx = doc->search(s, view->getTable()->currentColumn()-KV_EXTRA_COLS, 0, searchpos, word_beg, false);
    if (idx >= 0) {
      view->getTable()->setCurrentRow(idx, view->getTable()->currentColumn());
      searchpos = idx+1;
    }
    else
      searchpos = 0;
  }

  searchstr = s;
  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}

void kvoctrainApp::slotViewToolBar()
{
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off
  bViewToolbar=!bViewToolbar;
  menuBar()->setItemChecked(ID_VIEW_TOOLBAR, bViewToolbar);
  if (bViewToolbar)
     toolBar()->show();
  else
     toolBar()->hide();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotViewInline()
{
  inline_edit = !inline_edit;
  menuBar()->setItemChecked(ID_VIEW_INLINE, inline_edit);
  view->setInlineEnabled(inline_edit);
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotViewStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off
  bViewStatusbar=!bViewStatusbar;
  menuBar()->setItemChecked(ID_VIEW_STATUSBAR, bViewStatusbar);
  if (bViewStatusbar)
    statusBar()->show();
  else
    statusBar()->hide();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotStatusMsg(const QString &/*text*/)
{
/*
  ///////////////////////////////////////////////////////////////////
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text, ID_STATUS_MSG );
*/
}


void kvoctrainApp::invokeHelp( void )
{
  QFile helpfile (locate ("data", "default/kvoctrain/index.html"));
  if (helpfile.exists() )
    kapp->invokeHTMLHelp("kvoctrain/index.html", QString() );
  else
//   fall back to english if no native !
    kapp->invokeHTMLHelp("../en/kvoctrain/index.html", QString());
}


void kvoctrainApp::aboutToShowLearn()
{
  learn_menu->clear();

  vector<QString> set_names;
  for (int i = 0; i < (int) langset.size(); i++) {
    if(langset.longId(i).isEmpty() )
      set_names.push_back(langset.shortId(i));
    else
      set_names.push_back(langset.longId(i));
  }

  vector<QString> main_names;
  for (int j = 0; j < (int) doc->numLangs(); j++) {
   int i;
   QString did = j == 0 ? doc->getOriginalIdent()
                        : doc->getIdent(j);
   if ((i = langset.indexShortId(did)) >= 0)
     main_names.push_back(langset.longId(i));
   else
     main_names.push_back(did);
  }

  for (int header = 0; header < (int) doc->numLangs(); header++) {
    // show pixmap and long name if available
    int j;
    header_m = new QPopupMenu();
    if (header != 0 ) {
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-query.xpm")), i18n("Create random &query"), (header << 16) | IDH_START_QUERY);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-multi.xpm")), i18n("Create &multiple choice"), (header << 16) | IDH_START_MULTIPLE);

      header_m->setItemEnabled((header << 16) | IDH_START_MULTIPLE, doc->numLangs() > 1);
      header_m->setItemEnabled((header << 16) | IDH_START_QUERY,  doc->numLangs() > 1);
      header_m->insertSeparator();

      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-verb.xpm")), i18n("&Verbs"), (header << 16) | IDH_START_VERB);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-art.xpm")), i18n("&Articles"), (header << 16) | IDH_START_ARTICLE);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-adj.xpm")), i18n("&Comparison forms"), (header << 16) | IDH_START_ADJECTIVE);
      header_m->insertSeparator();
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-syno.xpm")), i18n("S&ynonyms"), (header << 16) | IDH_START_SYNONYM);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-anto.xpm")), i18n("A&ntonyms"), (header << 16) | IDH_START_ANTONYM);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-exmp.xpm")), i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-para.xpm")), i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);
    }
    else {
      QPopupMenu *query_m =  new QPopupMenu();
      QPopupMenu *multiple_m =  new QPopupMenu();

      for (int i = 1; i < (int) doc->numLangs(); i++) {
        // show pixmap and long name if available
        int j;
        if((j = langset.indexShortId(doc->getIdent(i))) >= 0
           && !langset.PixMapFile(j).isEmpty()
           && !langset.longId(j).isEmpty() ) {
          query_m->insertItem(QPixmap(langset.PixMapFile(j)), i18n("from ")+main_names[i],
              (i << (16+8)) |  IDH_START_QUERY);  // hack: IDs => header-ids + cmd
        }
        else {
          query_m->insertItem(i18n("from ")+doc->getIdent(i), (i << (16+8)) |  IDH_START_QUERY);
        }
      }

      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-query.xpm")), i18n("Create random &query"), query_m, (3 << 16) | IDH_NULL);
      connect (query_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
      connect (query_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

      for (int i = 1; i < (int) doc->numLangs(); i++) {
        // show pixmap and long name if available
        int j;
        if((j = langset.indexShortId(doc->getIdent(i))) >= 0
           && !langset.PixMapFile(j).isEmpty()
           && !langset.longId(j).isEmpty() ) {
          multiple_m->insertItem(QPixmap(langset.PixMapFile(j)), i18n("from ")+main_names[i],
              (i << (16+8)) |  IDH_START_MULTIPLE);  // hack: IDs => header-ids + cmd
        }
        else {
          multiple_m->insertItem(i18n("from ")+doc->getIdent(i), (i << (16+8)) |  IDH_START_MULTIPLE);
        }
      }
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-multi.xpm")), i18n("Create &multiple choice"), multiple_m, (4 << 16) | IDH_NULL);
      header_m->insertSeparator();

      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-verb.xpm")), i18n("Train &verbs"), (header << 16) | IDH_START_VERB);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-art.xpm")), i18n("&Article training"), (header << 16) | IDH_START_ARTICLE);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-adj.xpm")), i18n("&Comparison training"), (header << 16) | IDH_START_ADJECTIVE);
      header_m->insertSeparator();
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-syno.xpm")), i18n("&Synonyms"), (header << 16) | IDH_START_SYNONYM);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-anto.xpm")), i18n("&Antonyms"), (header << 16) | IDH_START_ANTONYM);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-exmp.xpm")), i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
      header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-para.xpm")), i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);

      connect (multiple_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
      connect (multiple_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
    }

    connect (header_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (header_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    QString id = header == 0 ? doc->getOriginalIdent()
                             : doc->getIdent(header);
    if((j = langset.indexShortId(id)) >= 0
       && !langset.PixMapFile(j).isEmpty()
       && !langset.longId(j).isEmpty() ) {
      learn_menu->insertItem(QPixmap(langset.PixMapFile(j)), main_names[header], header_m, IDH_NULL);
    }
    else {
      learn_menu->insertItem(id, header_m, IDH_NULL);
    }
  }

  learn_menu->insertSeparator();
  learn_menu->insertItem(QPixmap(locate("data", "kvoctrain/run-query.xpm")), S_RESUME_QUERY, ID_RESUME_QUERY );
  learn_menu->insertItem(QPixmap(locate("data", "kvoctrain/run-multi.xpm")), S_RESUME_MULTI, ID_RESUME_MULTIPLE );
  learn_menu->setItemEnabled(ID_RESUME_QUERY,  query_num != 0);
  learn_menu->setItemEnabled(ID_RESUME_MULTIPLE,  query_num != 0);

}


#undef  S_RESUME_QUERY
#undef  S_RESUME_MULTI


void kvoctrainApp::aboutToShowOptions()
{
  opts_menu->setItemEnabled(ID_SAVE_OPTIONS, !autosaveopts);
}


void kvoctrainApp::aboutToShowVocabulary() {
//  voc_menu->setItemEnabled(ID_RESUME_QUERY,  query_num != 0);

  if (doc != 0) {
    int pos = voc_menu->indexOf(ID_APPEND_LANG);
    voc_menu->removeItem (ID_APPEND_LANG);
    vector<QString> names;

    QPopupMenu *add_m = new QPopupMenu();

    for (int i = 0; i < (int) langset.size(); i++) {
      if(langset.longId(i).isEmpty() )
        names.push_back(langset.shortId(i));
      else
        names.push_back(langset.longId(i));
    }

    for (int i = 0; i < (int) langset.size(); i++) {
      if(   !langset.PixMapFile(i).isEmpty()
         && !langset.longId(i).isEmpty() )
        add_m->insertItem(QPixmap(langset.PixMapFile(i)), names[i],
          (i << 16) | IDH_APPEND);
      else
        add_m->insertItem(names[i],
          (i << 16) | IDH_APPEND);
    }
    add_m->insertItem(i18n("Another language"), (0xFF << 16) | IDH_APPEND);

    connect (add_m, SIGNAL(activated(int)), this, SLOT(slotAppendLang(int)));
    connect (add_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/append-col.xpm")),
                   i18n("&Append language"), add_m, ID_APPEND_LANG,
                   pos);


    pos = voc_menu->indexOf(ID_SET_LANG);
    voc_menu->removeItem (ID_SET_LANG);
    QPopupMenu *set_m = new QPopupMenu();

    for (unsigned header = 0; header < doc->numLangs(); ++header ) {
      // select one of the available languages for the column
      QPopupMenu *langs_m = new QPopupMenu();
      // hack: ID => header-id + language

      for (int i = 0; i < (int) langset.size(); i++) {
        if(   !langset.PixMapFile(i).isEmpty()
           && !langset.longId(i).isEmpty() )
          langs_m->insertItem(QPixmap(langset.PixMapFile(i)), names[i],
            (header << 16) | (i << (16+8)) | IDH_SET_LANG);
        else
          langs_m->insertItem(names[i],
            (header << 16) | (i << (16+8)) | IDH_SET_LANG);
      }
      connect (langs_m, SIGNAL(activated(int)), this, SLOT(slotSetHeaderProp(int)));
      connect (langs_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
      if (header == 0)
        set_m->insertItem(i18n("&Original"), langs_m, (2 << 16) | IDH_NULL);
      else {
        if (doc->numLangs() <= 2)
          set_m->insertItem(i18n("&Translation"), langs_m, (2 << 16) | IDH_NULL);
        else
          set_m->insertItem(i18n("&%1. Translation").arg(header), langs_m, (2 << 16) | IDH_NULL);
      }
    }
    voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/flags.xpm")),
                   i18n("Set &language"), set_m, ID_SET_LANG,
                   pos);



    pos = voc_menu->indexOf(ID_REMOVE_LANG);
    voc_menu->removeItem (ID_REMOVE_LANG);
    QPopupMenu *remove_m = new QPopupMenu();
    names.clear();
    for (int j = 1; j < (int) doc->numLangs(); j++) {
     int i;
     if ((i = langset.indexShortId(doc->getIdent(j))) >= 0)
       names.push_back(langset.longId(i));
     else
       names.push_back(doc->getIdent(j));
    }

    for (int i = 1; i < (int) doc->numLangs(); i++) {
      // show pixmap and long name if available
      int j;
      if((j = langset.indexShortId(doc->getIdent(i))) >= 0
         && !langset.PixMapFile(j).isEmpty()
         && !langset.longId(j).isEmpty() ) {
        remove_m->insertItem(QPixmap(langset.PixMapFile(j)), names[i-1],
            (i << 16) |  IDH_REMOVE);  // hack: IDs => header-ids + cmd
      }
      else {
        remove_m->insertItem(doc->getIdent(i), (i << 16) | IDH_REMOVE);
      }
    }
    voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/delete-col.xpm")),
                   i18n("&Remove language"), remove_m, ID_REMOVE_LANG,
                   pos);
    connect (remove_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (remove_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
  }
}


void kvoctrainApp::slotStatusHelpMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  if (pbar == 0 || !pbar->isVisible() )
    statusBar()->message(text, 3000);
}


void kvoctrainApp::commandCallback(int id_){
  id_ &=0xffff;
  switch (id_){
    ON_CMD(ID_FILE_NEW,                 slotFileNew())
    ON_CMD(ID_FILE_OPEN,                slotFileOpen())
    ON_CMD(ID_FILE_OPEN_XMP,            slotFileOpenExample())
    ON_CMD(ID_FILE_MERGE,               slotFileMerge())
    ON_CMD(ID_FILE_SAVE,                slotFileSave())
    ON_CMD(ID_FILE_SAVE_AS,             slotFileSaveAs())

    ON_CMD(ID_FILE_QUIT,                slotFileQuit())

    ON_CMD(ID_EDIT_COPY,                slotEditCopy())
    ON_CMD(ID_EDIT_PASTE,               slotEditPaste())
    ON_CMD(ID_SEARCH_CLIP,              slotSmartSearchClip())
    ON_CMD(ID_APPEND_ROW,               slotAppendRow())
    ON_CMD(ID_REMOVE_ROW,               slotRemoveRow())
    ON_CMD(ID_EDIT_ROW,                 slotEditRow())
    ON_CMD(ID_CLR_SEL,                  slotCancelSelection())
    ON_CMD(ID_SEL_ALL,                  slotSelectAll())
    ON_CMD(ID_SAVE_ROW,                 slotSaveSelection())

//    ON_CMD(ID_APPEND_LANG,              slotAppendLang())
    ON_CMD(ID_DOC_PROPS,                slotDocProps())
    ON_CMD(ID_DOC_PROPS_LANG,           slotDocPropsLang())
    ON_CMD(ID_GENERAL_OPTIONS,          slotGeneralOptions())
    ON_CMD(ID_QUERY_OPTIONS,            slotQueryOptions())
    ON_CMD(ID_SHOW_STAT,                slotShowStatist())
    ON_CMD(ID_RAND_CREATE,              slotCreateRandom())
    ON_CMD(ID_CLEANUP,                  slotCleanVocabulary())
//    ON_CMD(ID_RESUME_QUERY,             slotTimeOutRandomQuery())
//    ON_CMD(ID_RESUME_MULTIPLE,          slotTimeOutMultipleChoice())

    ON_CMD(ID_VIEW_INLINE,              slotViewInline())
    ON_CMD(ID_VIEW_TOOLBAR,             slotViewToolBar())
    ON_CMD(ID_VIEW_STATUSBAR,           slotViewStatusBar())
    ON_CMD(ID_SAVE_OPTIONS,             slotSaveOptions())
  }
}


void kvoctrainApp::statusCallback(int id_){
  id_ &= 0xffff;
  switch (id_){
    ON_STATUS_MSG(ID_FILE_NEW,          i18n("Creates a new document"))
    ON_STATUS_MSG(ID_FILE_OPEN,         i18n("Opens an existing document"))
    ON_STATUS_MSG(ID_FILE_OPEN_XMP,     i18n("Opens an example document from the kvoctrain package"))
    ON_STATUS_MSG(ID_FILE_OPEN_RECENT,  i18n("Opens one of your recent documents"))
    ON_STATUS_MSG(ID_FILE_MERGE,        i18n("Merges an existing document to the current vocabulary"))
    ON_STATUS_MSG(ID_FILE_SAVE,         i18n("Saves the current document"))
    ON_STATUS_MSG(ID_FILE_SAVE_AS,      i18n("Saves the document as..."))
    ON_STATUS_MSG(ID_FILE_QUIT,         i18n("Exits the program"))

    ON_STATUS_MSG(ID_EDIT_COPY,         i18n("Copies the selected section to the clipboard"))
    ON_STATUS_MSG(ID_EDIT_PASTE,        i18n("Pastes the clipboard contents to the end"))
    ON_STATUS_MSG(ID_SEARCH_CLIP,       i18n("Searches clipboard contents in vocabulary"))
    ON_STATUS_MSG(ID_APPEND_ROW,        i18n("Appends new entry to vocabulary"))
    ON_STATUS_MSG(ID_REMOVE_ROW,        i18n("Removes selected entries from the vocabulary"))
    ON_STATUS_MSG(ID_EDIT_ROW,          i18n("Edits properties of current selection"))
    ON_STATUS_MSG(ID_SAVE_ROW,          i18n("Writes selected rows to a file"))
    ON_STATUS_MSG(ID_SEL_ALL,           i18n("Selects all entries"))
    ON_STATUS_MSG(ID_CLR_SEL,           i18n("Deselects all entries"))

    ON_STATUS_MSG(ID_VIEW_TOOLBAR,      i18n("Enables / disables the current toolbar"))
    ON_STATUS_MSG(ID_VIEW_STATUSBAR,    i18n("Enables / disables the statusbar"))
    ON_STATUS_MSG(ID_VIEW_INLINE,       i18n("Enables / disables inline editing in the table view"))
    ON_STATUS_MSG(ID_SAVE_OPTIONS,      i18n("Saves options"))
    ON_STATUS_MSG(ID_DOC_PROPS,         i18n("Edits document properties"))
    ON_STATUS_MSG(ID_DOC_PROPS_LANG,    i18n("Edits language properties in current document"))

//    ON_STATUS_MSG(ID_APPEND_LANG,       i18n("Appends a column for a new language to the table"))
    ON_STATUS_MSG(ID_GENERAL_OPTIONS,   i18n("Shows general options dialog"))
    ON_STATUS_MSG(ID_QUERY_OPTIONS,     i18n("Shows query options dialog"))
    ON_STATUS_MSG(ID_SEARCH,            i18n("Enters smart search mode"))
    ON_STATUS_MSG(ID_SHOW_STAT,         i18n("Shows statistics"))
    ON_STATUS_MSG(ID_RAND_CREATE,       i18n("Creates random lessons with unassigned entries"))
    ON_STATUS_MSG(ID_CLEANUP,           i18n("Removes entries with same content from vocabulary"))
//    ON_STATUS_MSG(ID_RESUME_QUERY,      i18n("Resumes random query with existing selection"))
//    ON_STATUS_MSG(ID_RESUME_MULTIPLE,   i18n("Resumes multiple choice with existing selection"))
  }
}

#include "kvoctrain.moc"

#endif // __ONLY_TO_BE_SEEN_BY_XGETTEXT

