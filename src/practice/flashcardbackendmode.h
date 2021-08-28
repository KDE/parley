/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FLASHCARDBACKENDMODE_H
#define FLASHCARDBACKENDMODE_H

#include "abstractbackendmode.h"

namespace Practice
{
class FlashCardBackendMode : public AbstractBackendMode
{
    Q_OBJECT

public:
    FlashCardBackendMode(AbstractFrontend *frontend, QObject *parent);
    bool setTestEntry(TestEntry *current) override;

public Q_SLOTS:
    void checkAnswer() override;
    void hintAction() override;

private:
    bool m_solutionVisible;
    TestEntry *m_current;
    QString m_currentHint;
};

}

#endif // FLASHCARDBACKENDMODE_H
