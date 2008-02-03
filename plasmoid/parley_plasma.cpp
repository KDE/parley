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

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocexpression.h>

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
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);
    m_doc = 0;
}

void ParleyPlasma::init()
{
    KConfig parleyConfig("parleyrc");
    kDebug() << parleyConfig.groupList();
    KConfigGroup recentFilesGroup( &parleyConfig, "Recent Files" );

    // take the last file, but there are File1..n and Name1..n entries..
    QString file = recentFilesGroup.readEntry( recentFilesGroup.keyList().value(recentFilesGroup.keyList().count()/2-1),
            QString() );

    kDebug() << "Open vocabulary file: '" << file << "'";
    if (!file.isEmpty()) {
        m_doc = new KEduVocDocument(this);
        m_doc->open(file);
        m_vocabularyCount = m_doc->lesson()->entries(KEduVocContainer::Recursive).count();
    }
    m_random = new KRandomSequence( QDateTime::currentDateTime().toTime_t() );

    m_theme.setContentType(Plasma::Svg::SingleImage);

    m_label = new Plasma::Label(this);

    if (m_doc) {
        int expNum = m_random->getLong(m_vocabularyCount);
        KEduVocExpression *expression = m_doc->lesson()->entries(KEduVocContainer::Recursive).value(expNum);
        if (expression) {
            QString text;
            foreach (int index, expression->translationIndices()) {
                text += expression->translation(index)->text() + "\n\n";
            }
            m_label->setText(text);
        } else {
            m_label->setText(i18n("Could not open vocabulary document."));
        }
    }

    m_label->resize(boundingRect().width() /10*8, boundingRect().height());
    m_label->setPos(boundingRect().width() / 10, boundingRect().height()/5);

    KConfigGroup cg = config();
    m_label->setFont(cg.readEntry("font",m_font));
}

void ParleyPlasma::constraintsUpdated(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);
    setDrawStandardBackground(false);
    m_label->resize(boundingRect().width() /10*8, boundingRect().height());
    m_label->setPos(boundingRect().width() / 10, boundingRect().height()/5);
}

ParleyPlasma::~ParleyPlasma()
{
    delete m_dialog;
    delete m_random;
    delete m_doc;
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
