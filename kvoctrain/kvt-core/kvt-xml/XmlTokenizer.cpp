/* -*- C++ -*-

  $Id$

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
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "XmlTokenizer.h"
#include <ctype.h>

XmlTokenizer::XmlTokenizer (KOXML_ISTREAM& is) :
  last_chars (""), istrm (is), use_last (false), is_open (false), lineno(1) {
}

XmlTokenizer::~XmlTokenizer () {
}


/*
bool XmlTokenizer::hasMoreTokens () {
  return ! istrm.eof ();
}
*/

void XmlTokenizer::skipWhitespace () {
  KOXML_CHAR c;
  do {
    c = readchar ();
    if (c == '\n')
      lineno++;
    if (! isspace (c)) {
      putback (c);
      return;
    }
    else if (istrm.eof ())
      return;
  } while (1);
}


void XmlTokenizer::putback (KOXML_CHAR c) {
  last_chars += c;
}


KOXML_CHAR XmlTokenizer::readchar () {

  KOXML_CHAR c;

  if (last_chars.length() > 0) {
    c = last_chars[0];
    KOXML_STRING_REMOVE (last_chars, 0, 1);
  }
  else {

# ifndef KOXML_USE_STL
    istrm >> c;
# else
    istrm.get(c);
# endif
  }

  return c;
}


void XmlTokenizer::unget () {
  use_last = true;
}

XmlTokenizer::Token XmlTokenizer::nextToken () {
  KOXML_CHAR c;

  if (use_last) {
    use_last = false;
    return last_tok;
  }

  skipWhitespace ();
  if (istrm.eof ())
    return last_tok = Tok_EOF;

  c = readchar ();
  if (c == '\n')
    lineno++;

  if (!is_open) {
    if (c != '<') {
      putback (c);
      return last_tok = readText ();
    }
  }
  switch (c) {
  case '<':
    is_open = true;
    return last_tok = Tok_Lt;
    break;
  case '>':
    is_open = false;
    return last_tok = Tok_Gt;
    break;
  case '?':
    return last_tok = Tok_QSign;
    break;
  case '/':
    return last_tok = Tok_Slash;
    break;
  case '=':
    return last_tok = Tok_Eq;
    break;
  case '(':
    return last_tok = Tok_LParen;
    break;
  case ')':
    return last_tok = Tok_RParen;
    break;
  case '[':
    return last_tok = Tok_LBracket;
    break;
  case ']':
    return last_tok = Tok_RBracket;
    break;
  case '|':
    return last_tok = Tok_Bar;
    break;
  case '*':
    return last_tok = Tok_Asterisk;
    break;
  case '+':
    return last_tok = Tok_Plus;
    break;
  case ',':
    return last_tok = Tok_Comma;
    break;
  case ';':
    return last_tok = Tok_Semicolon;
    break;
  case '%':
    return last_tok = Tok_Percent;
    break;
  case '#':
    return last_tok = Tok_NSign;
    break;
  case '\'':
    return last_tok = Tok_Apostr;
    break;
  case '"':
    // String einlesen
    return last_tok = readString ();
    break;
  default:
    if (is_open) {
      if (isalpha (c) || isdigit (c)) {
	// Symbol (Element oder Attributbezeichner)
	putback (c);
	return last_tok = readSymbol ();
      }
      else if (c == '!') {
	c = readchar ();
        if (c == '\n')
          lineno++;
	putback (c);
	if (c == '-')
	  return last_tok = readComment ();
	else
	  return last_tok = Tok_Exclam;
      }
      else {
	return last_tok = Tok_Invalid;
      }
    }
    else {
      putback (c);
      return last_tok = readText ();
    }
    break;
  }
}


const KOXML_STRING& XmlTokenizer::element () {
  return elem;
}


XmlTokenizer::Token XmlTokenizer::readSymbol () {
  KOXML_CHAR c;
  elem = "";

  while (1) {
    c = readchar ();
    if (c == '\n')
      lineno++;
    if (istrm.eof () || isspace (c))
      // Symbol ist abgeschlossen
      break;
    else if (c == '=' || c == '/' || c == '>' || c == '?' || c == '|' ||
	     c == ')' || c == '\'' || c == ',' || c == ';') {
      // Symbol ist abgeschlossen, das gelesene Zeichen wird 
      // aber noch benoetigt
      putback (c);
      break;
    }
    else if (isalnum (c) || c == '-' || (c == '_' && elem.length () > 0))
      // korrektes Zeichen -> anhaengen
//      elem += tolower (c);   ?????????
      elem += c;
    else {
      // Zeichen nicht erlaubt ?
      return Tok_Invalid;
    }
  }
  // alle Grossbuchstaben in Kleinbuchstaben aendern !!!!
  return Tok_Symbol;
}


