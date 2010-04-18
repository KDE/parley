/***************************************************************************
    Copyright 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONJUGATIONOPTIONS_H
#define CONJUGATIONOPTIONS_H

#include <QWidget>

class KEduVocDocument;
class QTreeWidget;

class ConjugationOptions :public QWidget
{
    Q_OBJECT
public:
    ConjugationOptions(KEduVocDocument* doc, QWidget *parent);

public Q_SLOTS:
    void setLanguages(int from, int to);
    void updateSettings();
    
private:
    void setupTenses();

    KEduVocDocument* m_doc;
    int m_language;
    QTreeWidget* m_treeWidget;
};

#endif
