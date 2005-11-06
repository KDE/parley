/***************************************************************************

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin          : Sun Sep 12 15:38:31 1999

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

#ifndef UsageOptPage_included
#define UsageOptPage_included

#include <vector>

#include <Q3StrList>

#include "UsageOptPageForm.h"
#include <UsageManager.h>

using namespace std;

class kvoctrainDoc;
class Q3StrList;

class UsageOptPage : public QWidget, public Ui::UsageOptPageForm
{
  Q_OBJECT

public:
  UsageOptPage(const vector<QString> &usages, kvoctrainDoc *doc, QWidget *parent = 0);

  void getUsageLabels (vector<QString> &ret_usages, vector<int> &ret_Index);

  static void cleanUnused(kvoctrainDoc *doc, const vector<int> &lessonIndex, int old_lessons);

protected:
  void updateListBox(int start);

protected slots:
  void slotDeleteUsage();
  void slotNewUsage();
  void slotUsageChosen(int);
  void slotModifyUsage();
  void slotCleanup();

private:
  kvoctrainDoc  *doc;
  int            act_usage;
  vector<int>    usageIndex; // contains indices of usages on exec()
                            // negative values are new lessons
};

#endif // UsageOptPage_included
