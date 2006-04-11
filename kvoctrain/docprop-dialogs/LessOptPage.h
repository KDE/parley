/***************************************************************************

                   lesson properties dialog page

    -----------------------------------------------------------------------

    begin         : Thu Jun 3 22:03:50 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef LessOptPage_included
#define LessOptPage_included

#include "ui_LessOptPageForm.h"

class KEduVocDocument;
class QComboBox;

class LessOptPage : public QWidget, public Ui::LessOptPageForm
{
  Q_OBJECT

public:
  LessOptPage(QComboBox *lessons, KEduVocDocument *doc, QWidget *parent = 0);

  void getLesson (QComboBox *ret_lesson, QList<int>& ret_Index);

  static void cleanUnused(KEduVocDocument *doc,
                          const QComboBox *lessons,
                          const QList<int> &lessonIndex,
                          int old_lessons,
                          QList<int> &lessons_in_use);

protected:
  void updateListBox(int start);

protected slots:

  void slotDeleteLesson();
  void slotNewLesson();
  void slotLessonChosen(int);
  void slotModifyLesson();
  void slotCleanup();

private:
  KEduVocDocument  *doc;
  int            act_lesson;
  QList<int>    lessonIndex; // contains indices of lesson on exec()
                              // negative values are new lessons
};

#endif // LessOptPage_included
