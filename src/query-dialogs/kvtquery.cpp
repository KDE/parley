/***************************************************************************

                             manage queries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
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

#include <keduvoclesson.h>
#include <keduvocdocument.h>

#include <prefs.h>


KVTQuery::KVTQuery()
{
    m_doc = 0;
    m_indexFrom = 0;
    m_indexTo = 0;
    m_testType = Prefs::EnumTestType::WrittenTest;
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
        str = i18n("Never Practiced");
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


bool KVTQuery::compareType(int type, const QString & exprtype, const QString & wordtype)
{
    bool erg = true;
    switch (type) {
    case Prefs::EnumCompType::DontCare:
        erg = true;
        break;
    case Prefs::EnumCompType::EqualTo:
        erg = (exprtype == wordtype);
        break;     // type is same
    case Prefs::EnumCompType::NotEqual:
        erg = (exprtype != wordtype);
        break;     // other type
    default:
        break;
    }
    return erg;
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

void KVTQuery::setQueryType(int testType)
{
    m_testType = testType;
}

QuerySelection KVTQuery::queryEntries()
{
    if (m_doc == 0) {
        kError() << "KVTQuery::queryEntries(): Cannot create query without source document set." << endl;
    }

    // initialize vector with (m_doc->lessonCount() + 1) elements
    QuerySelection random(m_doc->lessonCount() + 1);

    // choose entries that we like by using isActive and validate
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

                kDebug() << " Add to query: lesson: " << expr->lesson() << " from translation: " << expr->translation(m_indexFrom).text() << " grade: " << expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade() << " grade (reversed): " << expr->translation(m_indexFrom).gradeFrom(m_indexTo).grade();
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
    // check type in both directions
    if ( !
        (compareType(Prefs::compType(Prefs::EnumType::WordType), expr->translation(m_indexTo).type(), Prefs::typeItem())
        ||
        compareType(Prefs::compType(Prefs::EnumType::WordType), expr->translation(m_indexFrom).type(), Prefs::typeItem()) )) {
        return false;
    }
    if(expr->translation(m_indexFrom).text().simplified().isEmpty()) {
        return false;
    }
    if(expr->translation(m_indexTo).text().simplified().isEmpty()) {
        return false;
    }

    // if expired, always take it
    if( compareExpiring(expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade(), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryDate(), Prefs::expire() ) ) {
        return true;
    }

    if ( !compareGrade(Prefs::compType(Prefs::EnumType::Grade), expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade(), Prefs::gradeItem()) ) {
        return false;
    }

    if ( !compareQuery(Prefs::compType(Prefs::EnumType::Query), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryCount(), Prefs::queryItem())) {
        return false;
    }
    if ( !compareBad(Prefs::compType(Prefs::EnumType::Bad), expr->translation(m_indexTo).gradeFrom(m_indexFrom).badCount(), Prefs::badItem())) {
        return false;
    }
    if ( !compareDate(Prefs::compType(Prefs::EnumType::Date), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryDate())) {
        return false;
    }
    if ( !compareBlocking(expr->translation(m_indexTo).gradeFrom(m_indexFrom).grade(), expr->translation(m_indexTo).gradeFrom(m_indexFrom).queryDate(), Prefs::block())) {
        return false;
    }
kDebug() << "Adding expression to query: " << expr->translation(m_indexTo).text();
    return true;
}

bool KVTQuery::validate(KEduVocExpression *expr)
{
    if ( !m_doc->lesson(expr->lesson()).inQuery() ) {
        return false;
    }

    switch (m_testType)
    {
    // The type queries so far do not consider any settings except lesson. So they return true as long as the type is right or there is data available.
    // This could be improved, but there are no open bugs concerning this atm.
    // So this is rather low priority.
    case Prefs::EnumTestType::SynonymTest:
        return !expr->translation(m_indexFrom).synonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::AntonymTest:
        return !expr->translation(m_indexFrom).antonym().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ParaphraseTest:
        return !expr->translation(m_indexFrom).paraphrase().simplified().isEmpty();
        break;
    case Prefs::EnumTestType::ExampleTest:
        return !expr->translation(m_indexFrom).example().simplified().isEmpty();
        break;

    case Prefs::EnumTestType::ArticleTest:
        return expr->translation(m_indexFrom).type() == QM_NOUN  QM_TYPE_DIV  QM_NOUN_S
                     || expr->translation(m_indexFrom).type() == QM_NOUN  QM_TYPE_DIV  QM_NOUN_M
                     || expr->translation(m_indexFrom).type() == QM_NOUN  QM_TYPE_DIV  QM_NOUN_F;
        break;
    case Prefs::EnumTestType::ComparisonAdjectiveTest:
        return  expr->translation(m_indexFrom).type() == QM_ADJ && !expr->translation(m_indexFrom).comparison().isEmpty();
        break;
    case Prefs::EnumTestType::ComparisonAdverbTest:
        return  expr->translation(m_indexFrom).type() == QM_ADV && !expr->translation(m_indexFrom).comparison().isEmpty();
        break;
    case Prefs::EnumTestType::ConjugationTest:
        return (expr->translation(m_indexFrom).type() == QM_VERB
                   || expr->translation(m_indexFrom).type() == QM_VERB  QM_TYPE_DIV  QM_VERB_IRR
                   || expr->translation(m_indexFrom).type() == QM_VERB  QM_TYPE_DIV  QM_VERB_REG
                  )
                  && expr->translation(m_indexFrom).conjugations().count() > 0;
        break;

    case Prefs::EnumTestType::WrittenTest: // Random and MC use the full settings:
    case Prefs::EnumTestType::MultipleChoiceTest:
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

