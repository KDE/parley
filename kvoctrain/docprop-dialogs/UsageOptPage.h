/***************************************************************************

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#ifndef UsageOptPage_included
#define UsageOptPage_included

#include "UsageOptPageForm.h"
#include <UsageManager.h>

class KEduVocDocument;

class UsageOptPage : public QWidget, public Ui::UsageOptPageForm
{
  Q_OBJECT

public:
  UsageOptPage(const QStringList &usages, KEduVocDocument *doc, QWidget *parent = 0);

  void getUsageLabels (QStringList &ret_usages, QList<int> &ret_Index);

  static void cleanUnused(KEduVocDocument *doc, const QList<int> &lessonIndex, int old_lessons);

protected:
  void updateListBox(int start);

protected slots:
  void slotDeleteUsage();
  void slotNewUsage();
  void slotUsageChosen(int);
  void slotModifyUsage();
  void slotCleanup();

private:
  KEduVocDocument  *doc;
  int            act_usage;
  QList<int>    usageIndex; // contains indices of usages on exec()
                            // negative values are new lessons
};

#endif // UsageOptPage_included
