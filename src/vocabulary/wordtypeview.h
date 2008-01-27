/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WORDTYPEVIEW_H
#define WORDTYPEVIEW_H

#include "wordtypeview.h"
#include "containerview.h"

class ParleyApp;

class WordTypeView : public ContainerView
{
    Q_OBJECT

public:
    WordTypeView(ParleyApp *parent);

public slots:
    /** Append a word type to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateNewWordType();

    /** Remove a word type. */
    void slotDeleteWordType();

};

#endif
