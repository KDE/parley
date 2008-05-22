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

//#include <keduvocexpression.h>
//#include <keduvocdocument.h>





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
    list.append(QPair<QString, QString>("1+1=", "2"));
    list.append(QPair<QString, QString>("3+7=", "10"));
    list.append(QPair<QString, QString>("12*2=", "24"));
    list.append(QPair<QString, QString>("9/3", "3"));
    list.append(QPair<QString, QString>("9/3", "3"));
    list.append(QPair<QString, QString>("Capital of the USA", "Washington D.C."));
    list.append(QPair<QString, QString>("Capital of Minnesota", "St. Paul"));
    list.append(QPair<QString, QString>("Capital of Japan", "Tokyo"));
    list.append(QPair<QString, QString>("Capital of Mexico", "Mexico City"));
    list.append(QPair<QString, QString>("Capital of Brazil", "Buenos Aires"));

}

Prompt::~Prompt()
{}

Prompt::Prompt()
{}

void TextualPrompt::slotNewPrompt()
{
    QPair<QString, QString> qps = list.takeFirst();
    m_test_question = qps.first;
    setText(m_test_question);
    emit signalPromptChanged();
    emit signalAnswerChanged(qps.second);
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
