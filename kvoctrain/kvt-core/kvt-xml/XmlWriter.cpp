/* -*- C++ -*-

  $Id$

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99

  -----------------------------------------------------------------------

  $Log$
  Revision 1.1  2001/10/05 15:43:20  arnold
  import of version 0.7.0pre8 to kde-edu


  -----------------------------------------------------------------------

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by  
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU Library General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "XmlWriter.h"

XmlWriter::XmlWriter (KOXML_OSTREAM& os)
  : strm (os)
{
  autoendl = true;
  isapo = false;
  apo = 0;
  strm << "<?xml version=\"1.0\"?>" << endl; // "encoding=UTF8"
}


XmlWriter::~XmlWriter () {
  flush ();
}


void XmlWriter::startTag (KOXML_STRING id, bool closeIt, bool empty, bool eol)
{
  if (!id) return;

  strm << "<" << id;
  if (!empty) {
    lastTags.push (id);
  }

  if (closeIt) {
    if (empty) 
      strm << "/";
    strm << ">";
    if (eol || autoendl) {
      isapo = false;
      apo = 0;
      strm << endl;
    }
  }
}
 

void XmlWriter::endTag (KOXML_STRING id, bool eol)
{
  strm << "</";
  if (id.length() != 0)
    strm << id;
  else {
    KOXML_STRING tag = lastTags.top ();
    lastTags.pop ();
    strm << tag;
  }
  strm << ">";
  if (eol || autoendl) {
    isapo = false;
    apo = 0;
    strm << endl;
  }
}

void XmlWriter::closeTag (bool empty, bool eol)
{
  if (empty) {
    strm << "/";
    lastTags.pop ();
  }
  strm << ">";
  if (eol || autoendl) {
    isapo = false;
    apo = 0;
    strm << endl;
  }
}


void XmlWriter::setAutoEndl (const bool flag)
{
  autoendl = flag;
}


void XmlWriter::endline()
{
  isapo = false;
  apo = 0;
  strm << endl;
}


void XmlWriter::addAttribute (KOXML_STRING name, const KOXML_STRING& value)
{
  if (!name) return;

  KOXML_STRING val = value;
  // escape dangerous characters in sgml-style
  int pos = 0;
  while ((pos = val.find ('&', pos)) >= 0) {
    KOXML_STRING_INSERT( val, pos+1, "amp;");
    pos += 5; // skip &amp;
  }
  pos = 0;
  while ((pos = val.find ('<', pos)) >= 0) {
    KOXML_STRING_REMOVE( val, pos, 1);
    KOXML_STRING_INSERT( val, pos, "&lt;");
    pos += 4; // skip &nl;
  }
  pos = 0;
  while ((pos = val.find ('\n', pos)) >= 0) {
    KOXML_STRING_REMOVE( val, pos, 1);
    KOXML_STRING_INSERT( val, pos, "&nl;");
    pos += 4; // skip &nl;
  }
  pos = 0;
  while ((pos = val.find ('\r', pos)) >= 0) {
    KOXML_STRING_REMOVE( val, pos, 1);
    KOXML_STRING_INSERT( val, pos+1, "lf;");
    pos += 4; // skip &lf;
  }
  pos = 0;
  while ((pos = val.find ('\"', pos)) >= 0) {
    KOXML_STRING_REMOVE( val, pos, 1);
    KOXML_STRING_INSERT( val, pos, "&quot;");
    pos += 6; // skip &qout;
  }

  strm << " ";
  strm << name << "=\"";
  strm << val;
  strm << "\"";
}


void XmlWriter::addAttribute (KOXML_STRING name, int value)
{
  if (name.length() == 0)
    return;

  strm << " ";
  strm << name << "=\"";
  strm << value;
  strm << "\"";
}


void XmlWriter::addAttribute (KOXML_STRING name, float value)
{
  if (name.length() == 0)
    return;

  strm << " ";
  strm << name << "=\"";
  strm << value;
  strm << "\"";
}


void XmlWriter::addAttribute (KOXML_STRING name, double value)
{
  if (name.length() == 0)
    return;

  strm << " ";
  strm << name << "=\"";
  strm << value;
  strm << "\"";
}


void XmlWriter::writeText (KOXML_STRING c)
{
  int i = 0;
  while (i < (int) c.length()) {
    if (c[i] == '<')
      strm << "&lt;";
    else if (c[i] == '&')
      strm << "&amp;";
    else if (c[i] == '>')
      strm << "&gt;";
    else if (c[i] == '\"' || c[i] == '\'' || c[i] == '`') {
      strm << c[i];
      if (isapo) {
        if (apo == c[i])
          isapo = false;
      }
      else {
        isapo = true;
        apo = c[i];
      }
    }
    else if (c[i] == '\n') {
      if (isapo)
        strm << "&nl;";
      else
        strm << c[i];
    }
    else if (c[i] == '\r') {
      if (isapo)
         strm << "&lf;";
      else
        strm << c[i];
    }
    else
      strm << c[i];
    i++;
  }
}


void XmlWriter::indent (int i)
{
  for (; i > 0; i--)
    strm << " ";
}


void XmlWriter::writeTag (KOXML_STRING s)
{
  strm << "<"
       << s
       << ">";
}


void XmlWriter::flush ()
{
#ifndef KOXML_USE_STL
  stream()->flush ();
#else
  strm.flush();
#endif
}
