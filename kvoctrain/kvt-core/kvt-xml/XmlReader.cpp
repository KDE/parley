/* -*- C++ -*-

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99

  -----------------------------------------------------------------------

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#include <iostream>
using namespace std;
#include "XmlReader.h"

XmlReader::XmlReader (KOXML_ISTREAM& is)
  : tokenizer (is)
{
}

XmlReader::~XmlReader () {
}

bool XmlReader::validHeader () {
  // Check for: <? xml version="1.0" ?>
  if (tokenizer.nextToken () != XmlTokenizer::Tok_Lt)
    return false;
  if (tokenizer.nextToken () != XmlTokenizer::Tok_QSign)
    return false;

  if (tokenizer.nextToken () != XmlTokenizer::Tok_Symbol)
    return false;
  else if (tokenizer.element () != "xml")
    return false;

  if (tokenizer.nextToken () != XmlTokenizer::Tok_Symbol)
    return false;
  else if (tokenizer.element () != "version")
    return false;
  if (tokenizer.nextToken () != XmlTokenizer::Tok_Eq)
    return false;
  if (tokenizer.nextToken () != XmlTokenizer::Tok_String)
    return false;
  else if (tokenizer.element () != "1.0")
    return false;

  if (tokenizer.nextToken () != XmlTokenizer::Tok_QSign)
    return false;
  if (tokenizer.nextToken () != XmlTokenizer::Tok_Gt)
    return false;

  // Check for: <(!)doctype symbol system string>

  if (tokenizer.nextToken () != XmlTokenizer::Tok_Lt)
    return false;

  XmlTokenizer::Token token = tokenizer.nextToken ();
  if (token == XmlTokenizer::Tok_Exclam)
    token = tokenizer.nextToken ();

  if (token != XmlTokenizer::Tok_Symbol)
    return false;
  else if (tokenizer.element().lower() != "doctype")
    return false;

  if (tokenizer.nextToken () != XmlTokenizer::Tok_Symbol)
    return false;
  s_dtype = tokenizer.element ();

  if (tokenizer.nextToken () != XmlTokenizer::Tok_Symbol)
    return false;
  else if (tokenizer.element().lower() != "system")
    return false;

  if (tokenizer.nextToken () != XmlTokenizer::Tok_String)
    return false;
  s_dtd = tokenizer.element ();

  if (tokenizer.nextToken () != XmlTokenizer::Tok_Gt)
    return false;

  return true;
}

const KOXML_STRING& XmlReader::doctype () const {
  return s_dtype;
}

const KOXML_STRING& XmlReader::dtd () const {
  return s_dtd;
}

const KOXML_STRING& XmlReader::getText () {
  return text;
}

bool XmlReader::readElement (XmlElement& elem) {
  XmlTokenizer::Token tok;
  bool result = false;
  bool ready = false;

  elem.reset ();

  while (! ready) {
    tok = tokenizer.nextToken ();
    if (tok == XmlTokenizer::Tok_Lt) {
      tok = tokenizer.nextToken ();
      if (tok == XmlTokenizer::Tok_Comment) {
	// skip comment
      }
      else {
	if (tok == XmlTokenizer::Tok_Slash)
	  result = parseEndElement (elem);
	else if (tok == XmlTokenizer::Tok_Symbol)
	  result = parseElement (tokenizer.element (), elem);
	else if (tok == XmlTokenizer::Tok_EOF)
	  result = false;
	ready = true;
      }
    }
    else if (tok == XmlTokenizer::Tok_Text) {
      elem.tagId = "#PCDATA";
      text = tokenizer.element ();
      result = true;
      ready = true;
    }
    else
      return false;
  }
  return result;
}

bool XmlReader::parseEndElement (XmlElement& elem) {
  bool result = false;

  XmlTokenizer::Token tok = tokenizer.nextToken ();
  if (tok == XmlTokenizer::Tok_Symbol) {
    KOXML_STRING tag = tokenizer.element ();
    tok = tokenizer.nextToken ();
    if (tok == XmlTokenizer::Tok_Gt) {
      elem.tagId = tag;
      elem.endTag = true;
      elem.closed = true;
      result = true;
    }
  }
  return result;
}

bool XmlReader::parseElement (const KOXML_STRING& id, XmlElement& elem) {
  KOXML_STRING tag = id;
  bool closed = false;
  std::list<XmlAttribute> attrib_list;

  XmlTokenizer::Token tok = tokenizer.nextToken ();
  while (tok != XmlTokenizer::Tok_Gt) {
    if (tok == XmlTokenizer::Tok_Slash) {
      if (closed)
        return false;
      else
	closed = true;
    }
    else if (tok == XmlTokenizer::Tok_Symbol) {
      if (! attrib_list.empty () || closed)
        return false;
      tokenizer.unget ();
      if (! readAttributes (attrib_list))
        return false;
    }
    else
      return false;

    tok = tokenizer.nextToken ();
  }
  elem.tagId = tag;
  elem.endTag = false;
  elem.closed = closed;
  elem.attribs = attrib_list;
  return true;
}

bool XmlReader::readAttributes (std::list<XmlAttribute>& attrib_list) {
  XmlTokenizer::Token tok = tokenizer.nextToken ();

  while (tok != XmlTokenizer::Tok_Gt) {
    if (tok == XmlTokenizer::Tok_Symbol) {
      KOXML_STRING id = tokenizer.element ();
      if (tokenizer.nextToken () == XmlTokenizer::Tok_Eq) {
	if (tokenizer.nextToken () == XmlTokenizer::Tok_String) {

          // un-escape dangerous characters in reverted order
          KOXML_STRING val = tokenizer.element();
          int pos = 0;
          while ((pos = val.find("&quot;", pos)) >= 0) {
            KOXML_STRING_REMOVE( val, pos, 6);
            KOXML_STRING_INSERT( val, pos, "\"");
            pos += 1; // skip "
          }
          pos = 0;
          while ((pos = val.find("&lf;", pos)) >= 0) {
            KOXML_STRING_REMOVE( val, pos, 4);
            KOXML_STRING_INSERT( val, pos, "\r");
            pos += 1; // skip \r
          }
          pos = 0;
          while ((pos = val.find("&nl;", pos)) >= 0) {
            KOXML_STRING_REMOVE( val, pos, 4);
            KOXML_STRING_INSERT( val, pos, "\n");
            pos += 1; // skip \n
          }
          pos = 0;
          while ((pos = val.find("&lt;", pos)) >= 0) {
            KOXML_STRING_REMOVE( val, pos, 4);
            KOXML_STRING_INSERT( val, pos, "<");
            pos += 1; // skip <
          }
          pos = 0;
          while ((pos = val.find ("&amp;", pos)) >= 0) {
            KOXML_STRING_REMOVE( val, pos+1, 4);
            pos += 1; // skip &
          }

	  XmlAttribute attrib (id, val);
	  attrib_list.push_back (attrib);
	}
        else {
          cerr << "invalid attribute value" << endl;
          return false;
        }
      }
      else {
        cerr << "missing '='" << endl;
	return false;
      }
    }
    else if (tok == XmlTokenizer::Tok_Slash) {
      break;
    }
    else {
      cerr << "invalid attribute name: " << (int) tok << endl;
      return false;
    }
    tok = tokenizer.nextToken ();
  }
  tokenizer.unget ();
  return true;
}
