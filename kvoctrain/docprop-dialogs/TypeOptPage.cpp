/***************************************************************************

                    types options dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#include "TypeOptPage.h"
#include <QueryManager.h>

#define TYPE_TAG ". "

TypeOptPage::TypeOptPage(const QStringList &types, KEduVocDocument *_doc,  QWidget *parent)
  : QWidget(parent), doc(_doc)
{
  setupUi(this);
  titleGroupBox->setTitle(i18n("Type Descriptions"));

  connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotTypeChosen(int)));
  connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
  connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteType()));
  connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyType()));
  connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewType()));

  QString str;
  for (int i = 0; i < (int) types.size(); i++) {
    str.setNum (i+1);
    if (i <= 9)
      str.insert (0, " ");
    optionsList->addItem(str+TYPE_TAG+types[i]);
    typeIndex.append(i);
  }

  act_type = 0;
  if (types.size() != 0)
    optionsList->setCurrentRow(act_type);

  modifyButton->setEnabled(optionsList->count() != 0);
  deleteButton->setEnabled(optionsList->count() != 0);

  optionsList->setFocus();
}


void TypeOptPage::slotTypeChosen(int index)
{
  act_type = index;
}


void TypeOptPage::slotNewType()
{
  bool ok;
  QString getType = KInputDialog::getText(
              i18n( "Type Description" ), i18n( "Enter type description:" ), QString(), &ok, this );
  if( !ok )
    return;
  QString str;
  int i = optionsList->count()+1;
  str.setNum (i);
  if (i <= 9)
    str.insert (0, " ");
  optionsList->addItem (str+TYPE_TAG+getType.simplified());
  typeIndex.push_back(-(i-1));
  act_type = optionsList->count();
  optionsList->setCurrentRow(i-1);
  modifyButton->setEnabled(true);
  deleteButton->setEnabled(true);
}


void TypeOptPage::slotModifyType()
{
  if (optionsList->count() != 0
      && (int) optionsList->count() > act_type) {
    QString str = optionsList->item(act_type)->text();
    int pos = str.indexOf (TYPE_TAG);
    str.remove (0, pos+strlen (TYPE_TAG));
    bool ok;
    QString getType = KInputDialog::getText(
                i18n( "Type Description" ), i18n( "Enter type description:" ), str, &ok, this );
    if( !ok )
      return;
    QString str2;
    str2.setNum (act_type+1);
    if (act_type <= 9)
      str2.insert (0, " ");
    optionsList->item(act_type)->setText(str2+TYPE_TAG+getType.simplified());
  }
}


void TypeOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) optionsList->count(); i++) {
    str = optionsList->item(i)->text();
    int pos = str.indexOf (TYPE_TAG);
    str.remove (0, pos+strlen (TYPE_TAG));
    str2.setNum (i+1);
    if (i <= 9)
      str2.insert (0, " ");
    optionsList->item(i)->setText(str2+TYPE_TAG+str);
  }
}


void TypeOptPage::slotDeleteType()
{
  int act = act_type;
  if (optionsList->count() != 0
      && (int) optionsList->count() > act) {

    QString t;
    t.setNum(typeIndex[act_type]+1);
    t.insert (0, QM_USER_TYPE);
    for (int ent = 0; ent < doc->numEntries(); ent++) {
      // FIXME: ProgressDlg here?
      KEduVocExpression *exp = doc->entry(ent);
      for (int lang = 0; lang < (int) doc->numIdentifiers(); lang++) {
        if (exp->type(lang) == t) {
          KMessageBox::information(this,
                    i18n("This user defined type could not be deleted\nbecause it is in use."),
                    i18n("Deleting Type Description"));
          return;
        }
      }
    }

    delete optionsList->takeItem(act);
    typeIndex.erase (typeIndex.begin() + act);

    if ((int) optionsList->count() <= act)
      act = optionsList->count()-1;
    else
      updateListBox(act); // update items after current

    if (act >= 0)
      optionsList->setCurrentRow(act);
  }
  modifyButton->setEnabled(optionsList->count() != 0);
  deleteButton->setEnabled(optionsList->count() != 0);
}


void TypeOptPage::getTypeNames (QStringList &ret_type, QList<int> &ret_Index)
{
  QString str;    ret_type.clear();
  for (int i = 0; i < (int) optionsList->count(); i++) {
    str = optionsList->item(i)->text();
    int pos = str.indexOf (TYPE_TAG);
    str.remove (0, pos+strlen (TYPE_TAG));
    ret_type.append(str);
  }
  ret_Index = typeIndex;
}


void TypeOptPage::slotCleanup()
{
  QVector<bool> used_type;
  for (int i = 0; i <= (int) optionsList->count(); i++)
    used_type.push_back(false);

  for (int col = 0; col < doc->numIdentifiers(); col++)
    for (int i = 0; i < (int) doc->numEntries(); i++) {
      QString t = doc->entry(i)->type(col);
      if (t.left(strlen(QM_USER_TYPE)) == QM_USER_TYPE) {
        t.remove (0, 1);
        int idx = t.toInt();
        if ((int) used_type.size() < idx)
          used_type.resize(idx);
        if (idx != 0)
          used_type[idx-1] = true ;
      }
    }

  for (int i = used_type.size()-1; i >= 0; i--)
    if (!used_type[i]) {
      for (int u = 0; u < (int) typeIndex.size() ; u++) {
        if (typeIndex[u] == i || typeIndex[u] < 0) {
          act_type = i;
          slotDeleteType();
          break;
        }
      }
    }

  act_type = 0;
  optionsList->setCurrentRow(act_type);
}


void TypeOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &typeIndex, int old_types)
{
  QList<int> translate_index;
  QList<QString> new_typeStr;

  /////////////////////////////////////////////////////
  // translate_index contains new index number for each
  // old index
  for (int i = 0; i <= qMax (old_types, (int) typeIndex.size()); i++)
    translate_index.push_back(0);

  // now adjust lesson descriptions to new index

  for (int i = 0; i < (int) typeIndex.size(); i++) {
    if (typeIndex[i] >= 0)
      translate_index[typeIndex[i]+1] = i+1;
  }

  // only keep remaining type indices

  // set type index to 0 when not needed any more
  // and translate to new index

  for (int col = 0; col < doc->numIdentifiers(); col++) {
    for (int i = 0; i < doc->numEntries(); i++) {
      QString old = doc->entry(i)->type (col);
      if (!old.isEmpty() && old.left(strlen(QM_USER_TYPE)) == QM_USER_TYPE) {
        old.remove (0, 1);
        int o = old.toInt();

        QString newtype;
        if (translate_index[o] != 0) {
          newtype.setNum (translate_index[o]);
          newtype.insert (0, QM_USER_TYPE);
          doc->entry(i)->setType (col, newtype);
        }
        else
          doc->entry(i)->setType (col, "");
      }
    }
  }

}

#include "TypeOptPage.moc"
