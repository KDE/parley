/***************************************************************************

                             manage queries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "kvtquery.h"

#include <klocale.h>
#include <kconfig.h>

#include <keduvocdocument.h>

#include <prefs.h>

QStringList KVTQuery::userTypes;

struct t_type_rel
{
    const char *short_ref;
    const char *long_ref;
};

// types are hierarchical !
// - . divides main from sub type
// - sub types must follow main type

// user types are strings-references like this: #1

static t_type_rel InternalTypeRelations [] =
    {
        { QM_ADJ,                           I18N_NOOP("Adjective") },

        { QM_ADV,                           I18N_NOOP("Adverb") },

        { QM_ART,                           I18N_NOOP("Article") },
        { QM_ART QM_TYPE_DIV QM_ART_DEF,    I18N_NOOP("Article Definite") },
        { QM_ART QM_TYPE_DIV QM_ART_IND,    I18N_NOOP("Article Indefinite") },

        { QM_CON,                           I18N_NOOP("Conjunction") },

        { QM_NAME,                          I18N_NOOP("Name") },   // old type "3"

        { QM_NOUN,                          I18N_NOOP("Noun") },   // old type "2"
        { QM_NOUN QM_TYPE_DIV QM_NOUN_M,    I18N_NOOP("Noun Male") },
        { QM_NOUN QM_TYPE_DIV QM_NOUN_F,    I18N_NOOP("Noun Female") },
        { QM_NOUN QM_TYPE_DIV QM_NOUN_S,    I18N_NOOP("Noun Neutral") },

        { QM_NUM,                           I18N_NOOP("Numeral") },
        { QM_NUM QM_TYPE_DIV QM_NUM_ORD,    I18N_NOOP("Numeral Ordinal") },
        { QM_NUM QM_TYPE_DIV QM_NUM_CARD,   I18N_NOOP("Numeral Cardinal") },

        { QM_PHRASE,                        I18N_NOOP("Phrase") },

        { QM_PREP,                          I18N_NOOP("Preposition") },

        { QM_PRON,                          I18N_NOOP("Pronoun") },
        { QM_PRON QM_TYPE_DIV QM_PRON_POS,  I18N_NOOP("Pronoun Possessive") },
        { QM_PRON QM_TYPE_DIV QM_PRON_PER,  I18N_NOOP("Pronoun Personal") },

        { QM_QUEST,                         I18N_NOOP("Question") },

        { QM_VERB,                          I18N_NOOP("Verb") },   // old type "1"
        { QM_VERB  QM_TYPE_DIV QM_VERB_IRR, I18N_NOOP("Verb Irregular") },
        { QM_VERB  QM_TYPE_DIV QM_VERB_REG, I18N_NOOP("Verb Regular") },

        { 0, 0 }  // the end
    };


QString KVTQuery::getSubType(const QString & type)
{
    int i;
    QString t = type;
    if ((i = t.indexOf(QM_TYPE_DIV)) >= 0) {
        t.remove(0, i+1);
        return t;
    } else
        return QString();
}


QString KVTQuery::getMainType(const QString & type)
{
    int i;
    if ((i = type.indexOf(QM_TYPE_DIV)) >= 0)
        return type.left(i);
    else
        return type;
}


KVTQuery::KVTQuery()
{
    m_doc = 0;
    m_indexFrom = 0;
    m_indexTo = 0;
    m_queryType = RandomQuery;
}


QString KVTQuery::compStr(Prefs::EnumCompType::type type)
{
    QString str = "???";
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        str = i18n("Do not Care");
        break;
    case Prefs::EnumCompType::WorseThan:
        str = i18n("Worse Than");
        break;
    case Prefs::EnumCompType::WorseEqThan:
        str = i18n("Equal/Worse Than");
        break;
    case Prefs::EnumCompType::MoreThan:
        str = i18n(">");
        break;
    case Prefs::EnumCompType::MoreEqThan:
        str = i18n(">=");
        break;
    case Prefs::EnumCompType::BetterEqThan:
        str = i18n("Equal/Better Than");
        break;
    case Prefs::EnumCompType::BetterThan:
        str = i18n("Better Than");
        break;
    case Prefs::EnumCompType::LessEqThan:
        str = i18n("<=");
        break;
    case Prefs::EnumCompType::LessThan:
        str = i18n("<");
        break;
    case Prefs::EnumCompType::EqualTo:
        str = i18n("Equal To");
        break;
    case Prefs::EnumCompType::NotEqual:
        str = i18n("Not Equal");
        break;
    case Prefs::EnumCompType::OneOf:
        str = i18n("Contained In");
        break;
    case Prefs::EnumCompType::NotOneOf:
        str = i18n("Not Contained In");
        break;
    case Prefs::EnumCompType::Within:
        str = i18n("Within Last");
        break;
    case Prefs::EnumCompType::Before:
        str = i18n("Before");
        break;
    case Prefs::EnumCompType::NotQueried:
        str = i18n("Not Queried");
        break;
    case Prefs::EnumCompType::Current:
        return i18n("Current Lesson");
        break;
    case Prefs::EnumCompType::NotAssigned:
        return i18n("Not Assigned");
        break;
    default:
        ;
    }
    return str;
}


QString KVTQuery::gradeStr(int i)
{
    switch (i) {
    case KV_NORM_GRADE:
        return i18n(KV_NORM_TEXT);
        break;
    case KV_LEV1_GRADE:
        return i18n(KV_LEV1_TEXT);
        break;
    case KV_LEV2_GRADE:
        return i18n(KV_LEV2_TEXT);
        break;
    case KV_LEV3_GRADE:
        return i18n(KV_LEV3_TEXT);
        break;
    case KV_LEV4_GRADE:
        return i18n(KV_LEV4_TEXT);
        break;
    case KV_LEV5_GRADE:
        return i18n(KV_LEV5_TEXT);
        break;
    case KV_LEV6_GRADE:
        return i18n(KV_LEV6_TEXT);
        break;
    case KV_LEV7_GRADE:
        return i18n(KV_LEV7_TEXT);
        break;
    default:
        return i18n(KV_LEV1_TEXT);
        break;
    }
}


QList<TypeRelation> KVTQuery::getRelation(bool only_maintypes)
{
    QList<TypeRelation> vec;
    for (int i = 0; i < userTypes.count(); i++) {
        QString s;
        s.setNum(i + 1);
        s.prepend(QM_USER_TYPE);
        vec.append(TypeRelation(s, userTypes[i]));
    }

    t_type_rel *type = InternalTypeRelations;
    while (type->short_ref != 0) {
        if (!only_maintypes || strstr(type->short_ref, QM_TYPE_DIV) == 0)
            vec.append(TypeRelation(type->short_ref, i18n(type->long_ref)));
        type++;
    }

    return vec;
}


QString KVTQuery::typeStr(const QString &id)
{
    if (id.left(1) == QM_USER_TYPE) {
        QString num = id;
        num.remove(0, 1);
        int i = num.toInt()-1;
        if (i >= 0 && i < userTypes.count())
            return userTypes[i];
        else
            return QString();
    } else {
        t_type_rel *type = InternalTypeRelations;
        while (type->short_ref != 0) {
            if (type->short_ref == id)
                return i18n(type->long_ref);
            type++;
        }
    }
    return QString();
}


bool KVTQuery::compareBlocking(int grade, const QDateTime &date, bool use_it)
{
    if (grade == KV_NORM_GRADE || Prefs::blockItem(grade) == 0 || !use_it) // don't care || all off
        return true;
    else
        return date.addSecs(Prefs::blockItem(grade)) < QDateTime::currentDateTime();
}


bool KVTQuery::compareExpiring(int grade, const QDateTime &date, bool use_it)
{
    if (grade == KV_NORM_GRADE || Prefs::expireItem(grade) == 0 || !use_it) // don't care || all off
        return false;
    else
        return date.addSecs(Prefs::expireItem(grade)) < QDateTime::currentDateTime();
}


bool KVTQuery::compareDate(int type, const QDateTime &qd)
{
    QDateTime now = QDateTime::currentDateTime();
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::Before:
        erg = qd.toTime_t() == 0 || qd < now.addSecs(-Prefs::dateItem());
        break; // never queried or older date
    case Prefs::EnumCompType::Within:
        erg = qd >= now.addSecs(-Prefs::dateItem());
        break; // newer date
    case Prefs::EnumCompType::NotQueried:
        erg = qd.toTime_t() == 0;
        break;
    default:
        ;
    }
    return erg;
}


bool KVTQuery::compareQuery(int type, int qgrade, int limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::MoreThan:
        erg = qgrade > limit;
        break; // sel has higher query count
    case Prefs::EnumCompType::MoreEqThan:
        erg = qgrade >= limit;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = qgrade == limit;
        break;
    case Prefs::EnumCompType::NotEqual:
        erg = qgrade != limit;
        break;
    case Prefs::EnumCompType::LessEqThan:
        erg = qgrade <= limit;
        break; // sel has less count
    case Prefs::EnumCompType::LessThan:
        erg = qgrade < limit;
        break;
    default:
        ;
    }
    return erg;
}


bool KVTQuery::compareBad(int type, int bcount, int limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::MoreThan:
        erg = bcount > limit;
        break;   // sel has higher bad count
    case Prefs::EnumCompType::MoreEqThan:
        erg = bcount >= limit;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = bcount == limit;
        break;
    case Prefs::EnumCompType::NotEqual:
        erg = bcount != limit;
        break;
    case Prefs::EnumCompType::LessEqThan:
        erg = bcount <= limit;
        break;  // sel has less count
    case Prefs::EnumCompType::LessThan:
        erg = bcount < limit;
        break;
    default:
        ;
    }
    return erg;
}


bool KVTQuery::compareGrade(int type, grade_t qgrade, grade_t limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::WorseThan:
        erg = qgrade < limit;
        break; // sel has worse grade
    case Prefs::EnumCompType::WorseEqThan:
        erg = qgrade <= limit;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = qgrade == limit;
        break;
    case Prefs::EnumCompType::NotEqual:
        erg = qgrade != limit;
        break;
    case Prefs::EnumCompType::BetterEqThan:
        erg = qgrade >= limit;
        break; // sel has better grade
    case Prefs::EnumCompType::BetterThan:
        erg = qgrade > limit;
        break;
    default:
        ;
    }
    return erg;
}


bool KVTQuery::compareType(int type, const QString & exprtype, const QString & limit)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = getMainType(exprtype) == getMainType(limit);
        break;     // type is same
    case Prefs::EnumCompType::NotEqual:
        erg = getMainType(exprtype) != getMainType(limit);
        break;     // other type
    default:
        break;
    }
    return erg;
}



void KVTQuery::setTypeNames(const QStringList &names)
{
    userTypes = names;
}

void KVTQuery::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
}

void KVTQuery::setFromTranslation(int indexFrom)
{
    m_indexFrom = indexFrom;
}

void KVTQuery::setToTranslation(int indexTo)
{
    m_indexTo = indexTo;
}

void KVTQuery::setQueryType(QueryType queryType)
{
    m_queryType = queryType;
}

QuerySelection KVTQuery::queryEntries()
{
    if (m_doc == 0) {
        kError() << "KVTQuery::queryEntries(): Cannot create query without source document set." << endl;
    }

    // initialize vector with (m_doc->lessonCount() + 1) elements
    QuerySelection random(m_doc->lessonCount() + 1);

    // disable every single entry
    for (int i = 0; i < m_doc->entryCount(); i++)
        m_doc->entry(i)->setInQuery(false);

    // reenable those that we like by using isActive and validate
    //Note that Leitner style learning (altlearn) normally only uses 20
    //entries, we just ignore that here
    for (int i = 0; i < m_doc->entryCount(); i++) {
        KEduVocExpression *expr = m_doc->entry(i);
        if (expr->isActive()) {
            if (validate(expr)) {
                int lessonNumber;
                if (Prefs::altLearn()) {
                    lessonNumber = 0; //We only use a single array in Leitner style
                } else {
                    lessonNumber = expr->lesson();
                }
                random[lessonNumber].append(QueryEntry(expr, i));
                expr->setInQuery(true);

                kDebug() << " Add to query: lesson: " << expr->lesson() << " from translation: " << expr->translation(m_indexFrom).translation() << " grade: " << expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade() << " grade (reversed): " << expr->translation(m_indexFrom).gradeFrom(m_indexTo).grade();
            }
        }
    }

    // remove empty lesson elements - backwards to not interfere with smaller indexes...
    for (int i = (int) random.size()-1; i >= 0; i--)
        if (random[i].size() == 0)
            random.erase(random.begin() + i);

    // vector of list (lessons) of entries
    return random;
}


bool KVTQuery::validateWithSettings(KEduVocExpression *expr)
{
    if ( (compareExpiring(expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade(), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryDate(), Prefs::expire() )
                ||

                (
                    compareGrade(Prefs::compType(Prefs::EnumType::Grade), expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade(), Prefs::gradeItem())
                    && compareQuery(Prefs::compType(Prefs::EnumType::Query), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryCount(), Prefs::queryItem())
                    && compareBad(Prefs::compType(Prefs::EnumType::Bad), expr->translation(m_indexTo).gradeFrom(m_indexFrom).badCount(), Prefs::badItem())
                    && compareDate(Prefs::compType(Prefs::EnumType::Date), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryDate())
                    && compareBlocking(expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade(), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryDate(), Prefs::block())
                )
            )
                && compareType(Prefs::compType(Prefs::EnumType::WordType), expr->translation(m_indexTo).type(), Prefs::typeItem())
                && !expr->translation(m_indexFrom).translation().simplified().isEmpty()
                && !expr->translation(m_indexTo).translation().simplified().isEmpty()
        ) {
            return true;
        }
    return false;
}

bool KVTQuery::validate(KEduVocExpression *expr)
{
    if ( !m_doc->lessonsInQuery().contains( expr->lesson() ) ) {
        return false;
    }

    switch (m_queryType)
    {
    // The type queries so far do not consider any settings except lesson. So they return true as long as the type is right or there is data available.
    // This could be improved, but there are no open bugs concerning this atm.
    // So this is rather low priority.
    case KVTQuery::SynonymQuery:
        return !expr->translation(m_indexFrom).synonym().simplified().isEmpty();
        break;
    case KVTQuery::AntonymQuery:
        return !expr->translation(m_indexFrom).antonym().simplified().isEmpty();
        break;
    case KVTQuery::ParaphraseQuery:
        return !expr->translation(m_indexFrom).paraphrase().simplified().isEmpty();
        break;
    case KVTQuery::ExampleQuery:
        return !expr->translation(m_indexFrom).example().simplified().isEmpty();
        break;

    case KVTQuery::ArticleQuery:
        return expr->translation(m_indexFrom).type() == QM_NOUN  QM_TYPE_DIV  QM_NOUN_S
                     || expr->translation(m_indexFrom).type() == QM_NOUN  QM_TYPE_DIV  QM_NOUN_M
                     || expr->translation(m_indexFrom).type() == QM_NOUN  QM_TYPE_DIV  QM_NOUN_F;
        break;
    case KVTQuery::ComparisonAdjectiveQuery:
        return  expr->translation(m_indexFrom).type() == QM_ADJ && !expr->translation(m_indexFrom).comparison().isEmpty();
        break;
    case KVTQuery::ComparisonAdverbQuery:
        return  expr->translation(m_indexFrom).type() == QM_ADV && !expr->translation(m_indexFrom).comparison().isEmpty();
        break;
    case KVTQuery::ConjugationQuery:
        return (expr->translation(m_indexFrom).type() == QM_VERB
                   || expr->translation(m_indexFrom).type() == QM_VERB  QM_TYPE_DIV  QM_VERB_IRR
                   || expr->translation(m_indexFrom).type() == QM_VERB  QM_TYPE_DIV  QM_VERB_REG
                  )
                  && expr->translation(m_indexFrom).conjugation().entryCount() > 0;
        break;

    case KVTQuery::RandomQuery: // Random and MC use the full settings:
    case KVTQuery::MultipleChoiceQuery:
        if ( validateWithSettings(expr) ) {
            return true;
        }
        if (Prefs::swapDirection()) {
            int temp = m_indexFrom;
            m_indexFrom = m_indexTo;
            m_indexTo = temp;
            return validateWithSettings(expr);
        } // swapDirection
        break;

    default:
        kError() << "Trying to validate with unknown query type!" << endl;
    }
    return false;
}

