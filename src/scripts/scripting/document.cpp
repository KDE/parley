/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "document.h"
#include <KEduVocWordtype>

#include "translation.h"

#include <QDebug>

namespace Scripting
{

Document::Document(QObject* parent)
{
    m_doc = new KEduVocDocument(parent);
}

Document::Document(KEduVocDocument * doc)
    : QObject(), m_doc(doc)
{
}

Document::~Document()
{
}

QVariantList Document::allLessons()
{
    Lesson * l = new Lesson(m_doc->lesson());
    return QVariantList() << QVariant::fromValue(static_cast<QObject*>(l)) <<  l->childLessons(true);
}

KEduVocWordType * Document::wordTypeFromString(const QString & name)
{
    QList<KEduVocContainer*> list = Container::flattenContainer(m_doc->wordTypeContainer());
    list.removeFirst();

//         foreach ( KEduVocContainer * child, list )
//             qDebug() << static_cast<KEduVocWordType*>(child)->name();

    foreach(KEduVocContainer * child, list) {
        KEduVocWordType * wt = static_cast<KEduVocWordType*>(child);
        if (name == wt->name())
            return wt;
    }
    return 0;
}

void Document::setWordType(QObject * translation, const QString & wordtype)
{
    Translation * tr = dynamic_cast<Translation*>(translation);
    if (!tr) {
        qDebug() << "Invalid lesson entry";
        return;
    }
    KEduVocWordType * wt = wordTypeFromString(wordtype);
    if (wt)
        tr->setWordType(wt);
    else
        qDebug() << "Invalid given wordtype: " << wordtype;
}

QStringList Document::wordTypes()
{
    QList<KEduVocContainer*> list = Container::flattenContainer(m_doc->wordTypeContainer());
    list.removeFirst();
    QStringList strList;
    foreach(KEduVocContainer * child, list) {
        strList << child->name();
    }
    return strList;
}

QVariantList Document::identifiers()
{
    QVariantList list;
    for (int i = 0; i < m_doc->identifierCount(); i++) {
        QObject * obj = new Identifier(m_doc->identifier(i));
        list << QVariant::fromValue(obj);
    }
    return list;
}

void Document::appendNewIdentifier(const QString& name, const QString& locale)
{
    KEduVocIdentifier ident;
    ident.setName(name);
    ident.setLocale(locale);
    m_doc->appendIdentifier(ident);
}

void Document::appendLesson(QObject * lesson)
{
    Lesson * l = dynamic_cast<Lesson*>(lesson);
    if (l)
        m_doc->lesson()->appendChildContainer(l->kEduVocContainer());
}

QObject * Document::appendNewLesson(const QString & name)
{
    KEduVocLesson * lesson = new KEduVocLesson(name, m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);
    return new Lesson(lesson);
}

QObject * Document::appendNewLesson(const QString & name, Lesson * parent)
{
    KEduVocLesson * lesson = new KEduVocLesson(name, parent->kEduVocContainer());
    parent->kEduVocContainer()->appendChildContainer(lesson);
    return new Lesson(lesson);
}

QObject * Document::findLesson(const QString& name)
{
    Lesson tmpLesson(m_doc->lesson());
    return tmpLesson.findChildLesson(name);
}

}
