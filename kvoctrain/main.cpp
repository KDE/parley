/***************************************************************************

    $Id$

                        entry gate to kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.16  2001/12/29 10:40:03  arnold
    merged fixes from POST-branch

    Revision 1.15  2001/12/22 09:06:54  arnold
    fixed version string handling

    Revision 1.14  2001/12/18 21:17:05  howells
    * Get rid of old "make install" message
    * Use new bug dialog

    Revision 1.13  2001/12/17 20:09:31  howells
    Descriptions for all apps

    Revision 1.12  2001/12/16 16:11:34  howells
    Don't break

    Revision 1.11  2001/12/16 08:37:22  howells
    Use I18N_NOOP()

    Revision 1.10  2001/12/08 15:22:57  arnold
    added contributors

    Revision 1.9  2001/12/05 09:07:21  howells
    Fix some spelling/gramattical errors, and make the program fit better with the style guide

    Revision 1.8  2001/11/24 11:43:58  arnold
    updated credits

    Revision 1.7  2001/11/18 12:28:25  arnold
    provided menu entry for example files

    Revision 1.6  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/10/28 10:15:46  arnold
    quick 'n dirty fixes for new query dialogs

    Revision 1.4  2001/10/21 15:28:43  arnold
    removed all the 'charset' stuff

    Revision 1.3  2001/10/20 12:09:40  arnold
    removed aboutdialog files

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:36:34  arnold
    import of version 0.7.0pre8 to kde-edu

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "kvoctrain.h"
#include "kvoctraincore.h"
#include <iostream.h>

# include <kcmdlineargs.h>
# include <kaboutdata.h>
# include <klocale.h>

int main(int argc, char* argv[]) {

static KCmdLineOptions options[] =
{
  { I18N_NOOP("+[file]"),   I18N_NOOP("Document file to open."), 0 },
  { 0, 0, 0}
};

static const char *description =
	I18N_NOOP("Vocabulary Trainer");

static const char *version = KVOCTRAIN_VERSION_STRING;

  KAboutData aboutData("kvoctrain",
                       "KVocTrain",
                       version,
                       description,
                       KAboutData::License_GPL,
                       "(c) 1999-2002\tEwald Arnold\n"
                       "(c) 2001-2002\tThe KDE team\n",
                       I18N_NOOP("Helps you train your vocabulary"),
		      "http://kvoctrain.sourceforge.net/",
                      "submit@bugs.kde.org");

  aboutData.addAuthor("Ewald Arnold", I18N_NOOP("Author and maintainer"), "kvoctrain@ewald-arnold.de",
		      "http://www.ewald-arnold.de" );

  aboutData.addCredit("Waldo Bastian",
                      I18N_NOOP("Help with port to Qt3/KDE3"));

  aboutData.addCredit("Andrea Marconi",
                      I18N_NOOP("Initial Italian localization"));

  aboutData.addCredit("Hans Kottmann",
                      I18N_NOOP("Initial French localization"));

  aboutData.addCredit("Grzegorz Ilczuk",
                      I18N_NOOP("Initial Polish localization"));

  aboutData.addCredit("Eric Bischoff",
                      I18N_NOOP("Converting documentation to docbook format"));

  aboutData.addCredit("Kevin Kramer",
                      I18N_NOOP("Tool to create lists with ISO639 codes"));

  aboutData.addCredit("Andreas Neuper",
                      I18N_NOOP("Converter script \"langen2kvtml\" (download files at http://www.vokabeln.de/files.htm)"));

  aboutData.addCredit("The whole KDE team",
                      I18N_NOOP("Many small enhancements"));

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );
  KApplication app;

/*
    bool query = false;
    for(int i = 0; i < argc; i++){

        if(strcmp(argv[i],"--query") == 0){
	    query = true;
	}
	
    }
*/

  kvoctrainApp *kva = 0;
  if (app.isRestored()) {
    int n = 1;
    while (KMainWindow::canBeRestored(n)){
      kva = new kvoctrainApp;
      kva->restore(n);

      kva->hide();
      if (kva->isQueryMode()) {
        kva->slotRestartQuery();   // query was running at last exit
// FIXME: remove querymode stuff?
        if (!kva->isQueryMode()) { // query mode could not re-start
          kva->show();
        }
      }
      else {
        kva->show();
      }
      n++;
    }
  }
  else {

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if ( args && args->count() == 1 )
       kva = new kvoctrainApp (args->arg(0));
    else
      kva = new kvoctrainApp;

    kva->show();
  }
  return app.exec();
}  
 
