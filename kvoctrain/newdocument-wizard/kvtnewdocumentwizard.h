//
// C++ Interface: kvtnewdocumentwizard
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTNEWDOCUMENTWIZARD_H
#define KVTNEWDOCUMENTWIZARD_H


#include "kvtnewdocumentwizardintropage.h"
#include "kvtnewdocumentwizardchoicelanguageother.h"
#include "kvtnewdocumentwizardlanguagepage.h"
#include "kvtnewdocumentwizardotherpage.h"
#include "kvtnewdocumentwizardfinalpage.h"

class WizardIdentifier
{
public:
    WizardIdentifier( bool isLanguage, QString identifier, QString identifierShort)
    {
        m_isLanguage = isLanguage;
        m_identifierShort = identifierShort;
        m_identifier = identifier;
    }
    
    const bool isLanguage() {return m_isLanguage; }
    const QString identifier() {return m_identifier; }
    const QString identifierShort() {return m_identifierShort; }
private:
    bool m_isLanguage;
    QString m_identifierShort;
    QString m_identifier;
};

/**
A wizard to assist in the creation of kvtml documents with KVocTrain. Helps to set up the languages of the document.

	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTNewDocumentWizard : public QWizard
{
Q_OBJECT
public:
    /// The pages of this wizard
    enum NewDocumentWizardPages{
        WizardIntroPage,
        WizardChoiceLanguageOtherPage,
        WizardFirstLanguagePage,
        WizardSecondLanguagePage,
        WizardOtherPage,
        WizardFileOpenPage,
        WizardFinalPage
    };

    enum Options{
        NoFileOpen,
        ShowFileOpen
    };

    KVTNewDocumentWizard(Options options, QWidget *parent = 0);

    void initializePage(int id);
    int nextId() const;
    
    QList<WizardIdentifier>  identifiers();
private:
    KVTNewDocumentWizardLanguagePage *m_firstLanguagePage;
    KVTNewDocumentWizardLanguagePage *m_secondLanguagePage;
    bool m_showFileOpen;
signals:
    void setLanguage(int column, int languageIndex);
};

#endif
