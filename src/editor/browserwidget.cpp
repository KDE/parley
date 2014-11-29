/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "browserwidget.h"

#include <keduvocexpression.h>
#include <KHTMLPart>

#include <QDragEnterEvent>

using namespace Editor;

BrowserWidget::BrowserWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;
    m_entry = 0;
    m_currentProvider = 0; ///@todo kconfig?

    m_htmlPart = new KHTMLPart(widget);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    widget->setLayout(layout);
    layout->addWidget(m_htmlPart->widget());

    connect(m_htmlPart->browserExtension(), SIGNAL(openUrlRequest(const QUrl&, const KParts::OpenUrlArguments&, const KParts::BrowserArguments&)),
            this, SLOT(openUrl(const QUrl&)));

    connect(showCurrentButton, &QPushButton::clicked, this, &BrowserWidget::showCurrentTranslation);

    setupProviders();
    connect(providerComboBox, static_cast<void (KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &BrowserWidget::providerChanged);

    m_htmlPart->setJavaEnabled(false);
    m_htmlPart->setPluginsEnabled(false);
    m_htmlPart->setJScriptEnabled(true);
    m_htmlPart->setMetaRefreshEnabled(true);
    m_htmlPart->setDNDEnabled(true);
}

void BrowserWidget::setupProviders()
{
    providerComboBox->clear();

    DictionaryProvider provider;
    provider.name = "De-Es Beolingus";
    provider.url = "http://beolingus.org/dings.cgi?service=dees&query=\\{@}";
    provider.languages << "de" << "es";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "De-En Beolingus";
    provider.url = "http://beolingus.org/dings.cgi?query=\\{@}";
    provider.languages << "de" << "en";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "De-En Leo";
    provider.url = "http://dict.leo.org/?search=\\{@}";
    provider.languages << "de" << "en";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "De-Es Leo";
    provider.url = "http://dict.leo.org/?lp=esde&search=\\{@}";
    provider.languages << "de" << "es";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "De-Fr Leo";
    provider.url = "http://dict.leo.org/?lp=frde&search=\\{@}";
    provider.languages << "de" << "fr";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "De-Es Pons";
    provider.url = "http://www.ponsline.de/cgi-bin/wb/w.pl?von=pons.de&Richtung=Spanisch&Begriff=\\{@}";
    provider.languages << "de" << "es";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "Ru-En Multitran";
    provider.url = "http://multitran.ru/c/m.exe?SHL=1&CL=1&l1=1&s=\\{@}";
    provider.languages << "ru" << "en";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "Ru-De Multitran";
    provider.url = "http://multitran.ru/c/m.exe?SHL=1&CL=1&l1=3&s=\\{@}";
    provider.languages << "ru" << "de";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "Ru-Fr Multitran";
    provider.url = "http://multitran.ru/c/m.exe?SHL=1&CL=1&l1=4&s=\\{@}";
    provider.languages << "ru" << "fr";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = "Heinzelnisse";
    provider.url = "http://heinzelnisse.info/dict?setOptions=true&searchItem=\\{@}&dictDeNoSearch=on&dictNoDeSearch=on&dictExactSearch=on&dictPhoneticSearch=on&wikiSearch=on&dictNynorskSearch=on&dictBokmaalSearch=checked&forumKeywordSearch=on&suggestion=on";
    provider.languages << "no" << "de";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

}

void BrowserWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_currentTranslation = translation;
    m_entry = entry;

    if (followTableCheckBox->isChecked()) {
        showCurrentTranslation();
    }
}

void BrowserWidget::showCurrentTranslation()
{
    if (m_entry) {
        if (m_entry->translation(m_currentTranslation)) {
            QString text = m_entry->translation(m_currentTranslation)->text();
            if (!text.isEmpty()) {
                m_htmlPart->openUrl(QUrl(QString(m_providers.value(m_currentProvider).url.replace("\\{@}", text))));
            }
        }
    }
}

void BrowserWidget::openUrl(const QUrl & targetUrl)
{
    m_htmlPart->openUrl(targetUrl);
}

void BrowserWidget::providerChanged(int provider)
{
    m_currentProvider = provider;
    showCurrentTranslation();
}
