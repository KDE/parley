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
    int randInt(int low, int high);

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
    QStandardItemModel *m_recentFilesModel;
    KUrl m_openUrl;
    QMap<QString, QString> recentFilesMap;
    Practice::ThemedBackgroundRenderer *m_themedBackgroundRenderer;
    Practice::ImageWidget* m_widget;
    QGridLayout *m_subGridLayout;
    QGridLayout *m_completedGridLayout;
    QLabel *nameLabel[N];
    QWidget *wordCloud[N];
    QPushButton *practiceButton[N];
    RemoveButton *removeButton[N];
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;
    KUrl urlArray[N];
    StatisticsMainWindow *statisticsWidget;
    int m_count;
};

#endif
