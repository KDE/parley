/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2006 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

/// @file src/main.cpp

#include "parleymainwindow.h"
#include <config-parley.h>

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <Kdelibs4ConfigMigrator>

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("parley");

    /* for migration*/
    QStringList configFiles;
    configFiles << QStringLiteral("parleyrc");
    Kdelibs4ConfigMigrator migrator(QStringLiteral("parley"));
    migrator.setConfigFiles(configFiles);
    migrator.migrate();

    KAboutData aboutData(QStringLiteral("parley"),  ki18n("Parley").toString(),  PARLEY_VERSION_STRING );
    aboutData.setShortDescription( ki18n("Vocabulary Trainer").toString() );
    aboutData.setLicense( KAboutLicense::GPL );
    aboutData.setCopyrightStatement( ki18n("© 1999-2002\tEwald Arnold\n"
                                           "© 2001-2002\tThe KDE team\n"
                                           "© 2004-2007\tPeter Hedlund\n"
                                           "© 2007-2010\tFrederik Gladhorn\n").toString() );
    aboutData.setOtherText( ki18n("Helps you train your vocabulary").toString() );
    aboutData.setHomepage( QStringLiteral("https://edu.kde.org/parley") );
    aboutData.setBugAddress( "submit@bugs.kde.org");

    aboutData.addAuthor(ki18n("Inge Wallin").toString(),
                        ki18n("Developer and Co-maintainer").toString(),
                        QStringLiteral("inge@lysator.liu.se"));

    aboutData.addAuthor(ki18n("Amarvir Singh").toString(),
                        ki18n("Developer and Co-maintainer").toString(),
                        QStringLiteral("amarvir.ammu.93@gmail.com"));

    aboutData.addAuthor(ki18n("Frederik Gladhorn").toString(),
                        ki18n("Developer, former maintainer").toString(),
                        QStringLiteral("gladhorn@kde.org"));

    aboutData.addAuthor(ki18n("Daniel Laidig").toString(),
                        ki18n("Developer").toString(),
                        QStringLiteral("d.laidig@gmx.de"));

    aboutData.addAuthor(ki18n("David Capel").toString(),
                        ki18n("Practice Dialogs").toString(),
                        QStringLiteral("wot.narg@gmail.com"));

    aboutData.addAuthor(ki18n("Avgoustinos Kadis").toString(),
                        ki18n("Scripting").toString(),
                        QStringLiteral("avgoustinos.kadis@kdemail.net"));

    aboutData.addAuthor(ki18n("Peter Hedlund").toString(),
                        ki18n("Countless fixes, former maintainer, port to KDE4").toString(),
                        QStringLiteral("peter.hedlund@kdemail.net"));

    aboutData.addAuthor(ki18n("Ewald Arnold").toString(), ki18n("Original Author").toString(),
                        QStringLiteral("kvoctrain@ewald-arnold.de"),
                        QStringLiteral("http://www.ewald-arnold.de"));

    aboutData.addCredit(ki18n("Lee Olson").toString(),
                        ki18n("Artwork and Oxygen Icons").toString());

    aboutData.addCredit(ki18n("Anne-Marie Mahfouf").toString(),
                        ki18n("Port to KConfig XT").toString());

    aboutData.addCredit(ki18n("Jeremy Whiting").toString(),
                        ki18n("Rewriting the kvtml library for KDE4").toString());

    aboutData.addCredit(ki18n("Markus Büchele").toString(),
                        ki18n("Bug reports and testing on the way to KDE4").toString());

    aboutData.addCredit(ki18n("Ramona Knapp").toString(),
                        ki18n("Conceived the name Parley").toString());

    KAboutData::setApplicationData(aboutData);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication::setApplicationName(QStringLiteral("parley"));
    QApplication::setApplicationVersion(PARLEY_VERSION_STRING);
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QApplication::setApplicationDisplayName(i18n("Parley"));

    KCrash::initialize();

    QCommandLineParser parser;
    parser.addPositionalArgument(ki18n( "[file]" ).toString(), ki18n("Document file to open").toString() );

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    if (app.isSessionRestored()) {
        int n = 1;
        while (KMainWindow::canBeRestored(n)) {
            (new ParleyMainWindow())->restore(n);
            n++;
        }
        return app.exec();
    } else {
        KMainWindow *parleyApp;

        const QStringList args = parser.positionalArguments();

        if (!args.isEmpty()) {
            parleyApp = new ParleyMainWindow(QUrl::fromLocalFile(args.first() ));
        } else {
            parleyApp = new ParleyMainWindow();
        }

        parleyApp->show();
        return app.exec();
    }
}
