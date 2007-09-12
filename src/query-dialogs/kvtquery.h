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

#ifndef kvtquery_included
#define kvtquery_included

#include <QVector>

#include <keduvocgrade.h>
#include <keduvocexpression.h>
#include "prefs.h"

class KEduVocDocument;
class KEduVocExpression;
class Prefs;
class EnumCompType;

// internal types, indented are subtypes

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

class TypeRelation
{
public:
    TypeRelation(const QString & _short, const QString & _long) : shortId(_short), longId(_long)
    {}

    QString shortStr() const
    {
        return shortId;
    }
    QString longStr()  const
    {
        return longId;
    }

protected:
    QString  shortId;
    QString longId;
};


class QueryEntry
{
public:
    QueryEntry(KEduVocExpression *_exp, int _nr) : exp(_exp), m_index(_nr)
    {
        m_statisticTotalCount = 0;
        m_statisticBadCount = 0;
        m_statisticTimeout = 0;
        m_statisticSkipUnknown = 0;
        m_statisticSkipKnown = 0;
    }

    KEduVocExpression *exp;
    int m_index;

    // these are for the CURRENT query only, so we can display statistics.
    int m_statisticTotalCount;
    int m_statisticBadCount;
    int m_statisticTimeout;
    int m_statisticSkipUnknown;
    int m_statisticSkipKnown;
};

typedef QList<QueryEntry> QueryEntryList;
// Why the hey do use QVector here?
typedef QVector<QueryEntryList> QuerySelection;


class KVTQuery
{
public:

    KVTQuery();

    /// vector of list of entries
    QuerySelection queryEntries();

    static QString compStr(Prefs::EnumCompType::type type);
    static QString gradeStr(int i);

    /**
     * Set the doc to be used for the query
     * @param doc
     */
    void setDocument(KEduVocDocument *doc);
    /**
     * Set the index of the from language. Note that queries using only one language need to setToTranslation as well - to the same index!
     * @param indexFrom
     */
    void setFromTranslation(int indexFrom);
    /**
     * Set the index of the to language. Note that queries using only one language need to setFromTranslation as well - to the same index!
     * @param indexTo
     */
    void setToTranslation(int indexTo);
    /**
     * Set the query type
     * @param queryType
     */
    void setQueryType(int queryType);


    /**
     * DO NOT USE OUTSIDE THIS CLASS! This should become private as soon as it is no longer needed by
     * void QueryManager::slotQueryExpressionResult(QueryDlgBase::Result res)
     * (determine if Prefs::swapDirection() is ok)... DO NOT USE OUTSIDE THIS CLASS!
     * Check if @p expr is valid for the current query settings.
     * @param expr
     * @return
     */
    bool validate(KEduVocExpression *expr);
private:
    bool validateWithSettings(KEduVocExpression *expr);
    bool compareBlocking(int grade, const QDateTime &limit, bool use_it);
    bool compareExpiring(int grade, const QDateTime &limit, bool use_it);
    bool compareDate(int type, const QDateTime &qd);
    bool compareQuery(int type, int qgrade, int limit);
    bool compareBad(int type, int bcount, int limit);
    bool compareGrade(int type, grade_t qgrade, grade_t limit);
    bool compareType(int type, const QString & exprtype, const QString & limit);

    KEduVocDocument *m_doc;
    int m_indexFrom;
    int m_indexTo;
    int m_testType;
};

#endif // kvtquery_included
