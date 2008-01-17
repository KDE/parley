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

#include "PhoneticEntryPage.h"

#include <KCharSelect>
#include <KLocalizedString>

PhoneticEntryPage::PhoneticEntryPage(const QFont &ipafont, QWidget *parent) : KDialog(parent)
{
    setCaption(i18n("Select Characters From Phonetic Alphabet"));
    setButtons(User1|Close);
    setDefaultButton(Close);

    setButtonText(User1, i18nc("Letter selection dialog for phonetic symbols.", "Insert Letter"));
    setButtonIcon(User1, KIcon("insert-text"));
    connect(this, SIGNAL(user1Clicked()), SLOT(slotInsertChar()));

    setModal(false);

    m_charSelect = new KCharSelect(this, KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable);
    setMainWidget(m_charSelect);
    m_charSelect->setCurrentChar(0x0250);
    connect(m_charSelect, SIGNAL(charSelected(const QChar &)), SIGNAL(charSelected(const QChar &)));
}


void PhoneticEntryPage::slotInsertChar()
{
    emit(charSelected(m_charSelect->currentChar()));
}


#include "PhoneticEntryPage.moc"
