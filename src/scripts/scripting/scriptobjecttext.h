//
// C++ Interface: text
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTINGTEXT_H
#define SCRIPTINGTEXT_H

#include <QObject>
#include <keduvoctext.h>

namespace Scripting
{

    /**
    exposes KEduVocText to Parley scripting

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Text : public QObject
    {
            Q_OBJECT
            Q_PROPERTY ( QString text READ text WRITE setText )
            Q_PROPERTY ( unsigned int practiceCount READ practiceCount WRITE setPracticeCount )
            Q_PROPERTY ( unsigned int badCount READ badCount WRITE setBadCount )
            Q_PROPERTY ( unsigned int grade READ grade() WRITE setGrade )
            /// @todo check if QDateTime is accessible by scripts
            Q_PROPERTY ( QDateTime practiceDate READ practiceDate WRITE setPracticeDate )
        public:
            /** default constructor */
            Text ( const QString& text = QString() );

            /** copy constructor
             * provides safe copy of d pointer
             * @param other object to copy from
             */
//             KEduVocText ( const KEduVocText &other );

            /**
             * Constructor from KEduVocText (not used by scripts)
             * @param text KEduVocText to initialize Scripting::Text
             */
            Text ( KEduVocText * text );

            ~Text();

            KEduVocText * kEduVocText() { return m_text; }

            /**
             * The translation as string (the word itself)
             * @return the translation
             */
            QString text() const { return m_text->text(); }

            /**
             * Sets the translation
             * @param expr
             */
            void setText ( const QString & expr ) { m_text->setText ( expr ); }

            /**
             * Equal operator to copy grades.
             * @param other grades copied
             * @return reference to the new grades
             */
//             Text& operator= ( const Text &other ) { m_text->operator=(*(other.kEduVocText())); }
            /**
             * Compare two sets of grades.
             * @param other
             * @return true if equal
             */
//             bool operator== ( const KEduVocText &other ) const;


            /** returns how often this entry has been practiced as int
            * @returns total count
            */
            unsigned int practiceCount() const { return m_text->practiceCount(); }

            /** set how often this entry has been practiced as int
            * @param count the new count
            */
            void setPracticeCount ( unsigned int count ) { m_text->setPracticeCount ( count ); }

            /** returns bad query count as int
            * @returns bad query count
            */
            unsigned int badCount() const { return m_text->badCount(); }

            /** set bad query count as int
            * @param count the new count
            */
            void setBadCount ( unsigned int count ) { m_text->setBadCount ( count ); }

            /** sets the grade
            * @param grade number of knowlegde: 0=known, x=numbers not knows
            */
            void setGrade ( unsigned int grade ) { m_text->setGrade ( grade ); }

            /** returns grade as int
            * @returns number of knowlegde: 0=known, x=numbers not knows
            */
            unsigned int grade() const { return m_text->grade(); }

            /** returns last practice date as int
            */
            QDateTime practiceDate() const { return m_text->practiceDate(); }

            /** Set last query date
            * @param date             the new date
            */
            void setPracticeDate ( const QDateTime & date ) { m_text->setPracticeDate ( date ); }

            /**
             * If the string inside is empty this returns true.
             * @return
             */
            bool isEmpty() { return m_text->isEmpty(); }

//             void fromKVTML2 ( QDomElement& parent );
//             void toKVTML2 ( QDomElement& parent );

        public slots:
            /** increment bad query count of given translation by 1 */
            void incBadCount() { m_text->incBadCount(); }

            /** increment query count of given translation by 1 */
            void incPracticeCount() { m_text->incPracticeCount(); }

            /**
             * Clears grading and date information.
             */
            void resetGrades() { m_text->resetGrades(); }

            /** increments grade */
            void incGrade() { m_text->incGrade(); }

            /** decrements grade */
            void decGrade() { m_text->decGrade(); }

        protected:
            KEduVocText * m_text;

    };

}

#endif
