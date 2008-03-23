/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "synonymwidget.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <KDebug>

#include <QStringListModel>
#include <QDragEnterEvent>

SynonymWidget::SynonymWidget(SynonymWidgetType type, QWidget *parent) : QWidget(parent)
{
    m_type = type;
    m_currentTranslation = 0;
    m_lastTranslation = 0;
    setupUi(this);

//     connect(translationLineEdit, SIGNAL(), SLOT());
    connect(synonymButton, SIGNAL(clicked()), SLOT(togglePair()));
//     connect(listView, SIGNAL(), SLOT());
// 
    m_listModel = new QStringListModel(this);     listView->setModel(m_listModel);

    setEnabled(false);
}


void SynonymWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    if (m_currentTranslation) {
        m_lastTranslation = m_currentTranslation;
    }

    m_currentTranslation = entry->translation(translation);

    if (m_currentTranslation) {
        synonymButton->setEnabled(true);
        updateList();
        if (m_lastTranslation) {
            if (m_currentTranslation->synonyms().contains(m_lastTranslation)) {
                synonymButton->setText(i18n("%1 and %2 are not Synonyms", m_currentTranslation->text(), m_lastTranslation->text()));
            } else {
                synonymButton->setText(i18n("%1 and %2 are Synonyms", m_currentTranslation->text(), m_lastTranslation->text()));
            }
        }
    }

    if (m_lastTranslation || m_currentTranslation) {
        setEnabled(true);
    }

}


void SynonymWidget::updateList()
{
    synonymLabel->setText(i18nc("Title for a list of synonyms for a word", "Synonyms of %1", m_currentTranslation->text()));

    // load list of old synonyms
    m_listModel->removeRows(0, m_listModel->rowCount());
    QList< KEduVocTranslation* > list;
    switch(m_type) {
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
    foreach (KEduVocTranslation* translation, list) {
        int row = m_listModel->rowCount();
        m_listModel->insertRow(row);
        m_listModel->setData(m_listModel->index(row), translation->text());
    }
}

void SynonymWidget::togglePair()
{
    if (m_currentTranslation->synonyms().contains(m_lastTranslation)) {
        // break up
        
    } else {
        m_currentTranslation->addSynonym(m_lastTranslation);
        m_lastTranslation->addSynonym(m_currentTranslation);

        int row = m_listModel->rowCount();
        m_listModel->insertRow(row);
        m_listModel->setData(m_listModel->index(row), m_lastTranslation->text());

        synonymButton->setText("not synonyms");
    }
}


#include "synonymwidget.moc"

