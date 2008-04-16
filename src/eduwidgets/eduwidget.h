///\file eduwidgets.h
// C++ Interface: eduwidget
//
// Description: The base class of all eduwidgets (widgets for the parley practice dialogs)
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef EDUWIDGET_H
#define EDUWIDGET_H

// I feel like I'll need a base class at some point, but I don't have much to put in it right now.

/**
* @class EduWidget
* @author David Capel <wot.narg@gmail.com>
* @brief This is the base class for all EduWidgets; it should never be initialized directly.
*/
class EduWidget
{
    Q_OBJECT

    public:        
        virtual EduWidget(QObject* parent = 0) = 0;
        virtual ~EduWidget();

};

#endif
