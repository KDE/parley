/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2011 Jan Gerrit Marker <jangerrit@weiler-marker.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef INFLECTIONWIDGET_H
#define INFLECTIONWIDGET_H

#include <memory>

#include <QStackedWidget>

#include <KEduVocDeclension>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{
class DeclensionWidget;
class ConjugationWidget;

class InflectionWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit InflectionWidget(QWidget *parent = 0);

public slots:
    void setDocument(const std::shared_ptr<KEduVocDocument> &doc);
    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void setWordType();

private:
    QWidget *m_wordTypeWidget;
    DeclensionWidget *m_declensionWidget;
    ConjugationWidget *m_conjugationWidget;

    std::shared_ptr<KEduVocDocument> m_doc;
    KEduVocExpression *m_entry;
    int m_translation;
};

}

#endif
