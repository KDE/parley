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


#include "parley_plasma.h"

#include <KDialog>
#include <KConfigGroup>
#include <KFontDialog>
#include <KColorDialog>
#include <KDebug>

ParleyPlasma::ParleyPlasma(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_theme("widgets/parley_plasma_card", this),
    m_dialog(0),
    m_font(QFont()),
    m_size(256,160)

{
    m_dialog = 0;
    m_label = 0;
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);
}

void ParleyPlasma::init()
{
    Plasma::DataEngine* parleyEngine = dataEngine("parley");
    parleyEngine->connectSource("Random", this, 10000);

    m_theme.setContentType(Plasma::Svg::SingleImage);

    m_layout = new Plasma::VBoxLayout(this);
    m_layout->setGeometry(QRectF(0, 0, m_theme.size().width(), m_theme.size().height()));

    m_label = new Plasma::Label(this);
    m_layout->addItem(m_label);
//     m_label->setTextWidth(m_theme.width());

    KConfigGroup cg = config();
    m_label->setFont(cg.readEntry("font",m_font));
}

void ParleyPlasma::constraintsUpdated(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);
    setDrawStandardBackground(false);
}

ParleyPlasma::~ParleyPlasma()
{
    delete m_dialog;
}

void ParleyPlasma::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(source);
// kDebug() << (data["Random"]).toString();

    if ( m_label) {
        QString text = (data["Random"]).toString();
        m_label->setText(text);
    }
}

void ParleyPlasma::setContentSize(const QSizeF& size)
{
    m_size = size;
}

QSizeF ParleyPlasma::contentSizeHint() const
{
    return m_size;
}

void ParleyPlasma::paintInterface(QPainter *p,
                           const QStyleOptionGraphicsItem *option,
                           const QRect &contentsRect)
{
    Q_UNUSED(option);

    m_theme.resize((int)contentsRect.width(),
                         (int)contentsRect.height());
    m_theme.paint(p,
                       (int)contentsRect.left(),
                       (int)contentsRect.top());
}

void ParleyPlasma::showConfigurationInterface()
{
    if (m_dialog == 0) {
        m_dialog = new KDialog;
        m_dialog->setWindowIcon(KIcon("parley"));
        m_dialog->setCaption( i18n("ParleyPlasma Configuration") );
        ui.setupUi(m_dialog->mainWidget());
        m_dialog->mainWidget()->layout()->setMargin(0);
        m_dialog->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );
        connect( m_dialog, SIGNAL(applyClicked()), this, SLOT(configAccepted()) );
        connect( m_dialog, SIGNAL(okClicked()), this, SLOT(configAccepted()) );
        connect( ui.fontSelectButton, SIGNAL(clicked()), this, SLOT(showFontSelectDlg()) );
    }

    m_dialog->show();
}

void ParleyPlasma::showFontSelectDlg()
{
    KFontDialog::getFont(m_font);
}

void ParleyPlasma::configAccepted()
{
    prepareGeometryChange();

    KConfigGroup cg = config();
    cg.writeEntry("font", m_font);
    m_label->setFont(m_font);
    m_label->resize(boundingRect().width() /10*8, boundingRect().height());
    m_label->setPos(boundingRect().width() / 10, boundingRect().height()/5);
    emit configNeedsSaving();
}

#include "parley_plasma.moc"
