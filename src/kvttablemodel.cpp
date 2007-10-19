/***************************************************************************

                             kvttablemodel

    -----------------------------------------------------------------------

    copyright     : (C) 2006-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "kvttablemodel.h"

#include "prefs.h"
#include "languagesettings.h"

#include <KIcon>
#include <KLocale>
#include <KDebug>
#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvocgrade.h>
#include <QSize>

KVTTableModel::KVTTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_doc = 0;
}


void KVTTableModel::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    //reset();
}


int KVTTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_doc->entryCount();
}


int KVTTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // languages + lesson column + active/inQuery column
    return 2 + m_doc->identifierCount();
}


QVariant KVTTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    //int defaultLessonIndex;

    switch (role) {
    case KVTTableModel::LessonsRole: {
            QStringList sl = m_doc->lessonNames();
            return QVariant(sl);
            break;
        }

    case KVTTableModel::LessonRole: {
            return QVariant(m_doc->entry(index.row())->lesson());
            break;
        }

    case KVTTableModel::StateRole: {
            if (m_doc->entry(index.row())->isActive()) {
                return 1;
            }
            return 0;
            break;
        }

    case KVTTableModel::GradeRole: {
            if (index.column() > KV_COL_TRANS) { // translation

                //kDebug() << "KVTTableModel::GradeRole column: " << index.column();

                return QVariant(m_doc->entry(index.row())->translation(index.column() - KV_COL_TRANS).gradeFrom(0).grade());

            } else if (index.column() == 2) { // original
                QList<QVariant> result;
                for (int i = 1; i < m_doc->identifierCount(); ++i) {
                    if (m_doc->entry(index.row())->translation(0).gradeFrom(i).queryCount() != 0)
                        result.append(QVariant(m_doc->entry(index.row())->translation(0).gradeFrom(i).grade()));
                    else
                        result.append(QVariant(KV_NORM_GRADE));
                }
                return QVariant(result);
            }
            break;
        }

    case Qt::FontRole: {
            return QVariant(Prefs::tableFont());
            break;
        }

    case Qt::DisplayRole: {
            QVariant result;
            if (index.column() == 0) {
                // Lesson is set to zero if none is set, but we don't allow this any more. Entries are rather moved into a default lesson.
                result = m_doc->lesson(m_doc->entry(index.row())->lesson()).name();
            } else if (index.column() == 1) {
                if (m_doc->entry(index.row())->isActive()) {
//                     return KIcon("ok");
                    return "@active@";
                } else
//                     return KIcon("no");
                    return "@inactive@";
            } else {
                result = m_doc->entry(index.row())->translation(index.column() - KV_COL_TRANS).text();
            }
            return result;
            break;
        }
//     case KVTTableModel::ExpressionRole: {
//             QVariant v;
//             v.setValue(m_doc->entry(index.row()));
//             return v;
//             break;
//         }

        /*case Qt::DecorationRole: {
         if (index.column() == 1)
         {
          if (!m_doc->entry(index.row())->isActive())
             return m_pixInactive;
           if (m_doc->entry(index.row())->isInQuery())
             return m_pixInQuery;
         }
         return QVariant();
         break;
        }*/

    default:
        break;
    }
    return QVariant();
}


QVariant KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            if (section == 0) // lesson
                return i18n("Lesson");
            else if (section == 1) {
                return QString();
            } else { // translations
                return m_doc->identifier(section - KV_COL_TRANS).name();
            }
        }
        if (role == Qt::DecorationRole) {
            if ( m_headerPixmaps.contains(section) ) {
                return m_headerPixmaps.value(section);
            }
        }
        if (role == Qt::SizeHintRole) {
            switch (section) {
            case 0:
                return QSize(m_doc->sizeHint(-1), 25);
                break;
            case 1:
                return QSize(25, 25);
                break;
            default:
                return QSize(m_doc->sizeHint(section - KV_COL_TRANS), 25);
                break;
            }
        }
        return QVariant();
    } else
        return QAbstractTableModel::headerData(section, orientation, role);
}


