/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef PARLEYENGINE_H
#define PARLEYENGINE_H

#include <KCoreAddons/KRandomSequence>

#include "plasma/dataengine.h"

class KEduVocDocument;

class ParleyEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    ParleyEngine(QObject* parent, const QVariantList& args);
    ~ParleyEngine();
    QStringList sources() const;

protected:
    bool sourceRequestEvent(const QString& name);
    bool updateSourceEvent(const QString& source);

private:
    void openDocument(const QString& file);

    QHash<QString, KEduVocDocument*> m_docs;
    KRandomSequence *m_random;
};

K_EXPORT_PLASMA_DATAENGINE(parley, ParleyEngine)

#endif
