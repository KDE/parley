#ifndef _SQLPARSER_H_
#define _SQLPARSER_H_
#include <db/field.h>
#include "parser.h"
#include "sqltypes.h"

bool parseData(KexiDB::Parser *p, const char *data);
/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SQLPARSER_TAB_H_INCLUDED
# define YY_YY_SQLPARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SQL_TYPE = 258,
     AS = 259,
     ASC = 260,
     AUTO_INCREMENT = 261,
     BIT = 262,
     BITWISE_SHIFT_LEFT = 263,
     BITWISE_SHIFT_RIGHT = 264,
     BY = 265,
     CHARACTER_STRING_LITERAL = 266,
     CONCATENATION = 267,
     CREATE = 268,
     DESC = 269,
     DISTINCT = 270,
     DOUBLE_QUOTED_STRING = 271,
     FROM = 272,
     JOIN = 273,
     KEY = 274,
     LEFT = 275,
     LESS_OR_EQUAL = 276,
     SQL_NULL = 277,
     SQL_IS = 278,
     SQL_IS_NULL = 279,
     SQL_IS_NOT_NULL = 280,
     ORDER = 281,
     PRIMARY = 282,
     SELECT = 283,
     INTEGER_CONST = 284,
     REAL_CONST = 285,
     RIGHT = 286,
     SQL_ON = 287,
     DATE_CONST = 288,
     DATETIME_CONST = 289,
     TIME_CONST = 290,
     TABLE = 291,
     IDENTIFIER = 292,
     IDENTIFIER_DOT_ASTERISK = 293,
     QUERY_PARAMETER = 294,
     VARCHAR = 295,
     WHERE = 296,
     SCAN_ERROR = 297,
     NOT_LIKE = 298,
     __LAST_TOKEN = 299,
     EXCEPT = 300,
     UNION = 301,
     INTERSECT = 302,
     OR = 303,
     XOR = 304,
     AND = 305,
     NOT = 306,
     GREATER_OR_EQUAL = 307,
     NOT_EQUAL2 = 308,
     NOT_EQUAL = 309,
     NOT_SIMILAR_TO = 310,
     SIMILAR_TO = 311,
     ILIKE = 312,
     LIKE = 313,
     SQL_IN = 314,
     BETWEEN = 315,
     UMINUS = 316
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2053 of yacc.c  */
#line 513 "sqlparser.y"

	QString* stringValue;
	qint64 integerValue;
	bool booleanValue;
	struct realType realValue;
	KexiDB::Field::Type colType;
	KexiDB::Field *field;
	KexiDB::BaseExpr *expr;
	KexiDB::NArgExpr *exprList;
	KexiDB::ConstExpr *constExpr;
	KexiDB::QuerySchema *querySchema;
	SelectOptionsInternal *selectOptions;
	OrderByColumnInternal::List *orderByColumns;
	QVariant *variantValue;


/* Line 2053 of yacc.c  */
#line 135 "sqlparser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_SQLPARSER_TAB_H_INCLUDED  */
#endif
