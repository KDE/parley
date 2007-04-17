//
// C++ Implementation: kvtnewdocumentwizardlanguagepage
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <QWizardPage>
#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

#include <KLocale>
#include <KDebug>

#include "kvtnewdocumentwizardotherpage.h"

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
