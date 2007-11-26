/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "comparisonwidget.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <KDebug>

ComparisonWidget::ComparisonWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(makeAdjectiveButton, SIGNAL(clicked()), SLOT(slotMakeAdjectiveButton()));
    connect(makeAdverbButton, SIGNAL(clicked()), SLOT(slotMakeAdverbButton()));

    connect(comparativeLineEdit, SIGNAL(), SLOT());
    connect(superlativeLineEdit, SIGNAL(), SLOT());
}

void ComparisonWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    comparisonFormsGroup->setEnabled(false);
    comparativeLineEdit->setText(QString());
    superlativeLineEdit->setText(QString());

    if (entry) {
        m_translation = entry->translation(translation);
    } else {
        m_translation = 0;
    }

    if (m_translation) {
        absoluteEntryLabel->setText(m_translation->text());
        setEnabled(true);

        if(m_translation->wordType()) {
            if(m_translation->wordType()->containerType() == KEduVocLesson::WordTypeAdjectiveContainer
                || m_translation->wordType()->containerType() == KEduVocLesson::WordTypeAdverbContainer) {
                comparisonFormsGroup->setEnabled(true);

        ///@todo the comparison forms api is ugly
                comparativeLineEdit->setText(m_translation->comparison().l2());
                superlativeLineEdit->setText(m_translation->comparison().l3());
            }
        }
    } else {
        setEnabled(false);
        absoluteEntryLabel->setText(QString());
    }
}

void ComparisonWidget::slotMakeAdjectiveButton()
{
}

void ComparisonWidget::slotMakeAdverbButton()
{
}




#include "comparisonwidget.moc"
