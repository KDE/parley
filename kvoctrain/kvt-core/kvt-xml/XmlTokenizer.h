/* -*- C++ -*-

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99

*/

#ifndef XmlTokenizer_h_
#define XmlTokenizer_h_

#include "koxml_config.h"

/**
 * The XMLTokenizer class allows an application to break a XML stream
 * into tokens.
 *
 * @short     A class for tokenizing an XML stream.
 * @author    Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de), modifications: Ewald Arnold (kvoctrain@ewald-arnold.de)
 * @version   2000/07/02
 */
class XmlTokenizer {
public:
  /**
   * The expected tokens for a XML stream.
   */
  enum Token { Tok_Invalid,  /*   0 */
	       Tok_EOF,      /*   1 */
	       Tok_Symbol,   /*   2 */
	       Tok_String,   /*   3 */
	       Tok_Text,     /*   4 */
	       Tok_Comment,  /*   5 */
	       Tok_Lt,       /* < 6 */
	       Tok_Gt,       /* > 7 */
	       Tok_QSign,    /* ? 8 */
	       Tok_Eq,       /* = 9 */
	       Tok_Slash,    /* / 10 */
	       Tok_Exclam,   /* ! 11 */
	       Tok_Bar,      /* | 12 */
	       Tok_LParen,   /* ( 13 */
	       Tok_RParen,   /* ) 14 */
	       Tok_LBracket, /* [ 15 */
	       Tok_RBracket, /* ] 16 */
	       Tok_Plus,     /* + 17 */
	       Tok_Asterisk, /* * 18 */
	       Tok_Comma,    /* , 19 */
	       Tok_Semicolon,/* ; 20 */
	       Tok_NSign,    /* # 21 */
	       Tok_Apostr,   /* ' 22 */
	       Tok_Percent   /* % 23 */
	       };

  /**
   * Create a XmlTokenizer instance for the given input stream.
   *
   * @param is   The open input stream for reading.
   */
  XmlTokenizer (KOXML_ISTREAM& is);

  /**
   * Destructor.
   */
  ~XmlTokenizer ();

  /**
    * Return the next token from the stream.
   * @return       The next token from the stream.
   */
  Token nextToken ();

  /**
   * Return the string representation of the current token.
   *
   * @return  The string representation.
   */
  const KOXML_STRING& element ();

  KOXML_CHAR readchar ();
  void putback (KOXML_CHAR c);

  /**
   * Cause the next call to method @p nextToken of this tokenizer
   * to return the current token.
   */
  void unget ();

  inline int lineNumber() { return lineno; }

protected:
  void  skipWhitespace ();
  Token readString ();
  Token readSymbol ();
  Token readText ();
  Token readComment ();

private:
//  QIODevice *strm;
  KOXML_STRING   last_chars;
  KOXML_ISTREAM &istrm;
  KOXML_STRING   elem;
  Token          last_tok;
  bool           use_last;
  bool           is_open;
  int            lineno;
};

#endif
