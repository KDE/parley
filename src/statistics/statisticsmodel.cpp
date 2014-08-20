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

#include "statisticsmodel.h"

#include "keduvoctranslation.h"
#include "keduvocexpression.h"

#include <KLocalizedString>
#include <QGradient>
#include <QDebug>

StatisticsModel::StatisticsModel(QObject * parent)
    : ContainerModel(KEduVocContainer::Lesson, parent)
{
    setSupportedDragActions(0);
}

QVariant StatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= FirstDataColumn) {
        if (role == Qt::DisplayRole
            && m_doc->identifierCount() > (section - FirstDataColumn) ) {
            return i18nc("Confidence level in language, table header", "Confidence (%1)"
                         , m_doc->identifier(section - FirstDataColumn).name());
        }
    }
    return ContainerModel::headerData(section, orientation, role);
}

QVariant StatisticsModel::data(const QModelIndex & index, int role) const
{
    if (index.column() >= FirstDataColumn) {
        KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());
        switch (role) {
        case AllFractions: //Calculate the percent each grade and pregrade occupies
        {
            QVector<double> sums( KV_MAX_GRADE + KV_MAX_GRADE + 1, 0);
            double count( 0 );
            foreach (KEduVocExpression *entry, container->entries( KEduVocContainer::Recursive ) ) {
                KEduVocTranslation & trans( *entry->translation(index.column() - FirstDataColumn) );
                ++count;
                if ( !trans.isEmpty() ) {
                    if ( trans.grade() != 0 ) {
                        sums[trans.grade() + KV_MAX_GRADE + 1] += 1;
                    } else {
                        sums [trans.preGrade() + 1] += 1;
                    }
                }else{
                    sums[0] += 1;
                 }
             }
            QList< QVariant > fractions;
            for( int ii =0 ;ii < KV_MAX_GRADE + KV_MAX_GRADE + 1; ++ii) {
                fractions.push_back(( double )( sums[ii] / count ));
            }
            return fractions;
        }
        case TotalPercent: // Average grade
            return container->averageGrade(index.column() - FirstDataColumn, KEduVocContainer::Recursive);
        case TotalCount:
            return container->entryCount(KEduVocContainer::Recursive);
        default:
            if (role >= Qt::UserRole) {
                return container->expressionsOfGrade(
                    index.column() - FirstDataColumn, role - Grade0, KEduVocContainer::Recursive);
            }
        }
    }

    // checkboxes
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        KEduVocContainer *container = static_cast<KEduVocContainer*>(index.internalPointer());
        if (container->inPractice()) {
            return Qt::Checked;
        } else {
            return Qt::Unchecked;
        }
    }

    return ContainerModel::data(index, role);
}



Qt::ItemFlags StatisticsModel::flags(const QModelIndex & index) const
{
    if (index.isValid()) {
        if (index.column() == 0) {
            return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        }
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
    return 0;
}

int StatisticsModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_doc->identifierCount() + FirstDataColumn;
}

KEduVocContainer * StatisticsModel::rootContainer() const
{
    if (!m_doc) {
        return 0;
    }
    return m_doc->lesson();
}

#include "statisticsmodel.moc"
