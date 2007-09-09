/***************************************************************************

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "startpracticewidget.h"

#include "languagesettings.h"

#include <keduvocdocument.h>

#include <KStandardDirs>
#include <KLocale>
#include <QLabel>
#include <QtDBus>

/**
 *
 * @param parent
 */
StartPracticeWidget::StartPracticeWidget(KEduVocDocument* doc, QWidget * parent)
    : QWidget(parent)
{
    m_doc = doc;
    setupUi(this);

}


void StartPracticeWidget::commitData()
{
//     LanguageSettings settings(locale);
//     settings.setIcon(icon);
//     settings.writeConfig();
}


#include "startpracticewidget.moc"
