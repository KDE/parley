/***************************************************************************

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "startpracticewidget.h"

#include "languagesettings.h"
#include "prefs.h"

#include <keduvocdocument.h>

#include <KStandardDirs>
#include <KLocale>
#include <QLabel>
#include <QtDBus>

/**
 *
 * @param parent
 */
StartPracticeWidget::StartPracticeWidget(KEduVocDocument* doc, QWidget * parent)
    : QWidget(parent)
{
    m_doc = doc;
    setupUi(this);



    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();
        LanguageFromList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );
    }

    connect(LanguageFromList, SIGNAL(currentRowChanged(int)), this, SLOT(fromLanguageSelected(int)));
    connect(GrammarRadio, SIGNAL(toggled(bool)), this, SLOT(grammarTestToggled(bool)));

    LanguageFromList->setCurrentRow(Prefs::fromIdentifier());

    GrammarGroupBox->setEnabled( Prefs::testType() == Prefs::EnumTestType::GrammarTest );
}


void StartPracticeWidget::updateSettings()
{
    Prefs::setFromIdentifier(LanguageFromList->currentRow());
    Prefs::setToIdentifier(LanguageToList->currentItem()->data(Qt::UserRole).toInt());
}

void StartPracticeWidget::fromLanguageSelected(int identifierFromIndex)
{
    LanguageToList->clear();
    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        if ( i != identifierFromIndex ) {

            LanguageSettings currentSettings(m_doc->identifier(i).locale());
            currentSettings.readConfig();
            QString icon = currentSettings.icon();
            LanguageToList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );

            LanguageToList->item(LanguageToList->count()-1)->setData(Qt::UserRole, i);
            if ( i == Prefs::toIdentifier() ) {
                LanguageToList->setCurrentRow(i);
            }
        }
    }
    if ( LanguageToList->currentRow() < 0 ) {
        LanguageToList->setCurrentRow(0);
    }
}

void StartPracticeWidget::grammarTestToggled(bool state)
{
    GrammarGroupBox->setEnabled(state);
}


void StartPracticeWidget::updateWidgets()
{
    LanguageFromList->setCurrentRow(Prefs::fromIdentifier());
}

bool StartPracticeWidget::hasChanged()
{
    kDebug()<< "StartPracticeWidget::hasChanged()";
    int toRow = LanguageToList->currentRow();
    if ( toRow <= LanguageFromList->currentRow() ) {
        toRow++;
    }
    ///@todo implement so that the current settings are respected
    return false;
        //LanguageFromList->currentRow() == Prefs::fromIdentifier() &&
        //    toRow == Prefs::toIdentifier();
}

bool StartPracticeWidget::isDefault()
{
    ///@todo language selection
    return true;
//         LanguageFromList->currentRow() == 0 &&
//         LanguageToList->currentRow() == 1;
}


#include "startpracticewidget.moc"
