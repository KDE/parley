/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONJUGATIONWIDGET_H
#define CONJUGATIONWIDGET_H

#include "ui_conjugationwidget.h"

#include <KEduVocWordFlags>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{
class ConjugationWidget : public QWidget, public Ui::ConjugationWidget
{
    Q_OBJECT

public:
    explicit ConjugationWidget(QWidget *parent = 0);

public Q_SLOTS:
    void setDocument(KEduVocDocument *doc);
    void setTranslation(KEduVocExpression *entry, int translation);

Q_SIGNALS:
    void sigModified();

private Q_SLOTS:
    void textChanged(const QString &);
    void slotTenseSelected(int);
    void slotNextTense();
    void tenseEditingFinished();

private:
    void updateVisiblePersons();

    void showWidgets(bool tenses, bool singular, bool dual, bool plural, bool maleVisible, bool femaleVisible, bool neuterVisible);

    /**
     * Fill the line edits
     */
    void updateEntries();

    QString m_lastTenseSelection;
    int m_identifier{-1};
    KEduVocExpression *m_entry{nullptr};
    KEduVocDocument *m_doc{nullptr};

    /**
     * All line edits and labels, index corresponding to KEduVocWordFlag::indexOf
     */
    QMap<KEduVocWordFlags, QLineEdit *> m_conjugationLineEdits;
};

}

#endif
