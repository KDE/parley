/***************************************************************************
    Copyright 2008-2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include "ui_welcomescreen.h"

#include <KXmlGuiWindow>

class ParleyMainWindow;
class KUrl;
class QStandardItemModel;

namespace Practice {
    class ThemedBackgroundRenderer;
    class ImageWidget;
}

class WelcomeScreen : public KXmlGuiWindow
{
Q_OBJECT
public:
    WelcomeScreen(ParleyMainWindow *parent);
    ~WelcomeScreen();

public slots:
    void slotOpenUrl(const KUrl& url);
    void slotPracticeUrl(const KUrl& url);
    void slotDoubleClicked(const QModelIndex& index);
    void slotDoubleClickOpen();
    void updateRecentFilesModel();

private slots:
    void setTheme();
    void backgroundChanged(const QPixmap& pixmap);
    void updateFontColors();
    void updateBackground();

private:
    Ui::WelcomeScreen *ui;
    ParleyMainWindow *m_parleyApp;
    QStandardItemModel *m_recentFilesModel;
    KUrl m_openUrl;
    Practice::ThemedBackgroundRenderer *m_themedBackgroundRenderer;
    Practice::ImageWidget* m_widget;
};

#endif
