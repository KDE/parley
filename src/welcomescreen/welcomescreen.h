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
#include "statistics/statisticsmainwindow.h"

#include <KXmlGuiWindow>
#include <QSignalMapper>

static const int N = 50;

class ParleyMainWindow;
class KUrl;
class QStandardItemModel;

namespace Practice
{
class ThemedBackgroundRenderer;
class ImageWidget;
}


class RemoveButton;


class WelcomeScreen : public KXmlGuiWindow
{
    Q_OBJECT
public:
    WelcomeScreen(ParleyMainWindow *parent);
    ~WelcomeScreen();

public slots:
    void slotOpenUrl(const KUrl& url);
    void slotPracticeUrl(const KUrl& url);
    //void slotDoubleClicked(const QModelIndex& index);
    void slotDoubleClickOpen();
    //void updateRecentFilesModel();
    void populateGrid();
    void populateMap();
    void clearGrid();
    void slotPracticeButtonClicked(const QString& urlString);
    void slotRemoveButtonClicked(const QString& urlString);
    void statisticsHandler(KUrl url);
    void remove(QGridLayout *layout, int row, int column, bool deleteWidgets);
    void deleteChildWidgets(QLayoutItem *item);

private slots:
    void setTheme();
    void backgroundChanged(const QPixmap& pixmap);
    void updateFontColors();
    void updateBackground();

private:
    Ui::WelcomeScreen *ui;
    ParleyMainWindow *m_mainWindow;
    StatisticsMainWindow *statisticsWidget;
    Practice::ThemedBackgroundRenderer *m_themedBackgroundRenderer;
    Practice::ImageWidget* m_widget;

    QStandardItemModel *m_recentFilesModel;
    QMap<QString, QString> recentFilesMap;  // url, name
    KUrl m_openUrl;
    QGridLayout *m_subGridLayout;
    QGridLayout *m_completedGridLayout;

    // The parts of the collections
    QLabel       *nameLabel[N];
    QWidget      *wordCloud[N];
    QPushButton  *practiceButton[N];
    RemoveButton *removeButton[N];
    KUrl          urlArray[N];

    QSignalMapper *practiceSignalMapper;  // For the practice buttons
    QSignalMapper *removeSignalMapper; // For the remove buttons 

    int m_count;
};

#endif
