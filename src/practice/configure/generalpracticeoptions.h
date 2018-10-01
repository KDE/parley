/***************************************************************************

                      query options for Parley

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                          (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                          (C) 2014 Inge Wallin <inge@lysator.liu.se>

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

#ifndef GENERALPRACTICEOPTIONS_H
#define GENERALPRACTICEOPTIONS_H

#include "ui_generalpracticeoptions.h"

class GeneralPracticeOptions : public QWidget, public Ui::GeneralPracticeOptions
{
    Q_OBJECT
public:
    explicit GeneralPracticeOptions(QWidget* parent = 0);
};

#endif
