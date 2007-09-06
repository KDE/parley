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

    void setSearchRegExp(const QRegExp& filter);
    void setWordType(const QRegExp& wordType);

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

    void setLessonSelection(int lessonSelection);
    void slotCurrentLessonChanged(int);
    void slotLessonsInQueryChanged();

protected:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

private:
    /// used by filterAcceptsRow()
    bool checkLesson(int sourceRow) const;
    bool checkSearch(int sourceRow) const;
    bool checkWordType(int sourceRow) const;

    KVTTableModel * m_sourceModel;
    int             m_lessonSelection;
    QRegExp         m_searchFilter;
    QRegExp         m_wordType;

    bool m_restoreNativeOrder;
};

#endif
