/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICESESSIONMANAGERCONTINUOUS_H
#define PRACTICESESSIONMANAGERCONTINUOUS_H

#include "sessionmanagerbase.h"

// Qt
#include <QVector>

// kdeedulibs
#include <KEduVocExpression>

// Parley
#include "testentry.h"
#include "prefs.h"


class KEduVocDocument;

namespace Practice
{

    class SessionManagerContinuous : public SessionManagerBase
{
public:
    /**
     * Create a collection of entries to be practiced.
     */
    explicit SessionManagerContinuous(QWidget *parent);

    /**
     * destructor
     */
    virtual ~SessionManagerContinuous();

    /**
     * Prepare for practice using the entries in this document.
     */
    void setDocument(KEduVocDocument *doc) Q_DECL_OVERRIDE;

    /**
     * Initialize the lists of entries that will be used in the
     * training from the full set of available entries.
     */
    void initializeTraining() Q_DECL_OVERRIDE;


    // Not reimplemented.  The default version does what is needed.
    //virtual TestEntry* nextTrainingEntry();


 private:  // data
    // The default data in the base class is enough for us.
};

}

#endif // include guard
