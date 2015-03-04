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

#include "parley.h"

#include "editor/editor.h"
#include <keduvocvocabularymodel.h>
#include "vocabularyview.h"

#include "../scriptmanager.h"
#include "../translator.h"

#include "document.h"
#include "lesson.h"
#include "expression.h"
#include "translation.h"

#include <QDebug>
#include <KActionCollection>
#include <QMenuBar>


using namespace Editor;

namespace Scripting
{

Parley::Parley(EditorWindow * editor) : QObject(), m_editor(editor)
{
    m_translator = new Translator(this); //parameter has to be <this> cause it's used by Translator to access callTranslateWord
    m_doc = new Document(m_editor->m_mainWindow->parleyDocument()->document());
}

Parley::~Parley()
{
    delete m_translator;
    delete m_doc;
}

void Parley::callTranslateWord(const QString & word, const QString& fromLanguage, const QString& toLanguage)
{
    emit translationStarted(word, fromLanguage, toLanguage);
    emit translateWord(word, fromLanguage, toLanguage);
    emit translationFinished(word, fromLanguage, toLanguage);
}

void Parley::addTranslation(QString word, QString fromLanguage, QString toLanguage, QString translation)
{
    if (m_translator)
        m_translator->addTranslation(word, fromLanguage, toLanguage, translation);
}

QStringList Parley::locales()
{
    /// @todo Change it into a QMap property (Parley.languageCodes)
    return QLocale().uiLanguages();
}

QString Parley::localeName(QString locale)
{
    return QLocale( locale ).nativeLanguageName( );
}

void Parley::open(QString filename)
{
    QUrl url( QUrl::fromLocalFile(filename) );
    qDebug() << url;
    m_editor->m_mainWindow->parleyDocument()->open(url);
}

QObject* Parley::activeLesson()
{
    return new Lesson(m_editor->m_vocabularyModel->lesson());
}

QVariantList Parley::selectedEntries()
{
    QVariantList entries;

    //get selected indexes and active lesson
    QModelIndexList indexes = m_editor->m_vocabularyView->getSelectedIndexes();

    //get the unique selected entries
    QSet<KEduVocExpression*> kentries;
    foreach(const QModelIndex & index, indexes) {
//             qDebug() << index.row() << index.data(Qt::DisplayRole);
        KEduVocExpression * expr = qvariant_cast<KEduVocExpression*> (index.data(KEduVocVocabularyModel::EntryRole));
        kentries << expr;
    }

    //convert them to Expression objects and add them to the QVariantList
    foreach(KEduVocExpression * expr, kentries) {
//             Expression entry(expr);
//             qDebug() << entry.translationTexts();
        QObject * obj = new Expression(expr);
        entries << qVariantFromValue(obj);
    }

    return entries;
}

QVariantList Parley::selectedTranslations()
{
    QVariantList translations;

    //get selected indexes and active lesson
    QModelIndexList indexes = m_editor->m_vocabularyView->getSelectedIndexes();

    //get the unique selected entries
    QSet<KEduVocTranslation*> ktranslations;
//         const QModelIndex &index;
    foreach(const QModelIndex & index, indexes) {
        if (KEduVocVocabularyModel::columnType(index.column()) == KEduVocVocabularyModel::Translation) {
            KEduVocExpression * expr = qvariant_cast<KEduVocExpression*> (index.data(KEduVocVocabularyModel::EntryRole));
            ktranslations << expr->translation(KEduVocVocabularyModel::translation(index.column()));
        }
//             qDebug() << index.row() << index.data(Qt::DisplayRole);
    }

    //convert them to Expression objects and add them to the QVariantList
    foreach(KEduVocTranslation * tr, ktranslations) {
//             Translation transltion(tr);
//             qDebug() << entry.translationTexts();
        QObject * obj = new Translation(tr);
        translations << qVariantFromValue(obj);
    }

    return translations;
}

QObject * Scripting::Parley::newAction(const QString & name, const QString& text)
{
    //create new action
    QAction* action = new QAction(text, m_editor);
    m_editor->m_scriptManager->addScriptAction(name, action);
    return action;

}

}
