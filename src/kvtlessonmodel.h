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

#ifndef KVTLESSONMODEL_H
#define KVTLESSONMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>

#include <QObject>

#include <keduvocdocument.h>

/**
  * Model for the list of lessons. This is a list model for now. It obtains its data from the kvtml document.
  * QStringListModel is a bit simpler, but I want to handle the m_doc interface here to keep it simple from the outside.
  */
class KVTLessonModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /** When splitting a lesson into smaller ones - how to sort the entries into lessons.*/
    enum SplitLessonOrder {
        sorted,    /**< The order of the entries in the document */
        random /**< Randomized */
    };

    KVTLessonModel(QObject *parent = 0) : QAbstractListModel(parent)
    {
        m_doc = 0;
    }
    /** Set the new source kvtml file
     * @param doc the new file */
    void setDocument(KEduVocDocument * doc);
    KEduVocDocument * document() const
    {
        return m_doc;
    }
    /** The number of Lessons.
     * @param parent will always be QModelIndex() as long as we only have a list here
     * @return number of lessons */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /** Header of the treeview */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    /** flags of the items - see model/view
     * @param index item
     * @return flags */
    Qt::ItemFlags flags(const QModelIndex &index) const;
    /** Get the data of an entry.
     * @param index index of an entry
     * @param role Qt::DisplayRole = lesson name, Qt::CheckStateRole = checkbox state
     * @return data */
    QVariant data(const QModelIndex &index, int role) const;

    /** Change the name or checkbox of a lesson.
     * @param index which lesson
     * @param value new name
     * @param role
     * @return bool @c true it worked */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
//  bool newData(const QStringList &strings);

    int currentLesson() const
    {
        return m_doc->currentLesson();
    }
    void setCurrentLesson(int index)
    {
        m_doc->setCurrentLesson(index);
        setModified(true);
    } //move this into cpp?
    void setModified(bool modified)
    {
        m_doc->setModified(modified);
    }

    /** Make all lessons checked for query */
    void setAllLessonsInPractice();
    /** Make all lessons UNchecked for query */
    void setNoLessonsInPractice();

    // add a lesson - returns lesson index
    int addLesson(const QString &lessonName = QString());
    // returns whether it was possible to delete
    bool deleteLesson(int lessonIndex, KEduVocDocument::LessonDeletion mode);

    Qt::DropActions supportedDropActions() const;

    bool removeRows(int row, int count, const QModelIndex &parent);
    bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);


    /**
     * Divide a lesson into smaller ones.
     * Tip: If you create a lesson that is >= the original one and use random order, you get your lesson reshuffled. Maybe that is sometimes useful. For now the lessons are appended at the end.
     * @param lessonIndex lesson to split
     * @param entriesPerLesson number of entries in each new lesson
     * @param order one of SplitLessonOrder
     */
    void splitLesson(int lessonIndex, int entriesPerLesson, SplitLessonOrder order);
//signals:
    //void signalLessonsInPracticeChanged(QList<int> intLessons);
private:
    /** The data file */
    KEduVocDocument *m_doc;

//public slots:
    //void slotLessonSelectionChanged(const QModelIndex &, const QModelIndex &);
};

#endif
