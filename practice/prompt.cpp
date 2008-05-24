//
// C++ Implementation: prompt
//
// Description: Implementation of the prompt (or question) displaying class
//
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

#include "prompt.h"
//#include "languagesettings.h"

#include <KDebug>
//#include "prefs.h"




//void Prompt::slotSetEntry(KEduVocExpression* entry)
//{
//    m_entry = entry;
//}

//void Prompt::slotNewPrompt()
//{
//    setText(m_entry->entry()->translation(Prefs::questionLanguage()));
//    emit signalAnswerChanged(m_entry->entry()->translation(Prefs::solutionLanguage()));
//}

TextualPrompt::TextualPrompt(QWidget * parent)
        : QLabel(parent), Prompt()
{
}

Prompt::~Prompt()
{}

Prompt::Prompt()
    : m_iter(QList<KEduVocExpression*>()) // it has no empty constructor. Annoying...
{
    m_entries = QList<KEduVocExpression*>();
}

void Prompt::open(KEduVocDocument* doc)
{
    m_doc = doc;
    if (m_doc && m_doc->lesson())
    {
        m_entries = m_doc->lesson()->entries(KEduVocContainer::Recursive);
        kDebug() << "count " << m_doc->lesson()->entryCount(KEduVocContainer::Recursive);
        m_iter = m_entries;
    }
    else
    {
        kDebug() << "bad" << m_doc << m_doc->lesson();
    }
}

void TextualPrompt::slotNewPrompt()
{
    if (m_iter.hasNext())
    {
        m_entry = m_iter.next();
        KEduVocTranslation * original = m_entry->translation(0);
        kDebug() << original->text();
        setText(original->text());

        if (!original->imageUrl().isEmpty())
            emit signalNewImage(original->imageUrl());
        else
            emit signalNewImage(KUrl()); // Empty KUrl signals no image;

        if (!original->soundUrl().isEmpty())
            emit signalNewSound(original->soundUrl());
        else
            emit signalNewSound(KUrl());
        
        emit signalPromptChanged(m_entry);
        emit signalAnswerChanged(m_entry->translation(1)); // TODO not hardcode translation 1 ;)
    }
    else
    {
        emit signalSetFinished();
        kDebug() << "finished";
    }

}

/*
void TextualPrompt::slotSetEntry(const QString& entry, const QString& answer)
{
    kDebug() << "setEntry: " << entry << " " << answer;
    m_test_question = entry;
    m_test_answer = answer;
    setText(m_test_question);
    emit signalAnswerChanged(m_test_answer);
}*/

#include "prompt.moc"
