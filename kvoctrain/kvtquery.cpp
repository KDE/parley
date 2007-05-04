/***************************************************************************

                             manage queries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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
        {
            QM_ADJ,                           I18N_NOOP("Adjective")
        },

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
    lessonitems.clear();
}


QuerySelection KVTQuery::select(KEduVocDocument *doc, int act_lesson, int oindex, int tindex)
{
    kDebug() << "select()" << endl;

    QuerySelection random;
    random.resize(doc->lessonCount() + 1);
    for (int i = 0; i < doc->entryCount(); i++)
        doc->entry(i)->setInQuery(false);

    //Note that Leitner style learning (altlearn) normally only uses 20
    //entries, we just ignore that here
    for (int i = 0; i < doc->entryCount(); i++) {
        KEduVocExpression *expr = doc->entry(i);
        int lessonno;
        if (Prefs::altLearn())
            lessonno = 0; //We only use a single array in Leitner style
        else
            lessonno = expr->lesson();

        if (expr->isActive()) {
            if (Prefs::swapDirection()) {
                if (validate(expr, act_lesson, oindex, tindex) || validate(expr, act_lesson, tindex, oindex)) {
                    random[lessonno].append(QueryEntry(expr, i));
                    expr->setInQuery(true);
                    kDebug() << " Add to query (swap): " << expr->lesson() << " - " << expr->original() << endl;

                }
            } else {
                if (validate(expr, act_lesson, oindex, tindex)) {
                    random[lessonno].append(QueryEntry(expr, i));
                    expr->setInQuery(true);
                    kDebug() << " Add to query (noswap): " << expr->lesson() << " - "  << expr->original() << endl;
                }
            }
        }
    }

    // remove empty lesson elements
    for (int i = random.count() - 1; i >= 0; i--)
        if (random[i].count() == 0)
            random.erase(random.begin() + i);
    return random;
}


bool KVTQuery::validate(KEduVocExpression *expr, int act_lesson, int oindex, int tindex)
{
// USED when using default: kDebug() << "validate(KEduVocExpression *expr, int act_lesson, int oindex, int tindex)" << endl;

    int index = tindex ? tindex : oindex;
    if ((compareExpiring(expr->grade(index, oindex != 0), expr->queryDate(index, oindex != 0), Prefs::expire())
            ||

            (
                compareGrade(Prefs::compType(Prefs::EnumType::Grade), expr->grade(index, oindex != 0), Prefs::gradeItem())
                && compareQuery(Prefs::compType(Prefs::EnumType::Query), expr->queryCount(index, oindex != 0), Prefs::queryItem())
                && compareBad(Prefs::compType(Prefs::EnumType::Bad), expr->badCount(index, oindex != 0), Prefs::badItem())
                && compareDate(Prefs::compType(Prefs::EnumType::Date), expr->queryDate(index, oindex != 0))
                && compareBlocking(expr->grade(index, oindex != 0), expr->queryDate(index, oindex != 0), Prefs::block())
            )
        )
            // lesson + word type must ALWAYS match (and there must be a word on both sides)
            && compareLesson(Prefs::compType(Prefs::EnumType::Lesson), expr->lesson(), lessonitems, act_lesson)
            && compareType(Prefs::compType(Prefs::EnumType::WordType), expr->type(index), Prefs::typeItem())
            && !expr->original().simplified().isEmpty()
            && !expr->translation(index).simplified().isEmpty()
       )
        return true;
    else
        return false;
}


QuerySelection KVTQuery::select(KEduVocDocument *doc, int act_lesson, int idx, const QString &type)
{
    kDebug() << "select(KEduVocDocument *doc, int act_lesson, int idx, QString type)" << endl;
    // initialize vector with (doc->lessonCount() + 1) elements
    QuerySelection random(doc->lessonCount() + 1);

    // disable every single entry
    for (int i = 0; i < doc->entryCount(); i++)
        doc->entry(i)->setInQuery(false);

    // reenable those that we like by using isActive and validate
    for (int i = 0; i < doc->entryCount(); i++) {
        KEduVocExpression *expr = doc->entry(i);
        if (expr->isActive() && validate(expr, act_lesson, idx, type)) {
            random[expr->lesson()].append(QueryEntry(expr, i));
            expr->setInQuery(true);
        }
    }

    // remove empty lesson elements - backwards to not interfere with smaller indexes...
    for (int i = (int) random.size()-1; i >= 0; i--)
        if (random[i].size() == 0)
            random.erase(random.begin() + i);

    // vector of list (lessons) of entries
    return random;
}


bool KVTQuery::validate(KEduVocExpression *expr, int act_lesson, int idx, const QString &query_type)
{
    kDebug() << "validate(KEduVocExpression *expr, int act_lesson, int idx, QString query_type)" << endl;
    QString qtype;
    int pos = query_type.indexOf(QM_TYPE_DIV);
    if (pos >= 0)
        qtype = query_type.left(pos);
    else
        qtype = query_type;

    QString expr_type = expr->type(idx);
    bool type_ok = false;
    if (qtype == QM_NOUN) {
        type_ok =    expr_type == QM_NOUN  QM_TYPE_DIV  QM_NOUN_S
                     || expr_type == QM_NOUN  QM_TYPE_DIV  QM_NOUN_M
                     || expr_type == QM_NOUN  QM_TYPE_DIV  QM_NOUN_F;

    } else if (qtype == QM_VERB) {
        type_ok = (expr_type == QM_VERB
                   || expr_type == QM_VERB  QM_TYPE_DIV  QM_VERB_IRR
                   || expr_type == QM_VERB  QM_TYPE_DIV  QM_VERB_REG
                  )
                  && expr->conjugation(idx).entryCount() > 0;

    } else if (qtype == QM_ADJ) {
        type_ok = expr_type == QM_ADJ && !expr->comparison(idx).isEmpty();
    } else
        return false;

    if (compareLesson(Prefs::compType(Prefs::EnumType::Lesson), expr->lesson(), lessonitems, act_lesson) && type_ok) {
        return true;
    } else {
        return false;
    }
}


QuerySelection KVTQuery::select(KEduVocDocument *doc, int act_lesson, int idx, QueryType type)
{
    kDebug() << "select(KEduVocDocument *doc, int act_lesson, int idx, QueryType type)" << endl;
    QuerySelection random;
    random.resize(doc->lessonCount() + 1);
    for (int i = 0; i < doc->entryCount(); i++)
        doc->entry(i)->setInQuery(false);

    for (int i = 0; i < doc->entryCount(); i++) {
        KEduVocExpression *expr = doc->entry(i);
        if (expr->isActive() && validate(expr, act_lesson, idx, type)) {
            random[expr->lesson()].append(QueryEntry(expr, i));
            expr->setInQuery(true);
        }
    }

    // remove empty lesson elements
    for (int i = (int) random.size()-1; i >= 0; i--)
        if (random[i].size() == 0)
            random.erase(random.begin() + i);

    return random;
}


bool KVTQuery::validate(KEduVocExpression *expr, int act_lesson, int idx, QueryType query_type)
{
    kDebug() << "validate(KEduVocExpression *expr, int act_lesson, int idx, QueryType query_type)" << endl;
    bool type_ok = false;
    if (query_type == KVTQuery::SynonymQuery) {
        type_ok = !expr->synonym(idx).simplified().isEmpty();
    } else if (query_type == KVTQuery::AntonymQuery) {
        type_ok = !expr->antonym(idx).simplified().isEmpty();
    } else if (query_type == KVTQuery::ParaphraseQuery) {
        type_ok = !expr->paraphrase(idx).simplified().isEmpty();
    } else if (query_type == KVTQuery::ExampleQuery) {
        type_ok = !expr->example(idx).simplified().isEmpty();
    }

    if (compareLesson(Prefs::compType(Prefs::EnumType::Lesson), expr->lesson(), lessonitems, act_lesson) && type_ok) {
        return true;
    } else {
        return false;
    }
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
        ;
    }
    return erg;
}


/**
 * Check if the lesson of an expression is in the query lessonitems.
 * @param type
 * @param less the lesson to check
 * @param limit
 * @param current
 * @return
 */
