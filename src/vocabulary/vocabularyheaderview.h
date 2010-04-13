/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VOCABULARYHEADERVIEW_H
#define VOCABULARYHEADERVIEW_H

#include <QHeaderView>

namespace Editor {
    
class VocabularyHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit VocabularyHeaderView ( Qt::Orientation orientation, QWidget * parent = 0 );

private slots:
    void updateSorting(int);

private:
    int m_sortSection;
};
}

#endif
