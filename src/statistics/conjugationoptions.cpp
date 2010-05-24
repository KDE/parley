/**************************************************************************
    Copyright 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "conjugationoptions.h"

#include "languagesettings.h"
#include "documentsettings.h"
#include "prefs.h"

#include <keduvocdocument.h>

#include <KStandardDirs>
#include <KLocale>
#include <QStackedLayout>
#include <QLabel>
#include <QtDBus>
#include <QTreeWidget>

ConjugationOptions::ConjugationOptions(KEduVocDocument* doc, QWidget * parent)
    : QWidget(parent)
    , m_doc(doc)
    , m_language(0)
{
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel(i18n("Tenses"));
    m_treeWidget->setRootIsDecorated(false);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_treeWidget);
    layout->setMargin(0);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

void ConjugationOptions::setLanguages(int from, int to)
{
    Q_UNUSED(from)
    m_language = to;
    setupTenses();
}

void ConjugationOptions::setupTenses()
{
    m_treeWidget->clear();

    DocumentSettings documentSettings(m_doc->url().url() + QString::number(m_language));
    documentSettings.readConfig();
    QStringList activeTenses = documentSettings.conjugationTenses();
    kDebug() << "activeTenses:" << activeTenses << " id tenses: " << m_doc->identifier(m_language).tenseList();
    QTreeWidgetItem* tenseItem;

    foreach ( const QString &tenseName, m_doc->identifier(m_language).tenseList() ) {
        tenseItem = new QTreeWidgetItem(m_treeWidget);
        tenseItem->setText(0, tenseName);
        if ( activeTenses.contains( tenseName ) ) {
            tenseItem->setCheckState(0, Qt::Checked);
        } else {
            tenseItem->setCheckState(0, Qt::Unchecked);
        }
        tenseItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        m_treeWidget->addTopLevelItem( tenseItem );
    }
}

void ConjugationOptions::updateSettings()
{
    kDebug() << "Save language selection";
    QTreeWidgetItem* parentItem = m_treeWidget->invisibleRootItem();
    QStringList activeTenses;
    for ( int i = 0; i < parentItem->childCount(); i++ ) {
        QTreeWidgetItem* tenseItem = parentItem->child(i);
        if ( tenseItem->checkState(0) == Qt::Checked ) {
            activeTenses.append(tenseItem->text(0));
        }
    }
    DocumentSettings documentSettings(m_doc->url().url() + QString::number(m_language));
    documentSettings.setConjugationTenses(activeTenses);
    documentSettings.writeConfig();
}

#include "conjugationoptions.moc"