Qt::ItemFlags KVTTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


bool KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == 0)
            m_doc->entry(index.row())->setLesson(value.toInt());
        else if (index.column() == 1) {
            m_doc->entry(index.row())->setActive(value.toInt());
        } else {
            m_doc->entry(index.row())->setTranslation(index.column() - 2, value.toString());
        }
        emit dataChanged(index, index);
        m_doc->setModified(true);
        return true;
    } else if (index.isValid() && role == KVTTableModel::LessonRole) {
        m_doc->entry(index.row())->setLesson(value.toInt());
    }
    return false;
}


bool KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::SizeHintRole) {
            switch (section) {
            case 0:
                m_doc->setSizeHint(-1, qvariant_cast<QSize>(value).width());
                ;
            case 1: //
            default:
                m_doc->setSizeHint(section - KV_COL_TRANS, qvariant_cast<QSize>(value).width());
            }
        }
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


bool KVTTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count < 1 || row < 0) {
        return false;
    }

    int bottomRow = row + count -1;
    beginInsertRows(QModelIndex(), row, bottomRow);

    for (int i = bottomRow; i >= row; i--) {
        KEduVocExpression* entry = new KEduVocExpression(QString(), m_doc->currentLesson() );
        m_doc->insertEntry(entry, i);
    }

    endInsertRows();
    m_doc->setModified(true);
    return true;
}


bool KVTTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count < 1 || row < 0 || row + count > m_doc->entryCount()) {
        return false;
    }

    int bottomRow = row + count - 1;
    beginRemoveRows(QModelIndex(), row, bottomRow);

    for (int i = bottomRow; i >= row; i--)
        m_doc->removeEntry(i);

    endRemoveRows();
    m_doc->setModified(true);
    return true;
}


void KVTTableModel::appendTranslation()
{
    beginInsertColumns(QModelIndex(), columnCount(QModelIndex()), columnCount(QModelIndex()));

    m_doc->appendIdentifier();

    endInsertColumns();
    m_doc->setModified(true);
}


bool KVTTableModel::removeTranslation(int translationIndex)
{
    if( translationIndex < 0 || translationIndex >= m_doc->identifierCount() ) {
        return false;
    }

    beginRemoveColumns(QModelIndex(), translationIndex, translationIndex);
    m_doc->removeIdentifier(translationIndex - KV_COL_TRANS);
    endRemoveColumns();

    m_doc->setModified(true);
    return true;
}

void KVTTableModel::dataChangedFromOutside(const QModelIndex & firstRow, const QModelIndex & lastRow)
{
    kDebug() << "Updating: " << firstRow.row() << ", " << firstRow.column() << " to " << lastRow.row() << ", " << lastRow.column() << " columnCount: " << columnCount(QModelIndex());
    QModelIndex topLeft = index(firstRow.row()-1, 0);
    QModelIndex bottomRight = index(lastRow.row()+1, columnCount(QModelIndex()));
    emit dataChanged(topLeft, bottomRight);
}

void KVTTableModel::appendEntry()
{
    KEduVocExpression* entry = new KEduVocExpression(QString(), m_doc->currentLesson() );

    // insert one row
    beginInsertRows(QModelIndex(), m_doc->entryCount(), m_doc->entryCount());
    m_doc->insertEntry(entry, m_doc->entryCount());
    endInsertRows();
}

void KVTTableModel::loadLanguageSettings()
{
    reset();
    // Set the language headers of the table.
    for (int i=0; i<m_doc->identifierCount(); i++){
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();
        m_headerPixmaps[i + KV_COL_TRANS] = QPixmap(icon);

        emit headerDataChanged(Qt::Horizontal, i + KV_COL_TRANS, i + KV_COL_TRANS);
    }
}

#include "kvttablemodel.moc"


