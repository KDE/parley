/***************************************************************************

                             kvtnewdocumentwizard

    -----------------------------------------------------------------------

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

#ifndef KVTNEWDOCUMENTWIZARD_H
#define KVTNEWDOCUMENTWIZARD_H


#include "kvtnewdocumentwizardintropage.h"
#include "authortitlepage.h"
#include "firstidentifierlanguagepage.h"
#include "secondidentifierlanguagepage.h"

#include <keduvocdocument.h>

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
A wizard to assist in the creation of kvtml documents with Parley. Helps to set up the languages of the document.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class KVTNewDocumentWizard : public QWizard
{
Q_OBJECT
public:
    /// The pages of this wizard
    enum NewDocumentWizardPages{
        Intro,
        AuthorTitle,
        FirstIdentifierLanguage,
        SecondIdentifierLanguage
    };

    KVTNewDocumentWizard(KEduVocDocument* doc, QWidget *parent);

    int nextId() const;

    QList<WizardIdentifier>  identifiers();
    void accept();

signals:
    void setLanguage(int column, int languageIndex);

private:

    KEduVocDocument* m_doc;
};

#endif
