/***************************************************************************

             dialog page for characters from the phonetic alphabet

   -----------------------------------------------------------------------
    Copyright 2001-2002 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#ifndef PHONETICENTRYPAGE_H
#define PHONETICENTRYPAGE_H

#include <QPushButton>
#include <QKeyEvent>

#include <kdialog.h>
#include <keduvocdocument.h>

class KCharSelect;

class PhoneticEntryPage : public KDialog
{
    Q_OBJECT

public:
    PhoneticEntryPage(const QFont &ipafont, QWidget *parent);

signals:
    void charSelected(const QChar &c);

private slots:
    void slotInsertChar();

private:
    KCharSelect* m_charSelect;
};

#endif
