/***************************************************************************

                             kvttablemodel

    -----------------------------------------------------------------------

    copyright     : (C) 2006-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <KLocale>
#include <KDebug>

#include "prefs.h"
#include <keduvocexpression.h>

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

    int defaultLessonIndex;

    switch (role) {
    case KVTTableModel::LessonsRole: {
            QStringList sl = m_doc->lessonDescriptions();
            sl.prepend(m_doc->lessonDescription(0));
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

                return QVariant(m_doc->entry(index.row())->translation(index.column() - KV_EXTRA_COLS + 1).gradeFrom(0).grade());

            } else if (index.column() == 2) { // original
                QList<QVariant> result;
                for (int i = 1; i < m_doc->identifierCount(); ++i) {
                    if (m_doc->entry(index.row())->translation(0).gradeFrom(i).queryCount() != 0)
                        result.append(QVariant(m_doc->entry(index.row())->translation(0).gradeFrom(i /*+ KV_EXTRA_COLS*/).grade()));
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
                if (m_doc->entry(index.row())->lesson() == 0) {
                    // find entry for default lesson
                    defaultLessonIndex = m_doc->lessonIndex(i18n("Default lesson"));
                    if (defaultLessonIndex <= 0) { // create it if it does not exist
                        QStringList list = m_doc->lessonDescriptions();
                        list.append(i18n("Default lesson"));
                        m_doc->setLessonDescriptions(list);
                        defaultLessonIndex = m_doc->lessonIndex(i18n("Default lesson"));
                    }
                    m_doc->entry(index.row())->setLesson(defaultLessonIndex);
                    result = i18n("Default lesson");
                } else
                    result = m_doc->lessonDescription(m_doc->entry(index.row())->lesson());
            } else if (index.column() == 1) {
                if (m_doc->entry(index.row())->isActive()) {
                    if (m_doc->entry(index.row())->isInQuery())
                        return "@inquery@";
                    else
                        return "";
                } else
                    return "@inactive@";
            } else {
                result = m_doc->entry(index.row())->translation(index.column() - KV_EXTRA_COLS).translation();
            }
            return result;
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
            else if (section == 1)
                return "";
            else if (section == 2) { // original
                // try for known two letter code
                int id = m_languages.indexShortId(m_doc->originalIdentifier());

                if (id < 0) // otherwise for known longId
                    id = m_languages.indexLongId(m_doc->originalIdentifier());

                if (id < 0) // if still unknown take what's there
                    return m_doc->originalIdentifier();
                else // return the nice long language name
                    return m_languages[id].longId();

            } else { // translations
                // two letter?
                int id = m_languages.indexShortId(m_doc->identifier(section - 2));

                if (id < 0) // longId?
                    id = m_languages.indexLongId(m_doc->identifier(section - 2));

                if (id < 0) // nothing found
                    return m_doc->identifier(section - 2);
                else // long name
                    return m_languages[id].longId();
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
            case 2: {
                int id = m_languages.indexShortId(m_doc->originalIdentifier());

                if (id < 0)
                    return QVariant();
                else
                    return QPixmap(m_languages[id].pixmapFile());
                break;
            }
            default: {
                int id = m_languages.indexShortId(m_doc->identifier(section - 2));

                if (id < 0)
                    return QVariant();
                else
                    return QPixmap(m_languages[id].pixmapFile());
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
            else*/ if (section == 2)
                m_doc->setOriginalIdentifier(value.toString());
            else
                m_doc->setIdentifier(section - 2, value.toString());
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
        m_doc->setModified(true);
        return true;
    }
    return false;
}

void KVTTableModel::setLanguages(const KVTLanguageList & languages)
{
    m_languages = languages;
}

bool KVTTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
kDebug() << "KVTTableModel::insertRows() row: " << row << " count: " << count;

    //Q_UNUSED(parent);
    if (count < 1 || row < 0 || row > m_doc->entryCount()) {
        return false;
    }

    beginInsertRows(parent, row, row + count -1);

    for (int i = row; i < row + count; i++) {
        m_doc->insertEntry(new KEduVocExpression, i);
    }

    endInsertRows();
    m_doc->setModified(true);

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


bool KVTTableModel::insertColumns(int column, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    beginInsertColumns(QModelIndex(), column, column + count - 1);

    m_doc->appendIdentifier("");
    int num = m_doc->entryCount() - 1;
    for (int i = 0; i < (int) num; i++) {
        KEduVocExpression *expr = m_doc->entry(i);
        expr->translation(num).setType(expr->translation(0).type());
    }

    endInsertColumns();
    m_doc->setModified(true);
    return true;
}


bool KVTTableModel::removeColumns(int column, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    beginRemoveColumns(QModelIndex(), column, column + count - 1);

    m_doc->removeIdentifier(column - KV_EXTRA_COLS);

    endRemoveColumns();

    m_doc->setModified(true);
    return true;
}

#include "kvttablemodel.moc"
