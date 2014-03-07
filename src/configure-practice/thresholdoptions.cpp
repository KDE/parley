/***************************************************************************

                      threshold options for Parley

    -----------------------------------------------------------------------

    begin         : Tue Apr 5 2005

    copyright     : (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "thresholdoptions.h"

//#include "practice/sessionmanagerbase.h"
#include "prefs.h"

#include <keduvocdocument.h>
#include <keduvocwordtype.h>

ThresholdOptions::ThresholdOptions(KEduVocDocument* doc, QWidget* parent) : QWidget(parent)
{
    setupUi(this);
    m_doc = doc;
}


void ThresholdOptions::updateWidgets()
{
}


bool ThresholdOptions::isDefault()
{
    return true;
    ///@todo
}


void ThresholdOptions::updateSettings()
{

}

bool ThresholdOptions::hasChanged()
{
    return false;
}


#include "thresholdoptions.moc"
