/***************************************************************************
 *   Copyright (C) 2005 by Peter Hedlund                              *
 *   <peter.hedlund@kdemail.net>                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include <QDir>

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
  m_app = (KVocTrainApp*) parent;
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
    KUrl url = entry->payload();
    QString fileName = url.fileName();

    QString path = Prefs::installPath(); //default is Vocabularies which will be created in the user's home directory
    QString file;

    if (path.isEmpty())
      file = KNewStuff::downloadDestination(entry); //fall back on a temp file, should never happen
    else
    {
      file = QDir::home().path() + '/' + path + '/';
      KStandardDirs::makeDir(file); //ensure the directory exists
      file += fileName;
    }
    return file;
  }
  else
    return QString();
}


QString KVTNewStuff::downloadDestination(KNS::Entry * entry)
{
  QString file = destinationPath(entry);

  if (KStandardDirs::exists(file))
  {
    int result = KMessageBox::warningContinueCancel(parentWidget(),
        i18n("The file '%1' already exists. Do you want to overwrite it?",
         file),
        QString(), KStdGuiItem::overwrite() );
    if (result == KMessageBox::Cancel)
      return QString();
  }
  KMessageBox::information(parentWidget(),
    i18n("<qt>The selected file will now be downloaded and saved as\n<b>'%1'</b>.</qt>",
     file),
    QString(),
    "NewStuffDownloadLocation");
  return file;
}


#include "kvtnewstuff.moc"
