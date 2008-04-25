/***************************************************************************

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kapplication.h>

#include "parley.h"
#include "version.h"

int main(int argc, char* argv[])
{
    static const char description[] = I18N_NOOP("Vocabulary Trainer");
    static const char version[]     = PARLEY_VERSION_STRING;

    KAboutData aboutData("parley", 0,
                         ki18n("Parley"),
                         version,
                         ki18n(description),
                         KAboutData::License_GPL,
                         ki18n("© 1999-2002\tEwald Arnold\n"
                                   "© 2001-2002\tThe KDE team\n"
                                   "© 2004-2007\tPeter Hedlund\n"
                                   "© 2007\tFrederik Gladhorn\n"),
                         ki18n("Helps you train your vocabulary"),
                         "http://edu.kde.org/parley",
                         "submit@bugs.kde.org");

    aboutData.addAuthor(ki18n("Frederik Gladhorn"),
                        ki18n("Current Maintainer"),
                        "frederik.gladhorn@kdemail.net");

    aboutData.addAuthor(ki18n("Peter Hedlund"),
                        ki18n("Countless fixes, former maintainer, port to KDE4"),
                        "peter.hedlund@kdemail.net");

    aboutData.addAuthor(ki18n("Ewald Arnold"), ki18n("Original Author"),
                        "kvoctrain@ewald-arnold.de",
                        "http://www.ewald-arnold.de");

    aboutData.addCredit(ki18n("Waldo Bastian"),
                        ki18n("Help with port to Qt3/KDE3"));

    aboutData.addCredit(ki18n("Eric Bischoff"),
                        ki18n("Converting documentation to docbook format"));

    aboutData.addCredit(ki18n("Kevin Kramer"),
                        ki18n("Tool to create lists with ISO639 codes"));

    aboutData.addCredit(ki18n("Andreas Neuper"),
                        ki18n("Converter script \"langen2kvtml\" \nDownload files at http://www.vokabeln.de/files.htm"));

    aboutData.addCredit(ki18n("Dennis Haney"),
                        ki18n("Patch to implement Leitner learning method"));

    aboutData.addCredit(ki18n("Anne-Marie Mahfouf"),
                        ki18n("Port to KConfig XT"));

    aboutData.addCredit(ki18n("Jeremy Whiting"),
                        ki18n("Rewriting the kvtml library for KDE4"));

    aboutData.addCredit(ki18n("Markus Büchele"),
                        ki18n("Bug reports and testing on the way to KDE4"));

    aboutData.addCredit(ki18n("Johannes Simon"),
                        ki18n("Bug fixes and testing on the way to KDE4"));

    aboutData.addCredit(ki18n("Ramona Knapp"),
                        ki18n("Conceived the name Parley"));

    aboutData.addCredit(ki18n("Lee Olson"),
                        ki18n("Oxygen icons"));

    aboutData.addCredit(ki18n("KDE Team"),
                        ki18n("Many small enhancements"));

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add(I18N_NOOP("+[file]"), ki18n("Document file to open"));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;
    app.setQuitOnLastWindowClosed(false); 
    ParleyApp *parleyApp = 0;
    if (app.isSessionRestored()) {
        int n = 1;
        while (KXmlGuiWindow::canBeRestored(n)) {
            parleyApp = new ParleyApp(KCmdLineArgs::appName());
            parleyApp->restore(n);
            parleyApp->show();
            n++;
        }
    } else {
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        parleyApp = new ParleyApp(KCmdLineArgs::appName());
        if (args && args->count() == 1) {
            parleyApp->loadFileFromPath(args->url(0), true);
            args->clear();
        }
        parleyApp->show();
    }
    // for i18n of the lib strings
    KGlobal::locale()->insertCatalog("libkdeedu");

    int ret = app.exec();
//     delete parleyApp;
    return  ret;
}

