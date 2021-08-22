/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "languageproperties.h"

#include "languagepropertiespage.h"
#include "languagesettings.h"

#include <KEduVocDocument>

#include <KMessageBox>
#include <QLabel>
#include <KLocalizedString>
#include <QToolTip>
#include <QDialogButtonBox>

LanguageProperties::LanguageProperties(KEduVocDocument* doc, QWidget * parent)
    : KPageDialog(parent), m_doc(doc)
{
    setWindowTitle(i18n("Edit Languages"));
    setFaceType(List);
    setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QAbstractButton * addLangButton(
        buttonBox()->addButton(i18n("Add language"), QDialogButtonBox::ActionRole) );
    addLangButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));

    QAbstractButton * removeLangButton(
        buttonBox()->addButton(i18n("Remove language"), QDialogButtonBox::ActionRole) );
    removeLangButton->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")) );

    connect(addLangButton, &QAbstractButton::clicked, this, &LanguageProperties::slotAppendIdentifier);
    connect(removeLangButton, &QAbstractButton::clicked, this, &LanguageProperties::slotDeleteIdentifier);

    for (int i = 0; i < m_doc->identifierCount(); i++) {
        createPage(i);
    }
}

KPageWidgetItem*  LanguageProperties::createPage(int i)
{
    LanguagePropertiesPage* page = new LanguagePropertiesPage(m_doc, i);

    QString name = i18n("New Language");

    // check if this language already exists in the doc
    if (m_doc->identifierCount() <= i) {
      m_doc->appendIdentifier();
    }
    if (m_doc->identifierCount() > i) {
        name = m_doc->identifier(i).name();
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.load();
    }

    KPageWidgetItem* editPage = new KPageWidgetItem(page, name);
    editPage->setHeader(i18nc("Edit language properties", "Properties for %1", name));

    m_pages.append(editPage);
    addPage(editPage);

    editPage->setIcon(QIcon::fromTheme(QStringLiteral("set-language")));

    connect(page->identifierNameLineEdit, &QLineEdit::textChanged, this, &LanguageProperties::pageNameChanged);
    connect(this, &LanguageProperties::accepted, page, &LanguagePropertiesPage::accept);

    return editPage;
}

void LanguageProperties::accept()
{
    KEduVocDocument *doc = m_doc;
    int deleted = 0;

    for (int index = 0; index < m_pages.count(); index++) {
        if (m_pages.value(index)->isEnabled()) {
            if (index - deleted >= doc->identifierCount()) {
                // add a language
                doc->appendIdentifier();
            }
            LanguagePropertiesPage * page = static_cast<LanguagePropertiesPage *>(m_pages.value(index)->widget());
            page->setLanguageIdentifierIndex(index - deleted);
        } else {
            // page is disabled, delete the language
            if (index < doc->identifierCount()) {
                if (KMessageBox::warningYesNo(this, i18n("Really delete language: %1?", doc->identifier(index - deleted).name()), i18n("Remove Language")) == KMessageBox::Yes) {
                    doc->removeIdentifier(index - deleted);
                    deleted++;
                }
            }
        }
    }

    KPageDialog::accept();
}


void LanguageProperties::slotAppendIdentifier()
{
    // if a page that was "removed" is selected, simply enable it again to not delete it.
    if (currentPage() && !currentPage()->isEnabled()) {
        currentPage()->setEnabled(true);
        return;
    }

    int i = m_pages.count();
    KPageWidgetItem* newPage = createPage(i);
    setCurrentPage(newPage);
}

void LanguageProperties::slotDeleteIdentifier()
{
    currentPage()->setEnabled(false);
}

void LanguageProperties::pageNameChanged(const QString & newName)
{
    currentPage()->setName(newName);
}
