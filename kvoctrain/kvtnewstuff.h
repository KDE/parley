//
// C++ Interface: kvtnewstuff
//
// Description:
//
//
// Author: Peter Hedlund <peter@peterandlinda.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWSTUFF_H
#define KVTNEWSTUFF_H

#include <knewstuff/knewstuff.h>

#include "kvoctrain.h"

/**
@author Peter Hedlund
*/
class KVTNewStuff : public QObject, public KNewStuff
{
Q_OBJECT
public:
  KVTNewStuff(QWidget *parent = 0, const char *name = 0);

  bool install(const QString &fileName);
  bool createUploadFile(const QString &fileName);
private:
  kvoctrainApp * m_app;
};

#endif
