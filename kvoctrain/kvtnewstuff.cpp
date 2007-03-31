/***************************************************************************
 *   Copyright (C) 2005, 2007 by Peter Hedlund                             *
 *   <peter.hedlund@kdemail.net>                                           *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QDir>

#include <k3process.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <knewstuff/entry.h>

#include "kvoctrain.h"
#include "kvtnewstuff.h"
#include "prefs.h"

KVTNewStuff::KVTNewStuff(QWidget *parent) : QObject(), KNewStuff("kdeedu/vocabulary", parent)
{
    m_app = (KVocTrainApp*) parent;
}


bool KVTNewStuff::install(const QString & fileName)
{
    if (m_app->queryExit())
        m_app->loadFileFromPath(fileName, true);
    return true;
}


bool KVTNewStuff::createUploadFile(const QString & fileName)
{
    Q_UNUSED(fileName);
    return true;
}


QString KVTNewStuff::destinationPath(KNS::Entry * entry)
{
    if (entry) {
        KUrl url = entry->payload();
        QString fileName = url.fileName();

        QString path = Prefs::installPath(); //default is $HOME/Vocabularies
        QString file;

        if (path.isEmpty())
            file = KNewStuff::downloadDestination(entry); //fall back on a temp file, should never happen
        else {
            KStandardDirs::makeDir(path); //ensure the directory exists
            file = KStandardDirs::realFilePath(path + '/' + fileName);
        }
        return file;
    } else
        return QString();
}


QString KVTNewStuff::downloadDestination(KNS::Entry * entry)
{
    QString file = destinationPath(entry);

    if (KStandardDirs::exists(file)) {
        int result = KMessageBox::warningContinueCancel(parentWidget(),
                     i18n("The file '%1' already exists. Do you want to overwrite it?",
                          file),
                     QString(), KStandardGuiItem::overwrite());
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
