/***************************************************************************

                     clipboard part of kvoctrain

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

//#include <algorithm>

#include <QClipboard>

#include <klineedit.h>
#include <klocale.h>

#include "prefs.h"
#include "kvoctrain.h"

void kvoctrainApp::slotSmartSearchClip()
{
  QString s;
  QString entries = QApplication::clipboard()->text();

  if (!entries.isEmpty()) {
    int pos = entries.indexOf ('\n'); // search for a line end
    if (pos < 0)
      pos = entries.indexOf ('\r');

    if (pos < 0)    // just first "line"
      s = entries;
    else
      s = entries.left(pos);

    searchpos = 0;                      // search from beginning
    searchstr = s.simplified();    // in case RETURN is pressed
    searchLine->setFocus();
    searchLine->setText (searchstr);
  }
  else
    searchLine->setFocus();
}


QList<int> kvoctrainApp::getCsvOrder(KEduVocDocument *doc)
{
  QList<int> csv_order;
  QStringList lang_order = Prefs::pasteOrder();

  if (!Prefs::useCurrent())
  {
    if (/*lang_order &&*/ lang_order.count() != 0) {
      for (int i = 0; i < (int) lang_order.count(); i++)
      {
        int j = doc->findIdentifier((lang_order)[i]);
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
      csv_order.erase(csv_order.begin() + i);
*/
  // append indices from doc if no order given
  for (int i = 0; i < doc->numIdentifiers(); i++)
    if (qFind (csv_order.begin(), csv_order.end(), i) == csv_order.end())
       csv_order.push_back(i);
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


void kvoctrainApp::slotEditCopy()
{
  slotStatusMsg(i18n("Copying selection to clipboard..."));

  QApplication::setOverrideCursor( Qt::WaitCursor );
  QString exp;
  QString s;

  QList<int> csv_order = getCsvOrder(doc);

  KVocTrainTable *table = view->getTable();

  for (int j = table->numRows()-1; j >= 0; j--) {
    if (table->isRowSelected(j))
    {
      KEduVocExpression *expr = table->getRow(j);
      if (expr == 0 ) return;

      bool sep =  false;
      for (int i = 0; i < (int) csv_order.size(); i++) {
        if (!sep)
          sep = true;
        else
          exp += Prefs::separator();

        if (csv_order[i] >= 0) {
          if (csv_order[i] == 0)
            exp += expr->original();
          else
            exp += expr->translation(csv_order[i]);
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
  slotStatusMsg(i18n("Inserting clipboard contents..."));

  QApplication::setOverrideCursor( Qt::WaitCursor );
  QString s;
  QString entries = QApplication::clipboard()->text();

  QList<int> csv_order = getCsvOrder(doc);

  bool changed = false;
  QString num;
// view->setView(0, langset, gradecols);
  while (!entries.isEmpty()) {
    int pos = entries.indexOf ('\n'); // search for a line end
    if (pos < 0) {
      pos = entries.indexOf ('\r');   // mac style ?
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

    if (!s.simplified().isEmpty()) {
      if (Prefs::pasteOrder().count() != 0) {
        KEduVocExpression bucket (s, Prefs::separator(), act_lesson);
        KEduVocExpression expr;
        expr.setLesson(act_lesson);
        // now move columns according to paste-order
        QString s;
        for (int i = 0; i < (int) csv_order.size(); i++) {
          if (csv_order[i] >= 0) {
            if (i == 0)
              s = bucket.original();
            else
              s = bucket.translation(i);

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
        KEduVocExpression expr (s, Prefs::separator(), act_lesson);
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


