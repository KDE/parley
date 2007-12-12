/***************************************************************************

    Copyright 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VOCABULARYDELEGATE_H
#define VOCABULARYDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>

class VocabularyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    VocabularyDelegate(QObject *parent = 0);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setCurrentIndex(const QModelIndex &index);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
protected:
    void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const;

private slots:
    void commitAndCloseEditor();

private:
    /**
     * This should become a class of its own. It currently guesses the word type noun based on the articles.
     * @param entry
     * @param language
     * @return
     */
//     QPair< QString, QString > guessWordType(const QString & entry, int language) const;
    QModelIndex m_currentIndex;
};

#endif
