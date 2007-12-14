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

#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <QTableView>

//#include <vocabularysortfiltermodel.h>
//#include <vocabularydelegate.h>

#include <QMap>

class VocabularyModel;
class KEduVocExpression;

class KActionMenu;
class KAction;


class VocabularyView : public QTableView
{
    Q_OBJECT
public:
    VocabularyView(KActionMenu* vocabularyColumnsActionMenu, QWidget *parent = 0);

    void setModel(VocabularyModel * model);

//     void adjustContent();

public slots:
    void reset();
    void appendEntry();

signals:
    void translationChanged(KEduVocExpression*, int);

private slots:
    void slotCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

    void columnCountChanged ( int oldCount, int newCount );

    void slotToggleColumn(bool show);

// private slots:
//     void verticalHeaderResized(int, int, int);
//     void horizontalHeaderResized(int, int, int);
//     void slotCurrentColumnChanged(const QModelIndex & current, const QModelIndex & previous);
//     /** Show the lesson column of the table.
//      * @param show if @c true the lesson column is shown
//      */
//     void slotShowLessonColumn(bool show);
//     void slotShowActiveColumn(bool show);
// 
// protected:
//     /** resizes table when frame is resized */
//     void resizeEvent(QResizeEvent *);
//     void showEvent(QShowEvent *);
//     void keyPressEvent(QKeyEvent*);
// 
// private:
//     void newPage(QPainter &, int, int, int);
//     void endOfPage(QPainter &, int, int);
// 
//     KVTTableDelegate * m_delegate;
private:
    QMap <KAction*, int> m_columnActionMap;
    KActionMenu * m_vocabularyColumnsActionMenu;

    VocabularyModel* m_model;

};

#endif
