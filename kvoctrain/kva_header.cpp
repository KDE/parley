/***************************************************************************

                     header buttons part of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2006-2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/************x***************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QPixmap>
#include <QMenu>

#include <kcombobox.h>
#include <klocale.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kiconloader.h>

#include "kvoctrain.h"
#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"

#define SORT_ALPHA  I18N_NOOP("&Sort alphabetically")
#define SORT_NUM    I18N_NOOP("Sort by &index")

void KVocTrainApp::slotHeaderMenu(int header, int x, int y) /*FOLD00*/
{
  delete header_m;
  header_m = 0;

  if (header == KV_COL_LESS) {
    header_m = new QMenu();
    header_m->insertItem(KIcon("sort_incr"), i18n(SORT_ALPHA), (header << 16) | IDH_SORT_COL_ALPHA);
    header_m->insertItem(KIcon("sort_num"), i18n(SORT_NUM), (header << 16) | IDH_SORT_COL_NUM);

    connect (header_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (header_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    header_m->exec(QPoint (x, y));
    return;
  }

  header -= KV_EXTRA_COLS; // adjust doc == header button

  QList<QString> names;
  QString curr_lang;
  QString id = header == 0 ? m_doc->originalIdentifier() : m_doc->identifier(header);

  if (m_languages.indexShortId(id) >= 0)
    curr_lang = m_languages.longId(m_languages.indexShortId(id));

  // select one of the available languages for the column
  QMenu *langs_m = new QMenu();
  // hack: ID => header-id + language

  for (int i = 0; i < m_languages.count(); i++) {
    if(m_languages.longId(i).isEmpty() )
      names.push_back(m_languages.shortId(i));
    else
      names.push_back(m_languages.longId(i));
  }

  for (int i = 0; i < m_languages.count(); i++) {
    if(   !m_languages.pixmapFile(i).isEmpty()
       && !m_languages.longId(i).isEmpty() )
      langs_m->insertItem(QPixmap(m_languages.pixmapFile(i)), names[i],
        (header << 16) | (i << (16+8)) | IDH_SET_LANG);
    else
      langs_m->insertItem(names[i],
        (header << 16) | (i << (16+8)) | IDH_SET_LANG);
  }
  ///@todo port connect (langs_m, SIGNAL(activated(int)), this, SLOT(slotAssignLanguage(QAction *)));
  connect (langs_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  header_m = new QMenu();

  if (header != KV_COL_ORG - KV_EXTRA_COLS ) {
    header_m->insertItem(KIcon("run_query"), i18n("Create Random &Query"), (header << 16) | IDH_START_QUERY);
    header_m->insertItem(KIcon("run_multi"), i18n("Create &Multiple Choice"), (header << 16) | IDH_START_MULTIPLE);
    header_m->setItemEnabled((header << 16) | IDH_START_MULTIPLE, m_doc->numIdentifiers() > 1);
    header_m->setItemEnabled((header << 16) | IDH_START_QUERY,  m_doc->numIdentifiers() > 1);
    header_m->addSeparator();
    header_m->insertItem(i18n("&Verbs"), (header << 16) | IDH_START_VERB);
    header_m->insertItem(i18n("&Articles"), (header << 16) | IDH_START_ARTICLE);
    header_m->insertItem(i18n("&Comparison Forms"), (header << 16) | IDH_START_ADJECTIVE);
    header_m->addSeparator();
    header_m->insertItem(i18n("S&ynonyms"), (header << 16) | IDH_START_SYNONYM);
    header_m->insertItem(i18n("A&ntonyms"), (header << 16) | IDH_START_ANTONYM);
    header_m->insertItem(i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
    header_m->insertItem(i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);
    header_m->addSeparator();
    header_m->insertItem(KIcon("sort_incr"), i18n(SORT_ALPHA), (header+KV_EXTRA_COLS << 16) | IDH_SORT_COL_ALPHA);
    header_m->insertItem(KIcon("set_language"), i18n("Set &Language"), langs_m, (2 << 16) | IDH_NULL);
    header_m->addSeparator();
    header_m->insertItem(i18n("Reset &Grades"), (header << 16) | IDH_RESET_GRADE);
    header_m->insertItem(KIcon("delete_table_col"), i18n("&Remove Column"), (header << 16) | IDH_REMOVE );
  }
  else {
    QMenu *query_m =  new QMenu();
    QMenu *multiple_m =  new QMenu();

    names.clear();
    for (int j = 1; j < (int) m_doc->numIdentifiers(); j++) {
     int i;
     if ((i = m_languages.indexShortId(m_doc->identifier(j))) >= 0)
       names.push_back(m_languages.longId(i));
     else
       names.push_back(m_doc->identifier(j));
    }

    for (int i = 1; i < (int) m_doc->numIdentifiers(); i++) {
      // show pixmap and long name if available
      int j;
      if((j = m_languages.indexShortId(m_doc->identifier(i))) >= 0
         && !m_languages.pixmapFile(j).isEmpty()
         && !m_languages.longId(j).isEmpty() ) {
        query_m->insertItem(QPixmap(m_languages.pixmapFile(j)), i18n("From %1", names[i-1]),
            (i << (16+8)) |  IDH_START_QUERY);  // hack: IDs => header-ids + cmd
      }
      else {
        query_m->insertItem(i18n("From %1", m_doc->identifier(i)), (i << (16+8)) |  IDH_START_QUERY);
      }
    }
    header_m->insertItem(KIcon("run_query"), i18n("Create Random &Query"), query_m, (3 << 16) | IDH_NULL);
    connect (query_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (query_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
    for (int i = 1; i < (int) m_doc->numIdentifiers(); i++) {
      // show pixmap and long name if available
      int j;
      if((j = m_languages.indexShortId(m_doc->identifier(i))) >= 0
         && !m_languages.pixmapFile(j).isEmpty()
         && !m_languages.longId(j).isEmpty() ) {
        multiple_m->insertItem(QPixmap(m_languages.pixmapFile(j)), i18n("From %1", names[i-1]),
            (i << (16+8)) |  IDH_START_MULTIPLE);  // hack: IDs => header-ids + cmd
      }
      else {
        multiple_m->insertItem(i18n("From %1", m_doc->identifier(i)), (i << (16+8)) |  IDH_START_MULTIPLE);
      }
    }
    header_m->insertItem(KIcon("run_multi"), i18n("Create &Multiple Choice"), multiple_m, (4 << 16) | IDH_NULL);
    header_m->addSeparator();
    header_m->insertItem(i18n("Train &Verbs"), (header << 16) | IDH_START_VERB);
    header_m->insertItem(i18n("&Article Training"), (header << 16) | IDH_START_ARTICLE);
    header_m->insertItem(i18n("&Comparison Training"), (header << 16) | IDH_START_ADJECTIVE);
    header_m->addSeparator();
    header_m->insertItem(i18n("&Synonyms"), (header << 16) | IDH_START_SYNONYM);
    header_m->insertItem(i18n("&Antonyms"), (header << 16) | IDH_START_ANTONYM);
    header_m->insertItem(i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
    header_m->insertItem(i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);
    connect (multiple_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (multiple_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    header_m->addSeparator();
    header_m->insertItem(KIcon("sort_incr"), i18n(SORT_ALPHA), (header+KV_EXTRA_COLS << 16) | IDH_SORT_COL_ALPHA);
    header_m->insertItem(KIcon("set_language"), i18n("Set &Language"), langs_m, (2 << 16) | IDH_NULL);
  }

  connect (header_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect (header_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
  header_m->exec(QPoint(x, y));
}

void KVocTrainApp::slotHeaderStatus (int header_and_cmd) /*FOLD00*/
{
  int header1 = (header_and_cmd >> 16) & 0xFF;
  int header2 = header_and_cmd >> (16+8);
  int cmd     = header_and_cmd & 0xFFFF;

//cout << header1 << " " << header2 << " " << cmd << endl;

  switch (cmd) {
    case IDH_NULL:
    break;

    case ID_RESUME_QUERY:
        slotStatusHelpMsg(i18n("Resumes random query with existing selection"));
    break;

    case ID_RESUME_MULTIPLE:
        slotStatusHelpMsg(i18n("Resumes multiple choice with existing selection"));
    break;

    case IDH_SORT_COL_ALPHA:
        slotStatusHelpMsg(i18n("Sorts column alphabetically up/down"));
    break;

    case IDH_SORT_COL_NUM:
        slotStatusHelpMsg(i18n("Sorts column by lesson index up/down"));
    break;

    case IDH_START_QUERY:
    case IDH_START_MULTIPLE: {
      QString to = header1 ? m_doc->identifier(header1) : m_doc->originalIdentifier();
      if (!m_languages.findLongId(to).isEmpty())
        to = m_languages.findLongId(to);

      QString from = header2 ? m_doc->identifier(header2) : m_doc->originalIdentifier();
      if (!m_languages.findLongId(from).isEmpty())
        from = m_languages.findLongId(from);

      QString msg;
      QString format;

      if (m_doc->numIdentifiers() == 1) {
        if (cmd == IDH_START_QUERY)
          msg = i18n("Creates and starts query to %1", to);
        else
          msg = i18n("Creates and starts multiple choice to %1", to);
      }
      else {
        if (cmd == IDH_START_QUERY)
          msg = i18n("Creates and starts query from %1 to %2", from, to);
        else
          msg = i18n("Creates and starts multiple choice from %1 to %2", from, to);
      }

      slotStatusHelpMsg(msg);
    }
    break;

    case IDH_START_VERB: {
        slotStatusHelpMsg(i18n("Starts training with verbs"));
    }
    break;

    case IDH_START_ARTICLE: {
        slotStatusHelpMsg(i18n("Starts training with articles"));
    }
    break;

    case IDH_START_ADJECTIVE: {
        slotStatusHelpMsg(i18n("Starts training with adjectives"));
    }
    break;

    case IDH_START_SYNONYM: {
        slotStatusHelpMsg(i18n("Starts training with synonyms"));
    }
    break;

    case IDH_START_ANTONYM: {
        slotStatusHelpMsg(i18n("Starts training with antonyms"));
    }
    break;

    case IDH_START_EXAMPLE: {
        slotStatusHelpMsg(i18n("Starts training with examples"));
    }
    break;

    case IDH_START_PARAPHRASE: {
        slotStatusHelpMsg(i18n("Starts training with paraphrases"));
    }
    break;

    case IDH_CREATE_LESSON: {
        slotStatusHelpMsg(i18n("Creates lesson"));
    }
    break;

    default:
      kError() << "KVocTrainApp::slotHeaderStatus: got unknown command :" << cmd << endl;
  }
}


void KVocTrainApp::slotHeaderCallBack (int header_and_cmd) /*FOLD00*/
{
  int header1 = (header_and_cmd >> 16) & 0xFF;
  int header2 = header_and_cmd >> (16+8);
  int cmd     = header_and_cmd & 0xFFFF;

  switch (cmd) {
    case IDH_SORT_COL_ALPHA:
      ///@todo port view->getTable()->sortByColumn_alpha(header1);
      return;
    break;

    case IDH_SORT_COL_NUM:
      ///@todo port view->getTable()->sortByColumn_index(header1);
      return;
    break;
  }

  if (header1 >= (int) m_doc->numIdentifiers()) {
    kError() << "header1 >= (int) doc->numIdentifiers()\n";
    return;
  }

  if (header2 >= (int) m_doc->numIdentifiers()) {
    kError() << "header2 >= (int) doc->numIdentifiers()\n";
    return;
  }

  switch (cmd) {
    case IDH_NULL:
    break;

    case ID_RESUME_QUERY:
      queryType = QT_Random;
      slotRestartQuery();
    break;

    case ID_RESUME_MULTIPLE:
      queryType = QT_Multiple;
      slotRestartQuery();
    break;

    case IDH_START_QUERY:
      delete randomQueryDlg;
      randomQueryDlg = 0;
      queryType = QT_Random;
      slotStartQuery(header1 ? m_doc->identifier(header1) : m_doc->originalIdentifier(),
                     header2 ? m_doc->identifier(header2) : m_doc->originalIdentifier(), true);
    break;

    case IDH_START_MULTIPLE:
      delete mcQueryDlg;
      mcQueryDlg = 0;
      queryType = QT_Multiple;
      slotStartQuery(header1 ? m_doc->identifier(header1) : m_doc->originalIdentifier(),
                     header2 ? m_doc->identifier(header2) : m_doc->originalIdentifier(), true);
    break;

    case IDH_START_VERB: {
      delete verbQueryDlg;
      verbQueryDlg = 0;
      queryType = QT_Conjugation;
      slotStartTypeQuery (header1, QM_VERB);
    }
    break;

    case IDH_START_ARTICLE: {
      delete artQueryDlg;
      artQueryDlg = 0;
      queryType = QT_Articles;
      slotStartTypeQuery (header1, QM_NOUN);
    }
    break;

    case IDH_START_ADJECTIVE: {
      delete adjQueryDlg;
      adjQueryDlg = 0;
      queryType = QT_Comparison;
      slotStartTypeQuery (header1, QM_ADJ);
    }
    break;

    case IDH_START_SYNONYM: {
      delete simpleQueryDlg;
      simpleQueryDlg = 0;
      slotStartPropertyQuery (header1, QT_Synonym);
    }
    break;

    case IDH_START_ANTONYM: {
      delete simpleQueryDlg;
      simpleQueryDlg = 0;
      slotStartPropertyQuery (header1, QT_Antonym);
    }
    break;

    case IDH_START_EXAMPLE: {
      delete simpleQueryDlg;
      simpleQueryDlg = 0;
      slotStartPropertyQuery (header1, QT_Example);
    }
    break;

    case IDH_START_PARAPHRASE: {
      delete simpleQueryDlg;
      simpleQueryDlg = 0;
      slotStartPropertyQuery (header1, QT_Paraphrase);
    }
    break;

    case IDH_CREATE_LESSON:
      slotCreateLesson(header1);
    break;

    default:
       kError() << "KVocTrainApp::slotHeaderCallBack: got unknown command\n";

  }
  slotStatusMsg(IDS_DEFAULT);
}


