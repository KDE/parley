/***************************************************************************

                   types options dialog page

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

#ifndef TypeOptPage_included
#define TypeOptPage_included

#include "TypeOptPageForm.h"

#include <vector>
using namespace std;

class kvoctrainDoc;
class QStrList;

class TypeOptPage : public TypeOptPageForm
{
    Q_OBJECT

public:

  TypeOptPage
  (
    const vector<QString> &types,
    kvoctrainDoc    *doc,
    QWidget         *parent = NULL,
    const char      *name = NULL
  );

  void getTypeNames (vector<QString> &ret_types, vector<int> &ret_Index);

  static void cleanUnused(kvoctrainDoc *doc, const vector<int> &lessonIndex, int old_lessons);

protected:
  void updateListBox(int start);

protected slots:

  void slotDeleteType();
  void slotNewType();
  void slotTypeChosen(int);
  void slotModifyType();
  void slotCleanup();

private:
  kvoctrainDoc  *doc;
  int            act_type;
  vector<int>    typeIndex; // contains indices of types on exec()
                            // negative values are new lessons
};
#endif // TypeOptPage_included
