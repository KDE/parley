/***************************************************************************

    C++ Interface: kvtquerymanager

    -----------------------------------------------------------------------

    begin         : So Mai 27 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PRACTICEMANAGER_H
#define PRACTICEMANAGER_H

#include "testentrymanager.h"
#include "practicedialog.h"

class ParleyApp;
class KEduVocDocument;
class PracticeDialog;
class TestEntry;

class PracticeManager :public QObject
{
Q_OBJECT

public:
    PracticeManager(ParleyApp *app, KEduVocDocument *doc);
    ~PracticeManager();

    void startPractice();

public slots:
    void slotResult(PracticeDialog::Result res);
    void nextEntry();

private:
    void createDialog();

    /** exit query mode */
    void stopPractice();

    TestEntryManager    *m_entryManager;
    KEduVocDocument     *m_doc;
    ParleyApp           *m_app;

    /// The dialog
    PracticeDialog      *m_testDialog;

    int                  num_queryTimeout;
    int                  m_testType;

    QString              m_lastTestType;

    TestEntry* m_entry;
};

#endif
