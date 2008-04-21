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

#include "exportdialog.h"

#include <KStandardDirs>
#include <KUrl>
#include <KDebug>

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

ExportDialog::ExportDialog(const KUrl& file, QWidget * parent)
{
    ui = new Ui::ExportOptions();
    ui->setupUi(mainWidget());

    m_sourceFile = file;
    KUrl fileUrl = file;
    QString fileName = file.fileName().left(file.fileName().indexOf('.'));
    fileName.append(".html");
    fileUrl.setFileName(fileName);

    ui->fileName->setUrl(fileUrl);
}

KUrl ExportDialog::fileName()
{
    return ui->fileName->url();
}

void ExportDialog::accept()
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

    doc = xmlParseFile( (const char*) m_sourceFile.toLocalFile().toLatin1() );
    res = xsltApplyStylesheet(cur, doc, 0);
    FILE* result = fopen( (const char*) ui->fileName->url().toLocalFile().toLatin1(), "w");
    xsltSaveResultToFile(result, res, cur);
    fclose(result);

    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);

    xsltCleanupGlobals();
    xmlCleanupParser();

    kDebug() << "XSLT finished";
}

#include "exportdialog.moc"
