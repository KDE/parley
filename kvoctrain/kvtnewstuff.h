//
// C++ Interface: kvtnewstuff
//
// Description:
//
//
// Author: Peter Hedlund <peter.hedlund@kdemail.net>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWSTUFF_H
#define KVTNEWSTUFF_H

#include <knewstuff/knewstuff.h>

class kvoctrainApp;

/**
@author Peter Hedlund
*/
class KVTNewStuff : public QObject, public KNewStuff
{
Q_OBJECT
public:
  KVTNewStuff(QWidget *parent = 0, const char *name = 0);

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

  kvoctrainApp * m_app;
};

#endif
