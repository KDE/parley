/***************************************************************************

                   user tense options dialog page

    -----------------------------------------------------------------------

    begin         : Sun May 28 12:14:31 2000

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QPushButton>

#include <kinputdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "TenseOptPage.h"
#include <QueryManager.h>

#define TENSE_TAG ". "

TenseOptPage::TenseOptPage(const QStringList &tenses, KEduVocDocument *_doc, QWidget *parent) :
  QWidget(parent)
{
  setupUi(this);
  titleGroupBox->setTitle(i18n("Tense Descriptions"));

  connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotTenseChosen(int)));
  connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
  connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteTense()));
  connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyTense()));
  connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewTense()));

  QString str;
  for (int i = 0; i < (int) tenses.size(); i++) {
    str.setNum (i+1);
    if (i <= 9)
      str.insert (0, " ");
    optionsList->addItem(str+TENSE_TAG+tenses[i]);
    tenseIndex.push_back(i);
  }

  act_tense = 0;
  if (tenses.size() != 0)
    optionsList->setCurrentRow (act_tense);

  modifyButton->setEnabled(optionsList->count() != 0);
  deleteButton->setEnabled(optionsList->count() != 0);

  optionsList->setFocus();
  doc = _doc;
}


void TenseOptPage::slotTenseChosen(int index)
{
  act_tense = index;
}


void TenseOptPage::slotNewTense()
{
  bool ok;
  QString getTense = KInputDialog::getText(
              i18n( "Tense Description" ), i18n( "Enter tense description:" ), QString(), &ok, this );
  if( !ok )
    return;

  QString str;
  int i = optionsList->count()+1;
  str.setNum (i);
  if (i <= 9)
    str.insert (0, " ");
  optionsList->addItem(str+TENSE_TAG+getTense.simplified());
  tenseIndex.push_back(-(i-1));
  act_tense = optionsList->count();
  optionsList->setCurrentRow (i-1);
  modifyButton->setEnabled(true);
  deleteButton->setEnabled(true);
}


void TenseOptPage::slotModifyTense()
{
  if (optionsList->count() != 0 && (int) optionsList->count() > act_tense)
  {
    QString str = optionsList->item(act_tense)->text();
    int pos = str.indexOf (TENSE_TAG);
    str.remove (0, pos+strlen (TENSE_TAG));
    bool ok;
    QString getTense = KInputDialog::getText(
              i18n( "Tense Description" ), i18n( "Enter tense description:" ), str, &ok, this );
    if( !ok )
      return;
    QString str2;
    str2.setNum (act_tense+1);
    if (act_tense <= 9)
      str2.insert (0, " ");
    optionsList->item(act_tense)->setText(str2+TENSE_TAG+getTense.simplified());
  }
}


void TenseOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) optionsList->count(); i++)
  {
    str = optionsList->item(i)->text();
    int pos = str.indexOf (TENSE_TAG);
    str.remove (0, pos+strlen (TENSE_TAG));
    str2.setNum (i+1);
    if (i <= 9)
      str2.insert (0, " ");
    optionsList->item(i)->setText(str2+TENSE_TAG+str);
  }
}


void TenseOptPage::slotDeleteTense()
{
  int act = act_tense;
  if (optionsList->count() != 0
      && (int) optionsList->count() > act) {

    QString t;
    t.setNum(tenseIndex[act_tense]+1);
    t.insert (0, QM_USER_TYPE);
    for (int ent = 0; ent < doc->numEntries(); ent++) {
      // FIXME: ProgressDlg here?
      KEduVocExpression *exp = doc->entry(ent);
      for (int lang = 0; lang < doc->numIdentifiers(); lang++) {
        KEduVocConjugation conj = exp->conjugation(lang);
        for (int con = 0; con < conj.numEntries(); con++ ) {
          if (conj.getType(con) == t) {
            KMessageBox::information(this,
                      i18n("This user defined tense could not be deleted\nbecause it is in use."),
                      i18n("Deleting Tense Description"));
            return;
          }
        }
      }
    }

    delete optionsList->takeItem(act);
    tenseIndex.erase (tenseIndex.begin() + act);

    if ((int) optionsList->count() <= act)
      act = optionsList->count()-1;
    else
      updateListBox(act); // update items after current

    if (act >= 0)
      optionsList->setCurrentRow (act);
  }
  modifyButton->setEnabled(optionsList->count() != 0);
  deleteButton->setEnabled(optionsList->count() != 0);
}


void TenseOptPage::getTenseNames(QStringList &ret_tense, QList<int> &ret_Index)
{
  QString str;    ret_tense.clear();
  for (int i = 0; i < (int) optionsList->count(); i++)
  {
    str = optionsList->item(i)->text();
    int pos = str.indexOf (TENSE_TAG);
    str.remove (0, pos+strlen (TENSE_TAG));
    ret_tense.push_back (str);
  }
  ret_Index = tenseIndex;
}


void TenseOptPage::slotCleanup()
{
  QVector<bool> used_tense;
  for (int i = 0; i <= (int) optionsList->count(); i++)
    used_tense.push_back(false);

  for (int col = 0; col < doc->numIdentifiers(); col++)
    for (int i = 0; i < (int) doc->numEntries(); i++) {
      KEduVocConjugation conj = doc->entry(i)->conjugation(col);
      for (int ci = 0; ci < conj.numEntries(); ci++) {
        QString t = conj.getType(ci);
        if (t.left(strlen(UL_USER_TENSE)) == UL_USER_TENSE) {
          t.remove (0, strlen(UL_USER_TENSE));
          int idx = t.toInt();
          if ((int) used_tense.size() < idx)
            used_tense.resize(idx);
          if (idx != 0)
            used_tense[idx-1] = true;
        }
      }
    }

  for (int i = used_tense.size()-1; i >= 0; i--)
    if (!used_tense[i]) {
      for (int u = 0; u < (int) tenseIndex.size() ; u++) {
        if (tenseIndex[u] == i || tenseIndex[u] < 0) {
          act_tense = i;
          slotDeleteTense();
          break;
        }
      }
    }

  act_tense = 0;
  optionsList->setCurrentRow (act_tense);
}


void TenseOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &tenseIndex, int old_tenses)
{
  QList<int> translate_index;
  QStringList new_tenseStr;

  /////////////////////////////////////////////////////
  // translate_index contains new index number for each
  // old index
  for (int i = 0; i <= qMax (old_tenses, (int) tenseIndex.size()); i++)
    translate_index.push_back(0);

  // now adjust lesson descriptions to new index

  for (int i = 0; i < (int) tenseIndex.size(); i++) {
    if (tenseIndex[i] >= 0)
      translate_index[tenseIndex[i]+1] = i+1;
  }

  // only keep remaining tense indices

  // set tense index to 0 when not needed any more
  // and translate to new index

  for (int col = 0; col < doc->numIdentifiers(); col++) {
    for (int i = 0; i < doc->numEntries(); i++) {
      KEduVocConjugation conj = doc->entry(i)->conjugation (col);
      bool dirty = false;
      for (int ci = 0; ci < conj.numEntries(); ci++) {
        QString old = conj.getType(ci);
        if (!old.isEmpty() && old.left(strlen(QM_USER_TYPE)) == QM_USER_TYPE) {
          old.remove (0, 1);
          int o = old.toInt();

          dirty = true;
          QString newtense;
          if (translate_index[o] != 0) {
            newtense.setNum (translate_index[o]);
            newtense.insert (0, QM_USER_TYPE);
            conj.setType(ci, newtense);
          }
          else
            conj.setType(ci, "");
        }
      }
      if (dirty)
        doc->entry(i)->setConjugation (col, conj);
    }
  }
}

#include "TenseOptPage.moc"
