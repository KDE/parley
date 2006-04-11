/***************************************************************************

                   user tense options dialog page

    -----------------------------------------------------------------------

    begin         : Sun May 28 12:14:31 2000

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

#ifndef TenseOptPage_included
#define TenseOptPage_included

#include "ui_TenseOptPageForm.h"

class KEduVocDocument;

class TenseOptPage : public QWidget, public Ui::TenseOptPageForm
{
  Q_OBJECT

public:
  TenseOptPage(const QStringList &tenses, KEduVocDocument * doc, QWidget *parent = 0);

  void getTenseNames (QStringList &ret_types, QList<int> &ret_Index);

  static void cleanUnused(KEduVocDocument *doc, const QList<int> &tenseIndex, int old_tenses);

protected:
  void updateListBox(int start);

protected slots:
  void slotDeleteTense();
  void slotNewTense();
  void slotTenseChosen(int);
  void slotModifyTense();
  void slotCleanup();

private:
  KEduVocDocument  *doc;
  int            act_tense;
  QList<int>    tenseIndex; // contains indices of tenses on exec()
                              // negative values are new tenses
};

#endif // TenseOptPage_included
