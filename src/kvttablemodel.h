/***************************************************************************

                             kvttablemodel

    -----------------------------------------------------------------------

    copyright     : (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#ifndef KVTTABLEMODEL_H
#define KVTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QPixmap>

#include <keduvocdocument.h>

#define KV_COL_LESS      0   // col: lesson number
#define KV_COL_MARK      1   // col: mark
#define KV_COL_TRANS     2   // col: first translation

// To enable use of KEduVocExpression* in a QVariant
// Q_DECLARE_METATYPE ( KEduVocExpression* )

/**
  The model for the big table in the main window. Here the vocabulary can be edited directly.
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/
class KVTTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum KVTItemDataRole {
        LessonsRole = Qt::UserRole + 1,
        LessonRole,
        StateRole,
        GradeRole
//         ExpressionRole
    };

    KVTTableModel(QObject *parent = 0);

    void setDocument(KEduVocDocument * doc);
    KEduVocDocument * document() const
    {
        return m_doc;
    }

    bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());

    /**
     * Adds a new translation
     */
    void appendTranslation();

    /**
     * Remove a translation from the document
     * @param translationIndex is the id of the translation
     * @return true if the translation was removed
     */
    bool removeTranslation(int translationIndex);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

public slots:
    void dataChangedFromOutside(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void appendEntry();

private:
    KEduVocDocument * m_doc;
    QPixmap m_pixInactive;
    QPixmap m_pixInQuery;

    friend class KVocTrainApp;
    friend class KVTStatisticsDialog;
};

#endif
