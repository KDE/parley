/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTINGSCRIPTOBJECTENTRY_H
#define SCRIPTINGSCRIPTOBJECTENTRY_H

#include <keduvocexpression.h>

#include "lesson.h"

#include <QObject>
#include <QVariantList>

namespace Scripting
{

    class Translation;

    /**
    Expression class to be used by the scripts

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Expression : public QObject
    {
            Q_OBJECT
            Q_PROPERTY ( Lesson * lesson READ lesson )
            Q_PROPERTY ( bool active READ isActive WRITE setActive )
            Q_PROPERTY ( QVariantList translationIndices READ translationIndices )
            Q_PROPERTY ( QVariantList translations READ translations )
//             Q_PROPERTY ( int sizeHint READ sizeHint WRITE setSizeHint)
        public:
            /** default constructor for an empty vocabulary expression
             */
            Expression ();

            /** Constructor for a vocabulary expression with one translation
             *
             * @param expression       translation
             */
            Expression ( const QString & expression );

            /**
             * Constructor for vocabulary expression with more than one translation
             * @param translations
             */
            Expression ( const QStringList & translations );

            Expression ( const Expression & other );

            /**
             * Constructor from a KEduVocExpression (doens't exist in KEduVocExpression)
             * @param expression KEduVocExpression object
             */
            Expression ( KEduVocExpression * expression );

            ~Expression();

            KEduVocExpression * kEduVocEntry() const { return m_expression; }

            template <class T, class S>
            QVariantList toVariantList ( QList<T*> objList ) const;

            //Property: lesson [get method - read-only]
            Lesson * lesson() const { return new Lesson ( m_expression->lesson() ); }

            /* returns flag if entry is activated for queries */
            bool isActive() const { return m_expression->isActive(); }
            /* set entry active (enabled for queries) */
            void setActive ( bool flag = true ) { m_expression->setActive ( flag ); }

            //Property: sizeHing [get/set methods] (not implemented in KEduVocExpression)
            //int sizeHint() const { return m_expression->sizeHint(); }
            //void setSizeHint( int sizeHint ) { m_expression->setSizeHint(sizeHint); }

            //Property: translationIndices [get method - read-only]
            QVariantList translationIndices() const;

            //Property: translations [get method - read-only]
            QVariantList translations() const;

            //for assignlable type
            Expression & operator= ( const Expression &other );
            bool operator== ( const Expression &other ) const;

        public slots:
            /// for testing purposes only
            QString getTranslation() const;

            /** reset all grades of the entry
             * @param index     identifier (language)
             */
            void resetGrades ( int index ) { m_expression->resetGrades ( index ); }

            //not implemented in KEduVocExpression
//             void setTranslation( int index, QObject* translation );

            /**
            * Add a translation to this expression
            * @param index            number of translation = the identifier
            * @param expression       the translation
            */
            void setTranslation ( int index, const QString &expression )
            {
                m_expression->setTranslation ( index,expression );
            }

            /**
             * removes a translation
             * @param index            number of translation 1..x
             */
            void removeTranslation ( int index ) { m_expression->removeTranslation ( index ); }

            /**
             * Get a pointer to the translation
             * @param index of the language identifier
             * @return the translation
             */
            Translation* translation ( int index );
            Translation* translation ( int index ) const;


        private:
            KEduVocExpression* m_expression;
    };

    //Template functions should not be separated from their definition (must be in the header file)
    template <class T, class S>
    QVariantList Expression::toVariantList ( QList<T*> objList ) const
    {
        QVariantList list;
        foreach ( T * t, objList )
        {
            QObject * obj = new S ( t );
            list.push_back ( qVariantFromValue ( obj ) );
        }
        return list;
    }


}

#endif
