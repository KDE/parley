//
// C++ Implementation: kvtnewstuff
//
// Description:
//
//
// Author: Peter Hedlund <peter@peterandlinda.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <kapplication.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <qdir.h>

#include "kvtnewstuff.h"

KVTNewStuff::KVTNewStuff(QWidget *parent, const char *name) : QObject(), KNewStuff("kdeedu/vocabularies", parent)
{
  m_app = (kvoctrainApp*) parent;
}


bool KVTNewStuff::install(const QString & fileName)
{
  kdDebug() << "KVTNewStuff::install(): " << fileName << endl;

  m_app->loadfileFromPath(fileName, true);
  return true;

}


bool KVTNewStuff::createUploadFile(const QString & fileName)
{
  return true;
}


#include "kvtnewstuff.moc"
