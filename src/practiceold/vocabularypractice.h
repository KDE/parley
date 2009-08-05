/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef VOCABULARYPRACTICE_H_
#define VOCABULARYPRACTICE_H_

#include "testentrymanager.h"

#include <KMessageBox>
#include <QWidget>

#define MAX_PRACTICE_TIMEOUT 3

class VocabularyPractice :public QObject
{
Q_OBJECT
public:
    enum Result { Correct, SkipKnown, SkipUnknown, Wrong, Timeout, StopPractice };

    VocabularyPractice(KEduVocDocument *doc, QWidget * parent = 0);
    ~VocabularyPractice();

    void startPractice();

private Q_SLOTS:
    void entryDone();

    /**
     * The user has answered with this result, internal update.
     * @param result the result (PracticeDialog)
     */
    void slotResult(VocabularyPractice::Result res);

private:

    TestEntryManager m_testEntryManager;
    QWidget* m_parent;
    KEduVocDocument* m_doc;

    TestEntry* m_currentEntry;
    PracticeDialog* m_practiceDialog;

    /// after x timeouts we pause the whole show
    int m_practiceTimeoutCounter;
};

#endif /* VOCABULARYPRACTICE_H_ */
