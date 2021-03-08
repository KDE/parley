/*
    SPDX-FileCopyrightText: 2006 Mauricio Piacentini <mauricio@tabuleiro.com>
    SPDX-FileCopyrightText: 2007 Matt Williams <matt@milliams.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KGAMETHEMESELECTOR_H
#define KGAMETHEMESELECTOR_H

#include <QWidget>

//#include <libkdegames_export.h>

class KGameTheme;
class KConfigSkeleton;
class KGameThemeSelectorPrivate;

/**
 * \class KGameThemeSelector kgamethemeselector.h <KGameThemeSelector>
 *
 * @short A widget used to select the game's theme
 *
 * The most common way to use the theme selector is to add it as page to a KConfigDialog
 * \code
 * KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
 * dialog->addPage(new KGameThemeSelector(dialog, Settings::self()), i18n("Theme"), "game_theme");
 * dialog->show();
 * \endcode
 * This will create a page in your KConfigDialog with the title "Theme" and using the
 * "game_theme" icon. By default, the widget will search in the share/apps/appname/themes
 * directory for .desktop files with a group called "KGameTheme".
 *
 * @author Mauricio Piacentini
 **/
class KGameThemeSelector : public QWidget
{
    Q_OBJECT
public:
    enum NewStuffState {
        NewStuffDisableDownload,
        NewStuffEnableDownload
    };
    /**
     * Load a specific theme file.
     * @param parent the parent widget
     * @param config configuration data
     * @param knsflags KDE New Stuff flags
     * @param groupName the title of the config group in the theme .desktop file
     * @param directory subdirectory (of share/apps/appname) to search in
     * @return true if the theme files and properties could be loaded
     */
    KGameThemeSelector(QWidget* parent, KConfigSkeleton* config, KGameThemeSelector::NewStuffState knsflags = KGameThemeSelector::NewStuffEnableDownload, const QString &groupName = QStringLiteral("KGameTheme"), const QString &directory = QStringLiteral("themes"));
    virtual ~KGameThemeSelector();

private:
    KGameThemeSelectorPrivate* const d_ptr;

    Q_DISABLE_COPY(KGameThemeSelector)
    Q_DECLARE_PRIVATE(KGameThemeSelector)

    Q_PRIVATE_SLOT(d_func(), void _k_updatePreview())
    Q_PRIVATE_SLOT(d_func(), void _k_updateThemeList(const QString&))
    Q_PRIVATE_SLOT(d_func(), void _k_openKNewStuffDialog())
};

#endif
