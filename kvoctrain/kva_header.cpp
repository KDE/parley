/***************************************************************************

                     header buttons part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

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

#define SORT_ALPHA  I18N_NOOP("&Sort alphabetically")
#define SORT_NUM    I18N_NOOP("Sort by &index")

#include "kvoctrain.h"
#include "query-dialogs/RandomQueryDlg.h"
#include "query-dialogs/MCQueryDlg.h"
#include "query-dialogs/AdjQueryDlg.h"
#include "query-dialogs/VerbQueryDlg.h"
#include "query-dialogs/ArtQueryDlg.h"
#include "query-dialogs/SimpleQueryDlg.h"


#include <kcombobox.h>
#include <klocale.h>
#include <kdebug.h>
#include <kstandarddirs.h>

void kvoctrainApp::slotHeaderMenu(int header, int x, int y) /*FOLD00*/
{
  delete header_m;
  header_m = 0;

  if (header == KV_COL_LESS) {
    header_m = new QPopupMenu();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/sort_alpha.xpm")), i18n(SORT_ALPHA), (header << 16) | IDH_SORT_COL_ALPHA);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/sort_num.xpm")), i18n(SORT_NUM), (header << 16) | IDH_SORT_COL_NUM);

    connect (header_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (header_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    header_m->exec(QPoint (x, y));
    return;
  }

  header -= KV_EXTRA_COLS; // adjust doc == header button

  vector<QString> names;
  QString curr_lang;
  QString id = header == 0 ? doc->getOriginalIdent()
                           : doc->getIdent(header);

  if (langset.indexShortId(id) >= 0)
    curr_lang = langset.longId(langset.indexShortId(id));

  // select one of the available languages for the column
  QPopupMenu *langs_m = new QPopupMenu();
  // hack: ID => header-id + language

  for (int i = 0; i < (int) langset.size(); i++) {
    if(langset.longId(i).isEmpty() )
      names.push_back(langset.shortId(i));
    else
      names.push_back(langset.longId(i));
  }

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

  header_m = new QPopupMenu();

  if (header != KV_COL_ORG - KV_EXTRA_COLS ) {
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-query.xpm")), i18n("Create Random &Query"), (header << 16) | IDH_START_QUERY);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-multi.xpm")), i18n("Create &Multiple Choice"), (header << 16) | IDH_START_MULTIPLE);
    header_m->setItemEnabled((header << 16) | IDH_START_MULTIPLE, doc->numLangs() > 1);
    header_m->setItemEnabled((header << 16) | IDH_START_QUERY,  doc->numLangs() > 1);
    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-verb.xpm")), i18n("&Verbs"), (header << 16) | IDH_START_VERB);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-art.xpm")), i18n("&Articles"), (header << 16) | IDH_START_ARTICLE);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-adj.xpm")), i18n("&Comparison Forms"), (header << 16) | IDH_START_ADJECTIVE);
    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-syno.xpm")), i18n("S&ynonyms"), (header << 16) | IDH_START_SYNONYM);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-anto.xpm")), i18n("A&ntonyms"), (header << 16) | IDH_START_ANTONYM);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-exmp.xpm")), i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-para.xpm")), i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);
    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/sort_alpha.xpm")), i18n(SORT_ALPHA), (header+KV_EXTRA_COLS << 16) | IDH_SORT_COL_ALPHA);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/flags.xpm")), i18n("Set &Language"), langs_m, (2 << 16) | IDH_NULL);
    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/reset.xpm")), i18n("Reset &Grades"), (header << 16) | IDH_RESET_GRADE);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/delete-col.xpm")), i18n("&Remove Column"), (header << 16) | IDH_REMOVE );
  }
  else {
    QPopupMenu *query_m =  new QPopupMenu();
    QPopupMenu *multiple_m =  new QPopupMenu();

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
        query_m->insertItem(QPixmap(langset.PixMapFile(j)), i18n("from %1").arg(names[i-1]),
            (i << (16+8)) |  IDH_START_QUERY);  // hack: IDs => header-ids + cmd
      }
      else {
        query_m->insertItem(i18n("from %1").arg(doc->getIdent(i)), (i << (16+8)) |  IDH_START_QUERY);
      }
    }
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-query.xpm")), i18n("Create Random &Query"), query_m, (3 << 16) | IDH_NULL);
    connect (query_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (query_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
    for (int i = 1; i < (int) doc->numLangs(); i++) {
      // show pixmap and long name if available
      int j;
      if((j = langset.indexShortId(doc->getIdent(i))) >= 0
         && !langset.PixMapFile(j).isEmpty()
         && !langset.longId(j).isEmpty() ) {
        multiple_m->insertItem(QPixmap(langset.PixMapFile(j)), i18n("from %1").arg(names[i-1]),
            (i << (16+8)) |  IDH_START_MULTIPLE);  // hack: IDs => header-ids + cmd
      }
      else {
        multiple_m->insertItem(i18n("from %1").arg(doc->getIdent(i)), (i << (16+8)) |  IDH_START_MULTIPLE);
      }
    }
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-multi.xpm")), i18n("Create &Multiple Choice"), multiple_m, (4 << 16) | IDH_NULL);
    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-verb.xpm")), i18n("Train &Verbs"), (header << 16) | IDH_START_VERB);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-art.xpm")), i18n("&Article Training"), (header << 16) | IDH_START_ARTICLE);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-adj.xpm")), i18n("&Comparison Training"), (header << 16) | IDH_START_ADJECTIVE);
    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-syno.xpm")), i18n("&Synonyms"), (header << 16) | IDH_START_SYNONYM);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-anto.xpm")), i18n("&Antonyms"), (header << 16) | IDH_START_ANTONYM);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-exmp.xpm")), i18n("E&xamples"), (header << 16) | IDH_START_EXAMPLE);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/run-para.xpm")), i18n("&Paraphrase"), (header << 16) | IDH_START_PARAPHRASE);
    connect (multiple_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
    connect (multiple_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

    header_m->insertSeparator();
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/sort_alpha.xpm")), i18n(SORT_ALPHA), (header+KV_EXTRA_COLS << 16) | IDH_SORT_COL_ALPHA);
    header_m->insertItem(QPixmap(locate("data", "kvoctrain/flags.xpm")), i18n("Set &Language"), langs_m, (2 << 16) | IDH_NULL);
  }

  connect (header_m, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect (header_m, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
  header_m->exec(QPoint(x, y));
}


void kvoctrainApp::slotSetHeaderProp (int header_and_id) /*FOLD00*/
{
  int header1 = (header_and_id >> 16) & 0xFF;  // org, trans1, trans2, ..
  int id = header_and_id >> (16+8);            // language ident
//int cmd  = header_and_id & 0xFFFF;           // always SET_LANG

  if (id >= (int) langset.size())
    return; // ???

  QString lid = langset.shortId(id);
  if  (!langset.longId(id).isEmpty() )
    lid = langset.longId(id);

  QString pm = "";
  if (!langset.PixMapFile(id).isEmpty() )
    pm = langset.PixMapFile(id);

  view->setHeaderProp (header1+KV_EXTRA_COLS, lid, pm);

/*
  cout << "shp 1: " << (void*) doc << endl << flush;
  for (int i = 0; i < (int) langset.size(); i++) {
     cout << " " <<  EA_LOCAL(langset.shortId(i)) << "  "
          << EA_LOCAL(langset.longId(i))  << "  "
          << EA_LOCAL(langset.PixMapFile(i)) << "  "
          << hex << (const void*) EA_LOCAL(langset.PixMapFile(i)) << endl;
  }
*/
  if (header1 > 0)
    doc->setIdent(header1, langset.shortId(id));
  else
    doc->setOriginalIdent(langset.shortId(id));
/*
  for (int i = 0; i < (int) langset.size(); i++) {
     cout << " " <<  EA_LOCAL(langset.shortId(i)) << "  "
          << EA_LOCAL(langset.longId(i))  << "  "
          << EA_LOCAL(langset.PixMapFile(i)) << "  "
          << hex << (const void*) EA_LOCAL(langset.PixMapFile(i)) << endl;
  }
*/
  doc->setModified();
  slotStatusMsg(IDS_DEFAULT);
}


void kvoctrainApp::slotHeaderStatus (int header_and_cmd) /*FOLD00*/
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

    case IDH_SET_LANG: {
      QString from = langset.longId(header2);
      if (from.isEmpty())
        from = langset.shortId(header2);

      QString msg;
      if (header1 == 0) {
        QString format = i18n("Sets %1 as language for original");
        msg = format.arg(from);
      }
      else {
        QString format = i18n("Sets %1 as language for translation %2");
        msg = format.arg(from).arg(header1);
      }
      slotStatusHelpMsg(msg);
    }
    break;

    case IDH_APPEND: {

      QString msg;
      if (header1 == 0xFF) {
        msg = i18n("Appends a new language");
        slotStatusHelpMsg(msg);
      }
      else if (header1 < (int) langset.size() ) {
        QString from = langset.longId(header1);
        QString format = i18n("Appends %1 as new language");
        msg = format.arg(from);
        slotStatusHelpMsg(msg);
      }
    }
    break;

    case IDH_REMOVE: {
      QString from = header1 ? doc->getIdent(header1) : doc->getOriginalIdent();
      if (!langset.findLongId(from).isEmpty())
        from = langset.findLongId(from);
      QString msg;
      QString format = i18n("Removes %1 irrevocably from dictionary");
      msg = format.arg(from);
      slotStatusHelpMsg(msg);
    }
    break;

    case IDH_START_QUERY:
    case IDH_START_MULTIPLE: {
      QString to = header1 ? doc->getIdent(header1) : doc->getOriginalIdent();
      if (!langset.findLongId(to).isEmpty())
        to = langset.findLongId(to);

      QString from = header2 ? doc->getIdent(header2) : doc->getOriginalIdent();
      if (!langset.findLongId(from).isEmpty())
        from = langset.findLongId(from);

      QString msg;
      QString format;

      if (doc->numLangs() == 1) {
        if (cmd == IDH_START_QUERY)
          format = i18n("Creates and starts query to %1");
        else
          format = i18n("Creates and starts multiple choice to %1");
        msg = format.arg(to);
      }
      else {
        if (cmd == IDH_START_QUERY)
          format = i18n("Creates and starts query from %1 to %2");
        else
          format = i18n("Creates and starts multiple choice from %1 to %2");
        msg = format.arg(from).arg(to);
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

    case IDH_RESET_GRADE: {
      QString from = header1 ? doc->getIdent(header1) : doc->getOriginalIdent();
      if (!langset.findLongId(from).isEmpty())
        from = langset.findLongId(from);
      QString format = i18n("Resets all properties for %1");
      QString msg = format.arg(from);
      slotStatusHelpMsg(msg);
    }
    break;

    default:
      kdError() << "kvoctrainApp::slotHeaderStatus: got unknown command :" << cmd << endl;
  }
}


void kvoctrainApp::slotHeaderCallBack (int header_and_cmd) /*FOLD00*/
{
  int header1 = (header_and_cmd >> 16) & 0xFF;
  int header2 = header_and_cmd >> (16+8);
  int cmd     = header_and_cmd & 0xFFFF;

  kdDebug() << header1 << " " << header2 << " " << cmd << endl;

  switch (cmd) {
    case IDH_SORT_COL_ALPHA:
      view->getTable()->sortByColumn_alpha(header1);
      return;
    break;

    case IDH_SORT_COL_NUM:
      view->getTable()->sortByColumn_index(header1);
      return;
    break;
  }

  if (header1 >= (int) doc->numLangs()) {
    kdError() << "header1 >= (int) doc->numLangs()\n";
    return;
  }

  if (header2 >= (int) doc->numLangs()) {
    kdError() << "header2 >= (int) doc->numLangs()\n";
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

    case IDH_REMOVE: {
      QString msg;
      QString name;

      name = doc->getIdent(header1);
      int i= langset.indexShortId(name);
      if ( i >= 0
          && !langset.longId(i).isEmpty() )
        name = langset.longId(i);

      QString format = i18n("You are about to delete a language completely.\n"
                            "Do you really want to delete \"%1\"?");
      msg = format.arg(name);

      int exit = KMessageBox::warningYesNo(this, msg,
                    kapp->makeStdCaption(""));
      if(exit==KMessageBox::Yes) {
        doc->removeIdent(header1);
        view->setView (doc, langset, gradecols);
        doc->setModified();
      }
    }
    break;

    case IDH_START_QUERY:
      delete randomQueryDlg;
      randomQueryDlg = 0;
      queryType = QT_Random;
      slotStartQuery(header1 ? doc->getIdent(header1) : doc->getOriginalIdent(),
                     header2 ? doc->getIdent(header2) : doc->getOriginalIdent(), true);
    break;

    case IDH_START_MULTIPLE:
      delete mcQueryDlg;
      mcQueryDlg = 0;
      queryType = QT_Multiple;
      slotStartQuery(header1 ? doc->getIdent(header1) : doc->getOriginalIdent(),
                     header2 ? doc->getIdent(header2) : doc->getOriginalIdent(), true);
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

    case IDH_RESET_GRADE: {
      QString msg;
      QString name;
      QString format;

      if (act_lesson == 0) {
        name = doc->getIdent(header1);
        int i = langset.indexShortId(doc->getIdent(header1));
        if (i >= 0
            && !langset.longId(i).isEmpty() )
          name = langset.longId(i);

        format = i18n("You are about to reset the knowledge data of a "
                      "whole language.\n\nDo you really want to reset \"%1\"?");
      }
      else {
        name = lessons->text(act_lesson);
        format = i18n("You are about to reset the knowledge data of a "
                      "lesson.\n\nDo you really want to reset \"%1\"?");
      }

      msg = format.arg(name);

      int exit = KMessageBox::warningYesNo(this, msg,
                    kapp->makeStdCaption(""));
      if(exit==KMessageBox::Yes) {
        doc->resetEntry (header1, act_lesson);
        doc->setModified();
        view->getTable()->updateContents();
      }
    }
    break;

    default:
       kdError() << "kvoctrainApp::slotHeaderCallBack: got unknown command\n";

  }
  slotStatusMsg(IDS_DEFAULT);
}


