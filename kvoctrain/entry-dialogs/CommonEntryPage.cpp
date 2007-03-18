/***************************************************************************

                       edit common properties

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

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

#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QPixmap>

#include <kdebug.h>
#include <klocale.h>
#include <kicon.h>
#include <kdialog.h>

#include <kvtquery.h>
#include <kvtlanguages.h>
#include "DocPropDlg.h"
#include "PhoneticEntryPage.h"
#include "CommonEntryPage.h"
#include "EntryDlg.h"

CommonEntryPage::CommonEntryPage(KEduVocDocument *_doc, KVTQuery &_querymanager, QWidget *parent) : QWidget(parent), doc(_doc), querymanager(_querymanager)
{
  setupUi(this);

  connect(b_usageDlg, SIGNAL(clicked()), SLOT(invokeUsageDlg()));
  //connect(b_LessDlg, SIGNAL(clicked()), SLOT(invokeLessDlg()));
  connect(b_pronDlg, SIGNAL(clicked()), SLOT(invokePronDlg()));
  connect(b_TypeDlg, SIGNAL(clicked()), SLOT(invokeTypeDlg()));
  connect(usage_box, SIGNAL(itemSelectionChanged()), SLOT(slotUsageChanged()));
  connect(lesson_box, SIGNAL(activated(int)), SLOT(slotLessonSelected(int)));
  connect(subtype_box, SIGNAL(activated(int)), SLOT(slotSubTypeSelected(int)));
  connect(type_box, SIGNAL(activated(int)), SLOT(slotTypeSelected(int)));
  connect(c_active, SIGNAL(toggled(bool)), SLOT(slotActiveChanged(bool)));
  connect(pronounce_line, SIGNAL(textChanged(const QString&)), SLOT(slotPronounceSelected(const QString&)));
  connect(expr_line, SIGNAL(textChanged(const QString&)), SLOT(slotExprSelected(const QString&)));

  usage_label->setTitle(i18nc("Usage (area) of an Expression", "&Usage Labels"));
  pronounce_line->setFont(Prefs::iPAFont());

  QIcon list_pm = KIcon("list");
  b_LessDlg->setIcon(list_pm);
  b_LessDlg->setEnabled(false);
  b_TypeDlg->setIcon(list_pm);
  b_usageDlg->setIcon(list_pm);

  QIcon pron_pm = KIcon("character-set");
  b_pronDlg->setIcon(pron_pm);

  subDialog = 0;
}


void CommonEntryPage::setData(bool multi_sel, QString expr, int less, QString type, QString pronounce, QString usage, bool active)
{
  lesson = less;
  setLessonBox(lesson);

  usageCollection = usage;
  setUsageBox(usage);

  expr_line->setText(expr);

  m_type = type;
  setTypeBox(type);
  m_pronounce = pronounce;
  pronounce_line->setText(pronounce);

  entry_active = active;
  c_active->setChecked(active);

  int start = -1;
  int i = 0;
  while (start < 0 && i < (int) all_types.size()) {
    if (all_types [i].shortStr() == KVTQuery::getMainType(type))
      start = i;
    i++;
  }
  int offset = -1;
  while (offset < 0 && i < (int) all_types.size()) {
    if (all_types [i].shortStr() == type)
      offset = i - start;
    i++;
  }
  if (offset >= 0) {
    slotSubTypeSelected(offset);
    subtype_box->setCurrentIndex(offset);
  }

  m_largeSelection = multi_sel;
  if (m_largeSelection) {
    expr_line->setEnabled(false);
    pronounce_line->setEnabled(false);
    b_pronDlg->setEnabled(false);
    expr_line->setText("");
    pronounce_line->setText("");
    lesson_box->setCurrentIndex(-1);
    type_box->setCurrentIndex(-1);
    subtype_box->setCurrentIndex(-1);
  }

  m_usageIsModified = false;
  m_typeIsModified = false;
  m_lessonIsModified = false;
  m_activeIsModified = false;
  setModified(false);
}


void CommonEntryPage::setEnabled(int enable)
{
  bool ena = enable == EntryDlg::EnableAll;

  usage_box->setEnabled(ena);
  subtype_box->setEnabled(ena);
  type_box->setEnabled(ena);
  pronounce_line->setEnabled(ena && !m_largeSelection);
  expr_line->setEnabled(ena && !m_largeSelection);
  b_pronDlg->setEnabled(pronounce_line->isEnabled());
  lesson_box->setEnabled(ena || enable == EntryDlg::EnableOnlyCommon);
  c_active->setEnabled(ena || enable == EntryDlg::EnableOnlyCommon);
}


void CommonEntryPage::setTypeBox(const QString &act_type)
{
  all_types = KVTQuery::getRelation(false);
  all_maintypes = KVTQuery::getRelation(true);

  QString s = KVTQuery::getMainType(act_type)+QM_TYPE_DIV;
  int curr_type = 0;
  type_box->clear();
  type_box->addItem (i18n("<none>"));
  for (int i = 0; i < (int) all_maintypes.size(); i++) {
    type_box->addItem (all_maintypes[i].longStr());
    if (s == all_maintypes[i].shortStr()+QM_TYPE_DIV)
      curr_type = i+1;
  }
  type_box->setCurrentIndex(curr_type);
  slotTypeSelected(curr_type);
}


void CommonEntryPage::setLessonBox(int lesson)
{
  lesson_box->clear();
  //lesson_box->addItem(doc->lessonDescription(0));
  lesson_box->addItems(doc->lessonDescriptions());

  //if (lesson >= lesson_box->count())
  //  lesson = 0;
  lesson_box->setCurrentIndex(lesson-1);
}


void CommonEntryPage::setUsageBox(const QString & act_usage)
{
  usages = KVTUsage::getRelation();
  usage_box->clear();
  for (int i = 0; i < (int) usages.size(); i++) {
    usage_box->addItem(usages[i].longStr());
    if (KVTUsage::contains(QString(usages[i].identStr()), act_usage)) {
      usage_box->setCurrentRow(i);
    }
  }
  slotUsageChanged();
}


void CommonEntryPage::slotUsageChanged()
{
  setModified(true);
  m_usageIsModified = true;
  usageCollection = "";
  QString s;

  for (int i = 0; i < usage_box->count(); i++) {
    if (usage_box->item(i)->isSelected()) {

      if (!usageCollection.isEmpty())
        usageCollection += UL_USAGE_DIV;
      usageCollection += usages[i].identStr();

      if (!s.isEmpty())
        s += ", ";
      s += usages[i].shortStr();
    }
  }
  usage_line->setText(s);
}


void CommonEntryPage::slotLessonSelected (int l)
{
  setModified(true);
  m_lessonIsModified = true;
  lesson = l+1;
}


void CommonEntryPage::slotActiveChanged(bool state)
{
  setModified(true);
  m_activeIsModified = true;
  entry_active = state;
}


void CommonEntryPage::slotExprSelected (const QString& s)
{
  setModified(true);
  expression = s;
}


void CommonEntryPage::slotPronounceSelected (const QString& s)
{
  setModified(true);
  m_pronounce = s;
}


void CommonEntryPage::slotSubTypeSelected(int i)
{
  setModified(true);
  m_typeIsModified = true;
  if (i < (int) current_subtypes.size()) {
    m_type = current_subtypes[i];
    emit typeSelected(m_type);
  }
}


void CommonEntryPage::slotTypeSelected(int idx)
{
  setModified(true);
  m_typeIsModified = true;
  subtype_box->clear();
  current_subtypes.clear();
  bool first = true;

  if (idx == 0) { // 0 == none !
    m_type = "";
    emit typeSelected(m_type);
  }
  else {
    m_type = all_maintypes[idx-1].shortStr();
    emit typeSelected(m_type);
    QString main_patt = all_maintypes[idx-1].shortStr()+QM_TYPE_DIV;
    int sub_idx;
    if (idx-1 < (int) all_types.size()){
      for (sub_idx = 0; sub_idx < (int) all_types.size(); sub_idx++) {
        if (all_types[sub_idx].shortStr().left(main_patt.length()) == main_patt) {
          if (first) {
            subtype_box->addItem (i18n("<none>"));
            current_subtypes.push_back(all_maintypes[idx-1].shortStr());
            first = false;
          }
          QString s = all_types[sub_idx].longStr();
          subtype_box->addItem (s.simplified());
          current_subtypes.push_back(all_types[sub_idx].shortStr());
        }
      }
    }
  }

  subtype_box->setEnabled(!first);
  subtype_label->setEnabled(!first);
}


void CommonEntryPage::phoneticSelected(wchar_t wc)
{
  setModified(true);
  m_pronounce += QChar(wc);
  pronounce_line->setText(m_pronounce);
}


void CommonEntryPage::invokePronDlg()
{
  PhoneticEntryPage * phoneticDlg = new PhoneticEntryPage (Prefs::iPAFont(), this);
  connect (phoneticDlg, SIGNAL(charSelected(wchar_t)), SLOT(phoneticSelected(wchar_t)));
  phoneticDlg->show();
}


void CommonEntryPage::invokeUsageDlg()
{
  QList<int> usageIndex;
  QStringList new_usageStr;

  int old_usages = (int) doc->usageDescriptions().size();

  KDialog *subDialog= new KDialog(b_usageDlg);
  subDialog->setCaption(i18nc("usage (area) of an expression", "Edit User-Defined Usage Labels"));
  subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

  subDialog->setDefaultButton(KDialog::Ok);

  connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

  UsageOptPage *usageOptPage = new UsageOptPage(doc, this);
  subDialog->setMainWidget(usageOptPage);

  if (subDialog->exec() == QDialog::Accepted)
  {
    usageOptPage->getUsageLabels(new_usageStr, usageIndex);
    UsageOptPage::cleanUnused(doc, usageIndex, old_usages);
    KVTUsage::setUsageNames (new_usageStr);
    setUsageBox(usageCollection);
    doc->setUsageDescriptions(new_usageStr);
    doc->setModified();
  }
}

/*
void CommonEntryPage::invokeLessDlg()
{
  QList<int> lessonIndex;
  QStringList new_lessonStr;

  int old_lessons = (int) lesson_box->count();
  KDialog *subDialog = new KDialog(b_LessDlg);
  subDialog->setCaption(i18n("Edit Lesson Names"));
  subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

  subDialog->setDefaultButton(KDialog::Ok);

  connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

  LessOptPage *lessOptPage = new LessOptPage(doc, this);
  subDialog->setMainWidget(lessOptPage);

  QList<int> lessoninquery = doc->lessonsInQuery();
  if (subDialog->exec() == QDialog::Accepted)
  {
    lessOptPage->getLesson(new_lessonStr, lessonIndex);
    LessOptPage::cleanUnused(doc, lessonIndex, old_lessons, lessoninquery);
    doc->setLessonDescriptions(new_lessonStr);
    doc->setLessonsInQuery(lessoninquery);
    querymanager.setLessonItems(lessoninquery);
    doc->setModified();
  }
}
*/

