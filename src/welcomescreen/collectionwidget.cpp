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


// Own
#include "collectionwidget.h"

//#include "welcomescreen.h"
//#include "buttondelegate.h"
//#include "parleymainwindow.h"
//#include "parleydocument.h"
//#include "practice/themedbackgroundrenderer.h"
//#include "practice/imagewidget.h"
//#include "statistics/statisticsmainwindow.h"
//#include "statistics/statisticsmodel.h"
//#include <KMimeType>

// Qt
//#include <QSignalMapper>
//#include <QStandardItemModel>
//#include <QTimer>
//#include <QTime>
#include <QColor>
#include <QWidget>

//#include <QtGui>
//#include <Qt>


// Size constants for the collection widgets
int COLLWIDTH   = 140; // Width in pixels of a collection widget
int COLLHEIGHT1 = 250; // Height in pixels of a collection widget not yet fully learned
int COLLHEIGHT2 = 100; // Height in pixels of a collection widget fully learned


CollectionWidget::CollectionWidget()
    : QWidget()
{
}

CollectionWidget::~CollectionWidget()
{
}

