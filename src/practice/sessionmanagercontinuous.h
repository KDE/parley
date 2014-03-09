/***************************************************************************
    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
                    (C) 2014      Inge Wallin <inge@lysator.liu.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICESESSIONMANAGERCONTINUOUS_H
#define PRACTICESESSIONMANAGERCONTINUOUS_H

#include "sessionmanagerbase.h"

// Qt
#include <QVector>

// kdelibs
#include <KRandomSequence>

// kdeedulibs
#include <keduvocexpression.h>

// Parley
#include "testentry.h"
#include "prefs.h"


class KEduVocDocument;
class PracticeDialog;

namespace Practice
{

    class SessionManagerContinuous : public SessionManagerBase
{
public:
    /**
     * Create a collection of entries to be practiced.
     * @param doc
     */
    explicit SessionManagerContinuous(QWidget *parent);

    /**
     * destructor
     */
    virtual ~SessionManagerContinuous();

    /**
     * Prepare for practice using the entries in this document.
     */
    void setDocument(KEduVocDocument *doc);

    /**
     * Initialize the lists of entries that will be used in the
     * training from the full set of available entries.
     */
    virtual void initializeTraining();


    // Not reimplemented.  The default version does what is needed.
    //virtual TestEntry* nextTrainingEntry();


 private:  // data
    // The default data in the base class is enough for us.
};

}

#endif // include guard
