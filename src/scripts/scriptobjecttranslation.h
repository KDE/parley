//
// C++ Interface: scriptobjecttranslation
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTINGSCRIPTOBJECTTRANSLATION_H
#define SCRIPTINGSCRIPTOBJECTTRANSLATION_H

#include "keduvoctranslation.h"
#include "scriptobjectentry.h"

#include <QObject>

namespace Scripting
{

    /**
        KEduVocTranslation wrapper class for scripting with Kross

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Translation : public QObject
    {
            Q_OBJECT
            Q_PROPERTY (QString text READ text WRITE setText)
            Q_PROPERTY (QString pronunciation READ pronunciation WRITE setPronunciation )
            Q_PROPERTY (QString comment READ comment WRITE setComment )
        public:
            /**
             * Default constructor for an empty translation.
             */
            Translation ( Entry * entry );

            /**
             * Constructor
             * @param translation is used as translation
             */
            Translation ( Entry * entry, const QString & translation );

            /**
             * Constructor from KEduVocTranslation (not used by scripts)
             * @param translation KEduVocTranslation to initialize ScriptObjecTranslation
             */
            Translation ( KEduVocTranslation * translation );

            //Property: text (see KEduVocText for details)
            QString text() const { return m_translation->text(); }
            void setText( const QString & expr ) { m_translation->setText(expr); }

            //Property: pronunciation (see KEduVocTranslation for details)
            QString pronunciation() const { return m_translation->pronunciation(); }
            void setPronunciation ( const QString & expression ) { m_translation->setPronunciation(expression); }

            //Property: comment (see KEduVocTranslation for details)
            QString comment() const { return m_translation->comment(); }
            void setComment( const QString & expr ) { m_translation->setComment(expr); }

            /// @todo add more properties

            ~Translation();

        private:
            KEduVocTranslation * m_translation;

    };

}

#endif
