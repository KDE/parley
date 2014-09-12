/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

class KEduVocLesson;
class KEduVocExpression;

namespace Editor
{
class VocabularyModel;

class VocabularyFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    VocabularyFilter(QObject *parent = 0);

    void setSourceModel(VocabularyModel* model);

    QModelIndex appendEntry(KEduVocExpression *expression = 0);
    KEduVocLesson * lesson();

public slots:
    void setSearchString(const QString& expression);

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    virtual void setSourceModel(QAbstractItemModel *model) {
        Q_UNUSED(model)
    }

    VocabularyModel * m_model;
    QString m_filterString;
};
}

#endif
