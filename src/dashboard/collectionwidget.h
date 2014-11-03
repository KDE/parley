/***************************************************************************
    Copyright 2014 Andreas Xavier
    Copyright 2014 Inge Wallin
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QWidget>

#include "barwidget.h"


// Size constants for the collection widgets
extern int COLLWIDTH;    // Width in pixels of a collection widget
extern int COLLHEIGHT1;  // Height in pixels of a collection widget not yet fully learned
extern int COLLHEIGHT2;  // Height in pixels of a collection widget fully learned


class QLabel;
class QPushButton;
class RemoveButton;
class Collection;

// FIXME: Temporary struct until the real number of due words works.
struct DueWords {
    DueWords();

    int dueWords[8];
    int totalDueWords;
    int percentageCompleted;
};


class CollectionWidget : public QWidget
{
    Q_OBJECT

public:
    CollectionWidget(Collection *collection, DueWords *due,
		     QWidget *parent = 0);
    ~CollectionWidget();

    Collection *collection() const;
    void setCollection(Collection *collection);

Q_SIGNALS:
    void practiceButtonClicked();
    void removeButtonClicked();

private:
    void setupWidget(DueWords *due);
    void fillWidget();

private:
    QLabel       *m_titleLabel;
    QWidget      *m_thumbnail;
    BarWidget    *m_barWidget;
    RemoveButton *m_removeButton;
    QPushButton  *m_practiceButton;

    Collection   *m_collection;
};

