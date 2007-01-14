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
  titleGroupBox->setTitle(i18n("Lesson Descriptions"));

  connect(optionsList,   SIGNAL(currentRowChanged(int)), this, SLOT(slotLessonChosen(int)));
  connect(newButton,     SIGNAL(clicked()),              this, SLOT(slotNewLesson()));
  connect(modifyButton,  SIGNAL(clicked()),              this, SLOT(slotModifyLesson()));
  connect(deleteButton,  SIGNAL(clicked()),              this, SLOT(slotDeleteLesson()));
  connect(cleanUpButton, SIGNAL(clicked()),              this, SLOT(slotCleanup()));

  doc = _doc;

  int i = 1;
  foreach(const QString &lessonName, doc->lessonDescriptions()){
    optionsList->addItem(QString("%1").arg(i++, 2).append(LESS_TAG).append(lessonName));
    lessonIndex.append(i - 1);
  }

  m_currentLesson = 0;
  if (optionsList->count() > 0)
    optionsList->setCurrentRow(m_currentLesson);
  optionsList->setFocus();

  modifyButton->setEnabled(optionsList->count() > 0);
  deleteButton->setEnabled(optionsList->count() > 0);
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

  int i = optionsList->count() + 1;
  optionsList->addItem(QString("%1").arg(i, 2).append(LESS_TAG).append(getLesson.simplified()));
  lessonIndex.append(-(i - 1));

  m_currentLesson = optionsList->count();
  optionsList->setCurrentRow(i - 1);
  modifyButton->setEnabled(true);
  deleteButton->setEnabled(true);
}


void LessOptPage::slotModifyLesson()
{
  if (optionsList->count() > 0 && (int) optionsList->count() > m_currentLesson)
  {
    QString str = optionsList->item(m_currentLesson)->text();
    str = str.mid(str.indexOf(LESS_TAG) + QString(LESS_TAG).length());

    bool ok;
    QString getLesson = KInputDialog::getText(i18n("Lesson Description"), i18n("Enter lesson description:"), str, &ok, this);
    if( !ok )
      return;

    int i = m_currentLesson + 1;
    optionsList->item(m_currentLesson)->setText(QString("%1").arg(i, 2).append(LESS_TAG).append(getLesson.simplified()));
  }
}


void LessOptPage::updateListBox(int start)
{
  QString str;
  for (int i = start; i < (int) optionsList->count(); i++)
  {
    str = optionsList->item(i)->text();
    str = str.mid(str.indexOf(LESS_TAG) + QString(LESS_TAG).length());
    optionsList->item(i)->setText(QString("%1").arg(i + 1, 2).append(LESS_TAG).append(str));
  }
}


void LessOptPage::slotDeleteLesson()
{
  int act = m_currentLesson;
  if (optionsList->count() > 0 && (int) optionsList->count() > act) {

    for (int ent = 0; ent < doc->numEntries(); ent++) {
      if (doc->entry(ent)->lesson() == lessonIndex[act]) {
        KMessageBox::information(this, i18n("The selected lesson could not be deleted\nbecause it is in use."), i18n("Deleting Lesson"));
        return;
      }
    }

    delete optionsList->takeItem(act);
    lessonIndex.erase(lessonIndex.begin() + act);

    if ((int) optionsList->count() <= act)
      act = optionsList->count() - 1;
    else
      updateListBox(act); // update items after current

    if (act >= 0)
      optionsList->setCurrentItem(optionsList->item(act));
  }
  modifyButton->setEnabled(optionsList->count() > 0);
  deleteButton->setEnabled(optionsList->count() > 0);
}


void LessOptPage::slotCleanup()
{
  QVector<bool> used_lesson;
  for (int i = 0; i < (int) optionsList->count(); i++)
    used_lesson.append(false);

  for (int i = 0; i < (int) doc->numEntries(); i++) {
    int idx = doc->entry(i)->lesson();
    if ((int) used_lesson.count() < idx)
      used_lesson.resize(idx);
    if (idx != 0)
      used_lesson[idx - 1] = true ;
  }

  for (int i = used_lesson.count() - 1; i >= 0; i--)
    if (!used_lesson[i]) {
      for (int u = 0; u < (int) lessonIndex.count() ; u++) {
        if (lessonIndex[u] == i || lessonIndex[u] < 0) {
          m_currentLesson = i;
          slotDeleteLesson();
          break;
        }
      }
    }

  m_currentLesson = 0;
  optionsList->setCurrentItem(optionsList->item(m_currentLesson));
}


void LessOptPage::getLesson(QStringList &ret_lesson, QList<int> &ret_Index)
{
  ret_lesson.clear();

  QString str;
  for (int i = 0; i < (int) optionsList->count(); i++) {
    str = optionsList->item(i)->text();
    ret_lesson.append(str.mid(str.indexOf(LESS_TAG) + QString(LESS_TAG).length()));
  }
  ret_Index = lessonIndex;
}


void LessOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &lessonIndex, int old_lessons, QList<int> &lessons_in_query)
{
  QList<int> translate_index;

  /////////////////////////////////////////////////////
  // translate_index contains new index number for each
  // old index
  for (int i = 0; i <= qMax (old_lessons, (int) lessonIndex.count()); i++)
    translate_index.append(0);

  // now adjust lesson descriptions to new index
  for (int i = 0; i < (int) lessonIndex.size(); i++) {
    if (lessonIndex[i] >= 0)
      translate_index[lessonIndex[i] /*+ 1*/] = i + 1;
  }

  // now adjust for query selected lessons to new index
  for (int i = (int) lessons_in_query.count() - 1; i >= 0; i--) {
    if (translate_index[lessons_in_query[i]] > 0)
      lessons_in_query[i] = translate_index[lessons_in_query[i]];
    else
      lessons_in_query.erase(lessons_in_query.begin() + i);
  }

  // only keep remaining lesson member indices

  // set lesson index to 0 when not needed any more
  // and translate to new index
  for (int i = 0; i < doc->numEntries(); i++) {
    if (doc->entry(i)->lesson() != 0)
      doc->entry(i)->setLesson(translate_index[doc->entry(i)->lesson()]);
  }
}
#include "LessOptPage.moc"
