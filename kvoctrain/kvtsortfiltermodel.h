/***************************************************************************

                             kvtsortfiltermodel

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                    (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KVTSORTFILTERMODEL_H
#define KVTSORTFILTERMODEL_H

#include <QSortFilterProxyModel>


/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/

class KVTTableModel;

class KVTSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

    KVTSortFilterModel(QObject *parent = 0);

    void setSourceModel(KVTTableModel * sourceModel);
    KVTTableModel * sourceModel() const;

    /** Returns true if a row is to be displayed. Respects the lesson setting and the search expression */
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    void setLessonRegExp(const QRegExp& filter);


    void setSearchRegExp(const QRegExp& filter);

public slots:
    /**
     * React to changes in the search line.
     * @param s String to search for
     */
    void slotSearch(const QString& s);

    /**
     * Put entries in the order they are in the file.
     */
    void restoreNativeOrder();

protected:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

private:
    /// used by filterAcceptsRow()
    bool checkLesson(int sourceRow, const QModelIndex &sourceParent) const;
    bool checkSearch(int sourceRow, const QModelIndex &sourceParent) const;

    KVTTableModel * m_sourceModel;
    QRegExp m_lessonFilter;
    QRegExp m_searchFilter;

    bool m_restoreNativeOrder;
};

#endif
