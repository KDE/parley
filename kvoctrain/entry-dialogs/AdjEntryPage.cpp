/***************************************************************************

                   dialog page for adjectives (comparison)

    -----------------------------------------------------------------------

    begin         : Sat Dec 4 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#include "AdjEntryPage.h"

#include <QLineEdit>

#include <kvtlanguages.h>
#include "EntryDlg.h"

AdjEntryPage::AdjEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
    m_currentTranslation = -1;
    m_currentRow = -1;

    setupUi(this);

    connect(lev1Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(lev2Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
    connect(lev3Field, SIGNAL(textChanged(const QString&)), SLOT(slotDataChanged(const QString&)));
}


void AdjEntryPage::slotDataChanged(const QString& s)
{
    Q_UNUSED(s)
    emit sigModified();
}


bool AdjEntryPage::isModified()
{
    if ( m_currentRow < 0 || m_currentTranslation < 0 ) {
        return false;
    }

    KEduVocExpression *expr = m_doc->entry(m_currentRow);
    if ( expr->translation(m_currentTranslation).comparison().l1() != lev1Field->text() ) {
        return true;
    }
    if ( expr->translation(m_currentTranslation).comparison().l2() != lev2Field->text() ) {
        return true;
    }
    if ( expr->translation(m_currentTranslation).comparison().l3() != lev3Field->text() ) {
        return true;
    }
    return false;
}


void AdjEntryPage::setData(int row, int col)
{
    m_currentRow = row;
    m_currentTranslation = col;
    KEduVocExpression *expr = m_doc->entry(m_currentRow);
    lev1Field->setText(expr->translation(m_currentTranslation).comparison().l1());
    lev2Field->setText(expr->translation(m_currentTranslation).comparison().l2());
    lev3Field->setText(expr->translation(m_currentTranslation).comparison().l3());
}

void AdjEntryPage::commitData()
{
    KEduVocComparison comp;
    comp.setL1(lev1Field->text());
    comp.setL2(lev2Field->text());
    comp.setL3(lev3Field->text());

    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setComparison( comp );
}

void AdjEntryPage::clear()
{
    lev1Field->setText(QString());
    lev2Field->setText(QString());
    lev3Field->setText(QString());
}

#include "AdjEntryPage.moc"
