/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include "containermodel.h"

#include "prefs.h"
#include "documentsettings.h"


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

    explicit StatisticsModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /** Indicate supported drag actions
     @return enum of actions supported **/
    Qt::DropActions supportedDragActions() const  Q_DECL_OVERRIDE;

    void updateDocumentSettings();

public slots:
    virtual void setDocument(KEduVocDocument *doc) Q_DECL_OVERRIDE;

protected:
    KEduVocContainer *rootContainer() const Q_DECL_OVERRIDE;

private:
    int averageGradeForPracticeMode(KEduVocContainer *container, int translation) const;
    int entryCountForPracticeMode(KEduVocContainer *container, int translation) const;
    int expressionsOfGradeForPracticeMode(KEduVocContainer *container, int translation,
                                          grade_t grade) const;
    void loadDocumentsSettings();

private:
    QList<QSharedPointer<DocumentSettings>> m_documentSettings;
};

// For index.data()
Q_DECLARE_METATYPE(KEduVocContainer*)


#endif