XmlTokenizer::Token XmlTokenizer::readString () {
  KOXML_CHAR c;
  elem = "";

  while (1) {
    c = readchar ();
    if (c == '\n')
      lineno++;

    if (istrm.eof ())
      // String ist noch nicht abgeschlossen
      return Tok_Invalid;
    else if (c == '\\') {
      // naechstes Zeichen quoten
    }
    else if (c == '"') {
      // String ist abgeschlossen
      return Tok_String;
    }
    else
      elem += c;
  }
}

XmlTokenizer::Token XmlTokenizer::readComment () {
  KOXML_CHAR c1, c2;
  elem = "";

  c1 = readchar ();
  if (c1 == '\n')
    lineno++;
  c2 = readchar ();
  if (c2 == '\n')
    lineno++;

  if (c1 != '-' || c2 != '-' || istrm.eof ())
    return Tok_Invalid;

  while (1) {
    c1 = readchar ();
    if (istrm.eof ())
      return Tok_Invalid;
    else if (c1 == '\n')
      lineno++;
    else if (c1 == '>')
      return Tok_Comment;
    else
      elem += c1;
  }
}

XmlTokenizer::Token XmlTokenizer::readText () {
  KOXML_CHAR c;
  elem = "";

  while (1) {
    c = readchar ();
    if (c == '\n')
      lineno++;

    if (istrm.eof ())
      return Tok_EOF;
    else if (c == '<') {
      putback (c);
      return Tok_Text;
    }
    else if (c == '&') {
      KOXML_STRING s;
      while (c != ';') {
	s += c;
	c = readchar ();
        if (c == '\n')
          lineno++;
	if (istrm.eof ())
	  return Tok_EOF;
      }
      if (s == "&lt")
	elem += "<";
      else if (s == "&gt")
	elem += ">";
      else if (s == "&amp")
	elem += "&";
      else if (s == "&lf")
	elem += "\r";
      else if (s == "&nl")
	elem += "\n";
//    entities ?
//      elem += "[" + s +";]";
    }
    else
      elem += c;
  }
}

#ifdef TEST
int main (int argc, char** argv) {
  XmlTokenizer::Token tok;

  XmlTokenizer tokenizer (cin);

  while ((tok = tokenizer.nextToken ()) != XmlTokenizer::Tok_EOF) {
    switch (tok) {
    case XmlTokenizer::Tok_Exclam:
      cout << "! ";
      break;
    case XmlTokenizer::Tok_Bar:
      cout << "| ";
      break;
    case XmlTokenizer::Tok_LParen:
      cout << "( ";
      break;
    case XmlTokenizer::Tok_RParen:
      cout << ") ";
      break;
    case XmlTokenizer::Tok_LBracket:
      cout << "[ ";
      break;
    case XmlTokenizer::Tok_RBracket:
      cout << "] ";
      break;
    case XmlTokenizer::Tok_Plus:
      cout << "+ ";
      break;
    case XmlTokenizer::Tok_Asterisk:
      cout << "* ";
      break;
    case XmlTokenizer::Tok_Comma:
      cout << ", ";
      break;
    case XmlTokenizer::Tok_Semicolon:
      cout << "; ";
      break;
    case XmlTokenizer::Tok_NSign:
      cout << "# ";
      break;
    case XmlTokenizer::Tok_Apostr:
      cout << "' ";
      break;
    case XmlTokenizer::Tok_Percent:
      cout << "% ";
      break;
    case XmlTokenizer::Tok_Lt:
      cout << "< ";
      break;
    case XmlTokenizer::Tok_Gt:
      cout << "> ";
      break;
    case XmlTokenizer::Tok_QSign:
      cout << "? ";
      break;
    case XmlTokenizer::Tok_Slash:
      cout << "/ ";
      break;
    case XmlTokenizer::Tok_Eq:
      cout << "= ";
      break;
    case XmlTokenizer::Tok_Symbol:
      cout << "SYMBOL(" << tokenizer.element () << ") ";
      break;
    case XmlTokenizer::Tok_String:
      cout << "STRING(" << tokenizer.element () << ") ";
      break;
    case XmlTokenizer::Tok_Comment:
      cout << "COMMENT > ";
      break;
    case XmlTokenizer::Tok_Text:
      cout << "TEXT(" << tokenizer.element () << ") ";
      break;
    default:
      cout << "INVALID(" << tok << ")" << endl;
      return 1;
      break;
    }
  }
}
#endif
