#include <qtextstream.h>
#include <qstring.h>

///////////////////////////////////////////////////////////
//
// (c) Kevin Krammer <kevin.krammer@gmx.at>
//
// 2001-12-08
//
// automatically extracts a structure from
//   http://www.loc.gov/standards/iso639-2/englangn.html
//
// see also extractlang.README
//
///////////////////////////////////////////////////////////

QString twoLangs;
QString threeLangs;

void parseLanguage(QString name, QString three, QString two)
{
  int star = three.find('*');
  if (star > 0)
    three.remove(star, 1);

  QString three_1 = three;

  int slash = three_1.find('/');
  if (slash > 0) {
    three_1.remove(slash, 1);
    three_1.insert(slash, ", ");
  }

  if (!two.contains("&nbsp;")) {
      twoLangs.append("\t{{\"");
      twoLangs.append(two);
      twoLangs.append("\"}, {\"");
      twoLangs.append(three_1);
      twoLangs.append("\"}, {I18N_NOOP(\"");
      twoLangs.append(name);
      twoLangs.append("\")}},\n");
  }
  else
    two = "";

  slash = three.find('/');
  if (slash > 0) {
    if (two.length() == 0)
      two = three.mid(slash+1);
    three.remove(slash, three.length());
  }

  threeLangs.append("\t{{\"");
  threeLangs.append(two);
  threeLangs.append("\"}, {\"");
  threeLangs.append(three);
  threeLangs.append("\"}, {I18N_NOOP(\"");
  threeLangs.append(name);
  threeLangs.append("\")}},\n");
}

QString parseTD(QString& line)
{
  QCString cline = line.local8Bit();
  uint clineLength = cline.length();
  QCString result = cline;
  int pos = -1;
  bool inTag = false;

  for (uint i = 0; i < clineLength; ++i)
    {
      if (cline[i] == '<')
	inTag = true;
      else if (cline[i] == '>')
	inTag = false;
      else if (!inTag)
	{
	  pos++;
	  result[pos] = cline[i];
	}
    }

  QString retVal(result);
  retVal = retVal.left(pos+1);

  return retVal.simplifyWhiteSpace();
}

bool parseTR(QString& line, QTextIStream& in)
{
  QString text[4];

  line = in.readLine();
  if (line.isNull())
    return true;


  uint count = 0;
  while (line.contains("<td"))
    { 
      if (count > 3)
	return true;

      text[count] = parseTD(line);
      count++;
      line = in.readLine();
    }

  if (count == 4)
    {
      parseLanguage(text[0], text[2], text[3]);
    }

  if (line.contains("/tr"))
    return true;

  return false;
}

int main()
{
  QString line;
  bool readNext = true;

  QTextIStream in(stdin);
  QTextOStream out(stdout);

  while (!in.atEnd())
    {
      if (readNext)
	line = in.readLine();

      if (line.contains("<tr"))
	{
	  readNext = parseTR(line, in);
	}
      else
	readNext = true;
    }

  out << "struct KV_ISO639_Code {\n"
         "   const char *iso1code;\n"
         "   const char *iso2code;\n"
         "   const char *langname;\n"
         "};\n"
         "\n";

  out << "KV_ISO639_Code kv_iso639_1[] = {\n"
      << twoLangs.left(twoLangs.length()-1) << endl
      << "\t{{0}, {0}, {0}}\n"
      << "};\n";

  out << endl;

  out << "KV_ISO639_Code kv_iso639_2[] = {\n"
      << threeLangs.left(threeLangs.length()-1) << endl
      <<"\t{{0}, {0}, {0}}\n"
      << "};\n";

  return 0;
}
