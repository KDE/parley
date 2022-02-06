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
    explicit InflectionWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    void setDocument(const std::shared_ptr<KEduVocDocument> &doc);
    void setTranslation(KEduVocExpression *entry, int translation);

private Q_SLOTS:
    void setWordType();

private:
    QWidget *m_wordTypeWidget{nullptr};
    DeclensionWidget *m_declensionWidget{nullptr};
    ConjugationWidget *m_conjugationWidget{nullptr};

    std::shared_ptr<KEduVocDocument> m_doc{nullptr};
    KEduVocExpression *m_entry{nullptr};
    int m_translation{-1};
};

}

#endif
