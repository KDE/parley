//
// C++ Implementation: kvtnewdocumentwizard
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <KLocale>
#include <KComboBox>
#include <kvtlanguages.h>

#include "kvtnewdocumentwizard.h"

KVTNewDocumentWizard::KVTNewDocumentWizard(Options options, QWidget *parent)
 : QWizard(parent)
{
    if ( options == ShowFileOpen ) {
        m_showFileOpen = true;
    } else {
        m_showFileOpen = false;
    }

    resize(600,600);
    m_firstLanguagePage = new KVTNewDocumentWizardLanguagePage(true, this);
    m_secondLanguagePage = new KVTNewDocumentWizardLanguagePage(false, this);

    setPage(WizardIntroPage, new KVTNewDocumentWizardIntroPage);
    setPage(WizardChoiceLanguageOtherPage, new KVTNewDocumentWizardChoiceLanguageOther ( m_showFileOpen ) );

    setPage(WizardFirstLanguagePage, m_firstLanguagePage);
    setPage(WizardSecondLanguagePage, m_secondLanguagePage);

    setPage(WizardOtherPage, new KVTNewDocumentWizardOtherPage);
    setPage(WizardFinalPage, new KVTNewDocumentWizardFinalPage);

    setStartId(WizardIntroPage);

    //setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    //setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(i18n("Create a new KVocTrain document"));
}


void KVTNewDocumentWizard::initializePage(int id)
{
    QWizard::initializePage(id);
}


int KVTNewDocumentWizard::nextId() const
{
    switch (currentId()) {
    case WizardIntroPage:
        return WizardChoiceLanguageOtherPage;
    case WizardChoiceLanguageOtherPage:
        if (field("choiceLanguageOther.language").toBool()) {
            return WizardFirstLanguagePage;
        } else if (field("choiceLanguageOther.other").toBool()) {
            return WizardOtherPage;
        } else {
            return WizardFileOpenPage;
        }
    case WizardFirstLanguagePage:
        return WizardSecondLanguagePage;
    case WizardSecondLanguagePage:
        return WizardFinalPage;
    case WizardOtherPage:
        return WizardFinalPage;
    case WizardFinalPage:
    default:
        return -1;
    }
}

QList<WizardIdentifier> KVTNewDocumentWizard::identifiers()
{
    QList<WizardIdentifier> ident;
    if ( hasVisitedPage(WizardFirstLanguagePage) ) {
        kDebug() << "adding first page" << endl;
        ident.append(WizardIdentifier(true,
            field("firstLanguagePage.language").toString(),
            field("firstLanguagePage.languageShort").toString() ));
    }
    if ( hasVisitedPage(WizardSecondLanguagePage) ) {
        kDebug() << "adding second page" << endl;
        ident.append(WizardIdentifier(true,
            field("secondLanguagePage.language").toString(),
            field("secondLanguagePage.languageShort").toString() ));
    }
    if ( hasVisitedPage(WizardOtherPage) ) {
        ident.append(WizardIdentifier(false,
            field("otherPage.firstIdentifier").toString(),
            field("otherPage.firstIdentifier").toString() ));
        ident.append(WizardIdentifier(false,
            field("otherPage.secondIdentifier").toString(),
            field("otherPage.secondIdentifier").toString() ));
    }
    return ident;
}


#include "kvtnewdocumentwizard.moc"
