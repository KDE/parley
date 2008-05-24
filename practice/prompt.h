//
// C++ Interface: prompt
//
// Description: The prompt displays the question portion or original translation of an entry
///\file prompt.h
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef PROMPT_H
#define PROMPT_H


#include <QObject>
#include <QLabel>

#include <QList>

#include "../../libkdeedu/keduvocdocument/keduvocexpression.h"
#include "../../libkdeedu/keduvocdocument/keduvoctranslation.h"
#include "../../libkdeedu/keduvocdocument/keduvocdocument.h"



/**
* @class Prompt
* @author David Capel <wot.narg@gmail.com>
* @brief This shows the prompt (the question, original translation, image, etc) for each question.
* Depending on the mode, this will take a variety of forms.
* This class gets each question from the database and gives the answer
* to the @class Input object.
*/

class Prompt
{

    public: // slots:
        /// Sets the question entry.
        //virtual void slotSetEntry(KEduVocExpression* entry);
        /// Called to show a new prompt and perform other tasks to create a new question.
        virtual void slotNewPrompt() = 0;
        virtual void open(KEduVocDocument*);

        
        Prompt();
        virtual ~Prompt();
        
    protected:
        KEduVocDocument * m_doc;
        QList<KEduVocExpression*> m_entries;
        KEduVocExpression * m_entry;
        QListIterator<KEduVocExpression*> m_iter;
        
   // signals:
    protected:
        /// Emitted when the question is changed.
        /// This is used so @class Statistics can update the grades and related information.
        void signalPromptChanged(KEduVocExpression*);
        /// Emitted when the answer has changed.
        /// This is used to provide the answer to the @class Input object.
        void signalAnswerChanged(KEduVocTranslation*);
        /// Emitted when a new image is available.
        /// An empty KUrl signals that there is no assosiated image.
        void signalNewImage(const KUrl&);
        /// Emitted when a new sound is available.
        /// An empty KUrl signals that there is no assosiated sound.
        void signalNewSound(const KUrl&);
        /// Emitted when the set of questions is finished.
        void signalSetFinished();

};


class TextualPrompt : public QLabel, public Prompt
{
        Q_OBJECT

    public:
        TextualPrompt(QWidget * parent = 0);

    public slots:
        void slotNewPrompt();

    signals:
        /// Emitted when the question is changed.
        /// This is used so @class Statistics can update the grades and related information.
        void signalPromptChanged(KEduVocExpression*);
        /// Emitted when the answer has changed.
        /// This is used to provide the answer to the @class Input object.
        void signalAnswerChanged(KEduVocTranslation*);
        /// Emitted when a new image is available.
        /// An empty KUrl signals that there is no assosiated image.
        void signalNewImage(const KUrl&);
        /// Emitted when a new sound is available.
        /// An empty KUrl signals that there is no assosiated sound.
        void signalNewSound(const KUrl&);
        /// Emitted when the set of questions is finished.
        void signalSetFinished();

    private:
        // these are only for the initial layout testing ;)
        QString m_test_question;
        QString m_test_answer;
        QList<QPair<QString, QString> > list;
};


#endif

