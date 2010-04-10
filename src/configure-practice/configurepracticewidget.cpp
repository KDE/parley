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
    , m_doc(doc)
    , m_initalLanguageRow(0)
{
    setupUi(this);

    const int totalNumLanguages = m_doc->identifierCount();
    if (Prefs::questionLanguage() >= totalNumLanguages || Prefs::solutionLanguage() >= totalNumLanguages) {
        Prefs::setQuestionLanguage(0);
        Prefs::setSolutionLanguage(1);
    }

    for ( int i = 0; i < totalNumLanguages-1; i++ ) {
        for (int j = i+1; j < totalNumLanguages; j++) {
            QListWidgetItem* item = new QListWidgetItem(
                i18nc("pair of two languages that the user chooses to practice", "%1 to %2",
                m_doc->identifier(i).name(), m_doc->identifier(j).name()));
            item->setData(Qt::UserRole, i);
            item->setData(Qt::UserRole+1, j);
            LanguageList->addItem(item);

            if (i == Prefs::questionLanguage() && j == Prefs::solutionLanguage()) {
                LanguageList->setCurrentItem(item);
            }
            QListWidgetItem* item2 = new QListWidgetItem(
                i18nc("pair of two languages that the user chooses to practice", "%1 to %2",
                m_doc->identifier(j).name(), m_doc->identifier(i).name()));
            item2->setData(Qt::UserRole, j);
            item2->setData(Qt::UserRole+1, i);
            LanguageList->addItem(item2);

            if (j == Prefs::questionLanguage() && i == Prefs::solutionLanguage()) {
                LanguageList->setCurrentItem(item);
            }
        }
    }

    connect(LanguageList, SIGNAL(currentRowChanged(int)), SLOT(languagesSelected(int)));

    LanguageList->sortItems();
    setupTenses();
}

void ConfigurePracticeWidget::updateSettings()
{
    Prefs::setQuestionLanguage(LanguageList->currentItem()->data(Qt::UserRole).toInt());
    Prefs::setSolutionLanguage(LanguageList->currentItem()->data(Qt::UserRole+1).toInt());

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

void ConfigurePracticeWidget::languagesSelected(int identifierFromIndex)
{
    // TODO kcfg dialog changed
    setupTenses();
}

void ConfigurePracticeWidget::updateWidgets()
{
}

bool ConfigurePracticeWidget::hasChanged()
{
    if (LanguageList->currentRow() != m_initalLanguageRow) {
        return true;
    }

    return false;
    // @todo tenses
}

bool ConfigurePracticeWidget::isDefault()
{
    return true;
}

void ConfigurePracticeWidget::setupTenses()
{
    int index = LanguageList->currentItem()->data(Qt::UserRole+1).toInt();
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
