/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2011 Jan Gerrit Marker <jangerrit@weiler-marker.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef INFLECTIONWIDGET_H
#define INFLECTIONWIDGET_H

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
    void setDocument(KEduVocDocument* doc);
    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void setWordType();

private:
    QWidget *m_wordTypeWidget;
    DeclensionWidget *m_declensionWidget;
    ConjugationWidget *m_conjugationWidget;

    KEduVocDocument *m_doc;
    KEduVocExpression *m_entry;
    int m_translation;
};

}

#endif
