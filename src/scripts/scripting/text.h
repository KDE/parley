/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTINGTEXT_H
#define SCRIPTINGTEXT_H

#include <QObject>
#include <KEduVocText>

namespace Scripting
{

/**
 * Scripting::Text class object provides methods and properties for accessing Parley text's value and grades. It is inherited and mainly used by Scripting::Translation.
 * @code
 * #how to access a translation's object properties/methods
 * import Parley
 * for entry in Parley.document.rootLesson.entries(True):
 *     for translation in entry.translations():
 *         print translation.text
 *         print translation.grade
 *         translation.incBadCount()
 *         print translation.practiceDate("dd/MM/yyyy")
 *         translation.grade = 5
 * @endcode
 * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
 */
class Text : public QObject
{
    Q_OBJECT

    /// The translation as string (the word itself)
    Q_PROPERTY(QString text READ text WRITE setText)

    /// how often this entry has been practiced as int
    Q_PROPERTY(unsigned int practiceCount READ practiceCount WRITE setPracticeCount)

    /// how many times the word was not found correctly
    Q_PROPERTY(unsigned int badCount READ badCount WRITE setBadCount)

    /// word grade
    Q_PROPERTY(unsigned int grade READ grade() WRITE setGrade)

    // last date when this word was practiced
//             Q_PROPERTY ( QDateTime practiceDate READ practiceDate WRITE setPracticeDate )

    /// true if the word text is empty
    Q_PROPERTY(bool isEmpty READ isEmpty)

public:
    /* default constructor */
    explicit Text(const QString& text = QString());

    /* copy constructor
     * provides safe copy of d pointer
     * @param other object to copy from
     */
//             KEduVocText ( const KEduVocText &other );

    /*
     * Constructor from KEduVocText (not used by scripts)
     * @param text KEduVocText to initialize Scripting::Text
     */
    explicit Text(KEduVocText * text);

    explicit Text(const KEduVocText & text);

    ~Text();

    KEduVocText * kEduVocText() {
        return m_text;
    }

    /*
     * The translation as string (the word itself)
     * @return the translation
     */
    QString text() const {
        return m_text->text();
    }

    /*
     * Sets the translation
     * @param expr
     */
    void setText(const QString & expr) {
        m_text->setText(expr);
    }

    /*
     * Equal operator to copy grades.
     * @param other grades copied
     * @return reference to the new grades
     */
//             Text& operator= ( const Text &other ) { m_text->operator=(*(other.kEduVocText())); }
    /*
     * Compare two sets of grades.
     * @param other
     * @return true if equal
     */
//             bool operator== ( const KEduVocText &other ) const;


    /* returns how often this entry has been practiced as int
    * @returns total count
    */
    unsigned int practiceCount() const {
        return m_text->practiceCount();
    }

    /* set how often this entry has been practiced as int
    * @param count the new count
    */
    void setPracticeCount(unsigned int count) {
        m_text->setPracticeCount(count);
    }

    /* returns bad query count as int
    * @returns bad query count
    */
    unsigned int badCount() const {
        return m_text->badCount();
    }

    /* set bad query count as int
    * @param count the new count
    */
    void setBadCount(unsigned int count) {
        m_text->setBadCount(count);
    }

    /* sets the grade
    * @param grade number of knowlegde: 0=known, x=numbers not knows
    */
    void setGrade(unsigned int grade) {
        m_text->setGrade(grade);
    }

    /* returns grade as int
    * @returns number of knowlegde: 0=known, x=numbers not knows
    */
    unsigned int grade() const {
        return m_text->grade();
    }

    /*
     * If the string inside is empty this returns true.
     * @return
     */
    bool isEmpty() {
        return m_text->isEmpty();
    }

//             void fromKVTML2 ( QDomElement& parent );
//             void toKVTML2 ( QDomElement& parent );

public slots:
    /** increment bad query count of given translation by 1 */
    void incBadCount() {
        m_text->incBadCount();
    }

    /** increment query count of given translation by 1 */
    void incPracticeCount() {
        m_text->incPracticeCount();
    }

    /** Clears grading and date information. */
    void resetGrades() {
        m_text->resetGrades();
    }

    /** increments grade */
    void incGrade() {
        m_text->incGrade();
    }

    /** decrements grade */
    void decGrade() {
        m_text->decGrade();
    }

    /** returns the last date when this word was practiced */
    QString practiceDate() const {
        return m_text->practiceDate().toString();
    }

    /**
     * Last date this word was practiced
     * @param format Format of the date (see QDateTime toString(format) function
     * @return the last practice date in the format given by @p format
     * @code
     * print translation.practiceDate("dd/MM/yyyy")
     * print translation.practiceDate("M-d-yy, h:m:s")
     * @endcode
     */
    QString practiceDate(const QString & format) const {
        return m_text->practiceDate().toString(format);
    }

    /** Sets the last date when this word was practiced */
    void setPracticeDate(const QString & date) {
        m_text->setPracticeDate(QDateTime::fromString(date));
    }

    /**
     * Sets the laast date this word was practiced
     * @param date A string containing the date
     * @param format Format of the date (see QDateTime fromString(date,format) function
     * @code
     * translation.setPracticeDate("05/12/1999","dd/MM/yyyy")
     * translation.setPracticeDate("02 Jan 05","dd MMM yy")
     * @endcode
     */
    void setPracticeDate(const QString & date, const QString & format) {
        m_text->setPracticeDate(QDateTime::fromString(date, format));
    }

protected:
    KEduVocText * m_text;

};

}

#endif
