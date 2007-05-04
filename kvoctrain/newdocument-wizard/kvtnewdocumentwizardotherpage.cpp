/***************************************************************************

                             kvtnewdocumentwizardotherpage

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

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

#include "kvtnewdocumentwizardotherpage.h"

#include <QWizardPage>
#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

#include <KLocale>
#include <KDebug>

KVTNewDocumentWizardOtherPage::KVTNewDocumentWizardOtherPage(QWidget *parent)
 : QWizardPage(parent)
{
    setTitle(i18n("Your categories"));
    setSubTitle(i18n("Now select titles for what you will be practicing."));

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(new QLabel(i18n("This could be something like \"Difficult Word\" and \"Meaning\".")));

    m_labelFirst = new QLabel(i18n("First Category: "));
    m_labelFirst->setWordWrap(true);
    m_firstIdentifierLineEdit = new KLineEdit;
    registerField("otherPage.firstIdentifier", m_firstIdentifierLineEdit);
    m_labelFirst->setBuddy(m_firstIdentifierLineEdit);
    layout->addWidget(m_labelFirst);
    layout->addWidget(m_firstIdentifierLineEdit);

    m_labelSecond = new QLabel(i18n("Second Category: "));
    m_labelSecond->setWordWrap(true);
    m_secondIdentifierLineEdit = new KLineEdit;
    registerField("otherPage.secondIdentifier", m_secondIdentifierLineEdit);
    m_labelSecond->setBuddy(m_secondIdentifierLineEdit);
    layout->addWidget(m_labelSecond);
    layout->addWidget(m_secondIdentifierLineEdit);

    QLabel *labelHint = new QLabel(i18n("Please note that you can add more categories later. Use Vocabulary->AppendLanguage to do that."));
    labelHint->setWordWrap(true);
    layout->addWidget(labelHint);

    setLayout(layout);
}

#include "kvtnewdocumentwizardotherpage.moc"
