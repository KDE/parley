//
// C++ Interface: scriptobjectentry
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTINGSCRIPTOBJECTENTRY_H
#define SCRIPTINGSCRIPTOBJECTENTRY_H

#include <keduvocexpression.h>

#include <QObject>

namespace Scripting
{

    /**
    Expression class to be used by the scripts

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Expression : public QObject
    {
            Q_OBJECT
        public:
            /** default constructor for an empty vocabulary expression
             */
//             Expression () { }

            
            /**
             * Constructor from a KEduVocExpression
             * @param expression 
             */
            Expression ( KEduVocExpression * expression );
            Expression ( const Expression & other );

            ~Expression();
            KEduVocExpression * kEduVocEntry() const { return m_expression; }

            Expression & operator= ( const Expression &other );
            bool operator== ( const Expression &other ) const;

        public slots:
            /// for testing purposes only
            QString getTranslation() const;

        private:
            KEduVocExpression* m_expression;
    };

}

#endif
