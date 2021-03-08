/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SYNONYMWIDGET_H
#define SYNONYMWIDGET_H

#include "ui_synonymwidget.h"

#include <QModelIndex>

class KEduVocExpression;
class KEduVocTranslation;
class KEduVocDocument;
class QStringListModel;

namespace Editor
{

class SynonymWidget : public QWidget, public Ui::SynonymWidget
{
    Q_OBJECT
public:
    enum SynonymWidgetType {
        Synonym,
        Antonym,
        FalseFriend
    };

    explicit SynonymWidget(SynonymWidgetType type, QWidget *parent = 0);

public slots:
    /**
     * Register the document inside the widget so that it can be marked as modified.
     * @param doc
     */
    void setDocument(KEduVocDocument *doc);

    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void togglePair();
    void updateList();

private:
    KEduVocDocument  *m_doc;

    // the one that was selected before
    KEduVocTranslation* m_lastTranslation;
    // the one the user clicked on latest
    KEduVocTranslation* m_currentTranslation;
    QStringListModel * m_listModel;
    SynonymWidgetType m_type;
};
}
#endif
