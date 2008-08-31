/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SUMMARYMODEL_H
#define SUMMARYMODEL_H

#include <practice/practiceentry.h>
#include <QAbstractTableModel>


class SummaryModel : public QAbstractTableModel
{
Q_OBJECT
public:
    SummaryModel(QList<PracticeEntry*> entries, QObject *parent = 0);

    int rowCount(const QModelIndex&) const;
    int columnCount(const QModelIndex&) const;
    QVariant data(const QModelIndex&, int) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

private:
    enum columns {
        Original = 0,
        Translation,
        UserAnswer,
        Points
    };

    QList<PracticeEntry*> m_entries;
};


#endif
