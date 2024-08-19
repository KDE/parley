/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include "containermodel.h"

#include "documentsettings.h"
#include "prefs.h"

class StatisticsModel : public ContainerModel
{
    Q_OBJECT

public:
    enum GradeRoles {
        TotalPercent = Qt::UserRole,
        TotalCount,
        Grade0,
        Grade1,
        Grade2,
        Grade3,
        Grade4,
        Grade5,
        Grade6,
        Grade7,
        Container,
        ActiveConjugationTenses
    };

    explicit StatisticsModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /** Indicate supported drag actions
     @return enum of actions supported **/
    Qt::DropActions supportedDragActions() const override;

    void updateDocumentSettings();
    std::shared_ptr<KEduVocDocument> document() const;

public Q_SLOTS:
    void setDocument(const std::shared_ptr<KEduVocDocument> &doc) override;

protected:
    KEduVocContainer *rootContainer() const override;

private:
    int averageGradeForPracticeMode(KEduVocContainer *container, int translation) const;
    int entryCountForPracticeMode(KEduVocContainer *container, int translation) const;
    int expressionsOfGradeForPracticeMode(KEduVocContainer *container, int translation, grade_t grade) const;
    void loadDocumentsSettings();

private:
    QList<QSharedPointer<DocumentSettings>> m_documentSettings;
};

// For index.data()
Q_DECLARE_METATYPE(KEduVocContainer *)

#endif
