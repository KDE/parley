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


#ifndef PARLEYPLASMA_H
#define PARLEYPLASMA_H

#include <Plasma/Plasma>
#include <Plasma/Applet>
#include <Plasma/Svg>
#include <plasma/dataengine.h>
#include <plasma/plasma.h>

#include <QGraphicsTextItem>

#include "ui_config.h"

class QFont;
class QSizeF;
class KConfigDialog;

class ParleyPlasma : public Plasma::Applet
{
    Q_OBJECT

public:
    ParleyPlasma(QObject *parent, const QVariantList &args);
    ~ParleyPlasma();
    void init();
    void setContentSize(const QSizeF& size);
    QSizeF contentSizeHint() const;
    void paintInterface(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        const QRect& contentsRect);
    void constraintsEvent(Plasma::Constraints constraints);

public slots:
    void createConfigurationInterface(KConfigDialog *parent);
    void configAccepted();
    void showFontSelectDlg();
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
    // in the config dialog a file was selected
    void urlSelected(const QUrl &file);
    void configChanged();
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

private:
    enum ShowSolution {
        Hover = 0,
        Always
    };
    Plasma::Svg *m_theme;
    QGraphicsTextItem *m_label1;
    QGraphicsTextItem *m_label2;

    Ui::config ui;

    QFont m_font;
    QSizeF m_size;

    Plasma::DataEngine* m_engine;
    QUrl m_sourceFile;
    QStringList m_languages;
    int m_solutionType;
    int m_lang1;
    int m_lang2;
    int m_updateInterval;
};

K_EXPORT_PLASMA_APPLET(parley, ParleyPlasma)

#endif
