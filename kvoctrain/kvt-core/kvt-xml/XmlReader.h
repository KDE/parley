/* -*- C++ -*-

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99
*/

#ifndef XmlReader_h_
#define XmlReader_h_

#include "XmlElement.h"
#include "XmlTokenizer.h"

class KOXML_ISTREAM;

/**
 * The XMLReader class supports reading elements from a XML stream.
 *
 * @short     A class for reading XML elements from a stream.
 * @author    Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de), modifications: Ewald Arnold (kvoctrain@ewald-arnold.de)
 * @version   2000/07/02
 */
class XmlReader {
public:
  /**
   * Construct a XmlReader instance for the given input stream.
   *
   * @param is   The open input stream.
   */
  XmlReader (KOXML_ISTREAM& is);

  /**
   * Desctructor
   */
  ~XmlReader ();

  /**
   * Check the input stream for a valid XML header.
   * A header should look like
   * <pre>
   *  <?xml version="1.0"?>
   *  <!doctype dtype system dtd>
   * </pre>
   * where @p dtype and @p dtd are simple strings.
   *
   * @return  @p true if the header conforms to XML, otherwise
   *          @p false.
   */
  bool validHeader ();

  /**
   * Return the document type.
   *
   * @return   The name of the document type.
   */
  const KOXML_STRING& doctype () const;

  /**
   * Return the name of the document type definition.
   *
   * @return   The name of the DTD.
   */
  const KOXML_STRING& dtd () const;

  /**
   * Read a XML element from the stream. If the content is plain text
   * (no tag), an element with the pseudo ID @p #PCDATA is returned
   * and the text is available via method getText.
   *
   * @see #getText
   *
   * @param elem    The XML element which is initialized by the method.
   * @return        @p true for successful reading.
   */
  bool readElement (XmlElement& elem);

  /**
   * Read plain text from the stream.
   *
   * @return The text as a string.
   */
  const KOXML_STRING& getText ();

  /**
   * Returns current line number
   *
   * @return The current line number
   */
  inline int lineNumber() { return tokenizer.lineNumber(); }

protected:
  bool parseEndElement (XmlElement& elem);
  bool parseElement (const KOXML_STRING& id, XmlElement& elem);
  bool readAttributes (std::list<XmlAttribute>& attrib_list);

private:
  XmlTokenizer tokenizer;
  KOXML_STRING s_dtype,
               s_dtd;
  KOXML_STRING text;
};

#endif
