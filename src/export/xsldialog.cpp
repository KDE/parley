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

#include "xsldialog.h"

#include <keduvocdocument.h>
#include <keduvockvtml2writer.h>

#include <KStandardDirs>
#include <KUrl>
#include <KDebug>
#include <KFileDialog>

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

XslDialog::XslDialog(const KUrl &file, KEduVocDocument *doc, QWidget *parent)
{
    m_doc = doc;
    m_targetFile = file;

    ui = new Ui::ExportOptions();
    ui->setupUi(mainWidget());
}

void XslDialog::accept()
{
    KDialog::accept();

    QString xslFile;
    if (ui->flashCardRadio->isChecked()) {
        xslFile = KStandardDirs::locate( "data", "parley/xslt/flashcards.xsl");
    } else {
        xslFile = KStandardDirs::locate( "data", "parley/xslt/table.xsl");
    }

    kDebug() << "XSLT starting";

    xsltStylesheetPtr cur = NULL;
    xmlDocPtr doc, res;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;
    cur = xsltParseStylesheetFile((const xmlChar*) xslFile.toLatin1().constData());

    doc = xmlParseDoc( (const xmlChar*) m_doc->toByteArray(m_doc->generator()).constData() );

    res = xsltApplyStylesheet(cur, doc, 0);
    FILE* result = fopen( (const char*) m_targetFile.toLocalFile().toLatin1(), "w");
    xsltSaveResultToFile(result, res, cur);
    fclose(result);

    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);

    xsltCleanupGlobals();
    xmlCleanupParser();

    kDebug() << "XSLT finished";
}

#include "xsldialog.moc"
