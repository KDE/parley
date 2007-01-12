/***************************************************************************

                   types options dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#ifndef TypeOptPage_included
#define TypeOptPage_included

#include "ui_optionlistform.h"

class KEduVocDocument;

class TypeOptPage : public QWidget, public Ui::OptionListForm
{
  Q_OBJECT

public:
  TypeOptPage(KEduVocDocument *doc, QWidget *parent = 0);

  void getTypeNames(QStringList &ret_types, QList<int> &ret_Index);

  static void cleanUnused(KEduVocDocument *doc, const QList<int> &lessonIndex, int old_lessons);

protected:
  void updateListBox(int start);

protected slots:
  void slotDeleteType();
  void slotNewType();
  void slotTypeChosen(int);
  void slotModifyType();
  void slotCleanup();

private:
  KEduVocDocument  *doc;
  int               m_currentType;
  QList<int>        typeIndex; // contains indices of types on exec()
                               // negative values are new lessons
};

#endif // TypeOptPage_included
