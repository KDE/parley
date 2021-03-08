/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COLLECTIONWIDGET_H
#define COLLECTIONWIDGET_H

#include <QWidget>

#include "collection.h"

#include "barwidget.h"


// Size constants for the collection widgets
extern int COLLWIDTH;    // Width in pixels of a collection widget
extern int COLLHEIGHT1;  // Height in pixels of a collection widget not yet fully learned
extern int COLLHEIGHT2;  // Height in pixels of a collection widget fully learned


class QLabel;
class QPushButton;
class RemoveButton;
class Collection;


class CollectionWidget : public QWidget
{
    Q_OBJECT

public:
    CollectionWidget(Collection *collection, WordCount *dueWords,
		     QWidget *parent = 0);
    ~CollectionWidget();

    Collection *collection() const;
    void setCollection(Collection *collection);

    void updateDue();

Q_SIGNALS:
    void practiceButtonClicked();
    void removeButtonClicked();

private:
    void setupWidget(WordCount *dueWords);
    void fillWidget();

private:
    QLabel       *m_titleLabel;
    QLabel       *m_thumbnail;
    BarWidget    *m_barWidget;
    RemoveButton *m_removeButton;
    QPushButton  *m_practiceButton;

    Collection   *m_collection;
};

#endif
