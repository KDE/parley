/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "configurepracticedialog.h"

#include "generalpracticeoptions.h"
#include "blockoptions.h"
#include "thresholdoptions.h"
#include "specificpracticeoptions.h"

#include <KLocalizedString>
#include <KWindowConfig>

#include <QPushButton>

ConfigurePracticeDialog::ConfigurePracticeDialog(KEduVocDocument *doc, QWidget *parent,
                                                 const QString &name, KConfigSkeleton *config)
    : KConfigDialog(parent, name, config)
{
    m_config = config;

    setWindowTitle(i18nc("@title:window", "Configure Practice"));
    button(QDialogButtonBox::Ok)->setDefault( true );

    m_generalPracticeOptions = new GeneralPracticeOptions(this);
    addPage(m_generalPracticeOptions,
            i18nc("@title:group Configure general settings for practicing vocabulary, short title in config dialog.", "General"),
            QStringLiteral("general-setup"), i18nc("Configure general settings for practicing vocabulary.", "General Practice Settings"),
            true);

    m_blockOptions = new BlockOptions(this);
    addPage(m_blockOptions, i18nc("@title:group vocabulary can be set to be blocked for a certain amount of time", "Blocking"), QStringLiteral("cards-block"), i18n("Blocking Settings"), true);

    m_thresholdOptions = new ThresholdOptions(doc, this);
    addPage(m_thresholdOptions, i18nc("@title:group ignore vocabulary based on some properties like word type", "Thresholds"), QStringLiteral("practice-setup"), i18n("Threshold Settings"), true);

    m_specificPracticeOptions = new SpecificPracticeOptions(this);
    addPage(m_specificPracticeOptions, i18nc("@title:group Configure specific settings for practicing vocabulary, short title in config dialog.", "Specific"), QStringLiteral("specific-setup"), i18nc("Configure specific settings for practicing vocabulary.", "Specific Practice Settings"), true);

    setHelp(QString(), QStringLiteral("parley"));

    KConfigGroup cg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), "ConfigurePracticeDialog");
    this->resize( cg.readEntry("width",100 ), cg.readEntry("height",100 ) );
}

ConfigurePracticeDialog::~ConfigurePracticeDialog()
{
    KConfigGroup cg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), "ConfigurePracticeDialog");
    cg.writeEntry("width",  this->size().width() );
    cg.writeEntry("height",  this->size().height() );
}


bool ConfigurePracticeDialog::hasChanged()
{
    return  m_thresholdOptions->hasChanged() ||
            m_blockOptions->hasChanged();
}

bool ConfigurePracticeDialog::isDefault()
{
    return m_thresholdOptions->isDefault() &&
           m_blockOptions->isDefault();
}

void ConfigurePracticeDialog::updateSettings()
{
    m_thresholdOptions->updateSettings();
    m_blockOptions->updateSettings();
}

void ConfigurePracticeDialog::updateWidgets()
{
    m_thresholdOptions->updateWidgets();
    m_blockOptions->updateWidgets();
}

void ConfigurePracticeDialog::updateWidgetsDefault()
{
    m_config->useDefaults(true);
    updateWidgets();
    m_config->useDefaults(false);
}
