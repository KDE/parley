/***************************************************************************

                             kvtlessonmodel

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#ifndef CONTAINERMODEL_H
#define CONTAINERMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include <QObject>

#include <keduvocdocument.h>
#include <keduvoclesson.h>

/**
  * Model for the tree of lessons.
  */
class ContainerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /** When splitting a lesson into smaller ones - how to sort the entries into lessons.*/
    enum SplitLessonOrder {
        Sorted,    /**< The order of the entries in the document */
        Random /**< Randomized */
    };


    ContainerModel(KEduVocLesson::EnumContainerType type, QObject *parent = 0);
//     ~ContainerModel(); no need for cleanup - the doc will do that

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(KEduVocContainer* container) const;

    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    KEduVocContainer::EnumContainerType containerType();

    Qt::DropActions supportedDropActions () const;
    QStringList mimeTypes() const;
    QMimeData * mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
        int row, int column, const QModelIndex &parent);



    /** Change the name or checkbox of a lesson.
     * @param index which lesson
     * @param value new name
     * @param role
     * @return bool @c true it worked */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    // add a lesson - returns lesson index
//     QModelIndex addLesson(const QString &lessonName = QString());

    QModelIndex appendLesson(const QModelIndex& parent, const QString & lessonName = QString());

    // returns whether it was possible to delete
    void deleteLesson(const QModelIndex& lessonIndex);

    /**
     * Used for drag and drop, does not delete the lessons!
     * @param row 
     * @param count 
     * @param parent 
     * @return 
     */
//     bool removeRows(int row, int count, const QModelIndex &parent);


    /**
     * Divide a lesson into smaller ones.
     * Tip: If you create a lesson that is >= the original one and use random order, you get your lesson reshuffled. Maybe that is sometimes useful. For now the lessons are appended at the end.
     * @param lessonIndex lesson to split
     * @param entriesPerLesson number of entries in each new lesson
     * @param order one of SplitLessonOrder
     */
    void splitLesson(const QModelIndex& containerIndex, int entriesPerLesson, SplitLessonOrder order);

public slots:
    /** Set the new source kvtml file
     * @param doc the new file */
    void setDocument(KEduVocDocument *doc);

signals:
    /**
     * emitted when the inPractice state or name of a lesson changed.
     */
    void documentModified();


private:
    KEduVocContainer * m_container;
    KEduVocLesson::EnumContainerType m_type;
};


#include <QMimeData>

class ContainerMimeData :public QMimeData {
    Q_OBJECT
public:
    void addContainer(KEduVocContainer* container);
    QList<KEduVocContainer*> containerList() const;

private:
    QList<KEduVocContainer*> m_containers;
};


#endif
