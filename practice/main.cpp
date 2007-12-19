/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

    KAboutData aboutData("parley", 0,
                         ki18n("Parley"),
                         version,
                         ki18n(description),
                         KAboutData::License_GPL,
                         ki18n("© 2007\tFrederik Gladhorn\n"),
                         ki18n("Parley is sexy."),
                         "http://edu.kde.org/parley",
                         "submit@bugs.kde.org");

    aboutData.addAuthor(ki18n("Frederik Gladhorn"),
                        ki18n("Current Maintainer"),
                        "frederik.gladhorn@kdemail.net");

    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication app;
    KGlobal::locale()->insertCatalog("libkdeedu");

    ParleyPracticeMainWindow* window = new ParleyPracticeMainWindow();
    window->show();

    return app.exec();
}
