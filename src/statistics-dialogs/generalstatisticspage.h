/***************************************************************************

                      general statistics dialog page

    -----------------------------------------------------------------------

    begin         : Thu Sep 21 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>Õ*

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef GENERALSTATISTICSPAGE_H
#define GENERALSTATISTICSPAGE_H

#include "ui_generalstatisticspage.h"

class KEduVocDocument;

/**
 * Display general document information like author
 * and number of contained vocabulary.
 */
class GeneralStatisticsPage : public QWidget, public Ui::GeneralStatisticsPage
{
    Q_OBJECT
public:
    GeneralStatisticsPage(KEduVocDocument *doc, QWidget* parent);
};

#endif
