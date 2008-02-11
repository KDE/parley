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
#include <kdebug.h>
#include <KHTMLPart>

#include <QDragEnterEvent>


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

    connect(m_htmlPart->browserExtension(),
    SIGNAL(openUrlRequest(const KUrl&, const KParts::OpenUrlArguments&, const KParts::BrowserArguments&)),
    this, SLOT(openUrl(const KUrl&)));

    connect(showCurrentButton, SIGNAL(clicked()), SLOT(showCurrentTranslation()));

    setupProviders();
    connect(providerComboBox, SIGNAL(currentIndexChanged(int)), SLOT(providerChanged(int)));

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
    provider.name="De-Es Beolingus";
    provider.url="http://beolingus.org/dings.cgi?service=dees&query=\\{@}";
    provider.languages << "de" << "es";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name="De-En Beolingus";
    provider.url="http://beolingus.org/dings.cgi?query=\\{@}";
    provider.languages << "de" << "en";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name="De-En Leo";
    provider.url="http://dict.leo.org/?search=\\{@}";
    provider.languages << "de" << "en";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name="De-Es Leo";
    provider.url="http://dict.leo.org/?lp=esde&search=\\{@}";
    provider.languages << "de" << "es";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name="De-Fr Leo";
    provider.url="http://dict.leo.org/?lp=frde&search=\\{@}";
    provider.languages << "de" << "fr";
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name="De-Es Pons";
    provider.url="http://www.ponsline.de/cgi-bin/wb/w.pl?von=pons.de&Richtung=Spanisch&Begriff=\\{@}";
    provider.languages << "de" << "es";
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
            m_htmlPart->openUrl(KUrl(QString(m_providers.value(m_currentProvider).url.replace("\\{@}", text))));
        }
    }
}

void BrowserWidget::openUrl(const KUrl & targetUrl)
{
    m_htmlPart->openUrl(targetUrl);
}

void BrowserWidget::providerChanged(int provider)
{
    m_currentProvider = provider;
    showCurrentTranslation();
}


#include "browserwidget.moc"

