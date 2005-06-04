/* -*- C++ -*-

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  modified for kvoctrain by Ewald Arnold kvoctrain@ewald-arnold.dein April ´99

*/

#ifndef XmlElement_h_
#define XmlElement_h_

#include "koxml_config.h"
#include <list>

/**
 * An instance of XmlAttribute represents an attribute of
 * XML elements. It provides methods for accessing the attribute
 * value.
 *
 * @short     A class for representing attributes of XML elements.
 * @author    Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de), modifications: Ewald Arnold (kvoctrain@ewald-arnold.de)
 * @version   2000/07/02
 */
class XmlAttribute {
public:
  /**
   * Construct an attribute from the given name and value.
   *
   * @param n   The name of the attribute.
   * @param v   The attribute value.
   */
  XmlAttribute (const KOXML_STRING& n, const KOXML_STRING& v);

  /**
   * Copy constructor.
   */
  XmlAttribute (const XmlAttribute& attr);

  /**
   * Destructor.
   */
  ~XmlAttribute ();

  /**
   * The assignment operator.
   */
  XmlAttribute& operator= (const XmlAttribute& attr);

  /**
   * Return the name of the attribute.
   *
   * @return The attribute name.
   */
  inline const KOXML_STRING& name () const { return aname; }

  /**
   * Return a string representation of the attribute value.
   *
   * @return The attribute value as string.
   */
  inline const KOXML_STRING& stringValue () const { return value; }

  /**
   * Return a float representation of the attribute value.
   *
   * @return The attribute value as float.
   */
  float floatValue () const;

  /**
   * Return a integer representation of the attribute value.
   *
   * @return The attribute value as integer.
   */
  int intValue () const;

private:
  KOXML_STRING aname, value;
};

/**
 * An instance of XmlElement represents an element (object) of a XML
 * document. The elements consists of the tag (element ID) and a list
 * of attributes.
 *
 * @short     A class for representing XML elements.
 * @author    Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de), modifications: Ewald Arnold (kvoctrain@ewald-arnold.de)
 * @version   2000/07/02
 */
class XmlElement {
friend class XmlReader;
public:
  /**
   * Construct a new XML element.
   */
  XmlElement ();

  /**
   * Copy constructor.
   */
  XmlElement (const XmlElement& elem);

  /**
   * Destructor.
   */
  ~XmlElement ();

  void reset ();

  /**
   * The assignment operator.
   */
  XmlElement& operator= (const XmlElement& elem);

  /**
   * Return the ID of the element.
   *
   * @return The element ID.
   */
  inline const KOXML_STRING& tag () const { return tagId; }

  /**
   * Return the attributes of the element.
   * @see XmlAttribute
   *
   * @return The list of attributes.
   */
  inline const std::list<XmlAttribute>& attributes () const { return attribs; }

  /**
   * Return @p true if the element is closed.
   *
   * @return @p true for a closed element.
   */
  inline bool isClosed () const { return closed; }

  /**
   * Return @p true if the element is an end tag.
   *
   * @return @p true for an end tag.
   */
  inline bool isEndTag () const { return endTag; }

private:
  KOXML_STRING       tagId;
  bool               closed;
  bool               endTag;
  std::list<XmlAttribute> attribs;
};

#endif




