/***************************************************************************

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "configurepracticedialog.h"

#include "configurepracticewidget.h"
#include "advancedpracticeoptions.h"
#include "thresholdoptions.h"
#include "blockoptions.h"

#include <KLocale>

ConfigurePracticeDialog::ConfigurePracticeDialog(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config)
    :KConfigDialog(parent, name, config)
{
    m_config = config;

    setCaption(i18nc("@title:window", "Configure Practice"));
    setButtons(Default|Ok|Apply|Cancel|Help);
    setDefaultButton(Ok);


    m_configurePracticeWidget= new ConfigurePracticeWidget(doc, this);
    addPage(m_configurePracticeWidget, i18n("Practice"), "run_query", i18n("Practice options"), true);

    m_queryOptions = new QueryOptions(this);
    addPage(m_queryOptions, i18nc("Configure advanced settings for practicing vocabulary, short title in config dialog.", "Advanced"), "configure", i18nc("Configure advanced settings for practicing vocabulary.", "Advanced Practice Settings"), true);

    m_blockOptions = new BlockOptions(this);
    addPage(m_blockOptions, i18n("Blocking"), "configure", i18n("Blocking Settings"), true);

    m_thresholdOptions = new ThresholdOptions(doc, this);
    addPage(m_thresholdOptions, i18n("Thresholds"), "configure", i18n("Threshold Settings"), true);

    connect(m_thresholdOptions, SIGNAL(widgetModified()), SLOT(updateButtons()));
    connect(m_blockOptions, SIGNAL(widgetModified()), SLOT(updateButtons()));
}

ConfigurePracticeDialog::~ConfigurePracticeDialog()
{
//     KConfigGroup cg(KGlobal::config(), "DocumentPropertiesDialog");
//     KDialog::saveDialogSize(cg);
}


bool ConfigurePracticeDialog::hasChanged()
{
    return  m_configurePracticeWidget->hasChanged() ||
            m_thresholdOptions->hasChanged() ||
            m_blockOptions->hasChanged();
}

bool ConfigurePracticeDialog::isDefault()
{
    return m_configurePracticeWidget->isDefault() &&
           m_thresholdOptions->isDefault() &&
           m_blockOptions->isDefault();
}

void ConfigurePracticeDialog::updateSettings()
{
    m_configurePracticeWidget->updateSettings();
//     m_queryOptions->updateSettings();
    m_thresholdOptions->updateSettings();
    m_blockOptions->updateSettings();
}

void ConfigurePracticeDialog::updateWidgets()
{
    m_configurePracticeWidget->updateWidgets();
    m_queryOptions->updateWidgets();
    m_thresholdOptions->updateWidgets();
    m_blockOptions->updateWidgets();
}

void ConfigurePracticeDialog::updateWidgetsDefault()
{
    m_config->useDefaults(true);
    updateWidgets();
    m_config->useDefaults(false);
}


#include "configurepracticedialog.moc"
