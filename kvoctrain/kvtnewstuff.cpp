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

#include <qdir.h>

#include <kprocess.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <knewstuff/entry.h>

#include "kvoctrain.h"
#include "kvtnewstuff.h"
#include "prefs.h"

KVTNewStuff::KVTNewStuff(QWidget *parent, const char * /*name*/) : QObject(), KNewStuff("kdeedu/vocabulary", parent)
{
  m_app = (kvoctrainApp*) parent;
}


bool KVTNewStuff::install(const QString & fileName)
{
  if (m_app->queryExit())
    m_app->loadfileFromPath(fileName, true);
  return true;
}


bool KVTNewStuff::createUploadFile(const QString & /*fileName*/)
{
  return true;
}


QString KVTNewStuff::destinationPath(KNS::Entry * entry)
{
  if (entry)
  {
    KURL url = entry->payload();
    QString fileName = url.fileName();

    QString path = Prefs::installPath(); //default is Vocabularies which will be created in the user's home directory
    QString file;

    if (path.isEmpty())
      file = KNewStuff::downloadDestination(entry); //fall back on a temp file, should never happen
    else
    {
      file = QDir::home().path() + "/" + path + "/";
      KStandardDirs::makeDir(file); //ensure the directory exists
      file += fileName;
    }
    return file;
  }
  else
    return QString::null;
}


QString KVTNewStuff::downloadDestination(KNS::Entry * entry)
{
  QString file = destinationPath(entry);

  if (KStandardDirs::exists(file))
  {
    int result = KMessageBox::warningContinueCancel(parentWidget(),
        i18n("The file '%1' already exists. Do you want to overwrite it?")
        .arg(file),
        QString::null, i18n("Overwrite") );
    if (result == KMessageBox::Cancel)
      return QString::null;
  }
  KMessageBox::information(parentWidget(),
    i18n("<qt>The selected file will now be downloaded and saved as\n<b>'%1'</b>.</qt>")
    .arg(file),
    QString::null,
    "NewStuffDownloadLocation");
  return file;
}


#include "kvtnewstuff.moc"
