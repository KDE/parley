/***************************************************************************

    C++ Implementation: keduvocwordtype

    -----------------------------------------------------------------------

    begin         : Mi Aug 22 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "keduvocwordtype.h"

#include <klocale.h>

#define QM_VERB           "v"    // go
#define   QM_VERB_IRR     "ir"
#define   QM_VERB_REG     "re"
#define QM_NOUN           "n"    // table, coffee
#define   QM_NOUN_F       "f"
#define   QM_NOUN_M       "m"
#define   QM_NOUN_S       "s"
#define QM_NAME           "nm"
#define QM_ART            "ar"   // article
#define   QM_ART_DEF      "def"  // definite    a/an
#define   QM_ART_IND      "ind"  // indefinite  the
#define QM_ADJ            "aj"   // adjective   expensive, good
#define QM_ADV            "av"   // adverb      today, strongly
#define QM_PRON           "pr"   // pronoun     you, she
#define   QM_PRON_POS     "pos"  // possessive  my, your
#define   QM_PRON_PER     "per"  // personal
#define QM_PHRASE         "ph"
#define QM_NUM            "num"  // numeral
#define   QM_NUM_ORD      "ord"  // ordinal     first, second
#define   QM_NUM_CARD     "crd"  // cardinal    one, two
#define QM_INFORMAL       "ifm"
#define QM_FIG            "fig"
#define QM_CON            "con"  // conjuncton  and, but
#define QM_PREP           "pre"  // preposition behind, between
#define QM_QUEST          "qu"   // question    who, what

// type delimiters

#define QM_USER_TYPE  "#"   // designates number of user type
#define QM_TYPE_DIV   ":"   // divide main from subtype



// class KEduVocWordType::WordType {
// public:
//     WordType(const QString& mainType = QString());
//
//     QString     m_mainType;
//     QStringList m_subTypes;
// };

// KEduVocWordType::WordType::WordType(const QString & mainType)
// {
//     m_mainType = mainType;
// }

KEduVocWordType::KEduVocWordType()
{
}

KEduVocWordType::~KEduVocWordType()
{}

void KEduVocWordType::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    m_userTypeDescriptions = m_doc->typeDescriptions();
    initOldTypeLists();
}


QString KEduVocWordType::getMainTypeFromOldFormat(const QString & typeSubtypeString) const
{
    QString mainType;
    int i;
    /// @todo check for user type first!!!
    //"#"
    if ((i = typeSubtypeString.indexOf(":")) >= 0)
        mainType = typeSubtypeString.left(i);
    else
        mainType = typeSubtypeString;

    if ( mainType.startsWith("#") ) {
        mainType.remove(0, 1);
        i = mainType.toInt()-1;
        if (i >= 0 && i < m_userTypeDescriptions.count())
            return m_userTypeDescriptions[i];
        else
            return QString();
    }

    QString wt = m_oldMainTypeNames.value( mainType );
    if ( wt == QString() ) {
        kDebug() << "Unknown old maintype: " << typeSubtypeString;
        return typeSubtypeString;
    }
    return wt;
}


QString KEduVocWordType::getSubTypeFromOldFormat(const QString & typeSubtypeString) const
{
    int i;
    QString t = typeSubtypeString;
    if ((i = t.indexOf(":")) >= 0) {
        t.remove(0, i+1);
    } else {
        return QString();
    }

    QString wt = m_oldSubTypeNames.value( t );
    if ( wt == QString() ) {
        kDebug() << "Unknown old maintype: " << typeSubtypeString;
        return typeSubtypeString;
    }
    return wt;
}


void KEduVocWordType::initOldTypeLists()
{
    m_oldMainTypeNames.clear();

    m_oldMainTypeNames.insert("v", i18n("Verb"));
    m_oldMainTypeNames.insert("n", i18n("Noun"));
    m_oldMainTypeNames.insert("nm", i18n("Name"));
    m_oldMainTypeNames.insert("ar", i18n("Article"));
    m_oldMainTypeNames.insert("aj", i18n("Adjective"));
    m_oldMainTypeNames.insert("av", i18n("Adverb"));
    m_oldMainTypeNames.insert("pr", i18n("Pronoun"));
    m_oldMainTypeNames.insert("ph", i18n("Phrase"));
    m_oldMainTypeNames.insert("num", i18n("Numeral"));
    m_oldMainTypeNames.insert("con", i18n("Conjunction"));
    m_oldMainTypeNames.insert("pre", i18n("Preposition"));
    m_oldMainTypeNames.insert("qu", i18n("Question"));
    m_oldMainTypeNames.insert("ifm", i18n("Informal"));
    m_oldMainTypeNames.insert("fig", i18n("Figuratively"));

    foreach ( QString mainWordType, m_oldMainTypeNames.values() ) {
        m_wordTypeList.insert( mainWordType, QStringList() );
    }

    m_oldSubTypeNames.clear();
    m_oldSubTypeNames.insert("ord", i18n("Numeral Ordinal"));
    m_oldSubTypeNames.insert("crd", i18n("Numeral Cardinal"));
    m_oldSubTypeNames.insert("def", i18n("Article Definite"));
    m_oldSubTypeNames.insert("ind", i18n("Article Indefinite"));
    m_oldSubTypeNames.insert("re", i18n("Verb Regular"));
    m_oldSubTypeNames.insert("ir", i18n("Verb Irregular"));
    m_oldSubTypeNames.insert("pos", i18n("Pronoun Possessive"));
    m_oldSubTypeNames.insert("per", i18n("Pronoun Personal"));
    m_oldSubTypeNames.insert("m", i18n("Noun Male"));
    m_oldSubTypeNames.insert("f", i18n("Noun Female"));
    m_oldSubTypeNames.insert("s", i18n("Noun Neutral"));

    // add subtypes:
    m_wordTypeList[i18n("Verb")] << "Verb Regular" << "Verb Irregular";
    m_wordTypeList[i18n("Noun")] << "Noun Male" << "Noun Female" << "Noun Neutral";

    m_wordTypeList[i18n("Numeral")] << "Numeral Ordinal" << "Numeral Cardinal";
    m_wordTypeList[i18n("Article")] << "Article Definite" << "Article Indefinite";
    m_wordTypeList[i18n("Pronoun")] << "Pronoun Possessive" << "Pronoun Personal";
}

QStringList KEduVocWordType::getSubTypeList(const QString & mainType) const
{
    return m_wordTypeList.value( mainType );
}



QStringList KEduVocWordType::getMainTypeList() const
{
    QStringList mainTypeList = m_wordTypeList.keys();
    mainTypeList << m_userTypeDescriptions;
    kDebug() << "m_userTypeDescriptions: " << m_userTypeDescriptions;
    return mainTypeList;
}

QString KEduVocWordType::getOldType(const QString & mainType, const QString & subType) const
{
    QString oldType;
    oldType = m_oldMainTypeNames.key(mainType);
    if ( subType != QString() ) {
        oldType.append(":");
        oldType.append(m_oldSubTypeNames.key(subType));
    }

    if ( oldType.isEmpty() ) {
        kDebug() << "Not found in preset types.";
        int index = m_userTypeDescriptions.indexOf(mainType);
        if ( index >= 0 ) {
            kDebug() << "Found user type.";
            // for some reason we count from one
            oldType = QString("#%1").arg(index + 1);
        }
    }

    kDebug() << "KEduVocWordType::getOldType(): " << mainType << ", "<< subType << " gives: " << oldType;
    return oldType;
}
