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

#include "configurepracticewidget.h"

#include "ui_comparisonoptionswidget.h"
#include "ui_conjugationoptionswidget.h"
#include "ui_writtenpracticeoptionswidget.h"
#include "ui_multiplechoiceoptionswidget.h"
#include "ui_flashcardsoptionswidget.h"

#include "languagesettings.h"
#include "documentsettings.h"
#include "prefs.h"

#include <keduvocdocument.h>

#include <KStandardDirs>
#include <KLocale>
#include <QStackedLayout>
#include <QLabel>
#include <QtDBus>

/**
 *
 * @param parent
 */
ConfigurePracticeWidget::ConfigurePracticeWidget(KEduVocDocument* doc, QWidget * parent)
    : QWidget(parent)
{
    m_doc = doc;
    m_bilingual = true;

    setupUi(this);

    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();
        if (icon.isEmpty()) {
            icon = QString("set-language");
        }
        LanguageFromList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );
    }

    connect(LanguageFromList, SIGNAL(currentRowChanged(int)), SLOT(fromLanguageSelected(int)));
    LanguageFromList->setCurrentRow(Prefs::questionLanguage());

    setupTenses();
}


void ConfigurePracticeWidget::updateSettings()
{
    Prefs::setQuestionLanguage(LanguageFromList->currentRow());
    Prefs::setSolutionLanguage(LanguageToList->currentItem()->data(Qt::UserRole).toInt());

    QTreeWidgetItem* parentItem = tenseSelectionTreeWidget->invisibleRootItem();
    QStringList activeTenses;
    for ( int i = 0; i < parentItem->childCount(); i++ ) {
        QTreeWidgetItem* tenseItem = parentItem->child(i);
        if ( tenseItem->checkState(0) == Qt::Checked ) {
            activeTenses.append(tenseItem->text(0));
        }
    }

    DocumentSettings documentSettings(m_doc->url().url());
    documentSettings.setConjugationTenses(activeTenses);
    documentSettings.writeConfig();
}

void ConfigurePracticeWidget::fromLanguageSelected(int identifierFromIndex)
{
    LanguageToList->clear();
    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        if ( i != identifierFromIndex ) {

            LanguageSettings currentSettings(m_doc->identifier(i).locale());
            currentSettings.readConfig();
            QString icon = currentSettings.icon();
            if (icon.isEmpty()) {
                icon = QString("set-language");
            }
            LanguageToList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );

            LanguageToList->item(LanguageToList->count()-1)->setData(Qt::UserRole, i);
            if ( i == Prefs::solutionLanguage() ) {
                LanguageToList->setCurrentRow(i);
            }
        }
    }
    if ( LanguageToList->currentRow() < 0 ) {
        LanguageToList->setCurrentRow(0);
    }
    setupTenses();
}

void ConfigurePracticeWidget::updateWidgets()
{
    LanguageFromList->setCurrentRow(Prefs::questionLanguage());
}

bool ConfigurePracticeWidget::hasChanged()
{
    kDebug()<< "ConfigurePracticeWidget::hasChanged()";
    int toRow = LanguageToList->currentRow();
    if ( toRow <= LanguageFromList->currentRow() ) {
        toRow++;
    }
    if (LanguageFromList->currentRow() == Prefs::questionLanguage() &&
        toRow == Prefs::solutionLanguage())
        return false;

    return true;

        /// @todo tenses
}

bool ConfigurePracticeWidget::isDefault()
{
    ///@todo language selection and tenses
    return true;
//         LanguageFromList->currentRow() == 0 &&
//         LanguageToList->currentRow() == 1;
}

void ConfigurePracticeWidget::setupTenses()
{
    int index = LanguageFromList->currentRow();
    if (index < 0) {
        index = 0;
    }

    QTreeWidget *tenseListWidget = tenseSelectionTreeWidget;
    tenseListWidget->clear();

    DocumentSettings currentSettings(m_doc->url().url() + QString::number(index));
    currentSettings.readConfig();
    QStringList activeTenses = currentSettings.conjugationTenses();
    QTreeWidgetItem* tenseItem;

    foreach ( const QString &tenseName, m_doc->identifier(index).tenseList() ) {
        tenseItem = new QTreeWidgetItem(tenseListWidget);
        tenseItem->setText(0, tenseName);
        if ( activeTenses.contains( tenseName ) ) {
            tenseItem->setCheckState(0, Qt::Checked);
        } else {
            tenseItem->setCheckState(0, Qt::Unchecked);
        }
        tenseItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        tenseListWidget->addTopLevelItem( tenseItem );
    }
    ///@todo emit changed when checkstate changed
}


#include "configurepracticewidget.moc"
