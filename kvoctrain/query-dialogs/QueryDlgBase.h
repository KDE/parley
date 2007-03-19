/***************************************************************************

                   base class for query dialogs

    -----------------------------------------------------------------------

    begin          : Thu Nov 25 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef Query_Dlg_Base_H
#define Query_Dlg_Base_H

#include <time.h>
#include <stdlib.h>

#include <QLabel>
#include <QCloseEvent>

#include <kdialog.h>

#include <kvtquery.h>
#include <keduvocgrammar.h>
#include "prefs.h"

class KEduVocExpression;
class KEduVocDocument;

class QLineEdit;
class QTextEdit;
class QLabel;
class QRadioButton;

class QueryDlgBase : public KDialog
{
  Q_OBJECT
public:
  enum Result { Unknown, Known, Timeout, StopIt };

  QueryDlgBase(const QString & caption, QWidget *parent = 0, const char *name = 0, bool modal = false);

  virtual ~QueryDlgBase ();

  /** compare two strings with simplified applied first */
  bool smartCompare (const QString&, const QString&, int level) const;

  /** compare lineEdit and string, set lineEdit style */
  bool verifyField(QLineEdit *field, const QString &really);
  /** set the lineEdit to default (enter mode, black font) */
  void resetField (QLineEdit *field);

  /** the same for QTextEdit and QRadioButton */
  bool verifyField(QTextEdit *field, const QString &really, bool mixed);
  void resetField (QTextEdit *field);

  void verifyButton(QRadioButton *radio, bool is_ok, QWidget *widget2 = 0);
  void resetButton (QRadioButton *radio, QWidget *widget2 = 0);

  // Show string after selecting known/unknown
  // depending on progress and randomness
  QString  getOKComment(int percent);
  QString  getNOKComment(int percent);
  QString  getTimeoutComment(int percent);

  int getRandom(int range)
    {
//        srand((unsigned int)time((time_t *)NULL));
        return (int) (range * ((1.0*rand())/RAND_MAX));
    }

  virtual void initFocus() const;

signals:
  void sigQueryChoice(QueryDlgBase::Result);
  void sigEditEntry(int row, int col);

protected:
  void setWidgetTextColorAndFontWeight(QWidget *widget, const QColor &color, QFont::Weight);

  virtual void closeEvent(QCloseEvent*e);
  virtual void slotUser1();

  struct RB_Label {
    RB_Label (QRadioButton* _rb, QLabel *_label)
      : rb(_rb), label(_label) {}

    QRadioButton  *rb;
    QLabel        *label;
  };

  int            q_row,
                 q_ocol,
                 q_tcol;
  KEduVocDocument  *kv_doc;
  KEduVocExpression *kv_exp;
  QString        translation;
  QTimer        *qtimer;
  int            timercount;
};


#endif // Query_Dlg_Base_H
