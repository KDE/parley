/***************************************************************************

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin         : Thu Mar 30 20:38:31 1999

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

#include <QGroupBox>
#include <QPushButton>

#include <kinputdialog.h>
#include <kmessagebox.h>
#include <klocale.h>

#include "UsageOptPage.h"
#include <QueryManager.h>

#define USAGE_TAG ". "

UsageOptPage::UsageOptPage(const QStringList &usages, KEduVocDocument *_doc, QWidget *parent)
 : QWidget(parent), doc(_doc)
{
  setupUi(this);
  titleGroupBox->setTitle(i18n("Usage Labels"));

  connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotUsageChosen(int)));  
  connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));
  connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteUsage()));
  connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyUsage()));
  connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewUsage()));

  QString str;
  for (int i = 0; i < (int) usages.size(); i++) {
    str.setNum (i+1);
    if (i <= 9)
      str.insert (0, " ");
    optionsList->addItem(str+USAGE_TAG+usages[i]);
    usageIndex.append(i);
  }

  act_usage = 0;
  if (usages.size() != 0)
    optionsList->setCurrentRow(act_usage);

  modifyButton->setEnabled(optionsList->count() != 0);
  deleteButton->setEnabled(optionsList->count() != 0);

  optionsList->setFocus();
}


void UsageOptPage::slotUsageChosen(int index)
{
   act_usage = index;
}


void UsageOptPage::slotNewUsage()
{
  bool ok;
  QString getUsage = KInputDialog::getText(
              i18nc( "usage (area) of an expression", "Usage Description" ), i18n( "Enter usage description:" ), QString(), &ok, this );
  if( !ok )
    return;
  QString str;
  int i = optionsList->count()+1;
  str.setNum (i);
  if (i <= 9)
    str.insert (0, " ");
  optionsList->addItem(str+USAGE_TAG+getUsage.simplified());
  usageIndex.append(-(i-1));
  act_usage = optionsList->count();
  optionsList->setCurrentRow(i-1);
  modifyButton->setEnabled(true);
  deleteButton->setEnabled(true);
}


void UsageOptPage::slotModifyUsage()
{
  if (optionsList->count() != 0
      && (int) optionsList->count() > act_usage) {
    QString str = optionsList->item(act_usage)->text();
    int pos = str.indexOf (USAGE_TAG);
    str.remove (0, pos+strlen (USAGE_TAG));

    bool ok;
    QString getUsage = KInputDialog::getText(
                i18nc( "usage (area) of an expression", "Usage Description" ), i18n( "Enter usage description:" ), str, &ok, this );
    if( !ok )
      return;
    QString str2;
    str2.setNum (act_usage+1);
    if (act_usage <= 9)
      str2.insert (0, " ");
    optionsList->item(act_usage)->setText(str2+USAGE_TAG+getUsage.simplified());
  }
}


void UsageOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) optionsList->count(); i++)
  {
    str = optionsList->item(i)->text();
    int pos = str.indexOf (USAGE_TAG);
    str.remove (0, pos+strlen (USAGE_TAG));
    str2.setNum (i+1);
    if (i <= 9)
      str2.insert (0, " ");
    optionsList->item(i)->setText(str2+USAGE_TAG+str);
  }
}


void UsageOptPage::slotDeleteUsage()
{
  int act = act_usage;
  if (optionsList->count() != 0 && (int) optionsList->count() > act)
  {
    QString t;
    t.setNum(usageIndex[act_usage]+1);
    t.insert (0,  UL_USER_USAGE);
    t += UL_USAGE_DIV;
    for (int ent = 0; ent < doc->numEntries(); ent++) {
      // FIXME: ProgressDlg here?
      KEduVocExpression *exp = doc->entry(ent);
      for (int lang = 0; lang < doc->numIdentifiers(); lang++) {
        QString ul = exp->usageLabel(lang) + UL_USAGE_DIV;
        if (ul.indexOf(t) >= 0 ) {
          KMessageBox::information(this,
                    i18nc("usage (area) of an expression",
                        "This user-defined usage label could not be deleted "
                        "because it is in use."),
                    i18nc("usage (area) of an expression",
                          "Deleting Usage Label"));
          return;
        }
      }
    }

    delete optionsList->takeItem(act);
    usageIndex.erase (usageIndex.begin() + act);

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


void UsageOptPage::getUsageLabels(QStringList &ret_usage, QList<int> &ret_Index)
{
  QString str;
  ret_usage.clear();
  for (int i = 0; i < (int) optionsList->count(); i++) {
    str = optionsList->item(i)->text();
    int pos = str.indexOf (USAGE_TAG);
    str.remove (0, pos+strlen (USAGE_TAG));
    ret_usage.append (str);
  }
  ret_Index = usageIndex;
}


void UsageOptPage::slotCleanup()
{
  QVector<bool> used_usage;
  for (int i = 0; i <= (int) optionsList->count(); i++)
    used_usage.append(false);

  for (int col = 0; col < doc->numIdentifiers(); col++)
    for (int i = 0; i < (int) doc->numEntries(); i++) {
      QString t = doc->entry(i)->usageLabel(col);
      QString n;
      while (t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
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

        int idx = n.toInt();
        if ((int) used_usage.size() < idx)
          used_usage.resize(idx);
        if (idx != 0)
          used_usage[idx-1] = true ;
      }
    }

  for (int i = used_usage.size()-1; i >= 0; i--) {
    if (!used_usage[i]) {
      for (int u = 0; u < (int) usageIndex.size() ; u++) {
        if (usageIndex[u] == i || usageIndex[u] < 0) {
          act_usage = i;
          slotDeleteUsage();
          break;
        }
      }
    }
  }

  act_usage = 0;
  optionsList->setCurrentRow(act_usage);
}


void UsageOptPage::cleanUnused(KEduVocDocument *doc,
                               const QList<int> &usageIndex,
                               int old_usages)
{
  QList<int> translate_index;
  QStringList new_usageStr;

  /////////////////////////////////////////////////////
  // translate_index contains new index number for each
  // old index
  for (int i = 0; i <= qMax (old_usages, (int) usageIndex.size()); i++)
    translate_index.append(0);

  // now adjust lesson descriptions to new index

  for (int i = 0; i < (int) usageIndex.size(); i++) {
    if (usageIndex[i] >= 0)
      translate_index[usageIndex[i]+1] = i+1;
  }

  // only keep remaining usage indices

  // set usage index to 0 when not needed any more
  // and translate to new index

  for (int col = 0; col < doc->numIdentifiers(); col++) {
    for (int i = 0; i < doc->numEntries(); i++) {
      QString t = doc->entry(i)->usageLabel (col);
      if (!t.isEmpty() && t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
        QString tg;
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

          QString newusage;
          int o = n.toInt();
          if (translate_index[o] != 0) {
            newusage.setNum (translate_index[o]);
            newusage.insert (0, UL_USER_USAGE);
          }

          if (tg.length() == 0)
            tg = newusage;
          else
            if (newusage.length() )
              tg += UL_USAGE_DIV + newusage;
        }

        if (tg.length() == 0)
          tg = t;
        else if (t.length() != 0)
          tg += UL_USAGE_DIV + t;

        doc->entry(i)->setUsageLabel (col, tg);
      }
    }
  }
}

#include "UsageOptPage.moc"
