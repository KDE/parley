/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VOCABULARYFILTER_H
#define VOCABULARYFILTER_H

#include <QSortFilterProxyModel>

class VocabularyModel;


class VocabularyFilter : public QSortFilterProxyModel
{
Q_OBJECT
public:
    VocabularyFilter(QObject *parent = 0);

    ~VocabularyFilter();

    void setSourceModel(VocabularyModel* model);

    QModelIndex appendEntry();

private:
    VocabularyModel * m_model;
};

#endif
