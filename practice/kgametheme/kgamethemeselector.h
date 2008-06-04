/*
    Copyright (C) 2006 Mauricio Piacentini  <mauricio@tabuleiro.com>
    Copyright (C) 2007 Matt Williams   <matt@milliams.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef KGAMETHEMESELECTOR_H
#define KGAMETHEMESELECTOR_H

#include <QtGui/QWidget>

#include <libkdegames_export.h>

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
class KDEGAMES_EXPORT KGameThemeSelector : public QWidget
{
    Q_OBJECT
    public:
      enum NewStuffState {
          NewStuffDisableDownload,
          NewStuffEnableDownload
        };
        /**
         * Load a specific theme file.
         * @param groupName the title of the config group in the theme .desktop file
         * @param directory subdirectory (of share/apps/appname) to search in
         * @return true if the theme files and properties could be loaded
         */
        KGameThemeSelector(QWidget* parent, KConfigSkeleton* config, KGameThemeSelector::NewStuffState knsflags = KGameThemeSelector::NewStuffEnableDownload, const QString &groupName = QLatin1String("KGameTheme"), const QString &directory = QLatin1String("themes"));
        virtual ~KGameThemeSelector();
    
    private:
        class KGameThemeSelectorPrivate;
        KGameThemeSelectorPrivate* const d;

        Q_DISABLE_COPY(KGameThemeSelector)

        Q_PRIVATE_SLOT(d, void _k_updatePreview())
        Q_PRIVATE_SLOT(d, void _k_openKNewStuffDialog())
};

#endif
