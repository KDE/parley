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

#include <keduvocarticle.h>
#include <keduvocconjugation.h>

class KEduVocDocument;

class LanguagePropertiesPage : public QWidget, public Ui::LanguagePropertiesPage
{
    Q_OBJECT
public:
    LanguagePropertiesPage(KEduVocDocument *doc, int identifierIndex, QWidget *parent = 0);

public slots:
    void accept();

signals:
    void nameChanged(const QString&);
    void iconSelected(const QString&);

private slots:
    void iconChanged(int iconIndex);
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

    // tenses
    int               m_currentTense;
    QList<int>        tenseIndex; // contains indices of tenses on exec()
    // negative values are new tenses

    // general
    KEduVocDocument* m_doc;
    int m_identifierIndex;
};

#endif
