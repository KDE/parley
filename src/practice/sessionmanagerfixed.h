/*
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICESESSIONMANAGERFIXED_H
#define PRACTICESESSIONMANAGERFIXED_H


#include "sessionmanagerbase.h"

// Parley
#include "testentry.h"
#include "prefs.h"


namespace Practice
{

/**
 * This class handles a session of a fixed size. Special features include:
 *  - at most x (default: 5) new words every session
 *  - entries with higher grades are prioritized.
 */
class SessionManagerFixed : public SessionManagerBase
{
public:
    /**
     * Create a collection of entries to be practiced.
     */
    explicit SessionManagerFixed(QWidget *parent);

    /**
     * destructor
     */
    virtual ~SessionManagerFixed();

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
