/***************************************************************************

                       edit common properties

    -----------------------------------------------------------------------

    begin          : Mon Jun 28 21:02:16 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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
#include <Q3ListBox>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QPixmap>

#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kdialogbase.h>

#include <QueryManager.h>
#include <langset.h>
#include "DocPropDlg.h"
#include "blockall.h"
#include "PhoneticEntryPage.h"
#include "CommonEntryPage.h"
#include "EntryDlg.h"

CommonEntryPage::CommonEntryPage
(
  kvoctrainDoc *_doc,
  bool          multi_sel,
  QString       expr,
  int           less,
  QComboBox    *lessbox,
  QString       lang,
  QString       act_type,
  QString       pron,
  QString       act_usage,
  QString       label,
  QueryManager &_querymanager,
  bool          active,
  const QFont&  _ipafont,
  QWidget      *parent
)
  :
  QWidget(parent),
  pronunce(pron),
  expression(expr),
  usageCollection (act_usage),
  lesson(less),
  type (act_type),
  doc(_doc),
  querymanager(_querymanager),
  entry_active(active),
  ipafont(_ipafont)
{
  setupUi(this);
  connect( b_usageDlg, SIGNAL(clicked()), SLOT(invokeUsageDlg()) );
  connect( b_LessDlg, SIGNAL(clicked()), SLOT(invokeLessDlg()) );
  connect( b_pronDlg, SIGNAL(clicked()), SLOT(invokePronDlg()) );
  connect( b_TypeDlg, SIGNAL(clicked()), SLOT(invokeTypeDlg()) );
  connect( usage_box, SIGNAL(selectionChanged()), SLOT(slotUsageChanged()) );
  connect( lesson_box, SIGNAL(activated(int)), SLOT(slotLessonSelected(int)) );
  connect( subtype_box, SIGNAL(activated(int)), SLOT(slotSubTypeSelected(int)) );
  connect( type_box, SIGNAL(activated(int)), SLOT(slotTypeSelected(int)) );
  connect( c_active, SIGNAL(toggled(bool)), SLOT(slotActiveChanged(bool)) );

  connect( pronunce_line, SIGNAL(textChanged(const QString&)), SLOT(slotPronunceSelected(const QString&)) );
  connect( expr_line, SIGNAL(textChanged(const QString&)), SLOT(slotExprSelected(const QString&)) );

  usage_label->setTitle(i18n("Usage (area) of an Expression", "&Usage Labels"));
  pronunce_line->setFont(ipafont);

  lesson_box->setValidator (new BlockAllValidator() );
  type_box->setValidator (new BlockAllValidator() );
  subtype_box->setValidator (new BlockAllValidator() );

  QPixmap list_pm = SmallIcon("view_text");
  b_LessDlg->setPixmap(list_pm);
  b_TypeDlg->setPixmap(list_pm);
  b_usageDlg->setPixmap(list_pm);

  QPixmap pron_pm = SmallIcon("view_icon");
  b_pronDlg->setPixmap(pron_pm);

  setData(multi_sel, expr, less, lessbox, lang, type, pronunce, act_usage, label, querymanager, active);
}


void CommonEntryPage::setData(
  bool          multi_sel,
  QString       expr,
  int           less,
  QComboBox    *lessBox,
  QString       /*lang*/,
  QString       type,
  QString       pronunce,
  QString       usage,
  QString       /*label*/,
  QueryManager &/*querymanager*/,
  bool          active)
{
  setLessonBox (lessBox, less);
  setUsageBox (usage);

  //expr_label->setText( label );
  expr_line->setText(expr);

  setTypeBox(type);
  pronunce_line->setText(pronunce);
  c_active->setChecked(active);

  int start = -1;
  int i = 0;
  while (start < 0 && i < (int) all_types.size()) {
    if (all_types [i].shortStr() == QueryManager::getMainType(type))
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
    subtype_box->setCurrentItem(offset);
  }

  if (multi_sel) {
    expr_line->setEnabled (false);
    pronunce_line->setEnabled (false);
    expr_line->setText ("");
    pronunce_line->setText ("");
    lesson_box->clearEdit();
    type_box->clearEdit();
    subtype_box->clearEdit();
  }

  lesson_dirty = false;
  type_dirty = false;
  usage_dirty = false;
  active_dirty = false;

  setModified(false);
}


