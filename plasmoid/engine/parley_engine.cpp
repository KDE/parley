/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "parley_engine.h"

#include <QStringList>

#include <QDebug>
#include <KConfigGroup>

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocexpression.h>
//@todo frameworks KGlobal
#include <KGlobal>


#include "plasma/datacontainer.h"

ParleyEngine::ParleyEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
    KGlobal::locale()->insertCatalog("parley");
    setMinimumPollingInterval(1000);
    qDebug() << "ParleyEngine::ParleyEngine";

    m_random = new KRandomSequence(QDateTime::currentDateTime().toTime_t());
}

ParleyEngine::~ParleyEngine()
{
    delete m_random;
}

void ParleyEngine::openDocument(const QString& file)
{
    qDebug() << "Open vocabulary file: '" << file << "'";
    if (!file.isEmpty()) {
        KEduVocDocument *doc = new KEduVocDocument(this);
        doc->open(QUrl::fromLocalFile(file));

        // check that there is at least one identifier, otherwise this won't work...
        if (doc->identifierCount() > 0) {
            m_docs.insert(file, doc);
            // vocabularyCount = doc->lesson()->entries(KEduVocContainer::Recursive).count();
        } else {
            delete doc;
        }
    }
}

QStringList ParleyEngine::sources() const
{
    return m_docs.keys();
}

bool ParleyEngine::sourceRequestEvent(const QString &source)
{
    if (!m_docs.contains(source)) {
        qDebug() << "open file: " << source;
        openDocument(source);
    }

    if (!m_docs.contains(source)) {
        qDebug() << "Could not open source file: " << source;
        return false;
    }
    return updateSourceEvent(source);
}

bool ParleyEngine::updateSourceEvent(const QString &source)
{
    KEduVocDocument* doc = m_docs.value(source);
    if (!doc) {
        return false;
    }

    int vocabularyCount = doc->lesson()->entries(KEduVocContainer::Recursive).count();
    if (!vocabularyCount) {
        setData(source, QVariant());
        return false;
    }

    ulong current = m_random->getLong(vocabularyCount);
    KEduVocExpression *expression = doc->lesson()->entries(KEduVocContainer::Recursive).value(current);

    QHash<QString, QVariant> data;
    for (int i = 0; i < doc->identifierCount(); i++) {
        data[doc->identifier(i).name()] = expression->translation(i)->text();
    }
    setData(source, data);

    return true;
}

#include "parley_engine.moc"
