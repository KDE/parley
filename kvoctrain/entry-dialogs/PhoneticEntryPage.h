 /***************************************************************************

    $Id:$

              dialog page for characters from the phonetic alphabet

    -----------------------------------------------------------------------

    begin                : Sun Dec 9 2001

    copyright            : (C) 1999-2002 Ewald Arnold
                           (C) 2001-2002 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log: $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PHONETICENTRYPAGE_H
#define PHONETICENTRYPAGE_H

#include <PhoneticEntryPageForm.h>

#include <qpushbutton.h>

class PhoneticEntryPage : public PhoneticEntryPageForm
{
    Q_OBJECT

public:
	PhoneticEntryPage(const QFont &ipafont, QWidget *parent = 0, const char *name = 0);
	
protected slots:
  void slotAbort();

signals:
  void wantClose();
  void charSelected(wchar_t);

protected:
  void keyPressEvent( QKeyEvent *e );

  friend class PhoneticButton;
};


class PhoneticButton : public QPushButton
{
  Q_OBJECT

public:

  PhoneticButton (const QString & text, QWidget * parent,
                  PhoneticEntryPage *_page, const char * name = 0)
    : QPushButton (text, parent, name),
      page(_page) {}

protected slots:
  void slotClicked();

protected:
  PhoneticEntryPage *page;
};


#endif
