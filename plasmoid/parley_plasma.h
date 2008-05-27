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

#include <Plasma/Applet>
#include <Plasma/Svg>
#include <plasma/dataengine.h>
#include <plasma/plasma.h>

#include <QGraphicsTextItem>

#include "ui_config.h"

class KDialog;
class QFont;
class QSizeF;
class KConfigDialog;

class ParleyPlasma : public Plasma::Applet
{
    Q_OBJECT

    // could include KWordQuiz
    // defines the data source
    enum RecentFiles {
        UserDefined,
        Parley
    };

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

    protected:
        virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
        virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

    private:
        Plasma::Svg *m_theme;
        QGraphicsTextItem *m_label1;
        QGraphicsTextItem *m_label2;

        Ui::config ui;
        KDialog *m_dialog;

        QFont m_font;
        QSizeF m_size;

        Plasma::DataEngine* m_engine;
        QString m_sourceFile;

        int m_updateInterval;
};

K_EXPORT_PLASMA_APPLET(parley, ParleyPlasma)

#endif
