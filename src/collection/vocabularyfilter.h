/*
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VOCABULARYFILTER_H
#define VOCABULARYFILTER_H

#include <QSortFilterProxyModel>

class KEduVocLesson;
class KEduVocExpression;

class VocabularyModel;

class VocabularyFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit VocabularyFilter(QObject *parent = 0);

    void setSourceModel(VocabularyModel* model);

    QModelIndex appendEntry(KEduVocExpression *expression = 0);
    KEduVocLesson * lesson();

public slots:
    void setSearchString(const QString& expression);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    void setSourceModel(QAbstractItemModel *model) Q_DECL_OVERRIDE {
        Q_UNUSED(model)
    }

    VocabularyModel * m_model;
    QString m_filterString;
};


#endif
