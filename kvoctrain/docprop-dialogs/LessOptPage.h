/***************************************************************************

                   lesson properties dialog page

    -----------------------------------------------------------------------

    begin          : Thu Jun 3 22:03:50 1999

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

#ifndef LessOptPage_included
#define LessOptPage_included

#include <vector>

#include "LessOptPageForm.h"

using namespace std;

class kvoctrainDoc;
class QComboBox;

class LessOptPage : public QWidget, public Ui::LessOptPageForm
{
  Q_OBJECT

public:
  LessOptPage(QComboBox *lessons, kvoctrainDoc *doc, QWidget *parent = 0);

  void getLesson (QComboBox *ret_lesson, vector<int>& ret_Index);

  static void cleanUnused(kvoctrainDoc *doc,
                          const QComboBox *lessons,
                          const vector<int> &lessonIndex,
                          int old_lessons,
                          vector<int> &lessons_in_use);

protected:
  void updateListBox(int start);

protected slots:

  void slotDeleteLesson();
  void slotNewLesson();
  void slotLessonChosen(int);
  void slotModifyLesson();
  void slotCleanup();

private:
  kvoctrainDoc  *doc;
  int            act_lesson;
  vector<int>    lessonIndex; // contains indices of lesson on exec()
                              // negative values are new lessons
};

#endif // LessOptPage_included