bool KVTQuery::compareLesson(int type, int less, const QList<int> &limit, int current)
{
    Q_UNUSED(type);
    Q_UNUSED(limit);
    Q_UNUSED(current);
    // maybe a bit minimalistic? but should work... the user has only to set the right checkmarks. that should be ok.
    if (lessonitems.contains(less))
        return true;
    return false;
}


void KVTQuery::setTypeNames(const QStringList &names)
{
    userTypes = names;
}

// the next two functions would not be necessary using our great lib ;)
void KVTQuery::setLessonItemStr(const QString & indices)
{
    int pos;
    QString indices_copy = indices;
    lessonitems.clear();
    while ((pos = indices_copy.indexOf(' ')) >= 0) {
        QString s = indices_copy.left(pos);
        indices_copy.remove(0, pos + 1);
        lessonitems.append(s.toInt());
    }
    if (indices_copy.length() != 0) {
        lessonitems.append(indices_copy.toInt());
    }
}

// this is used in profilesdialog.cpp to save the lesson numbers - so it's "1 2 4 66" - ???
QString KVTQuery::lessonItemStr() const
{
    QString s, ret;
    for (int i = 0; i < lessonitems.count(); i++) {
        s.setNum(lessonitems[i]);
        if (i != 0)
            ret += ' ';
        ret += s;
    }
    return ret;
}

