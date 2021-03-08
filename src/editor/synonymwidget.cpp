/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "synonymwidget.h"

#include <KEduVocDocument>
#include <KEduVocTranslation>
#include <KEduVocExpression>

#include <QStringListModel>
#include <QDragEnterEvent>
#include <KLocalizedString>

using namespace Editor;

SynonymWidget::SynonymWidget(SynonymWidgetType type, QWidget *parent) : QWidget(parent)
{
    m_type = type;
    m_currentTranslation = 0;
    m_lastTranslation = 0;
    setupUi(this);

    connect(synonymButton, &QPushButton::clicked, this, &SynonymWidget::togglePair);
    m_listModel = new QStringListModel(this);     listView->setModel(m_listModel);

    updateList();
}

void SynonymWidget::setDocument(KEduVocDocument *doc)
{
    m_doc = doc;
}

void SynonymWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    // ignore zeros
    if (entry) {
        m_lastTranslation = m_currentTranslation;
        m_currentTranslation = entry->translation(translation);
        updateList();
    } else {
        // better play save, the entry most likely has been deleted.
        m_lastTranslation = 0;
        m_currentTranslation = 0;
        updateList();
    }
}

void SynonymWidget::updateList()
{
    // clear the list
    m_listModel->removeRows(0, m_listModel->rowCount());

    // set the button text
    if (!(m_lastTranslation && m_currentTranslation)) {
        synonymButton->setEnabled(false);
        synonymButton->setText(i18n("Select Synonyms"));
    } else {
        synonymButton->setEnabled(true);
        switch (m_type) {
        case Synonym:
            if (m_currentTranslation->synonyms().contains(m_lastTranslation)) {
                synonymButton->setText(i18n("%1 and %2 are not Synonyms", m_currentTranslation->text(), m_lastTranslation->text()));
            } else {
                synonymButton->setText(i18n("%1 and %2 are Synonyms", m_currentTranslation->text(), m_lastTranslation->text()));
            }
            break;
        case Antonym:
            if (m_currentTranslation->antonyms().contains(m_lastTranslation)) {
                synonymButton->setText(i18n("%1 and %2 are not Antonyms", m_currentTranslation->text(), m_lastTranslation->text()));
            } else {
                synonymButton->setText(i18n("%1 and %2 are Antonyms", m_currentTranslation->text(), m_lastTranslation->text()));
            }
            break;
        case FalseFriend:
            if (m_currentTranslation->falseFriends().contains(m_lastTranslation)) {
                synonymButton->setText(i18n("%1 and %2 are not False Friends", m_currentTranslation->text(), m_lastTranslation->text()));
            } else {
                synonymButton->setText(i18n("%1 and %2 are False Friends", m_currentTranslation->text(), m_lastTranslation->text()));
            }
            break;
        }
    }

    if (m_currentTranslation) {
        switch (m_type) {
        case Synonym:
            synonymLabel->setText(i18nc("Title for a list of synonyms for a word", "Synonyms of %1:", m_currentTranslation->text()));
            break;
        case Antonym:
            synonymLabel->setText(i18nc("Title for a list of antonyms (opposites) for a word", "Antonyms of %1:", m_currentTranslation->text()));
            break;
        case FalseFriend:
            synonymLabel->setText(i18nc("Title for a list of false friend (things that sound similar but have different meanings) for a word", "False Friends of %1:", m_currentTranslation->text()));
            break;
        }

        // load list of synonyms/antonyms/ffs
        QList< KEduVocTranslation* > list;
        switch (m_type) {
        case Synonym:
            list = m_currentTranslation->synonyms();
            break;
        case Antonym:
            list = m_currentTranslation->antonyms();
            break;
        case FalseFriend:
            list = m_currentTranslation->falseFriends();
            break;
        }
        foreach(KEduVocTranslation * translation, list) {
            int row = m_listModel->rowCount();
            m_listModel->insertRow(row);
            m_listModel->setData(m_listModel->index(row), translation->text());
        }
    } else {
        synonymLabel->clear();
    }
}

void SynonymWidget::togglePair()
{
    // pair them up
    switch (m_type) {
    case Synonym:
        if (m_currentTranslation->synonyms().contains(m_lastTranslation)) {
            m_currentTranslation->removeSynonym(m_lastTranslation);
            m_lastTranslation->removeSynonym(m_currentTranslation);
        } else {
            m_currentTranslation->addSynonym(m_lastTranslation);
            m_lastTranslation->addSynonym(m_currentTranslation);
        }
        break;
    case Antonym:
        if (m_currentTranslation->antonyms().contains(m_lastTranslation)) {
            m_currentTranslation->removeAntonym(m_lastTranslation);
            m_lastTranslation->removeAntonym(m_currentTranslation);
        } else {
            m_currentTranslation->addAntonym(m_lastTranslation);
            m_lastTranslation->addAntonym(m_currentTranslation);
        }
        break;
    case FalseFriend:
        if (m_currentTranslation->falseFriends().contains(m_lastTranslation)) {
            m_currentTranslation->removeFalseFriend(m_lastTranslation);
            m_lastTranslation->removeFalseFriend(m_currentTranslation);
        } else {
            m_currentTranslation->addFalseFriend(m_lastTranslation);
            m_lastTranslation->addFalseFriend(m_currentTranslation);
        }
        break;
    }

    m_doc->setModified(true);

    updateList();
}
