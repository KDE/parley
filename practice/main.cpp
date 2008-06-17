/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include "parleypracticemainwindow.h"

#include "../src/version.h"

int main (int argc, char *argv[])
{
    static const char description[] = I18N_NOOP("Vocabulary Trainer");
    static const char version[]     = PARLEY_VERSION_STRING;

    KAboutData aboutData("Parley", 0,
                         ki18n("Parley"),
                         version,
                         ki18n(description),
                         KAboutData::License_GPL,
                         ki18n("© 2008\tDavid Capel\n"),
                         ki18n("ParleyPractice is practiced."),
                         "http://edu.kde.org/parley",
                         "submit@bugs.kde.org");

    aboutData.addAuthor(ki18n("David Capel"),
                        ki18n("Current Maintainer"),
                        "wot.narg@gmail.com");

    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication app;
    KGlobal::locale()->insertCatalog("libkdeedu");

    ParleyPracticeMainWindow* window = new ParleyPracticeMainWindow();
    window->show();

    return app.exec();
}
