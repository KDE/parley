/***************************************************************************
             profilesdialog.cpp  -  Parley profiles dialog

                             -------------------
    begin                : Fri Apr 29 2005

    copyright            : (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                           (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "profilesdialog.h"

#include <QComboBox>
#include <QPushButton>

#include <klocale.h>
#include <kinputdialog.h>
#include <keduvocdocument.h>
#include <keduvoclesson.h>

#include "groupoptions.h"
#include "presettings.h"
#include "prefs.h"
#include "query-dialogs/kvtquery.h" // for grading constants - would be nice to get rid of this...

SettingsProfile::SettingsProfile()
{}

SettingsProfile::SettingsProfile(const QString &n, const QString &q, const QString &t, const QString &b)
        : name(n), query_set(q), thresh_set(t), block_set(b)
{}

ProfilesDialog::ProfilesDialog(KEduVocDocument *doc, QWidget *parent) : KDialog(parent)
{
    setCaption(i18n("Profiles"));
    setButtons(Close);
    setModal(true);

    m_doc = doc;
    mw = new GroupOptions(this);
    setMainWidget(mw);

    connect(mw->ps_del, SIGNAL(clicked()), this, SLOT(slotDeleteGroup()));
    connect(mw->ps_new, SIGNAL(clicked()), this, SLOT(slotNewGroup()));
    connect(mw->ps_recall, SIGNAL(clicked()), this, SLOT(slotRecallGroup()));
    connect(mw->ps_store, SIGNAL(clicked()), this, SLOT(slotStoreGroup()));

    int ls = Prefs::numPreSetting();
    for (int i = 0 ; i < ls; i++) {
        PreSettings preSettings(QString::number(i));
        preSettings.readConfig();
        profiles.append(SettingsProfile(preSettings.name(), preSettings.query(),
                                        preSettings.threshold(), preSettings.blocking()));
        mw->ps_name->addItem(preSettings.name());
    }

    mw->updateButtons();
}

void ProfilesDialog::slotDeleteGroup()
{
    if (mw->ps_name->count() != 0) {
        int curr = mw->ps_name->currentIndex();
        mw->ps_name->removeItem(curr);
        profiles.removeAt(curr);
        if (curr >= mw->ps_name->count()-1)
            mw->ps_name->setCurrentIndex(mw->ps_name->count()-1);
    }
    saveProfiles();
    mw->updateButtons();
}

void ProfilesDialog::slotNewGroup()
{
    bool ok;
    QString newGroupName = KInputDialog::getText(i18n("Profile Description"),
                           i18n("Enter profile description:"), QString(), &ok);

    if (ok) {
        newGroupName = newGroupName.simplified();
        mw->ps_name->addItem(newGroupName);
        profiles.append(SettingsProfile(newGroupName, "", "", ""));
        modifyProfile(profiles.count()-1);
        mw->ps_name->setCurrentIndex(mw->ps_name->count()-1);
        selectProfile(mw->ps_name->currentIndex());
        mw->updateButtons();
    }
}

void ProfilesDialog::slotRecallGroup()
{
    if (mw->ps_name->count() != 0)
        selectProfile(mw->ps_name->currentIndex());
}


void ProfilesDialog::slotStoreGroup()
{
    if (mw->ps_name->count() != 0)
        modifyProfile(mw->ps_name->currentIndex());
}



static bool extract(QString &line, QString &ret)
{
    int lim = line.indexOf(",");
    if (lim < 0) {
        ret = "";
        line = "";
        return false;
    } else {
        ret = line.left(lim);
        line.remove(0, lim+1);
    }

    return true;
}

void ProfilesDialog::selectProfile(int profile)
{
    if (profile < (int) profiles.count()) {
        QString line = profiles[profile].query_set;
        QString s;
        Prefs::setMaxTimePer(0);
        Prefs::setSwapDirection(false);
        Prefs::setAltLearn(false);
        Prefs::setShowCounter(false);
        //kvq_timeout_t type_to = kvq_notimeout;
        Prefs::setSuggestions(false);
        Prefs::setSplit(false);
        Prefs::setPeriods(true);
        Prefs::setColons(false);
        Prefs::setSemicolons(true);
        Prefs::setCommas(false);
        Prefs::setFields(5);
        Prefs::setShowMore(true);
        Prefs::setIKnow(true);
        if (extract(line, s))
            Prefs::setMaxTimePer(s.toInt());
        if (extract(line, s))
            Prefs::setSwapDirection((bool) s.toInt());
        if (extract(line, s))
            Prefs::setAltLearn((bool) s.toInt());
        if (extract(line, s))
            Prefs::setShowCounter((bool) s.toInt());
        if (extract(line, s))
            Prefs::setQueryTimeout(s.toInt());
        if (extract(line, s))
            Prefs::setSuggestions((bool) s.toInt());
        if (extract(line, s))
            Prefs::setSplit((bool) s.toInt());
        if (extract(line, s))
            Prefs::setPeriods((bool) s.toInt());
        if (extract(line, s))
            Prefs::setColons((bool) s.toInt());
        if (extract(line, s))
            Prefs::setSemicolons((bool) s.toInt());
        if (extract(line, s))
            Prefs::setCommas((bool) s.toInt());
        if (extract(line, s))
            Prefs::setFields(s.toInt());
        if (extract(line, s))
            Prefs::setShowMore((bool) s.toInt());
        if (extract(line, s))
            Prefs::setIKnow((bool) s.toInt());

        line = profiles[profile].thresh_set;

        // Correct me if I'm wrong:
        // There is a line Treshold in the rc file.
        // Threshold=(3 7 11)13,aj,0,0,0,0,0,0,0,1800,0,
        // The lessons in the query are saved in this line.
        // No, I have no clue why.
        // So we first extract the part (3 7 11) and set our lessons accordingly.

        // The following seems to be somewhat downward compatible (?)
        line.simplified();
        QString lessonString = "";
        if (line.length() != 0 && line[0] == '(') { // new style: multiple lessons
            int pos;
            line.remove(0, 1);
            if ((pos = line.indexOf(')')) > 0) {
                lessonString = line.left(pos);
                line.remove(0, pos+1);
            } else {
                lessonString = line;
                line = "";
            }
        } else {
            extract(line, lessonString);
        }

        for ( int i = 0; i < m_doc->lessonCount(); i++) {
            m_doc->lesson(i).setInQuery(false);
        }

        int pos;
        QString indices_copy = lessonString;

        while ((pos = indices_copy.indexOf(' ')) >= 0) {
            QString s = indices_copy.left(pos);
            indices_copy.remove(0, pos + 1);
            m_doc->lesson(s.toInt()).setInQuery(true);
        }

        if (extract(line, s))
            Prefs::setCompType(Prefs::EnumType::Lesson, Prefs::EnumCompType::type(s.toInt()));
        if (extract(line, s))
            Prefs::setTypeItem(s);   // s.toInt()
        if (extract(line, s))
            Prefs::setCompType(Prefs::EnumType::WordType, Prefs::EnumCompType::type(s.toInt())) /*manager.setTypeComp(Prefs::EnumCompType::type(s.toInt()))*/;
        if (extract(line, s))
            Prefs::setGradeItem(s.toInt());
        if (extract(line, s))
            Prefs::setCompType(Prefs::EnumType::Grade, Prefs::EnumCompType::type(s.toInt()));
        if (extract(line, s))
            Prefs::setQueryItem(s.toInt());
        if (extract(line, s))
            Prefs::setCompType(Prefs::EnumType::Query, Prefs::EnumCompType::type(s.toInt()));
        if (extract(line, s))
            Prefs::setBadItem(s.toInt());
        if (extract(line, s))
            Prefs::setCompType(Prefs::EnumType::Bad, Prefs::EnumCompType::type(s.toInt()));
        if (extract(line, s))
            Prefs::setDateItem(s.toInt());
        if (extract(line, s))
            Prefs::setCompType(Prefs::EnumType::Date, Prefs::EnumCompType::type(s.toInt()));

        line = profiles[profile].block_set;
        Prefs::setBlock(false);
        Prefs::setExpire(false);

        if (extract(line, s))
            Prefs::setBlock((bool) s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV1_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV2_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV3_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV4_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV5_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV6_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setBlockItem(KV_LEV7_GRADE, s.toInt());

        if (extract(line, s))
            Prefs::setExpire((bool) s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV1_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV2_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV3_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV4_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV5_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV6_GRADE, s.toInt());
        if (extract(line, s))
            Prefs::setExpireItem(KV_LEV7_GRADE, s.toInt());
    }
    emit profileActivated();
}

