/* -*- C++ -*-

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99

*/

#ifndef XmlWriter_h_
#define XmlWriter_h_

#if defined(stack)
#undef stack
#endif

#include <stack>
#include <vector>
using namespace std;

#include "koxml_config.h"

class KOXML_OSTREAM;
/**
 * The XMLWriter class provides support for writing XML streams.
 * It contains methods for output XML elements with attributes.
 *
 * Sample code:
 * <pre>
 *  KOXML_OSTREAM os (fname);
 *  XmlWriter xml (os); // writes the XML header
 *
 *  xml.startTag ("head"); // writes &lt;head&gt;
 *
 *  // write &lt;layout format="a4" orientation="landscape"&gt;
 *  xml.startTag ("layout", false);
 *  xml.addAttribute ("format", "a4");
 *  xml.addAttribute ("orientation", "landscape");
 *  xml.closeTag (true);
 * </pre>
 *
 * @short     A helper class for writing XML.
 * @author    Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de), modifications: Ewald Arnold (kvoctrain@ewald-arnold.de)
 * @version   2000/07/02

 */
class XmlWriter {
public:
  /**
   * Create a XmlWriter instance for the given output stream.
   *
   * @param os       The open output stream for writing.
   */
  XmlWriter (KOXML_OSTREAM& os);

  /**
   * Desctructor.
   */
  ~XmlWriter ();

  /**
   * Write an element with the given ID to the XML stream. Attributes
   * can be added later.
   *
   * @param id       The element ID.
   * @param closeIt  If @p true the tag is closed by >,
   *                 otherwise not.
   * @param empty    If @p true an empty element is written, which
   *                 is closed by />.
   * @param eol      If @p true an eol char is appended
   *                 even if autoendl is false
   */
  void startTag (KOXML_STRING id, bool closeIt = true, bool empty = false, bool eol = false);

  /**
   * Write the end tag according to the given element ID or to the
   * last opened element.
   *
   * @param id       The element ID. If @p "" the last opened
   *                 element is ended. (default).
   * @param eol      If @p true an eol char is appended
   *                 even if autoendl is false
   */
  void endTag (KOXML_STRING id = "", bool eol = false);

  /**
   * flag, indicating to automatically append end-of-line character after
   * closing bracket
   *
   * @param autoflag     If @p true, append eol character. Otherwise caller has to do it
   */
  void setAutoEndl (const bool autoflag = true);

  /**
   * append end-of-line char to stream
   */
  void endline ();

  /**
   * Close the current open element.
   *
   * @param empty    If @p true an empty element is closed.
   * @param eol      If @p true an eol char is appended
   *                 even if autoendl is false
   */
  void closeTag (bool empty = false, bool eol = false);

  /**
   * Add an attribute with the given value to the current element.
   * This method doesn't check, if an element is open.
   *
   * @param name     The attribute name.
   * @param value    The string value of the attribute.
   */
  void addAttribute (KOXML_STRING name, const KOXML_STRING& value);

  /**
   * Add an attribute with the given value to the current element.
   * This method doesn't check, if an element is open.
   *
   * @param name     The attribute name.
   * @param value    The integer value of the attribute.
   */
  void addAttribute (KOXML_STRING name, int value);

  /**
   * Add an attribute with the given value to the current element.
   * This method doesn't check, if an element is open.
   *
   * @param name     The attribute name.
   * @param value    The float value od the attribute.
   */
  void addAttribute (KOXML_STRING name, float value);

  /**
   * Add an attribute with the given value to the current element.
   * This method doesn't check, if an element is open.
   *
   * @param name     The attribute name.
   * @param value    The double value of the attribute.
   */
  void addAttribute (KOXML_STRING name, double value);

  /**
   * Write text to the XML stream. The method encodes the special
   * characters @p <, @p > and @p &.
   *
   * @param s        The text.
   */
  void writeText (KOXML_STRING s);

  /**
   * Write a tag to the XML stream.
   *
   * @param s        The tag without the brackets.
   */
  void writeTag (KOXML_STRING s);

  /**
   * indents next text string with some spaces
   *
   * @param i        number of spaces to indent
   */
  void indent (int i);

  /**
   * Flush the XML output stream.
   */
  void flush ();

#ifndef KOXML_USE_STL
  /**
   * Get the raw output stream.
   */
  inline QIODevice *stream () { return strm.device(); }
#endif

private:
  stack<KOXML_STRING, vector<KOXML_STRING> > lastTags;

  KOXML_OSTREAM &strm;
  bool           autoendl;
  bool           isapo;
  KOXML_CHAR     apo;
};

#endif
