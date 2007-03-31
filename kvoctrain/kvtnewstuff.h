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


#ifndef KVTNEWSTUFF_H
#define KVTNEWSTUFF_H

#include <knewstuff/knewstuff.h>

class KVocTrainApp;

/**
@author Peter Hedlund
*/
class KVTNewStuff : public QObject, public KNewStuff
{
    Q_OBJECT
public:
    KVTNewStuff(QWidget *parent = 0);

    /**
      Installs a downloaded file according to the application's configuration.

      @param fileName filename of the donwloaded file
      @return @c true in case of installation success, @c false otherwise
    */
    bool install(const QString &fileName);

    /**
      Creates a file suitable for upload.
      Note that this method always fails, since using KNewStuffGeneric
      means that the provided file must already be in a usable format.

      @param fileName the name of the file to upload after its creation
      @return @c true in case of creation success, @c false otherwise
    */
    bool createUploadFile(const QString &fileName);

    /**
      Queries the preferred destination file for a download.

      @param entry a Hotstuff data entry
      @return destination filename, or 0 to return directory only
    */
    QString downloadDestination(KNS::Entry *entry);

private:
    QString destinationPath(KNS::Entry *entry);

    KVocTrainApp * m_app;
};

#endif
