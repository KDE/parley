/* -*- C++ -*-

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99

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

#include <stdlib.h>
#include "XmlElement.h"

XmlAttribute::XmlAttribute (const KOXML_STRING& n, const KOXML_STRING& v) :
  aname (n), value (v) {
}


XmlAttribute::XmlAttribute (const XmlAttribute& attr) :
  aname (attr.aname), value (attr.value) {
}

XmlAttribute::~XmlAttribute () {
}


XmlAttribute& XmlAttribute::operator= (const XmlAttribute& attr) {
  aname = attr.aname;
  value = attr.value;
  return *this;
}


float XmlAttribute::floatValue () const {
#ifndef KOXML_USE_STL
   return atof (value.local8Bit());
#else
  return atof (value.data());
#endif
}


int XmlAttribute::intValue () const {
#ifndef KOXML_USE_STL
   return atoi (value.local8Bit());
#else
  return atoi (value.data());
#endif
}


XmlElement::XmlElement () {
  closed = false;
  endTag = false;
}


XmlElement::XmlElement (const XmlElement& elem) :
  tagId (elem.tagId), closed (elem.closed), endTag (elem.endTag),
  attribs (elem.attribs) {
}


XmlElement::~XmlElement () {
}


void XmlElement::reset () {
  tagId = "";
  closed = false;
  endTag = false;
  attribs.erase (attribs.begin (), attribs.end ());
}


XmlElement& XmlElement::operator= (const XmlElement& elem) {
  tagId = elem.tagId;
  closed = elem.closed;
  endTag = elem.endTag;
  attribs =  elem.attribs;
  return *this;
}

