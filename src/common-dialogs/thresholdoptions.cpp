/***************************************************************************

                      threshold options for Parley

    -----------------------------------------------------------------------

    begin         : Tue Apr 5 2005

    copyright     : (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "thresholdoptions.h"

#include "query-dialogs/kvtquery.h"
#include "prefs.h"

#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include <KComboBox>
#include <KLocale>
#include <keduvocdocument.h>
#include <keduvocwordtype.h>

static Prefs::EnumCompType::type bad_complist[] =
    {
        Prefs::EnumCompType::DontCare, Prefs::EnumCompType::MoreThan, Prefs::EnumCompType::MoreEqThan,
        Prefs::EnumCompType::EqualTo, Prefs::EnumCompType::NotEqual, Prefs::EnumCompType::LessEqThan,
        Prefs::EnumCompType::LessThan, Prefs::EnumCompType::type(-1)
    };

static Prefs::EnumCompType::type  query_complist[] =
    {
        Prefs::EnumCompType::DontCare, Prefs::EnumCompType::MoreThan, Prefs::EnumCompType::MoreEqThan,
        Prefs::EnumCompType::EqualTo, Prefs::EnumCompType::NotEqual, Prefs::EnumCompType::LessEqThan,
        Prefs::EnumCompType::LessThan, Prefs::EnumCompType::type(-1)
    };

static Prefs::EnumCompType::type grade_complist[] =
    {
        Prefs::EnumCompType::DontCare, Prefs::EnumCompType::WorseThan, Prefs::EnumCompType::WorseEqThan,
        Prefs::EnumCompType::EqualTo, Prefs::EnumCompType::NotEqual, Prefs::EnumCompType::BetterEqThan,
        Prefs::EnumCompType::BetterThan, Prefs::EnumCompType::type(-1)
    };

static Prefs::EnumCompType::type type_complist[] =
    {
        Prefs::EnumCompType::DontCare, Prefs::EnumCompType::EqualTo, Prefs::EnumCompType::NotEqual,
        Prefs::EnumCompType::type(-1)
    };

static Prefs::EnumCompType::type date_complist[] =
    {
        Prefs::EnumCompType::DontCare, Prefs::EnumCompType::Before, Prefs::EnumCompType::Within,
        Prefs::EnumCompType::NotQueried, Prefs::EnumCompType::type(-1)
    };

struct ThreshListRef
{
    const char *text;
    long int num;
};

static ThreshListRef Threshdate_itemlist [] =
    {
        {
            I18N_NOOP("30 Min"),        30*60
        },
        {I18N_NOOP("1 Hour"),    1 * 60*60},
        {I18N_NOOP("2 Hours"),   2 * 60*60},
        {I18N_NOOP("4 Hours"),   4 * 60*60},
        {I18N_NOOP("8 Hours"),   8 * 60*60},
        {I18N_NOOP("12 Hours"), 12 * 60*60},
        {I18N_NOOP("18 Hours"), 18 * 60*60},

        {I18N_NOOP("1 Day"),    1 * 60*60*24},
        {I18N_NOOP("2 Days"),   2 * 60*60*24},
        {I18N_NOOP("3 Days"),   3 * 60*60*24},
        {I18N_NOOP("4 Days"),   4 * 60*60*24},
        {I18N_NOOP("5 Days"),   5 * 60*60*24},
        {I18N_NOOP("6 Days"),   6 * 60*60*24},

        {I18N_NOOP("1 Week"),   1 * 60*60*24*7},
        {I18N_NOOP("2 Weeks"),  2 * 60*60*24*7},
        {I18N_NOOP("3 Weeks"),  3 * 60*60*24*7},
        {I18N_NOOP("4 Weeks"),  4 * 60*60*24*7},

        {I18N_NOOP("1 Month"),  1 * 60*60*24*30},
        {I18N_NOOP("2 Months"), 2 * 60*60*24*30},
        {I18N_NOOP("3 Months"), 3 * 60*60*24*30},
        {I18N_NOOP("4 Months"), 4 * 60*60*24*30},
        {I18N_NOOP("5 Months"), 5 * 60*60*24*30},
        {I18N_NOOP("6 Months"), 6 * 60*60*24*30},
        {0, 0}
    };


ThresholdOptions::ThresholdOptions(KEduVocDocument* doc, KVTQuery* m, QWidget* parent) : QWidget(parent)
{
    setupUi(this);

    connect(typecomp,    SIGNAL(activated(int)), SLOT(slotSetTypeComp(int)));
    connect(typelist,    SIGNAL(activated(int)), SLOT(slotComboActivated(int)));
    connect(gradecomp,   SIGNAL(activated(int)), SLOT(slotSetGradeComp(int)));
    connect(gradelist,   SIGNAL(activated(int)), SLOT(slotComboActivated(int)));
    connect(querycomp,   SIGNAL(activated(int)), SLOT(slotSetQueryComp(int)));
    connect(querylist,   SIGNAL(activated(int)), SLOT(slotComboActivated(int)));
    connect(badcomp,     SIGNAL(activated(int)), SLOT(slotSetBadComp(int)));
    connect(badlist,     SIGNAL(activated(int)), SLOT(slotComboActivated(int)));
    connect(datecomp,    SIGNAL(activated(int)), SLOT(slotSetDateComp(int)));
    connect(datelist,    SIGNAL(activated(int)), SLOT(slotComboActivated(int)));

    m_doc = doc;
    m_queryManager = m;
    fillWidgets();
    updateWidgets();
}

void ThresholdOptions::fillWidgets()
{
    Prefs::EnumCompType::type *ct;
    ThreshListRef *ref;

    // limits and thresholds (the five comboboxes on the right)
    gradelist->clear();
    for (int i = 1; i <= KV_MAX_GRADE; i++)
        gradelist->addItem(m_queryManager->gradeStr(i));

    typelist->clear();
    typelist->addItems(m_doc->wordTypes()->typeNameList());

    badlist->clear();
    querylist->clear();
    for (int i = 0; i <= 15; i++) {
        badlist->addItem(QString::number(i));
        querylist->addItem(QString::number(i));
    }

    ref = Threshdate_itemlist;
    datelist->clear();
    while (ref->text != 0) {
        datelist->addItem(i18n(ref->text));
        ref++;
    }

    //compare-functions (the five comboboxes on the left)
    ct = type_complist;
    typecomp->clear();
    while (*ct != Prefs::EnumCompType::type(-1)) {
        typecomp->addItem(m_queryManager->compStr(*ct));
        ct++;
    }

    ct = query_complist;
    querycomp->clear();
    while (*ct != Prefs::EnumCompType::type(-1)) {
        querycomp->addItem(m_queryManager->compStr(*ct));
        ct++;
    }

    ct = bad_complist;
    badcomp->clear();
    while (*ct != Prefs::EnumCompType::type(-1)) {
        badcomp->addItem(m_queryManager->compStr(*ct));
        ct++;
    }

    ct = grade_complist;
    gradecomp->clear();
    while (*ct != Prefs::EnumCompType::type(-1)) {
        gradecomp->addItem(m_queryManager->compStr(*ct));
        ct++;
    }

    ct = date_complist;
    datecomp->clear();
    while (*ct != Prefs::EnumCompType::type(-1)) {
        datecomp->addItem(m_queryManager->compStr(*ct));
        ct++;
    }
}

void ThresholdOptions::updateWidgets()
{
    int index = 0;
    Prefs::EnumCompType::type *ct;
    ThreshListRef *ref;

    gradelist->setCurrentIndex(Prefs::gradeItem() - 1);

    typelist->setCurrentIndex(typelist->findText(Prefs::typeItem()));

    badlist->setCurrentIndex(Prefs::badItem());
    querylist->setCurrentIndex(Prefs::queryItem());

    ref = Threshdate_itemlist;
    while (ref->text != 0) {
        if (Prefs::dateItem() == ref->num)
            index = ref - Threshdate_itemlist;
        ref++;
    }
    datelist->setCurrentIndex(index);

    //compare-functions (the five comboboxes on the left)
    ct = type_complist;
    while (*ct != Prefs::compType(Prefs::EnumType::WordType))
        ct++;
    typecomp->setCurrentIndex(ct - type_complist);
    typelist->setEnabled(ct - type_complist != 0); // don`t care == 0

    ct = query_complist;
    while (*ct != Prefs::compType(Prefs::EnumType::Query))
        ct++;
    querycomp->setCurrentIndex(ct - query_complist);
    querylist->setEnabled(ct - query_complist != 0); // don`t care == 0

    ct = bad_complist;
    while (*ct != Prefs::compType(Prefs::EnumType::Bad))
        ct++;
    badcomp->setCurrentIndex(ct - bad_complist);
    badlist->setEnabled(ct - bad_complist != 0); // don`t care == 0

    ct = grade_complist;
    while (*ct != Prefs::compType(Prefs::EnumType::Grade))
        ct++;
    gradecomp->setCurrentIndex(ct - grade_complist);
    gradelist->setEnabled(ct - grade_complist != 0); // don`t care == 0

    ct = date_complist;
    while (*ct != Prefs::compType(Prefs::EnumType::Date))
        ct++;
    datecomp->setCurrentIndex(ct - date_complist);
    if (date_complist[ct - date_complist] == Prefs::EnumCompType::Before ||
            date_complist[ct - date_complist] == Prefs::EnumCompType::Within)
        datelist->setEnabled(true);
    else
        datelist->setEnabled(false);
}


void ThresholdOptions::slotComboActivated(int)
{
    emit widgetModified();
}


void ThresholdOptions::slotSetGradeComp(int i)
{
    gradelist->setEnabled(i != 0); // don`t care == 0
    emit widgetModified();
}

void ThresholdOptions::slotSetBadComp(int i)
{
    badlist->setEnabled(i != 0); // don`t care == 0
    emit widgetModified();
}

void ThresholdOptions::slotSetQueryComp(int i)
{
    querylist->setEnabled(i != 0); // don`t care == 0
    emit widgetModified();
}

void ThresholdOptions::slotSetTypeComp(int i)
{
    typelist->setEnabled(i != 0); // don`t care == 0
    emit widgetModified();
}


void ThresholdOptions::slotSetDateComp(int i)
{
    if (date_complist[i] == Prefs::EnumCompType::Before || date_complist[i] == Prefs::EnumCompType::Within)
        datelist->setEnabled(true);
    else
        datelist->setEnabled(false);
    emit widgetModified();
}

void ThresholdOptions::slotBlockExpire(bool block, bool expire)
{
    bool enable = !(block | expire);

    gradecomp->setEnabled(enable);
    gradelist->setEnabled(enable && gradecomp->currentIndex() != 0);
    label_grade->setEnabled(enable);

    datecomp->setEnabled(enable);
    datelist->setEnabled(enable && datecomp->currentIndex() != 0);
    label_qdate->setEnabled(enable);

    querycomp->setEnabled(enable);
    querylist->setEnabled(enable && querycomp->currentIndex() != 0);
    label_qcount->setEnabled(enable);

    badcomp->setEnabled(enable);
    badlist->setEnabled(enable && badcomp->currentIndex() != 0);
    label_bcount->setEnabled(enable);
}

bool ThresholdOptions::isDefault()
{
    if (typecomp->currentIndex() < 0
            || gradecomp->currentIndex() < 0 || querycomp->currentIndex() < 0
            || badcomp->currentIndex() < 0 || datecomp->currentIndex() < 0)
        return false;

    return type_complist[typecomp->currentIndex()]   == Prefs::EnumCompType::DontCare &&
           grade_complist[gradecomp->currentIndex()] == Prefs::EnumCompType::DontCare &&
           date_complist[datecomp->currentIndex()]   == Prefs::EnumCompType::DontCare &&
           query_complist[querycomp->currentIndex()] == Prefs::EnumCompType::DontCare &&
           bad_complist[badcomp->currentIndex()]     == Prefs::EnumCompType::DontCare;
}

void ThresholdOptions::updateSettings()
{
    if (typecomp->currentIndex() >= 0)
        Prefs::setCompType(Prefs::EnumType::WordType, type_complist[typecomp->currentIndex()]);
    if (typelist->currentIndex() >= 0)
        Prefs::setTypeItem(typelist->currentText());
    if (gradecomp->currentIndex() >= 0)
        Prefs::setCompType(Prefs::EnumType::Grade, grade_complist[gradecomp->currentIndex()]);
    Prefs::setGradeItem(gradelist->currentIndex() + 1);
    if (querycomp->currentIndex() >= 0)
        Prefs::setCompType(Prefs::EnumType::Query, query_complist[querycomp->currentIndex()]);
    Prefs::setQueryItem(querylist->currentIndex());
    if (badcomp->currentIndex() >= 0)
        Prefs::setCompType(Prefs::EnumType::Bad, bad_complist[badcomp->currentIndex()]);
    Prefs::setBadItem(badlist->currentIndex());
    if (datecomp->currentIndex() >= 0)
        Prefs::setCompType(Prefs::EnumType::Date, date_complist[datecomp->currentIndex()]);
    if (datelist->currentIndex() >= 0)
        Prefs::setDateItem(Threshdate_itemlist[datelist->currentIndex()].num);
}

bool ThresholdOptions::hasChanged()
{
    if (gradecomp->currentIndex() < 0 ||
            querycomp->currentIndex() < 0 ||
            badcomp->currentIndex()   < 0 ||
            datecomp->currentIndex()  < 0 ||
            typelist->currentIndex()  < 0 ||
            datelist->currentIndex()  < 0)
        return true;

    return type_complist[typecomp->currentIndex()]            != Prefs::compType(Prefs::EnumType::WordType) ||
           grade_complist[gradecomp->currentIndex()]          != Prefs::compType(Prefs::EnumType::Grade)    ||
           query_complist[querycomp->currentIndex()]          != Prefs::compType(Prefs::EnumType::Query)    ||
           bad_complist[badcomp->currentIndex()]              != Prefs::compType(Prefs::EnumType::Bad)      ||
           date_complist[datecomp->currentIndex()]            != Prefs::compType(Prefs::EnumType::Date)     ||
           typelist->currentText() != Prefs::typeItem() ||
           gradelist->currentIndex() + 1                      != Prefs::gradeItem()                         ||
           querylist->currentIndex()                          != Prefs::queryItem()                         ||
           badlist->currentIndex()                            != Prefs::badItem()                           ||
           Threshdate_itemlist[datelist->currentIndex()].num  != Prefs::dateItem();
}

#include "thresholdoptions.moc"