void CommonEntryPage::setTypeBox(const QString &act_type)
{
  all_types = QueryManager::getRelation(false);
  all_maintypes = QueryManager::getRelation(true);

  QString s = QueryManager::getMainType(act_type)+QM_TYPE_DIV;
  int curr_type = 0;
  type_box->clear();
  type_box->insertItem (i18n("<none>"));
  for (int i = 0; i < (int) all_maintypes.size(); i++) {
    type_box->insertItem (all_maintypes[i].longStr());
    if (s == all_maintypes[i].shortStr()+QM_TYPE_DIV)
      curr_type = i+1;
  }
  type_box->setCurrentItem(curr_type);
  slotTypeSelected(curr_type);
}


void CommonEntryPage::setLessonBox(QComboBox *lessbox, int lesson)
{
  lesson_box->clear();
  for (int i = 0; i < lessbox->count(); i++)
    lesson_box->insertItem (lessbox->text(i));
  if (lesson >= lesson_box->count() )
    lesson = 0;
  lesson_box->setCurrentItem(lesson);
}


void CommonEntryPage::setUsageBox(const QString & act_usage)
{
  usages = UsageManager::getRelation();
  usage_box->clear();
  for (int i = 0; i < (int) usages.size(); i++) {
    usage_box->insertItem (usages[i].longStr());
    if (UsageManager::contains(QString(usages[i].identStr()), act_usage)) {
      usage_box->setSelected (i, true);
    }
  }
  slotUsageChanged();
}


void CommonEntryPage::slotUsageChanged()
{
  setModified(true);
  usageCollection = "";
  usage_dirty = true;
  QString s;
  for (int i = 0; i < (int) usage_box->count(); i++) {
    if (usage_box->isSelected(i)) {

      if (!usageCollection.isEmpty() )
        usageCollection += UL_USAGE_DIV;
      usageCollection += usages[i].identStr();

      if (!s.isEmpty() )
        s += ", ";
      s += usages[i].shortStr();
    }
  }
  usage_line->setText (s);
}


void CommonEntryPage::slotLessonSelected (int l)
{
  setModified(true);
  lesson = l;
  lesson_dirty = true;
}


void CommonEntryPage::slotActiveChanged(bool state)
{
  setModified(true);
  entry_active = state;
  active_dirty = true;
}


void CommonEntryPage::slotExprSelected (const QString& s)
{
  setModified(true);
  expression = s;
}


void CommonEntryPage::slotPronunceSelected (const QString& s)
{
  setModified(true);
  pronunce = s;
}


void CommonEntryPage::slotSubTypeSelected(int i)
{
  setModified(true);
  if (i < (int) current_subtypes.size()) {
    type = current_subtypes[i];
    emit typeSelected(type);
    type_dirty = true;
  }
}


void CommonEntryPage::slotTypeSelected(int idx)
{
  setModified(true);
  subtype_box->clear();
  current_subtypes.clear();
  bool first = true;

  if (idx == 0) { // 0 == none !
    type = "";
    emit typeSelected(type);
  }
  else {
    type = all_maintypes[idx-1].shortStr();
    emit typeSelected(type);
    QString main_patt = all_maintypes[idx-1].shortStr()+QM_TYPE_DIV;
    int sub_idx;
    if ( idx-1 < (int) all_types.size() ){
      for (sub_idx = 0; sub_idx < (int) all_types.size(); sub_idx++) {
        if (all_types[sub_idx].shortStr().left(main_patt.length()) == main_patt) {
          if (first) {
            subtype_box->insertItem (i18n("<none>"));
            current_subtypes.push_back(all_maintypes[idx-1].shortStr());
            first = false;
          }
          QString s = all_types[sub_idx].longStr();
          subtype_box->insertItem (s.simplified());
          current_subtypes.push_back(all_types[sub_idx].shortStr());
        }
      }
    }
  }
  type_dirty = true;

  subtype_box->setEnabled(!first);
  subtype_label->setEnabled(!first);
}


