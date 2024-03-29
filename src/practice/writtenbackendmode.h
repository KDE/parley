/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef WRITTENBACKENDMODE_H
#define WRITTENBACKENDMODE_H

#include "abstractbackendmode.h"
#include "practice/sessionmanagerbase.h"
#include "practice/writtenpracticevalidator.h"
#include <QScopedPointer>

namespace Practice
{
class WrittenBackendMode : public AbstractBackendMode
{
    Q_OBJECT

public:
    WrittenBackendMode(Practice::AbstractFrontend *frontend, QObject *parent, Practice::SessionManagerBase *sessionManager, KEduVocDocument *doc);

    bool setTestEntry(TestEntry *current) override;

    void checkAnswer() override;

public Q_SLOTS:
    void hintAction() override;

private:
    QString getFeedbackString(TestEntry::ErrorTypes error);

    QString m_lastAnswer;
    QString m_currentHint;
    bool m_firstAttempt{true};
    SessionManagerBase *m_sessionManager{nullptr};
    QScopedPointer<WrittenPracticeValidator> m_validator;
    KEduVocDocument *m_doc{nullptr};
};

}

#endif // WRITTENBACKENDMODE_H
