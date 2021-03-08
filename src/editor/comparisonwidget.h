/*
    dialog page for multiple choice suggestions
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPARISONWIDGET_H
#define COMPARISONWIDGET_H

#include "ui_comparisonwidget.h"


class KEduVocExpression;
class KEduVocTranslation;
class KEduVocDocument;

namespace Editor
{

class ComparisonWidget : public QWidget, public Ui::comparisonWidget
{
    Q_OBJECT

public:
    explicit ComparisonWidget(QWidget *parent = 0);

public slots:
    /**
     * To know about word types that are currently defined.
     * @param doc
     */
    void setDocument(KEduVocDocument* doc);
    /**
     * Update the word
     * @param entry
     * @param translation
     */
    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void slotMakeAdjectiveButton();
    void slotMakeAdverbButton();

    void slotComparativeChanged();
    void slotSuperlativeChanged();

private:
    KEduVocTranslation* m_translation;
    KEduVocDocument* m_doc;
};

}

#endif
