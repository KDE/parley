/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef EXAMPLESENTENCEBACKENDMODE_H
#define EXAMPLESENTENCEBACKENDMODE_H

#include "writtenbackendmode.h"

namespace Practice
{

class ExampleSentenceBackendMode : public WrittenBackendMode
{
    Q_OBJECT
public:
    ExampleSentenceBackendMode(Practice::AbstractFrontend* frontend, QObject* parent,
                               Practice::SessionManagerBase* sessionManager, KEduVocDocument* doc);

    bool setTestEntry(TestEntry* current) Q_DECL_OVERRIDE;
};

}

#endif

