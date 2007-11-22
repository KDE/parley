/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin         : Mon Oct 29 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "MCEntryPage.h"

#include <keduvocmultiplechoice.h>
#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <QLineEdit>


MCEntryPage::MCEntryPage(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(mc1Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(mc2Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(mc3Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(mc4Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(mc5Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
}


void MCEntryPage::slotDataChanged(const QString&)
{

}


///@todo the lib now supports an arbitrary number of entries for multiple choice. We should reflect that and have a list rather than five fixed entries.
// also starting with 0 might be nicer. crashes though.
void MCEntryPage::setTranslation(KEduVocExpression * entry, int translation)
{
    if (entry) {
        KEduVocMultipleChoice mc = entry->translation(translation).multipleChoice();
        mc1Field->setText(mc.choice(0));
        mc2Field->setText(mc.choice(1));
        mc3Field->setText(mc.choice(2));
        mc4Field->setText(mc.choice(3));
        mc5Field->setText(mc.choice(4));
    }
}

void MCEntryPage::clear()
{
    mc1Field->setText(QString());
    mc2Field->setText(QString());
    mc3Field->setText(QString());
    mc4Field->setText(QString());
    mc5Field->setText(QString());
}



#include "MCEntryPage.moc"
