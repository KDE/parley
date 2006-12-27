/***************************************************************************

                     clipboard part of kvoctrain

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net

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

void KVocTrainApp::slotSmartSearchClip()
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
  //QString entries = QApplication::clipboard()->text();

  QTextStream ts;
  ts.setString(&QApplication::clipboard()->text(), QIODevice::Text);
  QList<int> csv_order = csvOrder();

  bool changed = false;
  QString num;

  while (!ts.atEnd()) {
    s = ts.readLine();
    kDebug() << s << endl;
    // similar block in kvd_csv.cpp::loadFromCsv()

    if (!s.isEmpty()) {
      m_tableModel->insertRows(m_tableModel->rowCount(QModelIndex()), 1, QModelIndex());
      QStringList sl = s.split(Prefs::separator(), QString::KeepEmptyParts);
      //KEduVocExpression expr;
      if (csv_order.count() > 0) {
        //expr.setLesson(act_lesson);
        // now move columns according to paste-order
        int j = 0;
        foreach(int i, csv_order)
        {
          kDebug() << "i= " << i << " j= " << j << endl;
          if (j < sl.count())
            m_tableModel->setData(m_tableModel->index(m_tableModel->rowCount(QModelIndex()) - 1, i + 2), sl[j], Qt::EditRole);
          j++;
        }
        /*QString s;
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
        }*/
        changed = true;
        //m_doc->appendEntry (&expr);
      }
      else {
        /*expr.setLesson(act_lesson);
        for (int i = 0; i < sl.count(); ++i)
        {
          if (i == 0)
            expr.setOriginal(sl[i]);
          else
            expr.setTranslation(i, sl[i]);
        }
        changed = true;
        m_doc->appendEntry (&expr);*/
      }
    }
  }

  if (changed) {
    m_doc->setModified();
    m_tableModel->reset();
  }

  QApplication::restoreOverrideCursor();
  slotStatusMsg(IDS_DEFAULT);
}


