/***************************************************************************

                             kvtnewdocumentwizardchoicelanguageother

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

#include "kvtnewdocumentwizardchoicelanguageother.h"

#include <QVBoxLayout>

#include <KLocale>

#include "kvtnewdocumentwizard.h"

KVTNewDocumentWizardChoiceLanguageOther::KVTNewDocumentWizardChoiceLanguageOther(bool showFileOpen, QWidget *parent)
 : QWizardPage(parent)
{

    setTitle(i18n("Languages or other"));
//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    m_label = new QLabel(i18n("Please select whether you will "
                            "be practicing languages or other "
                            "things (anything else).\n"
                            ));
    /// @todo eventually option to open a file, if this is the first time kvoc is started???
    m_label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_label);

    //layout->addWidget(new QLabel(i18n("Well it's a vocabulary trainer so languages are what you like to practice:")));
    m_languageRadioButton = new QRadioButton;
    m_languageRadioButton->setText(i18n("Languages"));
    m_languageRadioButton->setChecked(true);
    registerField("choiceLanguageOther.language", m_languageRadioButton);
    layout->addWidget(m_languageRadioButton);

    m_otherRadioButton = new QRadioButton;
    m_otherRadioButton->setText(i18n("Other"));
    registerField("choiceLanguageOther.other", m_otherRadioButton);
    layout->addWidget(m_otherRadioButton);
    layout->addWidget(new QLabel(i18n("(This can be anything. You simply name it.)")));

    if ( showFileOpen ) {
        m_fileOpenRadioButton = new QRadioButton;
        m_fileOpenRadioButton->setText(i18n("Open an Existing Document"));
        layout->addWidget(m_fileOpenRadioButton);
        layout->addWidget(new QLabel(i18n("(You already have a KVocTrain document.)")));
    }
    setLayout(layout);
}


#include "kvtnewdocumentwizardchoicelanguageother.moc"

