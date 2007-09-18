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


#ifndef KVTQUERYMANAGER_H
#define KVTQUERYMANAGER_H

#include "kvtquery.h"
#include "practicedialog.h"

class KVocTrainApp;
class KEduVocDocument;
class PracticeDialog;
class TestEntry;

class QueryManager :public QObject
{
Q_OBJECT

public:
    QueryManager(KVocTrainApp *app, KEduVocDocument *doc);
    void startPractice();

public slots:
    void slotResult(PracticeDialog::Result res);

private:
    void createDialog();
    bool queryIsEmpty();

    /** exit query mode */
    void stopPractice();

    TestEntryManager    *m_entryManager;
    KEduVocDocument     *m_doc;
    KVocTrainApp        *m_app;

    /// The dialog
    PracticeDialog      *m_testDialog;

    int                  num_queryTimeout;
    int                  m_testType;

    TestEntry* m_entry;
};

#endif
