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
    explicit VocabularyFilter(QObject *parent = nullptr);

    void setSourceModel(VocabularyModel *model);

    QModelIndex appendEntry(KEduVocExpression *expression = nullptr);
    KEduVocLesson *lesson();

public Q_SLOTS:
    void setSearchString(const QString &expression);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    void setSourceModel(QAbstractItemModel *model) override
    {
        Q_UNUSED(model)
    }

    VocabularyModel *m_model;
    QString m_filterString;
};

#endif
