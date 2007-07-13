/***************************************************************************

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "kvoctrain.h"
#include "version.h"

int main(int argc, char* argv[])
{

    static const char description[] = I18N_NOOP("Vocabulary Trainer");
    static const char version[]     = KVOCTRAIN_VERSION_STRING;

    KAboutData aboutData("kvoctrain", 0,
                         ki18n("KVocTrain"),
                         version,
                         ki18n(description),
                         KAboutData::License_GPL,
                         ki18n("(c) 1999-2002\tEwald Arnold\n"
                                   "(c) 2001-2002\tThe KDE team\n"
                                   "(c) 2004-2007\tPeter Hedlund\n"),
                         ki18n("Helps you train your vocabulary"),
                         "http://edu.kde.org/kvoctrain",
                         "submit@bugs.kde.org");

    aboutData.addAuthor(ki18n("Peter Hedlund"),
                        ki18n("Current Maintainer"), "peter.hedlund@kdemail.net");

    aboutData.addAuthor(ki18n("Frederik Gladhorn"),
                        ki18n("Improvements on the way to KDE4"), "frederik.gladhorn@gmx.de");

    aboutData.addAuthor(ki18n("Ewald Arnold"), ki18n("Original Author"), "kvoctrain@ewald-arnold.de",
                        "http://www.ewald-arnold.de");

    aboutData.addCredit(ki18n("Waldo Bastian"),
                        ki18n("Help with port to Qt3/KDE3"));

    aboutData.addCredit(ki18n("Andrea Marconi"),
                        ki18n("Initial Italian localization"));

    aboutData.addCredit(ki18n("Hans Kottmann"),
                        ki18n("Initial French localization"));

    aboutData.addCredit(ki18n("Grzegorz Ilczuk"),
                        ki18n("Initial Polish localization"));

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

    aboutData.addCredit(ki18n("KDE Team"),
                        ki18n("Many small enhancements"));

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add(I18N_NOOP("+[file]"), ki18n("Document file to open"));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    KVocTrainApp *kva = 0;
    if (app.isSessionRestored()) {
        int n = 1;
        while (KXmlGuiWindow::canBeRestored(n)) {
            kva = new KVocTrainApp;
            kva->restore(n);
            kva->show();
            n++;
        }
    } else {
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        kva = new KVocTrainApp;
        if (args && args->count() == 1) {
            kva->loadFileFromPath(args->url(0), true);
            args->clear();
        }
        kva->show();
    }
    return app.exec();
}

