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
#include <QStringListModel>

MCEntryPage::MCEntryPage(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

}


void MCEntryPage::slotDataChanged(const QString&)
{

}


///@todo the lib now supports an arbitrary number of entries for multiple choice. We should reflect that and have a list rather than five fixed entries.
// also starting with 0 might be nicer. crashes though.
void MCEntryPage::setTranslation(KEduVocExpression * entry, int translation)
{
    if (entry) {
        multipleChoiceListView->setModel(new QStringListModel(entry->translation(translation)->multipleChoice(), this));
    }
}

void MCEntryPage::clear()
{

}



#include "MCEntryPage.moc"
