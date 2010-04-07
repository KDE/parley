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

#include <parleydocument.h>
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
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

ExportDialog::ExportDialog(ParleyDocument *doc, QWidget *parent) : KDialog(parent)
{
    m_doc = doc;
    m_parent = parent;

    ui = new Ui::ExportOptions();
    ui->setupUi(mainWidget());

    setCaption(i18n("Export"));
}

void ExportDialog::accept()
{
    KDialog::accept();

    if (ui->csvRadio->isChecked()) {
        /// Find the CSV filter in the standard filter list
        //!@todo: good and clean solution
        QStringList defaultFilters = KEduVocDocument::pattern(KEduVocDocument::Writing).split('\n');
        QString filter = defaultFilters.filter("csv").join("\n");
        KUrl filename = getFileName(filter);
        if(filename != KUrl()) {
            m_doc->saveAs(filename);
        }
        return;
    }

    QString xslFile;
    if (ui->flashCardRadio->isChecked()) {
        xslFile = KStandardDirs::locate( "data", "parley/xslt/flashcards.xsl");
    } else {
        xslFile = KStandardDirs::locate( "data", "parley/xslt/table.xsl");
    }

    QString filter = "*.html|" + i18n("HTML document");
    KUrl filename = getFileName(filter);
    if(filename == KUrl()) {
        return;
    }

    kDebug() << "XSLT starting";

    xsltStylesheetPtr cur = NULL;
    xmlDocPtr doc, res;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;
    cur = xsltParseStylesheetFile((const xmlChar*) xslFile.toLatin1().constData());

    doc = xmlParseDoc( (const xmlChar*) m_doc->document()->toByteArray(m_doc->document()->generator()).constData() );

    res = xsltApplyStylesheet(cur, doc, 0);
    FILE* result = fopen( (const char*) filename.toLocalFile().toLatin1(), "w");
    xsltSaveResultToFile(result, res, cur);
    fclose(result);

    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);

    xsltCleanupGlobals();
    xmlCleanupParser();

    kDebug() << "XSLT finished";
}

KUrl ExportDialog::getFileName(const QString& filter)
{

    KFileDialog dlg( (m_doc->document()->url().fileName() == i18n("Untitled")) ? "": m_doc->document()->url().toLocalFile(), filter, m_parent );
    dlg.setOperationMode( KFileDialog::Saving );
    dlg.setMode( KFile::File );
    dlg.setWindowTitle(i18n("Export As"));
    if (dlg.exec() != QDialog::Accepted) {
        return KUrl();
    }

    return dlg.selectedFile();
}

#include "exportdialog.moc"
