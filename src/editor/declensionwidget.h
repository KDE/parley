/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DECLENSIONWIDGET_H
#define DECLENSIONWIDGET_H

#include "ui_declensionwidget.h"

#include <KEduVocDeclension>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{
class DeclensionWidget : public QWidget, public Ui::DeclensionWidget
{
    Q_OBJECT

public:
    explicit DeclensionWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    void setDocument(KEduVocDocument *doc);
    void setTranslation(KEduVocExpression *entry, int translation);

    /**
     * Fill the line edits
     */
    void updateEntries();

    void nextNumber();

Q_SIGNALS:
    void sigModified();

private Q_SLOTS:
    void textChanged(const QString &);

private:
    int currentAdditionalWordFlag();
    void setupLineEdits();

    int m_identifier{-1};
    KEduVocExpression *m_entry{nullptr};
    KEduVocDocument *m_doc{nullptr};

    /**
     * All line edits and labels, index corresponding to KEduVocWordFlag::indexOf
     */
    QMap<int, QLineEdit *> m_DeclensionLineEdits;
};

}

#endif
