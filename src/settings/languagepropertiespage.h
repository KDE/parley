/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LANGUAGEPROPERTIESPAGE_H
#define LANGUAGEPROPERTIESPAGE_H

#include "ui_languagepropertiespage.h"

class KEduVocDocument;

class LanguagePropertiesPage : public QWidget, public Ui::LanguagePropertiesPage
{
    Q_OBJECT
public:
    LanguagePropertiesPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent = nullptr);
    void setLanguageIdentifierIndex(int newIndex);

public Q_SLOTS:
    void accept();
    void downloadGrammar();

Q_SIGNALS:
    void nameChanged(const QString &);

private Q_SLOTS:
    void localeChanged(const QString &);

    // pronouns/conjugations
    void updateCheckBoxes();

    // tenses
    void slotDeleteTense();
    void slotNewTense();
    void slotTenseChosen(int);
    void slotModifyTense();

private:
    void updateListBox(int start);

    // initialize widgets with contents
    void loadGrammarFromDocument();

    int m_currentTense;
    QList<int> tenseIndex;

    KEduVocDocument *m_doc{nullptr};
    int m_identifierIndex{0};
};

#endif
