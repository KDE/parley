/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTINGEXPRESSION_H
#define SCRIPTINGEXPRESSION_H

#include <KEduVocExpression>

#include "lesson.h"

#include <QObject>
#include <QVariantList>

namespace Scripting
{

class Translation;

/**
* This class represents

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class Expression : public QObject
{
    Q_OBJECT
    /// Parent lesson (if any)
    Q_PROPERTY(QObject * lesson READ lesson)
    /// Specifies if the entry is active or not (enabled for queries or not)
    Q_PROPERTY(bool active READ isActive WRITE setActive)
//             Q_PROPERTY ( int sizeHint READ sizeHint WRITE setSizeHint)
public:
    /* default constructor for an empty vocabulary expression
     */
    Expression();

    /* Constructor for a vocabulary expression with one translation
     *
     * @param expression       translation
     */
    explicit Expression(const QString & expression);

    /*
     * Constructor for vocabulary expression with more than one translation
     * @param translations
     */
    explicit Expression(const QStringList & translations);

    Expression(const Expression & other);

    /*
     * Constructor from a KEduVocExpression (doesn't exist in KEduVocExpression)
     * @param expression KEduVocExpression object
     */
    explicit Expression(KEduVocExpression * expression);

    ~Expression();

    KEduVocExpression * kEduVocExpression() const {
        return m_expression;
    }

    template <class T, class S>
    QVariantList toVariantList(QList<T*> objList) const;

    //Property: lesson [get method - read-only]
    QObject * lesson() const {
        return new Lesson(m_expression->lesson());
    }

    /* returns flag if entry is activated for queries */
    bool isActive() const {
        return m_expression->isActive();
    }
    /* set entry active (enabled for queries) */
    void setActive(bool flag = true) {
        m_expression->setActive(flag);
    }

    //Property: sizeHing [get/set methods] (not implemented in KEduVocExpression)
    //int sizeHint() const { return m_expression->sizeHint(); }
    //void setSizeHint( int sizeHint ) { m_expression->setSizeHint(sizeHint); }

    //for assignlable type
//             Expression & operator= ( const Expression &other );
//             bool operator== ( const Expression &other ) const;

public slots:

    /** reset all grades of the entry
     * @param index     identifier (language)
     */
    void resetGrades(int index) {
        m_expression->resetGrades(index);
    }

    //not implemented in KEduVocExpression
//             void setTranslation( int index, QObject* translation );

    /**
     * Add a translation to this expression
     * @code
     * #how to set translations of an entry with setTranslation(index,expression)
     * import Parley
     * lesson = Parley.doc.findLesson("Lesson 2")
     * if lesson != None:
     *     new_entry = lesson.newEntry()
     *     new_entry.setTranslation(0,"day")
     *     new_entry.setTranslation(0,"jour")
     *     lesson.appendEntry(new_entry)
     * @endcode
     * @param index            number of translation = the identifier
     * @param expression       the translation
     */
    void setTranslation(int index, const QString &expression) {
        m_expression->setTranslation(index, expression);
    }

    /**
     * Sets the translations of an entry. All previous translations are removed
     * @code
     * #how to set the translations of an entry
     * import Parley
     * new_entry = Parley.activeLesson.newEntry()
     * new_entry.setTranslations(["good morning","bonjour"])
     * Parley.activeLesson.appendEntry(new_entry)
     * @endcode
     * @param translations A list of strings with the translations (must be in correct language order)
     */
    void setTranslations(const QStringList &translations);

    /**
     * removes a translation
     * @code
     * #how to remove all the translations of a language from a lesson
     * import Parley
     * for entry in Parley.activeLesson.entries(True):
     *     entry.removeTranslation(1)
     * @endcode
     * @param index            number of translation 1..x
     */
    void removeTranslation(int index) {
        m_expression->removeTranslation(index);
    }

    /**
     * Get the translation of a specified @p index (translation index)
     * @param index of the language identifier
     * @return A Translation object
     */
    QObject* translation(int index) const;

    /** Returns a list of integers (the translation indices) */
    QVariantList translationIndices() const;

    /**
     * Returns a list of all the translations of an entry
     * @code
     * #iterate through all the translations of all the entries of the root lesson and print their text
     * for entry in Parley.doc.rootLesson.entries(True):
     *    for tr in entry.translations():
     *        print tr.text
     * @endcode
     * @return A list of Translation objects
     */
    QVariantList translations() const;

    /**
     * Returns a list of all the translations texts of an entry
     * @code
     * #iterate through all the translations of all the entries of the root lesson and print their text
     * for entry in Parley.doc.rootLesson.entries(True):
     *     print entry.translationTexts()
     * @endcode
     * @return A list of strings (list of translation.text property of every translation)
     */
    QStringList translationTexts() const;

private:
    KEduVocExpression* m_expression;
};

//Template functions should not be separated from their definition (must be in the header file)
template <class T, class S>
QVariantList Expression::toVariantList(QList<T*> objList) const
{
    QVariantList list;
    foreach(T * t, objList) {
        QObject * obj = new S(t);
        list.push_back(QVariant::fromValue(obj));
    }
    return list;
}


}

#endif
