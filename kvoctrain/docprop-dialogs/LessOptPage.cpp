/***************************************************************************

                   lesson properties dialog page

    -----------------------------------------------------------------------

    begin         : Thu Jun 3 22:03:50 1999

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

#include <QComboBox>
#include <QPushButton>
#include <QLayout>
#include <QListWidgetItem>

#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "LessOptPage.h"
#include <keduvocdocument.h>

#define LESS_TAG ". "

LessOptPage::LessOptPage(KEduVocDocument *_doc, QWidget *parent) : QWidget(parent)
{
  setupUi(this);

  connect(lessonList, SIGNAL(currentRowChanged(int)), this, SLOT(slotLessonChosen(int)));
  connect(b_new,      SIGNAL(clicked()),              this, SLOT(slotNewLesson()));
  connect(b_modify,   SIGNAL(clicked()),              this, SLOT(slotModifyLesson()));
  connect(b_delete,   SIGNAL(clicked()),              this, SLOT(slotDeleteLesson()));
  connect(b_cleanup,  SIGNAL(clicked()),              this, SLOT(slotCleanup()));

  doc = _doc;

  int i = 1;
  foreach(QString lessonName, doc->lessonDescriptions()){
    lessonList->addItem(QString("%1. ").arg(i++, 2) + lessonName);
    lessonIndex.append(i - 1);
  }

  m_currentLesson = 0;
  if (lessonList->count() > 0)
    lessonList->setCurrentRow(m_currentLesson);
  lessonList->setFocus();

  b_modify->setEnabled(lessonList->count() > 0);
  b_delete->setEnabled(lessonList->count() > 0);
}


void LessOptPage::slotLessonChosen(int index)
{
  m_currentLesson = index;
}


void LessOptPage::slotNewLesson()
{
  bool ok;
  QString getLesson = KInputDialog::getText(i18n("Lesson Description"), i18n("Enter lesson description:"), QString(), &ok, this);
  if (!ok)
    return;

  int i = lessonList->count() + 1;
  lessonList->addItem(QString("%1. ").arg(i, 2) + getLesson.simplified());
  lessonIndex.append(-(i - 1));

  m_currentLesson = lessonList->count();
  lessonList->setCurrentRow(i - 1);
  b_modify->setEnabled(true);
  b_delete->setEnabled(true);
}


void LessOptPage::slotModifyLesson()
{
  if (lessonList->count() != 0 && (int) lessonList->count() > m_currentLesson)
  {
    QString str = lessonList->item(m_currentLesson)->text();
    int pos = str.indexOf (LESS_TAG);
    str.remove (0, pos+strlen (LESS_TAG));
    bool ok;
    QString getLesson = KInputDialog::getText(i18n("Lesson Description"), i18n("Enter lesson description:"), str, &ok, this);
    if( !ok )
      return;
    QString str2;
    str2.setNum (m_currentLesson + 1);
    if (m_currentLesson <= 9)
      str2.insert (0, " ");
    lessonList->item(m_currentLesson)->setText(str2+LESS_TAG+getLesson.simplified());
  }
}


void LessOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) lessonList->count(); i++)
  {
    str = lessonList->item(i)->text();
    int pos = str.indexOf (LESS_TAG);
    str.remove (0, pos+strlen (LESS_TAG));
    str2.setNum (i+1);
    if (i <= 9)
      str2.insert (0, " ");
    lessonList->item(i)->setText(str2+LESS_TAG+str);
  }
}


void LessOptPage::slotDeleteLesson()
{
  int act = m_currentLesson;
  if (lessonList->count() != 0
      && (int) lessonList->count() > act) {

    for (int ent = 0; ent < doc->numEntries(); ent++) {
      // FIXME: ProgressDlg here?
      if (doc->entry(ent)->lesson() == lessonIndex[m_currentLesson]+1) {
        KMessageBox::information(this,
                  i18n("This lesson could not be deleted\nbecause it is in use."),
                  i18n("Deleting Lesson"));
        return;
      }
    }

    lessonList->takeItem(act);
    lessonIndex.erase (lessonIndex.begin() + act);

    if ((int) lessonList->count() <= act)
      act = lessonList->count()-1;
    else
      updateListBox(act); // update items after current

    if (act >= 0)
      lessonList->setCurrentItem(lessonList->item(act));
  }
  b_modify->setEnabled(lessonList->count() != 0);
  b_delete->setEnabled(lessonList->count() != 0);
}


void LessOptPage::getLesson (QComboBox *ret_lesson, QList<int> &ret_Index)
{
  while (ret_lesson->count() > 1) /* first entry is "no lesson" */
    ret_lesson->removeItem (1);

  QString str;
  for (int i = 0; i < (int) lessonList->count(); i++) {
    str = lessonList->item(i)->text();
    int pos = str.indexOf (LESS_TAG);
    str.remove (0, pos+strlen (LESS_TAG));
    ret_lesson->addItem (str);
  }
  ret_Index = lessonIndex;
}


void LessOptPage::slotCleanup()
{
  QVector<bool> used_lesson;
  for (int i = 0; i < (int) lessonList->count(); i++)
    used_lesson.push_back(false);

  for (int i = 0; i < (int) doc->numEntries(); i++) {
    int idx = doc->entry(i)->lesson();
    if ((int) used_lesson.size() < idx)
      used_lesson.resize(idx);
    if (idx != 0)
      used_lesson[idx-1] = true ;
  }

  for (int i = used_lesson.size()-1; i >= 0; i--)
    if (!used_lesson[i]) {
      for (int u = 0; u < (int) lessonIndex.size() ; u++) {
        if (lessonIndex[u] == i || lessonIndex[u] < 0) {
          m_currentLesson = i;
          slotDeleteLesson();
          break;
        }
      }
    }

  m_currentLesson = 0;
  lessonList->setCurrentItem(lessonList->item(m_currentLesson));
}


void LessOptPage::cleanUnused (KEduVocDocument *doc,
                               const QComboBox * /*lessons*/,
                               const QList<int> &lessonIndex,
                               int old_lessons,
                               QList<int> &lessons_in_query)
{
  QList<int> translate_index;

  /////////////////////////////////////////////////////
  // translate_index contains new index number for each
  // old index
  for (int i = 0; i <= qMax (old_lessons, (int) lessonIndex.size()); i++)
    translate_index.push_back(0);

  // now adjust lesson descriptions to new index
  for (int i = 0; i < (int) lessonIndex.size(); i++) {
    if (lessonIndex[i] >= 0)
      translate_index[lessonIndex[i]+1] = i+1;
  }

  // now adjust for query selected lessons to new index
  for (int i = (int) lessons_in_query.size()-1; i >= 0; i--) {
    if (translate_index[lessons_in_query[i]] > 0)
      lessons_in_query[i] = translate_index[lessons_in_query[i]];
    else
      lessons_in_query.erase(lessons_in_query.begin() + i);
  }

  // only keep remaining lesson member indices

  // set lesson index to 0 when not needed any more
  // and translate to new index
  for (int i = 0; i < doc->numEntries(); i++) {
    if (doc->entry(i)->lesson () != 0)
      doc->entry(i)->setLesson (translate_index[doc->entry(i)->lesson ()]);
  }
}
#include "LessOptPage.moc"
