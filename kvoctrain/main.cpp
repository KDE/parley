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
#include <iostream.h>

# include <kcmdlineargs.h>
# include <kaboutdata.h>

#ifndef i18n_noop
# define i18n_noop(x) (x)
#endif

int main(int argc, char* argv[]) { 

static KCmdLineOptions options[] =
{
  { i18n_noop("+[file]"),   i18n_noop("Document file to open."), 0 },
  { 0, 0, 0}
};

static const char *description = 
	i18n_noop("KDE Vocabulary trainer");

static const char *version = VERSION;

  KAboutData aboutData("kvoctrain",
                       "kvoctrain",
                       version,
                       description,
                       KAboutData::License_GPL,
                       "(c) 1999-2000, Ewald Arnold",
                       i18n_noop("Helps you train your vocabulary"),
		      "http://kvoctrain.sourceforge.net/",
                      "kvoctrain@ewald-arnold.de");
  aboutData.addAuthor("Ewald Arnold", i18n_noop("Original author and naintainer"), "kvoctrain@ewald-arnold.de",
		      "http://www.ewald-arnold.de" );
  aboutData.addCredit("Waldo Bastian", i18n_noop("Porting to Qt3/KDE3"),  "");
  aboutData.addCredit("Andrea Marconi", i18n_noop("Initial localisator for italian"),  "");
  aboutData.addCredit("Hans Kottmann", i18n_noop("Initial localisator for french"),   "");
  aboutData.addCredit("Grzegorz Ilczuk", i18n_noop("Initial localisator for polsih"), "");
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
 
