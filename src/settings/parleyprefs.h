/*
    SPDX-FileCopyrightText: 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PARLEYPREFS_H
#define PARLEYPREFS_H

#include <KConfigWidgets/KConfigDialog>

/**
  *@author Peter Hedlund
*/

class GeneralOptions;
class ViewOptions;
class PasteOptions;
class PluginOptions;

class KEduVocDocument;
class KComboBox;

class KConfigSkeleton;

class ParleyPrefs : public KConfigDialog
{
    Q_OBJECT

public:
    /**
     * Creates a configuration dialog.
     */
    ParleyPrefs(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config);

protected slots:
    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings() Q_DECL_OVERRIDE;
    /**
     * Updates dialog widgets. Here only used after loading a profile.
     * Profiles only store the settings of the last three pages in the dialog.
     */
    void updateWidgets() Q_DECL_OVERRIDE;
    /**
     * Called when the user clicks Default
     */
    void updateWidgetsDefault() Q_DECL_OVERRIDE;

protected:
    /**
     * Returns true if the current state of the dialog is different from the saved settings
     */
    bool hasChanged() Q_DECL_OVERRIDE;
    /**
     * Returns true if the current state of the dialog represents the default settings.
     */
    bool isDefault() Q_DECL_OVERRIDE;

private:
    GeneralOptions * m_generalOptions;
    ViewOptions * m_viewOptions;
    PasteOptions * m_pasteOptions;
    PluginOptions * m_pluginOptions;

    KConfigSkeleton *m_config;
    KEduVocDocument *m_doc;
    KComboBox       *m_lessons;
};

#endif