void ProfilesDialog::modifyProfile(int profile)
{
    if (profile == -1)
        return;

    if (profile < (int) profiles.count()) {
        QString s;
        QString line;

        s.setNum(Prefs::maxTimePer());
        line += s + ',';
        s.setNum(Prefs::swapDirection());
        line += s + ',';
        s.setNum(Prefs::altLearn());
        line += s + ',';
        s.setNum((int) Prefs::showCounter());
        line += s + ',';
        s.setNum((int) Prefs::queryTimeout() /*getTypeQueryTimeout()*/);
        line += s + ',';
        s.setNum((int) Prefs::suggestions());
        line += s + ',';
        s.setNum((int) Prefs::split());
        line += s + ',';
        s.setNum((int) Prefs::periods());
        line += s + ',';
        s.setNum((int) Prefs::colons());
        line += s + ',';
        s.setNum((int) Prefs::semicolons());
        line += s + ',';
        s.setNum((int) Prefs::commas());
        line += s + ',';
        s.setNum(Prefs::fields());
        line += s + ',';
        s.setNum((int) Prefs::showMore());
        line += s + ',';
        s.setNum((int) Prefs::iKnow());
        line += s + ',';
        profiles[profile].query_set = line;

        ///@todo This is really ugly. Is there no better way to save a list?
        QString tempLessonIndex, lessonString;
        for (int i = 0; i < m_doc->lessonCount(); i++) {
            if ( m_doc->lesson(i).inQuery() ) {
                tempLessonIndex.setNum(i);
                lessonString += tempLessonIndex;
                lessonString += ' ';
            }
        }
        line = '(' + lessonString + ')';
        s.setNum((int) Prefs::compType(Prefs::EnumType::Lesson));

        line += s + ',';
        s = Prefs::typeItem();
        line += s + ',';
        s.setNum((int) Prefs::compType(Prefs::EnumType::WordType) /*manager.typeComp()*/);
        line += s + ',';
        s.setNum(Prefs::gradeItem());
        line += s + ',';
        s.setNum((int) Prefs::compType(Prefs::EnumType::Grade));
        line += s + ',';
        s.setNum(Prefs::queryItem());
        line += s + ',';
        s.setNum((int) Prefs::compType(Prefs::EnumType::Query));
        line += s + ',';
        s.setNum(Prefs::badItem());
        line += s + ',';
        s.setNum((int) Prefs::compType(Prefs::EnumType::Bad));
        line += s + ',';
        s.setNum(Prefs::dateItem());
        line += s + ',';
        s.setNum((int) Prefs::compType(Prefs::EnumType::WordType));
        line += s + ',';
        profiles[profile].thresh_set = line;

        s.setNum(Prefs::block());
        line = s + ',';
        for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
            s.setNum(Prefs::blockItem(i));
            line += s + ',';
        }

        s.setNum(Prefs::expire());
        line += s + ',';
        for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
            s.setNum(Prefs::expireItem(i));
            line += s + ',';
        }
        profiles[profile].block_set = line;
    }
    saveProfiles();
}

void ProfilesDialog::saveProfiles()
{
    Prefs::setNumPreSetting(profiles.count());
    for (int i = 0 ; i < (int) profiles.count(); i++) {
        PreSettings preSettings(QString::number(i));
        preSettings.setName(profiles[i].name);
        preSettings.setQuery(profiles[i].query_set);
        preSettings.setThreshold(profiles[i].thresh_set);
        preSettings.setBlocking(profiles[i].block_set);
        preSettings.writeConfig();
    }
}

#include "profilesdialog.moc"
