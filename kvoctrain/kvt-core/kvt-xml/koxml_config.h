//#define KOXML_USE_STL

/*
  -----------------------------------------------------------------------

  -----------------------------------------------------------------------
*/

#ifndef KOXML_USE_STL

# include <qtextstream.h>
# define KOXML_ISTREAM QTextStream
# define KOXML_OSTREAM QTextStream

# include <qstring.h>
# define KOXML_STRING  QString

// Unicode character
# define KOXML_CHAR QChar

# define KOXML_STRING_REMOVE( string_var, pos, count) string_var.remove (pos, count)
# define KOXML_STRING_INSERT( string_var, pos, str)   string_var.insert(pos, str)

#else

# include <iostream>
using namespace std;
# define KOXML_ISTREAM istream
# define KOXML_OSTREAM ostream

# define KOXML_CHAR char    // evtl. wchar

////////////////////////////////////////////////////////
//evtl. an MFC o.ä. anpassen
#ifdef __MFC_STRING_BENUTZT__

#else

# include <string>
# define KOXML_STRING  string // evtl. wstring

# define KOXML_STRING_REMOVE( string_var, pos, count) string_var.erase (pos, count)
# define KOXML_STRING_INSERT( string_var, pos, str)   string_var.insert(pos, (string)str)

#endif

#endif
