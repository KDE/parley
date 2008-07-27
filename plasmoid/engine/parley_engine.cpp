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

#include <KDebug>
#include <KConfigGroup>

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocexpression.h>


#include "plasma/datacontainer.h"

ParleyEngine::ParleyEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent)
{
    Q_UNUSED(args)
    setMinimumPollingInterval(1000);
    kDebug() << "ParleyEngine::ParleyEngine";
    m_doc = new KEduVocDocument(this);
    m_current = 0;

    m_random = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );
}

ParleyEngine::~ParleyEngine()
{
    delete m_random;
    delete m_doc;
}

void ParleyEngine::openDocument(const QString& file)
{
    kDebug() << "Open vocabulary file: '" << file << "'";
    if (!file.isEmpty()) {
        m_doc->open(KUrl(file));
//         m_vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
    }
}

QStringList ParleyEngine::sources() const
{
    QStringList list;
    if (m_doc) {
        for (int i = 0; i < m_doc->identifierCount(); i++) {
            list.append(m_doc->identifier(i).name());
        }
    }
    return list;
}

bool ParleyEngine::sourceRequestEvent(const QString &source)
{
    if (source != m_file) {
        kDebug() << "open file: " << source;
        m_file = source;
        openDocument(source);
    }
    if (m_file.isEmpty()) {
        kDebug() << "could not open source file";
        return false;
    }
    return updateSourceEvent(source);
}

bool ParleyEngine::updateSourceEvent(const QString &source)
{
    int vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
    if (!vocabularyCount) {
        setData(source, "no language",i18n("No document set."));
        return true;  // rather false?
    }

    m_current = m_random->getLong(vocabularyCount);
    KEduVocExpression *expression = m_doc->lesson()->entries(KEduVocContainer::Recursive).value(m_current);

    QHash<QString,QVariant> data;
    for (int i = 0; i < m_doc->identifierCount(); i++) {
        data[m_doc->identifier(i).name()] = expression->translation(i)->text();
    }
    setData(source, data);

    return true;
}

#include "parley_engine.moc"
