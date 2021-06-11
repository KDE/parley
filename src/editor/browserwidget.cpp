/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "browserwidget.h"

#include <KEduVocExpression>

#include <QDragEnterEvent>

using namespace Editor;

BrowserWidget::BrowserWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;
    m_entry = 0;
    m_currentProvider = 0; ///@todo kconfig?

    m_htmlPart = new QWebEngineView(widget);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    widget->setLayout(layout);
    layout->addWidget(m_htmlPart);

    connect(showCurrentButton, &QPushButton::clicked, this, &BrowserWidget::showCurrentTranslation);

    setupProviders();
    connect(providerComboBox, static_cast<void (KComboBox::*)(int)>(&KComboBox::currentIndexChanged), this, &BrowserWidget::providerChanged);

}

void BrowserWidget::setupProviders()
{
    providerComboBox->clear();

    DictionaryProvider provider;
    provider.name = QStringLiteral("De-Es Beolingus");
    provider.url = QStringLiteral("https://dict.tu-chemnitz.de/dings.cgi?service=dees&query=\\{@}");
    provider.languages << QStringLiteral("de") << QStringLiteral("es");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("De-En Beolingus");
    provider.url = QStringLiteral("https://dict.tu-chemnitz.de/dings.cgi?service=deen?query=\\{@}");
    provider.languages << QStringLiteral("de") << QStringLiteral("en");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("De-En Leo");
    provider.url = QStringLiteral("https://dict.leo.org/?search=\\{@}");
    provider.languages << QStringLiteral("de") << QStringLiteral("en");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("De-Es Leo");
    provider.url = QStringLiteral("https://dict.leo.org/?lp=esde&search=\\{@}");
    provider.languages << QStringLiteral("de") << QStringLiteral("es");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("De-Fr Leo");
    provider.url = QStringLiteral("https://dict.leo.org/?lp=frde&search=\\{@}");
    provider.languages << QStringLiteral("de") << QStringLiteral("fr");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("Ru-En Multitran");
    provider.url = QStringLiteral("https://multitran.com/c/m.exe?SHL=1&CL=1&l1=1&s=\\{@}");
    provider.languages << QStringLiteral("ru") << QStringLiteral("en");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("Ru-De Multitran");
    provider.url = QStringLiteral("https://multitran.com/c/m.exe?SHL=1&CL=1&l1=3&s=\\{@}");
    provider.languages << QStringLiteral("ru") << QStringLiteral("de");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("Ru-Fr Multitran");
    provider.url = QStringLiteral("https://multitran.com/c/m.exe?SHL=1&CL=1&l1=4&s=\\{@}");
    provider.languages << QStringLiteral("ru") << QStringLiteral("fr");
    m_providers.append(provider);
    providerComboBox->addItem(provider.name);

    provider.name = QStringLiteral("Heinzelnisse");
    provider.url = QStringLiteral("https://heinzelnisse.info/dict?setOptions=true&searchItem=\\{@}&dictDeNoSearch=on&dictNoDeSearch=on&dictExactSearch=on&dictPhoneticSearch=on&wikiSearch=on&dictNynorskSearch=on&dictBokmaalSearch=checked&forumKeywordSearch=on&suggestion=on");
    provider.languages << QStringLiteral("no") << QStringLiteral("de");
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
                m_htmlPart->load(QUrl(QString(m_providers.value(m_currentProvider).url.replace(QLatin1String("\\{@}"), text))));
                m_htmlPart->show();
            }
        }
    }
}

void BrowserWidget::openUrl(const QUrl & targetUrl)
{
    m_htmlPart->load(targetUrl);
    m_htmlPart->show();
}

void BrowserWidget::providerChanged(int provider)
{
    m_currentProvider = provider;
    showCurrentTranslation();
}
