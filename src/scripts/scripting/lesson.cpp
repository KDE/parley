/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "lesson.h"
#include "expression.h"

#include <KEduVocExpression>

#include <QDebug>

namespace Scripting
{
Lesson::Lesson(KEduVocLesson * lesson)
    : Container(lesson), m_lesson(lesson)
{
}

Lesson::Lesson(KEduVocContainer * container)
{
    m_lesson = static_cast<KEduVocLesson*>(container);
    m_container = m_lesson;
}

Lesson::Lesson(const QString& name)
{
    m_lesson = new KEduVocLesson(name);
    m_container = m_lesson;
}


Lesson::~Lesson()
{
}

QList<KEduVocLesson*>  flattenLessons(KEduVocLesson * rootLesson)
{
    QList<KEduVocLesson*> lessonsList;
    foreach(KEduVocContainer * child, rootLesson->childContainers()) {
        lessonsList << static_cast<KEduVocLesson*>(child);
        lessonsList += flattenLessons(static_cast<KEduVocLesson*>(child));
    }
    return lessonsList;
}


QVariantList Lesson::childLessons(bool recursive)
{
    if (recursive)
        return toVariantList<KEduVocLesson, Lesson> (flattenLessons(m_lesson));
    return toVariantList<KEduVocContainer, Lesson> (m_lesson->childContainers());
}

QVariantList Lesson::entries(bool recursive) const
{
    return toVariantList<KEduVocExpression, Expression> (m_lesson->entries(boolToEnum(recursive)));
}

void Lesson::setEntries(QVariantList entries)
{
    clearEntries();

    foreach(const QVariant & ventry, entries) {
        QObject * obj = qvariant_cast<QObject*> (ventry);
        Expression * entry = dynamic_cast<Expression*>(obj);
        if (entry)
            m_lesson->appendEntry(entry->kEduVocExpression());
//                 qDebug() << entry->translationTexts();
    }
}

QObject * Lesson::entry(int row, bool recursive)
{
    return new Expression(m_lesson->entry(row, boolToEnum(recursive)));
}

int Lesson::entryCount(bool recursive)
{
    return m_lesson->entryCount(boolToEnum(recursive));
}

void Lesson::appendEntry(Expression * entry)
{
    m_lesson->appendEntry(entry->kEduVocExpression());
}

void Lesson::insertEntry(int index, Expression * entry)
{
    m_lesson->insertEntry(index, entry->kEduVocExpression());
}

void Lesson::removeEntry(QObject * entry)
{
    /// @note parameter has to be QObject (tried with Expression * entry but didn't work)
    Expression * e = dynamic_cast<Expression*>(entry);
    if (e) {
        m_lesson->removeEntry(e->kEduVocExpression());
    } else {
        qDebug() << "The entry given does not exist";
    }
}

void Lesson::clearEntries()
{
    int N = m_lesson->entryCount(KEduVocLesson::NotRecursive);
    for (int i = 0; i < N; i++)
        m_lesson->removeEntry(m_lesson->entry(0, KEduVocLesson::NotRecursive));
}


QObject* Lesson::newEntry()
{
    return new Expression();
}

//     QObject* Lesson::newEntry ( const QString & expression )
//     {
//         return new Expression ( expression );
//     }

QObject* Lesson::newEntry(const QStringList &translations)
{
    return new Expression(translations);
}

void Lesson::appendNewEntry(const QStringList &translations)
{
    KEduVocExpression * expr = new KEduVocExpression(translations);
    m_lesson->appendEntry(expr);
}

QObject * Lesson::findChildLesson(const QString& name)
{
    KEduVocContainer * container = findContainer(name);
    if (container)
        return new Lesson(container);
    qDebug() << "not found";
    return 0;
}

}
