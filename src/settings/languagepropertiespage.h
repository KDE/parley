/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef LANGUAGEPROPERTIESPAGE_H
#define LANGUAGEPROPERTIESPAGE_H

#include "ui_languagepropertiespage.h"

class KEduVocDocument;
class ParleyDocument;

class LanguagePropertiesPage : public QWidget, public Ui::LanguagePropertiesPage
{
    Q_OBJECT
public:
    LanguagePropertiesPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent = 0);
    void setLanguageIdentifierIndex(int newIndex);

public slots:
    void accept();
    void downloadGrammar();

signals:
    void nameChanged(const QString&);

private slots:
    void localeChanged(const QString&);

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

    KEduVocDocument* m_doc;
    int m_identifierIndex;
};

#endif
