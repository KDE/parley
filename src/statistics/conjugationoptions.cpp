/*
    SPDX-FileCopyrightText: 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "conjugationoptions.h"

#include "languagesettings.h"
#include "documentsettings.h"
#include "prefs.h"

#include <KEduVocDocument>

#include <KLocalizedString>
#include <QStackedLayout>
#include <QLabel>
#include <QTreeWidget>

ConjugationOptions::ConjugationOptions(KEduVocDocument *doc, QWidget *parent)
    : QWidget(parent)
    , m_doc(doc)
    , m_language(0)
{
    m_treeWidget = new QTreeWidget(this);
    m_treeWidget->setHeaderLabel(i18n("Tenses"));
    m_treeWidget->setRootIsDecorated(false);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_treeWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(m_treeWidget, &QTreeWidget::itemChanged,
            this, &ConjugationOptions::processCheckBoxChanged);
}

void ConjugationOptions::setLanguages(int from, int to)
{
    Q_UNUSED(from)
    if (to >= m_doc->identifierCount()) {
        to = 0;
    }
    m_language = to;
    setupTenses();
}

void ConjugationOptions::setupTenses()
{
    m_treeWidget->blockSignals(true);
    m_treeWidget->clear();
    m_checkStates.clear();

    DocumentSettings documentSettings(m_doc->url().url() + QString::number(m_language));
    documentSettings.load();
    QStringList activeTenses = documentSettings.conjugationTenses();
    qDebug() << "activeTenses:" << activeTenses << " id tenses: " << m_doc->identifier(m_language).tenseList();
    QTreeWidgetItem* tenseItem;

    foreach(const QString & tenseName, m_doc->identifier(m_language).tenseList()) {
        tenseItem = new QTreeWidgetItem(m_treeWidget);
        tenseItem->setText(0, tenseName);
        if (activeTenses.contains(tenseName)) {
            tenseItem->setCheckState(0, Qt::Checked);
            m_checkStates[tenseItem] = Qt::Checked;
        } else {
            tenseItem->setCheckState(0, Qt::Unchecked);
            m_checkStates[tenseItem] = Qt::Unchecked;
        }
        tenseItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        m_treeWidget->addTopLevelItem(tenseItem);
    }
    m_treeWidget->blockSignals(false);
}

void ConjugationOptions::updateSettings()
{
    qDebug() << "Save language selection";
    QTreeWidgetItem *parentItem = m_treeWidget->invisibleRootItem();
    if (parentItem == nullptr) {
        return;
    }
    QStringList activeTenses;
    for (int i = 0; i < parentItem->childCount(); i++) {
        QTreeWidgetItem *tenseItem = parentItem->child(i);
        if (tenseItem->checkState(0) == Qt::Checked) {
            activeTenses.append(tenseItem->text(0));
        }
    }
    DocumentSettings documentSettings(m_doc->url().url() + QString::number(m_language));
    documentSettings.setConjugationTenses(activeTenses);
    documentSettings.save();
}

void ConjugationOptions::processCheckBoxChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) {
        return;
    }
    Qt::CheckState newCheckState = item->checkState(column);
    if (m_checkStates.contains(item) && (m_checkStates[item] != newCheckState)) {
        m_checkStates[item] = newCheckState;
        updateSettings();
        emit checkBoxChanged();
    }
}

