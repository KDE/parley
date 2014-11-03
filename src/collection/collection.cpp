/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "collection.h"

#include <QTimer>
#include <QLocale>

// KDE
#include <KEMailSettings>
//#include <KLocale>

// KEduVocDocument library
#include <keduvoclesson.h>
#include <keduvocleitnerbox.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>

// Parley
#include "../config-parley.h"
#include "prefs.h"
#include "entryfilter.h"


WordCount::WordCount()
{
    clear();
}

void WordCount::clear()
{
    for (int i = 0; i <= KV_MAX_GRADE; ++i) {
	grades[i] = 0;
    }
    initial = 0;
    totalWords = 0;

    invalid = 0;
}


// ----------------------------------------------------------------


void fetchGrammar(KEduVocDocument* doc, int languageIndex)
{
    QString locale = doc->identifier(languageIndex).locale();

    QUrl location(QString("http://edu.kde.org/parley/locale/") + locale + QString(".kvtml"));

    KEduVocDocument grammarDoc;
    if (grammarDoc.open(location) == KEduVocDocument::NoError) {
        doc->identifier(languageIndex).setArticle(grammarDoc.identifier(0).article());
        doc->identifier(languageIndex).setPersonalPronouns(grammarDoc.identifier(0).personalPronouns());
        // @todo        m_doc->identifier(index).setDeclension(grammarDoc.identifier(0).declension());
        doc->identifier(languageIndex).setTenseList(grammarDoc.identifier(0).tenseList());
    } else {
        qDebug() << "Download of " << location.url() << " failed.";
    }
}

Collection::Collection(KEduVocDocument *doc, QObject* parent)
    : QObject(parent)
    , m_doc(doc)
    , m_backupTimer(0)
{
}

Collection::Collection(QUrl *url, QObject* parent)
    : QObject(parent)
    , m_doc(new KEduVocDocument(this))
    , m_backupTimer(0)
{
    // We ignore file locks here because we open the file for readonly
    // purposes only.
    m_doc->open(*url, KEduVocDocument::FileIgnoreLock);
}

Collection::~Collection()
{
    close();

    // NOTE: No saving here because at this point the Collection class is a
    //       read-only wrapper around KEduVocDocument.
    if (m_doc) {
        emit documentChanged(0);
        m_doc->deleteLater();
        m_doc = 0;
    }

    delete m_backupTimer;
}


KEduVocDocument *Collection::eduVocDocument()
{
    // If there is no present vocabulary document, create an empty one.
    if (!m_doc) {
        m_doc = new KEduVocDocument();
    }

    return m_doc;
}

void Collection::setTitle(const QString& title)
{
    m_doc->setTitle(title);
}



void Collection::enableAutoBackup(bool enable)
{
    if (!enable) {
        if (m_backupTimer) {
            m_backupTimer->stop();
        }
    } else {
        if (!m_backupTimer) {
            m_backupTimer = new QTimer(this);
            connect(m_backupTimer, SIGNAL(timeout()), this, SLOT(save()));
        }
        m_backupTimer->start(Prefs::backupTime() * 60 * 1000);
    }
}

void Collection::numDueWords(WordCount &wc)
{
    // Get the entries from the collection. Cache them for future use.
    if (m_allTestEntries.isEmpty()) {
	EntryFilter  filter(m_doc, this);
	m_allTestEntries = filter.entries(false);
    }

    // Count the number of words due for each grade level.
    // TODO: Also take into account pregrades.
    foreach (const TestEntry *entry, m_allTestEntries) {
	int languageTo = entry->languageTo();
	KEduVocExpression *exp = entry->entry();

	wc.grades[exp->translation(languageTo)->grade()]++;
    }

    wc.totalWords = m_allTestEntries.count();
    //kDebug() << m_doc->title() << wc.totalWords << "entries";
}


// Slots


void Collection::close()
{
    enableAutoBackup(false);
    m_doc->close();
}


// ----------------------------------------------------------------
//                         private functions


void Collection::setDefaultDocumentProperties(KEduVocDocument *doc)
{
    KEMailSettings emailSettings;
    emailSettings.setProfile(emailSettings.defaultProfileName());
    doc->setAuthor(emailSettings.getSetting(KEMailSettings::RealName));
    doc->setAuthorContact(emailSettings.getSetting(KEMailSettings::EmailAddress));

    doc->setLicense(i18n("Public Domain"));
    doc->setCategory(i18n("Languages"));
    
    QString locale = QLocale().name();

    doc->appendIdentifier();
    doc->appendIdentifier();
    doc->identifier(0).setName(QLocale(locale).nativeLanguageName());
    doc->identifier(0).setLocale(locale);
    doc->identifier(1).setName(i18n("A Second Language"));
    doc->identifier(1).setLocale(locale);

    KEduVocLesson* lesson = new KEduVocLesson(i18n("Lesson 1"), doc->lesson());
    doc->lesson()->appendChildContainer(lesson);

    // add some entries
    for (int i = 0; i < 15 ; i++) {
        lesson->appendEntry(new KEduVocExpression());
    }

    doc->setModified(false);
}

void Collection::initializeDefaultGrammar(KEduVocDocument *doc)
{
    KEduVocWordType *root = doc->wordTypeContainer();
    KEduVocWordType *noun = new KEduVocWordType(i18n("Noun"), root);
    noun->setWordType(KEduVocWordFlag::Noun);
    root->appendChildContainer(noun);

    KEduVocWordType *nounChild = new KEduVocWordType(i18n("Masculine"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine);
    noun->appendChildContainer(nounChild);
    nounChild = new KEduVocWordType(i18n("Feminine"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine);
    noun->appendChildContainer(nounChild);
    nounChild = new KEduVocWordType(i18n("Neuter"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter);
    noun->appendChildContainer(nounChild);

    KEduVocWordType *verb = new KEduVocWordType(i18n("Verb"), root);
    verb->setWordType(KEduVocWordFlag::Verb);
    root->appendChildContainer(verb);

    KEduVocWordType *adjective = new KEduVocWordType(i18n("Adjective"), root);
    adjective->setWordType(KEduVocWordFlag::Adjective);
    root->appendChildContainer(adjective);

    KEduVocWordType *adverb = new KEduVocWordType(i18n("Adverb"), root);
    adverb->setWordType(KEduVocWordFlag::Adverb);
    root->appendChildContainer(adverb);

    KEduVocWordType *conjunction = new KEduVocWordType(i18n("Conjunction"), root);
    conjunction->setWordType(KEduVocWordFlag::Conjunction);
    root->appendChildContainer(conjunction);
}
