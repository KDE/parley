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

#include <QSize>
// #include <QIcon>

#include <KLocale>
#include <KDebug>

#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvocgrade.h>

#include "prefs.h"
#include "languagesettings.h"


KVTTableModel::KVTTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_doc = 0;
}

/*!
    \fn KVTTableModel::setDocument(KEduVocDocument * doc)
 */
void KVTTableModel::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    //reset();
}


/*!
    \fn KVTTableModel::rowCount(const QModelIndex &parent) const
 */
int KVTTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_doc->entryCount();
}


/*!
    \fn KVTTableModel::columnCount(const QModelIndex &parent) const
 */
int KVTTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    // languages + lesson column + active/inQuery column
    return 2 + m_doc->identifierCount();
}


/*!
    \fn KVTTableModel::data(const QModelIndex &index, int role) const
 */
QVariant KVTTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    //int defaultLessonIndex;

    switch (role) {
    case KVTTableModel::LessonsRole: {
            QStringList sl = m_doc->lessonNames();
            sl.prepend(m_doc->lesson(0).name());
            return QVariant(sl);
            break;
        }

    case KVTTableModel::LessonRole: {
            return QVariant(m_doc->entry(index.row())->lesson());
            break;
        }

    case KVTTableModel::StateRole: {
            if (!m_doc->entry(index.row())->isActive())
                return 2;
            else if (m_doc->entry(index.row())->isInQuery())
                return 1;
            else
                return 0;
            break;
        }

    case KVTTableModel::GradeRole: {
            if (index.column() > KV_EXTRA_COLS) { // translation

                //kDebug() << "KVTTableModel::GradeRole column: " << index.column();

                return QVariant(m_doc->entry(index.row())->translation(index.column() - KV_EXTRA_COLS).gradeFrom(0).grade());

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
                    if (m_doc->entry(index.row())->isInQuery())
                        return "@inquery@";
                    else
                        return "";
                } else
                    return "@inactive@";
            } else {
                result = m_doc->entry(index.row())->translation(index.column() - KV_EXTRA_COLS).text();
            }
            return result;
            break;
        }
    case KVTTableModel::ExpressionRole: {
            QVariant v;
            v.setValue(m_doc->entry(index.row()));
            return v;
            break;
        }

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


/*!
    \fn KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
 */
QVariant KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            if (section == 0) // lesson
                return i18n("Lesson");
            else if (section == 1) {
                return QString();
            } else { // translations
                return m_doc->identifier(section - KV_EXTRA_COLS).name();
            }
        }
        if (role == Qt::DecorationRole) {
            switch (section) {
            case 0:
                return QVariant();
                break;
            case 1:
                return QVariant();
                break;
            default: {
                kDebug() << "Getting pixmap...";
                LanguageSettings currentSettings(m_doc->identifier(section - KV_EXTRA_COLS).locale());
                currentSettings.readConfig();
                QString icon = currentSettings.icon();
                return QPixmap(icon);
                    ///@todo get pixmap from config
//                 int id = m_languages.indexShortId(m_doc->identifier(section - 2).name());

//                 if (id < 0)
//                     return QVariant();
//                 else
                    return QVariant(); ///@todo QPixmap(m_languages[id].pixmapFile());
                break;
            }
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
                return QSize(m_doc->sizeHint(section - KV_EXTRA_COLS), 25);
                break;
            }
        }
        return QVariant();
    } else
        return QAbstractTableModel::headerData(section, orientation, role);
}


/*!
    \fn KVTTableModel::flags(const QModelIndex &index) const
 */
Qt::ItemFlags KVTTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


/*!
    \fn KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
 */
bool KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (index.column() == 0)
            m_doc->entry(index.row())->setLesson(value.toInt());
        else if (index.column() == 1) {
            bool inq = false;
            bool act = true;
            if (value.toInt() == 0) {
                inq = false;
                act = true;
            } else if (value.toInt() == 1) {
                inq = true;
                act = true;
            } else if (value.toInt() == 2) {
                inq = false;
                act = false;
            }
            m_doc->entry(index.row())->setInQuery(inq);
            m_doc->entry(index.row())->setActive(act);
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


/*!
    \fn KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
 */
bool KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::EditRole) {
            /// @todo handle?
            /*if (section == 0)
              /// @todo handle
            else if (section == 1)
              /// @todo handle
            else*/
            if (section >= 2) {
                m_doc->identifier(section - 2).setName(value.toString());
            }
        }
        if (role == Qt::SizeHintRole) {
            switch (section) {
            case 0:
                m_doc->setSizeHint(-1, qvariant_cast<QSize>(value).width());
                ;
            case 1: //
            default:
                m_doc->setSizeHint(section - KV_EXTRA_COLS, qvariant_cast<QSize>(value).width());
            }
        }

        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

// void KVTTableModel::setLanguages(const KVTLanguageList & languages)
// {
//     m_languages = languages;
// }


bool KVTTableModel::appendEntry(KEduVocExpression *entry)
{
    // insert one row
    beginInsertRows(QModelIndex(), m_doc->entryCount(), m_doc->entryCount());
    m_doc->insertEntry(entry, m_doc->entryCount());
    endInsertRows();

kDebug() << "KVTTableModel::appendEntry() new lesson: " << entry->lesson() << "m_doc->e ->lesson: " << m_doc->entry(m_doc->entryCount()-1)->lesson() << " current lesson: " << m_doc->currentLesson();

    // should return a qmodelindex
    return true;
}



bool KVTTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count < 1 || row < 0 || row + count > m_doc->entryCount() || count >= m_doc->entryCount())
        return false;

    int bottomRow = row + count -1;
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = bottomRow; i >= row; i--)
        m_doc->removeEntry(i);

    endRemoveRows();
    m_doc->setModified(true);
    return true;
}


void KVTTableModel::appendTranslation()
{
    beginInsertColumns(QModelIndex(), columnCount(QModelIndex()), columnCount(QModelIndex()));

///@todo check if this is a good idea
    m_doc->appendIdentifier();
    int num = m_doc->entryCount() - 1;
    for (int i = 0; i < (int) num; i++) {
        KEduVocExpression *expr = m_doc->entry(i);
        expr->translation(num).setType(expr->translation(0).type());
    }

    endInsertColumns();
    m_doc->setModified(true);
}


bool KVTTableModel::removeTranslation(int translationIndex)
{
    if( translationIndex < 0 || translationIndex >= m_doc->identifierCount() ) {
        return false;
    }

    beginRemoveColumns(QModelIndex(), translationIndex, translationIndex);
    m_doc->removeIdentifier(translationIndex - KV_EXTRA_COLS);
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
    appendEntry( new KEduVocExpression(QString(), m_doc->currentLesson() ) );
}



#include "kvttablemodel.moc"
