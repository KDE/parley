/***************************************************************************

    $Id$

                     clipboard part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.5  2001/11/10 21:11:57  arnold
    removed icons and compatibilty for kde1

    Revision 1.4  2001/10/20 08:10:34  arnold
    fixed copying multiple lines to clipboard

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

#include <qclipboard.h>

#include <klineedit.h>
#include <klocale.h>
#include <kconfig.h>

#include <algo.h>

void kvoctrainApp::clipboardChanged() {
  QString s = QApplication::clipboard()->text();
  edit_menu->setItemEnabled(ID_EDIT_PASTE,  !s.isEmpty());
  edit_menu->setItemEnabled(ID_SEARCH_CLIP, !s.isEmpty());
  toolBar()->setItemEnabled(ID_EDIT_PASTE,  !s.isEmpty());
  toolBar()->setItemEnabled(ID_EDIT_PASTE,  !s.isEmpty());
#if defined(_WS_X11_)
//  disconnect(QApplication::clipboard(),SIGNAL(dataChanged()),
//    this,SLOT(clipboardChanged()));
#endif
}


void kvoctrainApp::slotSmartSearchClip()
{
  QString s;
  QString entries = QApplication::clipboard()->text();

  if (!entries.isEmpty()) {
    int pos = entries.find ('\n'); // search for a line end
    if (pos < 0)
      pos = entries.find ('\r');

    if (pos < 0)    // just first "line"
      s = entries;
    else
      s = entries.left(pos);

    searchpos = 0;                      // search from beginning
    searchstr = s.stripWhiteSpace();    // in case RETURN is pressed
    searchLine->setFocus();
    searchLine->setText (searchstr);
  }
}


void kvoctrainApp::aboutToShowEdit() {
  QString s = QApplication::clipboard()->text();
  edit_menu->setItemEnabled(ID_CLR_SEL,  hasSelection());
  edit_menu->setItemEnabled(ID_SAVE_ROW,  hasSelection());
  edit_menu->setItemEnabled(ID_EDIT_PASTE,  !s.isEmpty());
  edit_menu->setItemEnabled(ID_SEARCH_CLIP, !s.isEmpty());
  toolBar()->setItemEnabled(ID_SEARCH_CLIP, !s.isEmpty());
  toolBar()->setItemEnabled(ID_EDIT_PASTE,  !s.isEmpty());
}


vector<int> kvoctrainApp::getCsvOrder(kvoctrainDoc *doc,
                                      QStringList *lang_order)
{
  vector<int> csv_order;

  KConfig *config = KApplication::kApplication()->config();
  config->setGroup(CFG_GENERAL);
  bool useCurrent = config->readBoolEntry(CFG_USECURRENT, false);

  if (!useCurrent) {
    if (lang_order && lang_order->count() != 0) {
      for (int i = 0; i < (int) lang_order->count(); i++) {
        int j = doc->findIdent((*lang_order)[i]);
        if (j >= 0)
          csv_order.push_back (j);
        else
          csv_order.push_back (-1);
      }
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
      csv_order.erase(&csv_order[i], &csv_order[i+1]);
*/
  // append indices from doc if no order given
  for (int i = 0; i < doc->numLangs(); i++)
    if (::find (csv_order.begin(), csv_order.end(), i) == csv_order.end())
       csv_order.push_back(i);
/*
  if (csv_order.size() > doc->numLangs() )
    csv_order.erase(&csv_order[doc->numLangs()], csv_order.end());
*/

  // remove trailing garbage
  for (int i = csv_order.size()-1; i >= 0; i--) {
    if (csv_order[i] != -1)
      break;
    else
      csv_order.erase(&csv_order[i], &csv_order[i+1]);
  }

  return csv_order;
}


void kvoctrainApp::slotEditCopy()
{
  slotStatusMsg(i18n("Copying selection to Clipboard..."));

  QApplication::setOverrideCursor( waitCursor );
  QString exp;
  QString s;

  vector <int> csv_order = getCsvOrder(doc, &paste_order);

  RowTable *table = view->getTable();

  for (int j = table->numRows()-1; j >= 0; j--) {
    if (table->isRowSelected(j))
    {
      kvoctrainExpr *expr = table->getRow(j);
      if (expr == 0 ) return;

      bool sep =  false;
      for (int i = 0; i < (int) csv_order.size(); i++) {
        if (!sep)
          sep = true;
        else
          exp += separator;
  
        if (csv_order[i] >= 0) {
          if (csv_order[i] == 0)
            exp += expr->getOriginal();
          else
            exp += expr->getTranslation(csv_order[i]);
        }
      }
    }
    if (!exp.isEmpty())
      exp += '\n';
  }
  if (!exp.isEmpty()) {
#if defined(_WS_X11_)
//    disconnect(QApplication::clipboard(),SIGNAL(dataChanged()),this,0);
#endif
    QApplication::clipboard()->setText(exp);
#if defined(_WS_X11_)
//    connect(QApplication::clipboard(),SIGNAL(dataChanged()), this,SLOT(clipboardChanged()));
#endif
  }

  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotEditPaste()
{
  slotStatusMsg(i18n("Inserting Clipboard contents..."));

  QApplication::setOverrideCursor( waitCursor );
  QString s;
  QString entries = QApplication::clipboard()->text();

  vector <int> csv_order = getCsvOrder(doc, &paste_order);

  bool changed = false;
  QString num;
// view->setView(0, langset, gradecols);
  while (!entries.isEmpty()) {
    int pos = entries.find ('\n'); // search for a line end
    if (pos < 0) {
      pos = entries.find ('\r');   // mac style ?
    }

    if (pos < 0) {
      s = entries;
      entries = "";
    }
    else {
      s = entries.left(pos);
      entries.remove (0, pos+1);
    }

    // similar block in kvd_csv.cpp::loadFromCsv()

    if (!s.stripWhiteSpace().isEmpty()) {
      if (paste_order.count() != 0) {
        kvoctrainExpr bucket (s, separator, act_lesson);
        kvoctrainExpr expr;
        expr.setLesson(act_lesson);
        // now move columns according to paste-order
        QString s;
        for (int i = 0; i < (int) csv_order.size(); i++) {
          if (csv_order[i] >= 0) {
            if (i == 0)
              s = bucket.getOriginal();
            else
              s = bucket.getTranslation(i);
  
            if (csv_order[i] == 0)
              expr.setOriginal(s);
            else
              expr.setTranslation(csv_order[i], s);
          }
        }
        changed = true;
        doc->appendEntry (&expr);
      }
      else {
        kvoctrainExpr expr (s, separator, act_lesson);
        changed = true;
        doc->appendEntry (&expr);
      }
    }
  }

  if (changed) {
    doc->setModified();
    view->getTable()->updateContents(view->getTable()->numRows()-1, KV_COL_ORG);
  }

  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