void CommonEntryPage::invokeTypeDlg()
{
  QList<int> typeIndex;
  QStringList new_typeStr;

  int old_types = (int) doc->typeDescriptions().size();
  KDialog *subDialog = new KDialog(b_TypeDlg);
  subDialog->setCaption(i18n("Edit User Defined Types"));
  subDialog->setButtons(KDialog::Ok|KDialog::Cancel);

  subDialog->setDefaultButton(KDialog::Ok);

  connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));

  TypeOptPage *typeOptPage = new TypeOptPage(doc, this);
  subDialog->setMainWidget(typeOptPage);

  if (subDialog->exec() == QDialog::Accepted)
  {
    typeOptPage->getTypeNames(new_typeStr, typeIndex);
    TypeOptPage::cleanUnused(doc, typeIndex, old_types);
    KVTQuery::setTypeNames (new_typeStr);
    setTypeBox(m_type);
    doc->setTypeDescriptions(new_typeStr);
    doc->setModified();
  }
}


bool CommonEntryPage::isModified()
{
  return modified;
}


void CommonEntryPage::setModified(bool mod)
{
  modified = mod;
  if (mod)
    emit sigModified();
}

void CommonEntryPage::slotSubDialogClosed()
{
  if (subDialog)
  {
    disconnect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));
    subDialog->deleteLater();
    subDialog = 0L;
  }
}

#include "CommonEntryPage.moc"
