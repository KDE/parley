/***************************************************************************

    $Id$

                 convert spotlight list to kvtml

    -----------------------------------------------------------------------

    begin                : Thu Sep 30 20:00:00 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

#include <vector>
#include <iostream>
using namespace std;


#include <klocale.h>
#include <kapplication.h>

#define KVD_VERS_PREFIX " v"     //#include "kvoctraindoc.h"
#define QM_VERB         "v"      //#include "QueryManager.h"

#include "kvoctraincore.h"

#define SPOT_VERSION   "v0.2.1"

vector<QString> lesson_names;

struct spotty {
  QString  type;
  int      lesson;
  QString  en_rem,      // possibly remarks
           de_rem;
  QString  en,
           de;
};

vector<spotty> spottys;

void writeSpotty (QTextStream &os, spotty& spot_line, bool first) {
   os << " <e";

   if (spot_line.lesson != 0 )
     os << " m=\"" << spot_line.lesson << "\"";

   if (!spot_line.type.isEmpty() )
     os << " t=\"" << spot_line.type << "\"";

   os << "><o";

   if (first)
     os << " l=\"en\"";

   if (!spot_line.en_rem.isEmpty() )
     os << " r=\"" << spot_line.en_rem.stripWhiteSpace() << "\"";

   os << ">" << spot_line.en.stripWhiteSpace() << "</o><t";

   if (first)
     os << " l=\"de\"";

   if (!spot_line.de_rem.isEmpty() )
     os << " r=\"" << spot_line.de_rem.stripWhiteSpace() << "\"";

   os << ">" << spot_line.de.stripWhiteSpace() << "</t></e>\n";
}


void readToMem (QTextStream &is, QString month, QString year)
{
   QString line;
   bool    lesson_pending = false;
   QString lesson_str;
   spotty spot;

   bool first_line = true;
   while (!is.eof() && is.device()->status() == IO_Ok) {
     line = is.readLine();

     int pos;
     while ((pos = line.find("  ")) >= 0)
       line.remove (pos, 1);

     bool head_line = (!line.stripWhiteSpace().isEmpty() && line == line.upper() );

     if (first_line && !head_line) {
       line = is.readLine();
       head_line = (!line.stripWhiteSpace().isEmpty() && line == line.upper() );
     }
     first_line = false;

     if (   line.find ("see p.") >= 0
         || line.find ("see pp.") >= 0
         || line.find ("see also p") >= 0) {
       line = "";
     }

     int c = 0x92;
     while ((pos = line.find (c)) >= 0)
       line[pos] = '\'';

     c = 0x94;
     while ((pos = line.find (c)) >= 0)
       line[pos] = ' ';

     pos = 0;
     while ((pos = line.find ('&', pos)) >= 0) {
       line.insert (pos+1, "amp;");
       pos += 5; // skip &amp;
     }

     while ((pos = line.find ('<')) >= 0) {
       line.remove(pos, 1);
       line.insert (pos, "&lt;");
     }

     while ((pos = line.find ('>')) >= 0) {
       line.remove(pos, 1);
       line.insert (pos, "&gt;");
     }

     if (!line.isEmpty()) {

       if (head_line) {
         lesson_pending = true;
         lesson_str = line + ", "+month+" "+year;
       }
       else {
         pos = line.find ('\t');
         if (pos < 0)
           pos = line.find ('|');
         if (pos >= 0) {
           if (lesson_pending) {
             lesson_pending = false;
             lesson_names.push_back(lesson_str.stripWhiteSpace());
           }

           spot.lesson = lesson_names.size();
           spot.type = "";
           spot.en_rem = "";
           spot.de_rem = "";

           if (line.left (3) == "to ") {
             spot.type = QM_VERB;
             line.remove (0, 3);
             pos -= 3;
           }
           spot.en = line.mid(0, pos);
           spot.de = line.mid(pos+1, line.length()-pos-1);

           if ((pos = spot.en.find(" UK") ) >= 0) {
             spot.en_rem+= i18n("UK ").local8Bit();
             spot.en.remove (pos, 3);
           }
           if ((pos = spot.en.find("(UK)") ) >= 0) {
             spot.en_rem+= i18n("UK ").local8Bit();
             spot.en.remove (pos, 4);
           }

           if ((pos = spot.en.find(" N. Am.") ) >= 0) {
             spot.en_rem+= i18n("N. Am. ").local8Bit();
             spot.en.remove (pos, 7);
           }
           if ((pos = spot.en.find("(N. Am.)") ) >= 0) {
             spot.en_rem+= i18n("N. Am. ").local8Bit();
             spot.en.remove (pos, 8);
           }

           if ((pos = spot.en.find(" US") ) >= 0) {
             spot.en_rem+= i18n("US ").local8Bit();
             spot.en.remove (pos, 3);
           }
           if ((pos = spot.en.find("(US)") ) >= 0) {
             spot.en_rem+= i18n("US ").local8Bit();
             spot.en.remove (pos, 4);
           }

           if ((pos = spot.en.find("ifml.") ) >= 0) {
             spot.en_rem+= i18n("ifml. ").local8Bit();
             spot.en.remove (pos, 5);
           }
           if ((pos = spot.en.find("(ifml.)") ) >= 0) {
             spot.en_rem+= i18n("ifml. ").local8Bit();
             spot.en.remove (pos, 7);
           }

           if ((pos = spot.en.find("vulg.") ) >= 0) {
             spot.en_rem+= i18n("vulg. ").local8Bit();
             spot.en.remove (pos, 5);
           }
           if ((pos = spot.en.find("(vulg.)") ) >= 0) {
             spot.en_rem+= i18n("vulg. ").local8Bit();
             spot.en.remove (pos, 7);
           }

           if (!spot.en.isEmpty() && !spot.de.isEmpty())
             spottys.push_back(spot);
         }
       }
     }
   }
}

void writeToKvtml(QTextStream &os, QString month, QString year)
{
   os <<
           "<?xml version=\"1.0\"?>\n"
           "<!doctype kvtml system \"kvoctrain.dtd\">\n"
           "<!--\n"
           "This is a machine generated file.\n"
           "Be careful when editing here.\n"
           "\n"
           "Short definition:\n"
           "\n"
           "lesson       lesson group\n"
           " desc        name\n"
           "   %no       its index\n"
           "   %current  is current lesson\n"
           "type         type group\n"
           " desc        name\n"
           "   %no       its index\n"
           "e            entry of dictionary\n"
           "  %s         is selected\n"
           "  %m         lesson member\n"
           "  %t         common expression type\n"
           " o           original\n"
           "   %q        in query (\"o\" is given, \"t\" is wanted)\n"
           "   %l        language code\n"
           "   %r        remark\n"
           "   %p        pronunciation\n"
           "   %width    column width\n"
           "   %t        expression type\n"
           " t           translation ..\n"
           "   %q        in query (\"t\" is given, \"o\" is wanted)\n"
           "   %l        language code\n"
           "   %r        remark\n"
           "   %p        pronunciation\n"
           "   %width    column width\n"
           "   %t        expression type\n"
           "\n"
           "   %d        last query date (from;to)\n"
           "   %w        dito, compressed\n"
           "   %g        grade (from;to)\n"
           "   %c        count (from;to)\n"
           "   %b        bad count (from;to)\n"
           "\n"
           "\n"
           "Valid xml means:\n"
           " - Close all tags\n"
           " - Keep proper hierarchy\n"
           " - All options are quoted\n"
           "\n"
           "--\n"
           ">\n"
           "<kvtml\n"
           "  generator=\"spotlight2kvtml " SPOT_VERSION " kvoctrain" KVD_VERS_PREFIX "0.5.0\"\n"
           "  cols=\"2\"\n"
           "  lines=\""  << spottys.size() << "\"\n" <<
           "  title=\"" << i18n("Spotlight Online, issue ").local8Bit() << month << " " << "\'" << year << "\"\n"
           << "  author=" << i18n("\"Spotlight Online, www.spotlight-online.de (converted by spotlight2kvtml)").local8Bit()
           << "\">\n\n"
           << " <lesson>\n";

           for (int i = 0; i < (int) lesson_names.size(); i++) {
             os << "  <desc no=\"" << i+1 << "\">"
                << lesson_names[i] << "</desc>\n";
           }
           os << " </lesson>\n\n";

   if (spottys.size() != 0)
     writeSpotty (os, spottys[0], true);

   int l = 1;
   while (os.device()->status() == IO_Ok && l < (int) spottys.size() ) {
     writeSpotty (os, spottys[l], false);
     l++;
   }

   os << "</kvtml>\n";
}


int main(int argc, char **argv)
{
  // use i18n-strings from kvoctrain
  KApplication app(argc,argv, "kvoctrain");

  if (argc != 4) {
    cerr << i18n("usage: spotlight2kvtml spotfile month year\n\n").local8Bit();
    exit (1);
  }

  QString spot (argv[1]);
  QString kvtml;

  int dot = spot.findRev('.');
  if (dot < 0) {
    kvtml = spot + "." KVTML_EXT;
  }
  else {
    kvtml = spot;
    kvtml.remove (dot, kvtml.length()-dot);
    kvtml += "." KVTML_EXT;
  }

  QFile fs(spot);
  fs.open( IO_ReadOnly );
  QTextStream is( &fs );
  readToMem (is, argv[2], argv[3]);
  fs.close();

  if (is.device()->status() != IO_Ok ) {
    cerr << i18n("Could not read ").local8Bit() << spot.local8Bit() << endl;
    exit (1);
  }

  QFile fk(kvtml);
  fk.open( IO_WriteOnly );
  QTextStream os( &fk );
  writeToKvtml(os, argv[2], argv[3]);
  fk.close();

  if (os.device()->status() != IO_Ok ) {
    cerr << i18n("Could not write ").local8Bit() << kvtml.local8Bit() << endl;
    exit (1);
  }

  exit (0);
}
