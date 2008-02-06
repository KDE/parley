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
    setMinimumUpdateInterval(1000);

    kDebug() << "ParleyEngine::ParleyEngine";
    m_doc = new KEduVocDocument(this);

    KConfig parleyConfig("parleyrc");
    kDebug() << parleyConfig.groupList();
    KConfigGroup recentFilesGroup( &parleyConfig, "Recent Files" );
    // take the last file, but there are File1..n and Name1..n entries..
    QString file = recentFilesGroup.readEntry( recentFilesGroup.keyList().value(recentFilesGroup.keyList().count()/2-1), QString() );
    openDocument(file);
    m_random = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

}

ParleyEngine::~ParleyEngine()
{
    delete m_random;
}

void ParleyEngine::openDocument(const QString& file)
{
    kDebug() << "Open vocabulary file: '" << file << "'";
    if (!file.isEmpty()) {
        m_doc->open(file);
//         m_vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
    }
}

QStringList ParleyEngine::sources() const
{
   QStringList list;
   list << QLatin1String("Random");
   return list;
}

bool ParleyEngine::sourceRequested(const QString &source)
{
//     kDebug() << "ParleyEngine::sourceRequested " << source;
    return updateSource(source);
}

bool ParleyEngine::updateSource(const QString &source)
{
//     kDebug() << "ParleyEngine::updateSource()" << source;

    if (source == QLatin1String("Random")) {
        if (m_doc) {
            int vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
            int expNum = m_random->getLong(vocabularyCount);
            KEduVocExpression *expression = m_doc->lesson()->entries(KEduVocContainer::Recursive).value(expNum);    
            if (expression) {
                QString text;
                foreach (int index, expression->translationIndices()) {
                    text += "\n" + expression->translation(index)->text();
                }
                setData(QLatin1String("Random"), text);
                return true;
            }
        }
        return false;
    }
    // other sources
    return false;
}

#include "parley_engine.moc"