void CommonEntryPage::phoneticSelected(wchar_t wc)
{
  setModified(true);
  pronunce += QChar(wc);
  pronunce_line->setText(pronunce);
}


void CommonEntryPage::invokePronDlg()
{
  //if (phoneticDlg == 0) {
    PhoneticEntryPage * phoneticDlg = new PhoneticEntryPage (ipafont, this);
    connect (phoneticDlg, SIGNAL(charSelected(wchar_t)), SLOT(phoneticSelected(wchar_t)) );
    phoneticDlg->show();
  //}
  //else
    //phoneticDlg->show();
}


void CommonEntryPage::invokeUsageDlg()
{
  vector<int> usageIndex;
  vector<QString> new_usageStr;

  int old_usages = (int) doc->getUsageDescr().size();

  KDialogBase usageOpt(KDialogBase::Swallow,
    i18n("usage (area) of an expression", "Edit User-Defined Usage Labels"),
    KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, this, "usage", true);

  UsageOptPage *usageOptPage = new UsageOptPage (doc->getUsageDescr(), doc, this);
  usageOpt.setMainWidget(usageOptPage);

  if (usageOpt.exec() == QDialog::Accepted)
  {
    usageOptPage->getUsageLabels(new_usageStr, usageIndex);
    UsageOptPage::cleanUnused(doc, usageIndex, old_usages);
    UsageManager::setUsageNames (new_usageStr);
    setUsageBox(usageCollection);
    doc->setUsageDescr (new_usageStr);
    doc->setModified();
  }
}


void CommonEntryPage::invokeLessDlg()
{
  vector<int> lessonIndex;
  vector<QString> new_lessonStr;

  int old_lessons = (int) lesson_box->count();
  KDialogBase lessOpt(KDialogBase::Swallow, i18n("Edit Lesson Names"),
    KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, this, "lesson", true);

  LessOptPage *lessOptPage = new LessOptPage (lesson_box, doc, this);
  lessOpt.setMainWidget(lessOptPage);

  vector<int> lessoninquery = doc->getLessonsInQuery();
  if (lessOpt.exec() == QDialog::Accepted)
  {
    lessOptPage->getLesson(lesson_box, lessonIndex);
    LessOptPage::cleanUnused(doc, lesson_box, lessonIndex, old_lessons, lessoninquery);
    for (int i = 1; i < lesson_box->count(); i++)
      new_lessonStr.push_back(lesson_box->text(i));
    doc->setLessonDescr (new_lessonStr);
    doc->setLessonsInQuery(lessoninquery);
    querymanager.setLessonItems(lessoninquery);
    doc->setModified();
  }
}


void CommonEntryPage::invokeTypeDlg()
{
  vector<int> typeIndex;
  vector<QString> new_typeStr;

  int old_types = (int) doc->getTypeDescr().size();
  KDialogBase typeOpt(KDialogBase::Swallow, i18n("Edit User Defined Types"),
    KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, this, "types", true);

  TypeOptPage *typeOptPage = new TypeOptPage (doc->getTypeDescr(), doc, this);
  typeOpt.setMainWidget(typeOptPage);

  if (typeOpt.exec() == QDialog::Accepted)
  {
    typeOptPage->getTypeNames(new_typeStr, typeIndex);
    TypeOptPage::cleanUnused(doc, typeIndex, old_types);
    QueryManager::setTypeNames (new_typeStr);
    setTypeBox(type);
    doc->setTypeDescr (new_typeStr);
    doc->setModified();
  }
}


bool CommonEntryPage::isModified()
{
  return modified;
}


void CommonEntryPage::setEnabled(int enable)
{
  bool ena = enable == EntryDlg::EnableAll;

  usage_box->setEnabled(ena);
  subtype_box->setEnabled(ena);
  type_box->setEnabled(ena);
  pronunce_line->setEnabled(ena);
  expr_line->setEnabled(ena);

  lesson_box->setEnabled(ena || enable == EntryDlg::EnableOnlyCommon);
  c_active->setEnabled(ena || enable == EntryDlg::EnableOnlyCommon);
}


void CommonEntryPage::setModified(bool mod)
{
  modified = mod;
  if (mod)
    emit sigModified();
}

#include "CommonEntryPage.moc"
