/***************************************************************************

                       query parts of Parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

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

#include "practicemanager.h"

#include "parley.h"

#include "writtenpracticedialog.h"
#include "MCQueryDlg.h"
#include "AdjQueryDlg.h"
#include "VerbQueryDlg.h"
#include "ArtQueryDlg.h"
#include "SimpleQueryDlg.h"
#include "mixedletterpracticedialog.h"

#include "testentrymanager.h"
#include "prefs.h"

#include <keduvocwordtype.h>
#include <keduvoclesson.h>
#include <keduvocdocument.h>

#include <KDebug>
#include <KLocale>

PracticeManager::PracticeManager(ParleyApp *app, KEduVocDocument *doc)
{
    m_app = app;
    m_doc = doc;

    m_testDialog = 0;
}


void PracticeManager::startPractice()
{
    m_lastTestType = QString();
    m_testType = Prefs::testType();

    m_app->removeEntryDlg();

    m_app->hide();
    createDialog();
    connect(m_testDialog, SIGNAL(accepted()), SLOT(stopPractice()));
}


void PracticeManager::createDialog()
{
    if ( m_testDialog ) {
        m_testDialog->deleteLater();
        m_testDialog = 0;
    }

    QString specialWordType;

    switch ( m_testType ) {
    case Prefs::EnumTestType::WrittenTest:
        m_testDialog = new WrittenPracticeDialog(m_doc, m_app);
        break;
    case Prefs::EnumTestType::MultipleChoiceTest:
        m_testDialog = new MCQueryDlg(m_doc, m_app);
        break;
    case Prefs::EnumTestType::MixedLettersTest:
        m_testDialog = new MixedLetterPracticeDialog(m_doc, m_app);
        break;
    case Prefs::EnumTestType::ArticleTest:
        m_testDialog = new ArtQueryDlg(m_doc, m_app);
        break;
    case Prefs::EnumTestType::ComparisonTest:
        m_testDialog = new AdjQueryDlg(m_doc, m_app);
        break;
    case Prefs::EnumTestType::ConjugationTest:
        m_testDialog = new VerbQueryDlg(m_doc, m_app);
        break;
    // tests using the simple dialog
    case Prefs::EnumTestType::SynonymTest:
    case Prefs::EnumTestType::AntonymTest:
    case Prefs::EnumTestType::ExampleTest:
    case Prefs::EnumTestType::ParaphraseTest:
        m_testDialog = new SimpleQueryDlg(m_doc, m_app);
        break;
        break;
    default:
        kError() << "PracticeManager::startQuery: unknown type\n";
        stopPractice();
        return;
    }

    if ( !m_testDialog ) {
        kError() << "Test dialog was not created!";
        return;
    }
    m_testDialog->show();
}


void PracticeManager::stopPractice()
{
kDebug() << "stopPractice";
    if (m_testDialog) {
        m_testDialog->deleteLater();
        m_testDialog = 0;
    }

    m_app->show();
}

PracticeManager::~PracticeManager()
{
    if ( m_testDialog ) {
        m_testDialog->deleteLater();
    }
    deleteLater();
}


#include "practicemanager.moc"

