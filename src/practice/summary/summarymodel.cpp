/***************************************************************************
    Copyright 2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "practice/summary/summarymodel.h"

// #include <KIcon>
#include <KLocalizedString>
// #include <KDebug>
// #include <KMessageBox>
// #include <QPixmap>
#include <QColor>

SummaryModel::SummaryModel(QList<PracticeEntry*> entries, QObject *parent)
 : QAbstractTableModel(parent)
{
    m_entries = entries;
}


int SummaryModel::rowCount(const QModelIndex &index) const
{
    // only the root index has children because we have no hierarchical model.
    if (index == QModelIndex()) {
        return m_entries.size();
    }
    return 0;
}

int SummaryModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant SummaryModel::data(const QModelIndex & index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case Original:
            return m_entries.value(index.row())->expression()->translation(0)->text();
        case Translation:
            return m_entries.value(index.row())->expression()->translation(1)->text();
        case UserAnswer:
            return m_entries.value(index.row())->incorrectAnswers().join(i18nc("the separator for a list of wrong words that the user entered", "; "));
        case Points:
            if (m_entries.value(index.row())->correct() && (m_entries.value(index.row())->attempted() == 1)) {
                return 1;
            } else {
                return 0;
            }
        default:
            return QVariant();
        }
        break;

    case Qt::TextColorRole:
        if (index.column() == UserAnswer) {

            return Qt::red; ///@todo should really use NegativeText from kcm;
        }
    }

    return QVariant();
}


Qt::ItemFlags SummaryModel::flags(const QModelIndex & index) const
{
    return QAbstractItemModel::flags(index);
//         return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant SummaryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal ) {
        switch (role) {
            case Qt::DisplayRole:
                switch (section) {
                case Original:
                    return i18n("Original Word");
                case Translation:
                    return i18n("Translation");
                case UserAnswer:
                    return i18n("False Answers");
                case Points:
                    return i18n("Points");
                }

            break;
//         case Qt::DecorationRole:
//             switch(entryColumn){
//             case Audio:
//                 return KIcon("speaker");
//             case Image:
//                 return KIcon("view-preview");
//             }

//         case Qt::SizeHintRole:
//             switch (entryColumn) {
//             case Audio:
//             case Image:
//                 return QSize(25, 25);
//             }
        } // switch role
    } // if horizontal
    return QVariant();
}


#include "summarymodel.moc"


