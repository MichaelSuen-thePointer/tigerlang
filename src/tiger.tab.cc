// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "tiger.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "tiger.tab.hh"

// User implementation prologue.

#line 51 "tiger.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 26 "tiger.yy" // lalr1.cc:413


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>
#include "TigerScanner.hpp"

#undef yylex
#define yylex scanner.yylex


#line 66 "tiger.tab.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "tiger.yy" // lalr1.cc:479
namespace tiger {
#line 152 "tiger.tab.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  TigerParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  TigerParser::TigerParser (tiger::TigerScanner& scanner_yyarg, std::unique_ptr<Program>& prog_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      prog (prog_yyarg)
  {}

  TigerParser::~TigerParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  TigerParser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  TigerParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  TigerParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 58: // assignment-expression
        value.copy< Assignment* > (other.value);
        break;

      case 72: // declaration
        value.copy< Declaration* > (other.value);
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        value.copy< Expression* > (other.value);
        break;

      case 53: // for
        value.copy< For* > (other.value);
        break;

      case 77: // function-declaration
        value.copy< FunctionDeclaration* > (other.value);
        break;

      case 52: // if-then
        value.copy< IfThen* > (other.value);
        break;

      case 51: // if-then-else
        value.copy< IfThenElse* > (other.value);
        break;

      case 55: // let-expression
        value.copy< Let* > (other.value);
        break;

      case 75: // record-type
        value.copy< RecordType* > (other.value);
        break;

      case 74: // type
        value.copy< Type* > (other.value);
        break;

      case 73: // type-declaration
        value.copy< TypeDeclaration* > (other.value);
        break;

      case 80: // variable-declaration
        value.copy< VariableDeclaration* > (other.value);
        break;

      case 54: // while
        value.copy< While* > (other.value);
        break;

      case 44: // INT
        value.copy< int > (other.value);
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        value.copy< std::string > (other.value);
        break;

      case 71: // space-separated-declaration
        value.copy< std::vector<Declaration*> > (other.value);
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        value.copy< std::vector<Expression*> > (other.value);
        break;

      case 76: // comma-separated-field
        value.copy< std::vector<FieldDeclaration> > (other.value);
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        value.copy< std::vector<FunctionParameter> > (other.value);
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        value.copy< std::vector<std::tuple<std::string, Expression*, tiger::location>> > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 58: // assignment-expression
        value.copy< Assignment* > (v);
        break;

      case 72: // declaration
        value.copy< Declaration* > (v);
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        value.copy< Expression* > (v);
        break;

      case 53: // for
        value.copy< For* > (v);
        break;

      case 77: // function-declaration
        value.copy< FunctionDeclaration* > (v);
        break;

      case 52: // if-then
        value.copy< IfThen* > (v);
        break;

      case 51: // if-then-else
        value.copy< IfThenElse* > (v);
        break;

      case 55: // let-expression
        value.copy< Let* > (v);
        break;

      case 75: // record-type
        value.copy< RecordType* > (v);
        break;

      case 74: // type
        value.copy< Type* > (v);
        break;

      case 73: // type-declaration
        value.copy< TypeDeclaration* > (v);
        break;

      case 80: // variable-declaration
        value.copy< VariableDeclaration* > (v);
        break;

      case 54: // while
        value.copy< While* > (v);
        break;

      case 44: // INT
        value.copy< int > (v);
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        value.copy< std::string > (v);
        break;

      case 71: // space-separated-declaration
        value.copy< std::vector<Declaration*> > (v);
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        value.copy< std::vector<Expression*> > (v);
        break;

      case 76: // comma-separated-field
        value.copy< std::vector<FieldDeclaration> > (v);
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        value.copy< std::vector<FunctionParameter> > (v);
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        value.copy< std::vector<std::tuple<std::string, Expression*, tiger::location>> > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Assignment* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Declaration* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Expression* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const For* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const FunctionDeclaration* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const IfThen* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const IfThenElse* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Let* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const RecordType* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Type* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const TypeDeclaration* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const VariableDeclaration* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const While* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<Declaration*> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<Expression*> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<FieldDeclaration> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<FunctionParameter> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  TigerParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<std::tuple<std::string, Expression*, tiger::location>> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  TigerParser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  TigerParser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 58: // assignment-expression
        value.template destroy< Assignment* > ();
        break;

      case 72: // declaration
        value.template destroy< Declaration* > ();
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        value.template destroy< Expression* > ();
        break;

      case 53: // for
        value.template destroy< For* > ();
        break;

      case 77: // function-declaration
        value.template destroy< FunctionDeclaration* > ();
        break;

      case 52: // if-then
        value.template destroy< IfThen* > ();
        break;

      case 51: // if-then-else
        value.template destroy< IfThenElse* > ();
        break;

      case 55: // let-expression
        value.template destroy< Let* > ();
        break;

      case 75: // record-type
        value.template destroy< RecordType* > ();
        break;

      case 74: // type
        value.template destroy< Type* > ();
        break;

      case 73: // type-declaration
        value.template destroy< TypeDeclaration* > ();
        break;

      case 80: // variable-declaration
        value.template destroy< VariableDeclaration* > ();
        break;

      case 54: // while
        value.template destroy< While* > ();
        break;

      case 44: // INT
        value.template destroy< int > ();
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        value.template destroy< std::string > ();
        break;

      case 71: // space-separated-declaration
        value.template destroy< std::vector<Declaration*> > ();
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        value.template destroy< std::vector<Expression*> > ();
        break;

      case 76: // comma-separated-field
        value.template destroy< std::vector<FieldDeclaration> > ();
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        value.template destroy< std::vector<FunctionParameter> > ();
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        value.template destroy< std::vector<std::tuple<std::string, Expression*, tiger::location>> > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  TigerParser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  TigerParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 58: // assignment-expression
        value.move< Assignment* > (s.value);
        break;

      case 72: // declaration
        value.move< Declaration* > (s.value);
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        value.move< Expression* > (s.value);
        break;

      case 53: // for
        value.move< For* > (s.value);
        break;

      case 77: // function-declaration
        value.move< FunctionDeclaration* > (s.value);
        break;

      case 52: // if-then
        value.move< IfThen* > (s.value);
        break;

      case 51: // if-then-else
        value.move< IfThenElse* > (s.value);
        break;

      case 55: // let-expression
        value.move< Let* > (s.value);
        break;

      case 75: // record-type
        value.move< RecordType* > (s.value);
        break;

      case 74: // type
        value.move< Type* > (s.value);
        break;

      case 73: // type-declaration
        value.move< TypeDeclaration* > (s.value);
        break;

      case 80: // variable-declaration
        value.move< VariableDeclaration* > (s.value);
        break;

      case 54: // while
        value.move< While* > (s.value);
        break;

      case 44: // INT
        value.move< int > (s.value);
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        value.move< std::string > (s.value);
        break;

      case 71: // space-separated-declaration
        value.move< std::vector<Declaration*> > (s.value);
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        value.move< std::vector<Expression*> > (s.value);
        break;

      case 76: // comma-separated-field
        value.move< std::vector<FieldDeclaration> > (s.value);
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        value.move< std::vector<FunctionParameter> > (s.value);
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        value.move< std::vector<std::tuple<std::string, Expression*, tiger::location>> > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  TigerParser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  TigerParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  TigerParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  TigerParser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  TigerParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  TigerParser::by_type::type_get () const
  {
    return type;
  }
  // Implementation of make_symbol for each symbol type.
  TigerParser::symbol_type
  TigerParser::make_LPAREN (const location_type& l)
  {
    return symbol_type (token::LPAREN, l);
  }

  TigerParser::symbol_type
  TigerParser::make_RPAREN (const location_type& l)
  {
    return symbol_type (token::RPAREN, l);
  }

  TigerParser::symbol_type
  TigerParser::make_LBRACKET (const location_type& l)
  {
    return symbol_type (token::LBRACKET, l);
  }

  TigerParser::symbol_type
  TigerParser::make_RBRACKET (const location_type& l)
  {
    return symbol_type (token::RBRACKET, l);
  }

  TigerParser::symbol_type
  TigerParser::make_LBRACE (const location_type& l)
  {
    return symbol_type (token::LBRACE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_RBRACE (const location_type& l)
  {
    return symbol_type (token::RBRACE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_ASSIGN (const location_type& l)
  {
    return symbol_type (token::ASSIGN, l);
  }

  TigerParser::symbol_type
  TigerParser::make_COLON (const location_type& l)
  {
    return symbol_type (token::COLON, l);
  }

  TigerParser::symbol_type
  TigerParser::make_DOT (const location_type& l)
  {
    return symbol_type (token::DOT, l);
  }

  TigerParser::symbol_type
  TigerParser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::COMMA, l);
  }

  TigerParser::symbol_type
  TigerParser::make_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::SEMICOLON, l);
  }

  TigerParser::symbol_type
  TigerParser::make_TIMES (const location_type& l)
  {
    return symbol_type (token::TIMES, l);
  }

  TigerParser::symbol_type
  TigerParser::make_DIVIDE (const location_type& l)
  {
    return symbol_type (token::DIVIDE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_PLUS (const location_type& l)
  {
    return symbol_type (token::PLUS, l);
  }

  TigerParser::symbol_type
  TigerParser::make_MINUS (const location_type& l)
  {
    return symbol_type (token::MINUS, l);
  }

  TigerParser::symbol_type
  TigerParser::make_EQ (const location_type& l)
  {
    return symbol_type (token::EQ, l);
  }

  TigerParser::symbol_type
  TigerParser::make_NEQ (const location_type& l)
  {
    return symbol_type (token::NEQ, l);
  }

  TigerParser::symbol_type
  TigerParser::make_GT (const location_type& l)
  {
    return symbol_type (token::GT, l);
  }

  TigerParser::symbol_type
  TigerParser::make_LT (const location_type& l)
  {
    return symbol_type (token::LT, l);
  }

  TigerParser::symbol_type
  TigerParser::make_GE (const location_type& l)
  {
    return symbol_type (token::GE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_LE (const location_type& l)
  {
    return symbol_type (token::LE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_AND (const location_type& l)
  {
    return symbol_type (token::AND, l);
  }

  TigerParser::symbol_type
  TigerParser::make_OR (const location_type& l)
  {
    return symbol_type (token::OR, l);
  }

  TigerParser::symbol_type
  TigerParser::make_ARRAY (const location_type& l)
  {
    return symbol_type (token::ARRAY, l);
  }

  TigerParser::symbol_type
  TigerParser::make_BREAK (const location_type& l)
  {
    return symbol_type (token::BREAK, l);
  }

  TigerParser::symbol_type
  TigerParser::make_DO (const location_type& l)
  {
    return symbol_type (token::DO, l);
  }

  TigerParser::symbol_type
  TigerParser::make_ELSE (const location_type& l)
  {
    return symbol_type (token::ELSE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);
  }

  TigerParser::symbol_type
  TigerParser::make_FOR (const location_type& l)
  {
    return symbol_type (token::FOR, l);
  }

  TigerParser::symbol_type
  TigerParser::make_FUNCTION (const location_type& l)
  {
    return symbol_type (token::FUNCTION, l);
  }

  TigerParser::symbol_type
  TigerParser::make_IF (const location_type& l)
  {
    return symbol_type (token::IF, l);
  }

  TigerParser::symbol_type
  TigerParser::make_IN (const location_type& l)
  {
    return symbol_type (token::IN, l);
  }

  TigerParser::symbol_type
  TigerParser::make_LET (const location_type& l)
  {
    return symbol_type (token::LET, l);
  }

  TigerParser::symbol_type
  TigerParser::make_NIL (const location_type& l)
  {
    return symbol_type (token::NIL, l);
  }

  TigerParser::symbol_type
  TigerParser::make_OF (const location_type& l)
  {
    return symbol_type (token::OF, l);
  }

  TigerParser::symbol_type
  TigerParser::make_THEN (const location_type& l)
  {
    return symbol_type (token::THEN, l);
  }

  TigerParser::symbol_type
  TigerParser::make_TO (const location_type& l)
  {
    return symbol_type (token::TO, l);
  }

  TigerParser::symbol_type
  TigerParser::make_TYPE (const location_type& l)
  {
    return symbol_type (token::TYPE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_VAR (const location_type& l)
  {
    return symbol_type (token::VAR, l);
  }

  TigerParser::symbol_type
  TigerParser::make_WHILE (const location_type& l)
  {
    return symbol_type (token::WHILE, l);
  }

  TigerParser::symbol_type
  TigerParser::make_UNKNOWN (const location_type& l)
  {
    return symbol_type (token::UNKNOWN, l);
  }

  TigerParser::symbol_type
  TigerParser::make_INT (const int& v, const location_type& l)
  {
    return symbol_type (token::INT, v, l);
  }

  TigerParser::symbol_type
  TigerParser::make_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::STRING, v, l);
  }

  TigerParser::symbol_type
  TigerParser::make_IDENTIFIER (const std::string& v, const location_type& l)
  {
    return symbol_type (token::IDENTIFIER, v, l);
  }



  // by_state.
  inline
  TigerParser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  TigerParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  TigerParser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  TigerParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  TigerParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  TigerParser::symbol_number_type
  TigerParser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  TigerParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  TigerParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 58: // assignment-expression
        value.move< Assignment* > (that.value);
        break;

      case 72: // declaration
        value.move< Declaration* > (that.value);
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        value.move< Expression* > (that.value);
        break;

      case 53: // for
        value.move< For* > (that.value);
        break;

      case 77: // function-declaration
        value.move< FunctionDeclaration* > (that.value);
        break;

      case 52: // if-then
        value.move< IfThen* > (that.value);
        break;

      case 51: // if-then-else
        value.move< IfThenElse* > (that.value);
        break;

      case 55: // let-expression
        value.move< Let* > (that.value);
        break;

      case 75: // record-type
        value.move< RecordType* > (that.value);
        break;

      case 74: // type
        value.move< Type* > (that.value);
        break;

      case 73: // type-declaration
        value.move< TypeDeclaration* > (that.value);
        break;

      case 80: // variable-declaration
        value.move< VariableDeclaration* > (that.value);
        break;

      case 54: // while
        value.move< While* > (that.value);
        break;

      case 44: // INT
        value.move< int > (that.value);
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        value.move< std::string > (that.value);
        break;

      case 71: // space-separated-declaration
        value.move< std::vector<Declaration*> > (that.value);
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        value.move< std::vector<Expression*> > (that.value);
        break;

      case 76: // comma-separated-field
        value.move< std::vector<FieldDeclaration> > (that.value);
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        value.move< std::vector<FunctionParameter> > (that.value);
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        value.move< std::vector<std::tuple<std::string, Expression*, tiger::location>> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  TigerParser::stack_symbol_type&
  TigerParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 58: // assignment-expression
        value.copy< Assignment* > (that.value);
        break;

      case 72: // declaration
        value.copy< Declaration* > (that.value);
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        value.copy< Expression* > (that.value);
        break;

      case 53: // for
        value.copy< For* > (that.value);
        break;

      case 77: // function-declaration
        value.copy< FunctionDeclaration* > (that.value);
        break;

      case 52: // if-then
        value.copy< IfThen* > (that.value);
        break;

      case 51: // if-then-else
        value.copy< IfThenElse* > (that.value);
        break;

      case 55: // let-expression
        value.copy< Let* > (that.value);
        break;

      case 75: // record-type
        value.copy< RecordType* > (that.value);
        break;

      case 74: // type
        value.copy< Type* > (that.value);
        break;

      case 73: // type-declaration
        value.copy< TypeDeclaration* > (that.value);
        break;

      case 80: // variable-declaration
        value.copy< VariableDeclaration* > (that.value);
        break;

      case 54: // while
        value.copy< While* > (that.value);
        break;

      case 44: // INT
        value.copy< int > (that.value);
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        value.copy< std::string > (that.value);
        break;

      case 71: // space-separated-declaration
        value.copy< std::vector<Declaration*> > (that.value);
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        value.copy< std::vector<Expression*> > (that.value);
        break;

      case 76: // comma-separated-field
        value.copy< std::vector<FieldDeclaration> > (that.value);
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        value.copy< std::vector<FunctionParameter> > (that.value);
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        value.copy< std::vector<std::tuple<std::string, Expression*, tiger::location>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  TigerParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  TigerParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  TigerParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  TigerParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  TigerParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  TigerParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  TigerParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  TigerParser::debug_level_type
  TigerParser::debug_level () const
  {
    return yydebug_;
  }

  void
  TigerParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline TigerParser::state_type
  TigerParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  TigerParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  TigerParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  TigerParser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 58: // assignment-expression
        yylhs.value.build< Assignment* > ();
        break;

      case 72: // declaration
        yylhs.value.build< Declaration* > ();
        break;

      case 49: // breakable-expression
      case 50: // expression
      case 59: // logical-or-expression
      case 60: // logical-and-expression
      case 61: // equality-expression
      case 62: // additive-expression
      case 63: // multiplicative-expression
      case 64: // unary-expression
      case 65: // postfix-expression
      case 66: // lvalue-expression
        yylhs.value.build< Expression* > ();
        break;

      case 53: // for
        yylhs.value.build< For* > ();
        break;

      case 77: // function-declaration
        yylhs.value.build< FunctionDeclaration* > ();
        break;

      case 52: // if-then
        yylhs.value.build< IfThen* > ();
        break;

      case 51: // if-then-else
        yylhs.value.build< IfThenElse* > ();
        break;

      case 55: // let-expression
        yylhs.value.build< Let* > ();
        break;

      case 75: // record-type
        yylhs.value.build< RecordType* > ();
        break;

      case 74: // type
        yylhs.value.build< Type* > ();
        break;

      case 73: // type-declaration
        yylhs.value.build< TypeDeclaration* > ();
        break;

      case 80: // variable-declaration
        yylhs.value.build< VariableDeclaration* > ();
        break;

      case 54: // while
        yylhs.value.build< While* > ();
        break;

      case 44: // INT
        yylhs.value.build< int > ();
        break;

      case 45: // STRING
      case 46: // IDENTIFIER
      case 81: // nullable-colon-type
        yylhs.value.build< std::string > ();
        break;

      case 71: // space-separated-declaration
        yylhs.value.build< std::vector<Declaration*> > ();
        break;

      case 56: // semicolon-separated-expression
      case 57: // nonempty-semicolon-separated-expression
      case 69: // comma-separated-expression
      case 70: // nonempty-comma-separated-expression
        yylhs.value.build< std::vector<Expression*> > ();
        break;

      case 76: // comma-separated-field
        yylhs.value.build< std::vector<FieldDeclaration> > ();
        break;

      case 78: // comma-separated-parameter
      case 79: // nonempty-comma-separated-parameter
        yylhs.value.build< std::vector<FunctionParameter> > ();
        break;

      case 67: // comma-separated-record-field
      case 68: // nonempty-comma-separated-record-field
        yylhs.value.build< std::vector<std::tuple<std::string, Expression*, tiger::location>> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 85 "tiger.yy" // lalr1.cc:859
    {
        prog.reset(new Program(yystack_[0].value.as< Expression* > ()));
        prog->loc(yystack_[0].location);
    }
#line 1754 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 91 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Break(); yylhs.value.as< Expression* > ()->loc(yystack_[0].location); }
#line 1760 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 92 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1766 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 95 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< IfThenElse* > (); }
#line 1772 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 96 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< IfThen* > (); }
#line 1778 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 97 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< While* > (); }
#line 1784 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 98 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< For* > (); }
#line 1790 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 99 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Let* > (); }
#line 1796 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 100 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Assignment* > (); }
#line 1802 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 101 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1808 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 104 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< IfThenElse* > () = new IfThenElse(yystack_[4].value.as< Expression* > (), yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ());
        yylhs.value.as< IfThenElse* > ()->loc(yystack_[5].location);
    }
#line 1817 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 110 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< IfThen* > () = new IfThen(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); }
#line 1823 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 113 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< For* > () = new For(yystack_[6].value.as< std::string > (), yystack_[4].value.as< Expression* > (), yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ());
        yylhs.value.as< For* > ()->loc(yystack_[7].location);
    }
#line 1832 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 119 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< While* > () = new While(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ());
        yylhs.value.as< While* > ()->loc(yystack_[3].location);
    }
#line 1841 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 125 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< Let* > () = new Let(yystack_[3].value.as< std::vector<Declaration*> > (), yystack_[1].value.as< std::vector<Expression*> > ()); 
        yylhs.value.as< Let* > ()->loc(yystack_[4].location);
    }
#line 1850 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 131 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<Expression*> > () = std::vector<Expression*>(); }
#line 1856 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 132 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<Expression*> > () = yystack_[0].value.as< std::vector<Expression*> > (); }
#line 1862 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 135 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<Expression*> > () = std::vector<Expression*>(1, yystack_[0].value.as< Expression* > ());
    }
#line 1870 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 138 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<Expression*> > () = yystack_[2].value.as< std::vector<Expression*> > ();
        yylhs.value.as< std::vector<Expression*> > ().push_back(yystack_[0].value.as< Expression* > ());
    }
#line 1879 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 144 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Assignment* > () = new Assignment(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Assignment* > ()->loc(yystack_[2].location); }
#line 1885 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 147 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1891 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 148 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Or(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1897 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 151 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1903 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 152 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new And(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1909 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 155 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1915 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 156 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Eq(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1921 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 157 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Ne(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1927 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 158 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Lt(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1933 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 159 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Le(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1939 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 160 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Gt(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1945 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 161 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Ge(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1951 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 164 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1957 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 165 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Add(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1963 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 166 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Sub(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1969 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 169 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1975 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 170 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Mul(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1981 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 171 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Div(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 1987 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 39:
#line 174 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 1993 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 40:
#line 175 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Negation(yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[1].location); }
#line 1999 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 41:
#line 178 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = yystack_[0].value.as< Expression* > (); }
#line 2005 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 42:
#line 179 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new StringLiteral(yystack_[0].value.as< std::string > ()); yylhs.value.as< Expression* > ()->loc(yystack_[0].location); }
#line 2011 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 43:
#line 180 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new IntLiteral(yystack_[0].value.as< int > ()); yylhs.value.as< Expression* > ()->loc(yystack_[0].location); }
#line 2017 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 44:
#line 181 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Nil(); yylhs.value.as< Expression* > ()->loc(yystack_[0].location); }
#line 2023 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 45:
#line 182 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Sequence(yystack_[1].value.as< std::vector<Expression*> > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 2029 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 46:
#line 183 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Call(yystack_[3].value.as< std::string > (), yystack_[1].value.as< std::vector<Expression*> > ()); yylhs.value.as< Expression* > ()->loc(yystack_[3].location); }
#line 2035 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 47:
#line 184 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new RecordCreate(yystack_[3].value.as< std::string > (), yystack_[1].value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > ()); yylhs.value.as< Expression* > ()->loc(yystack_[3].location); }
#line 2041 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 48:
#line 185 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new ArrayCreate(yystack_[5].value.as< std::string > (), yystack_[3].value.as< Expression* > (), yystack_[0].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[5].location); }
#line 2047 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 49:
#line 188 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Identifier(yystack_[0].value.as< std::string > ()); yylhs.value.as< Expression* > ()->loc(yystack_[0].location); }
#line 2053 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 50:
#line 189 "tiger.yy" // lalr1.cc:859
    {
        auto i = new Identifier(yystack_[3].value.as< std::string > ());
        i->loc(yystack_[3].location);
        yylhs.value.as< Expression* > () = new Subscript(i, yystack_[1].value.as< Expression* > ());
        yylhs.value.as< Expression* > ()->loc(yystack_[3].location);
    }
#line 2064 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 51:
#line 195 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new Subscript(yystack_[3].value.as< Expression* > (), yystack_[1].value.as< Expression* > ()); yylhs.value.as< Expression* > ()->loc(yystack_[3].location); }
#line 2070 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 52:
#line 196 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Expression* > () = new FieldExpression(yystack_[2].value.as< Expression* > (), yystack_[0].value.as< std::string > ()); yylhs.value.as< Expression* > ()->loc(yystack_[2].location); }
#line 2076 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 53:
#line 199 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > () = std::vector<std::tuple<std::string, Expression*, tiger::location>>(); }
#line 2082 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 54:
#line 200 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > () = yystack_[0].value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > (); }
#line 2088 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 55:
#line 203 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > () = std::vector<std::tuple<std::string, Expression*, tiger::location>>();
        yylhs.value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > ().emplace_back(yystack_[2].value.as< std::string > (), yystack_[0].value.as< Expression* > (), yystack_[2].location);
    }
#line 2097 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 56:
#line 207 "tiger.yy" // lalr1.cc:859
    {
        yystack_[4].value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > ().emplace_back(yystack_[2].value.as< std::string > (), yystack_[0].value.as< Expression* > (), yystack_[4].location);
        yylhs.value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > () = yystack_[4].value.as< std::vector<std::tuple<std::string, Expression*, tiger::location>> > ();
    }
#line 2106 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 57:
#line 213 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<Expression*> > () = std::vector<Expression*>(); }
#line 2112 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 58:
#line 214 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<Expression*> > () = yystack_[0].value.as< std::vector<Expression*> > (); }
#line 2118 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 59:
#line 217 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<Expression*> > () = std::vector<Expression*>(1, yystack_[0].value.as< Expression* > ()); }
#line 2124 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 60:
#line 218 "tiger.yy" // lalr1.cc:859
    { yystack_[2].value.as< std::vector<Expression*> > ().push_back(yystack_[0].value.as< Expression* > ()); yylhs.value.as< std::vector<Expression*> > () = yystack_[2].value.as< std::vector<Expression*> > (); }
#line 2130 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 61:
#line 221 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<Declaration*> > () = std::vector<Declaration*>(); }
#line 2136 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 62:
#line 222 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<Declaration*> > () = yystack_[1].value.as< std::vector<Declaration*> > ();
        yylhs.value.as< std::vector<Declaration*> > ().push_back(yystack_[0].value.as< Declaration* > ()); 
    }
#line 2145 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 63:
#line 228 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Declaration* > () = yystack_[0].value.as< FunctionDeclaration* > (); }
#line 2151 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 64:
#line 229 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Declaration* > () = yystack_[0].value.as< VariableDeclaration* > (); }
#line 2157 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 65:
#line 230 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Declaration* > () = yystack_[0].value.as< TypeDeclaration* > (); }
#line 2163 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 66:
#line 233 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< TypeDeclaration* > () = new TypeDeclaration(yystack_[2].value.as< std::string > (), yystack_[0].value.as< Type* > ()); yylhs.value.as< TypeDeclaration* > ()->loc(yystack_[3].location); }
#line 2169 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 67:
#line 236 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Type* > () = new IdentifierType(yystack_[0].value.as< std::string > ()); yylhs.value.as< Type* > ()->loc(yystack_[0].location); }
#line 2175 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 68:
#line 237 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Type* > () = new ArrayType(yystack_[0].value.as< std::string > ()); yylhs.value.as< Type* > ()->loc(yystack_[2].location); }
#line 2181 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 69:
#line 238 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< Type* > () = yystack_[0].value.as< RecordType* > (); }
#line 2187 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 70:
#line 241 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< RecordType* > () = new RecordType(yystack_[1].value.as< std::vector<FieldDeclaration> > ()); yylhs.value.as< RecordType* > ()->loc(yystack_[2].location); }
#line 2193 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 71:
#line 244 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<FieldDeclaration> > () = std::vector<FieldDeclaration>();
        yylhs.value.as< std::vector<FieldDeclaration> > ().emplace_back(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::vector<FieldDeclaration> > ().back().loc(yystack_[2].location);
    }
#line 2203 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 72:
#line 249 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<FieldDeclaration> > () = yystack_[4].value.as< std::vector<FieldDeclaration> > ();
        yylhs.value.as< std::vector<FieldDeclaration> > ().emplace_back(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::vector<FieldDeclaration> > ().back().loc(yystack_[2].location);
    }
#line 2213 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 73:
#line 256 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< FunctionDeclaration* > () = new FunctionDeclaration(yystack_[7].value.as< std::string > (), yystack_[5].value.as< std::vector<FunctionParameter> > (), yystack_[2].value.as< std::string > (), yystack_[0].value.as< Expression* > ());
        yylhs.value.as< FunctionDeclaration* > ()->loc(yystack_[8].location); 
    }
#line 2222 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 74:
#line 260 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< FunctionDeclaration* > () = new FunctionDeclaration(yystack_[5].value.as< std::string > (), yystack_[3].value.as< std::vector<FunctionParameter> > (), "", yystack_[0].value.as< Expression* > ());
        yylhs.value.as< FunctionDeclaration* > ()->loc(yystack_[6].location); 
    }
#line 2231 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 75:
#line 266 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<FunctionParameter> > () = std::vector<FunctionParameter>(); }
#line 2237 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 76:
#line 267 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<FunctionParameter> > () = yystack_[0].value.as< std::vector<FunctionParameter> > (); }
#line 2243 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 77:
#line 270 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<FunctionParameter> > () = std::vector<FunctionParameter>();
        yylhs.value.as< std::vector<FunctionParameter> > ().emplace_back(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::vector<FunctionParameter> > ().back().loc(yystack_[2].location);
    }
#line 2253 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 78:
#line 275 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< std::vector<FunctionParameter> > () = yystack_[4].value.as< std::vector<FunctionParameter> > ();
        yylhs.value.as< std::vector<FunctionParameter> > ().emplace_back(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ());
        yylhs.value.as< std::vector<FunctionParameter> > ().back().loc(yystack_[2].location);
    }
#line 2263 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 79:
#line 282 "tiger.yy" // lalr1.cc:859
    {
        yylhs.value.as< VariableDeclaration* > () = new VariableDeclaration(yystack_[3].value.as< std::string > (), yystack_[2].value.as< std::string > (), yystack_[0].value.as< Expression* > ());
        yylhs.value.as< VariableDeclaration* > ()->loc(yystack_[4].location); 
    }
#line 2272 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 80:
#line 288 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 2278 "tiger.tab.cc" // lalr1.cc:859
    break;

  case 81:
#line 289 "tiger.yy" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 2284 "tiger.tab.cc" // lalr1.cc:859
    break;


#line 2288 "tiger.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  TigerParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  TigerParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char TigerParser::yypact_ninf_ = -37;

  const signed char TigerParser::yytable_ninf_ = -1;

  const short int
  TigerParser::yypact_[] =
  {
      92,    92,    60,   -36,    92,   -37,   -37,    92,   -37,   -37,
      28,    15,   -37,   -37,   -37,   -37,   -37,   -37,   -37,    -8,
      -4,   -37,     7,    47,   -37,   -37,    55,   -37,     5,   -37,
     -37,    27,    25,    36,   -29,    61,    92,    92,    39,   -37,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    92,    92,    45,   -37,    92,    92,    92,    51,
      92,    56,    64,   -37,   -37,   -37,   -37,    48,   -37,     9,
     -37,   105,    94,    68,   -37,    -4,   -37,    47,    47,    71,
      71,    71,    71,    71,    71,   -37,   -37,   107,   -37,   -37,
     -37,    75,    87,   114,   -11,   100,   109,   -37,   -37,   -37,
     -37,    92,    83,    92,   -37,    76,   -37,    92,    92,    78,
     -37,    52,    80,   112,   -37,    92,   -37,   115,   102,   -37,
     125,    10,   -37,    93,   103,   -37,   -37,   -37,   -37,    92,
     -37,    92,    48,    95,    -2,    96,   133,    74,    99,   -37,
     -37,   -37,   -37,   101,    92,   136,   104,   -37,   106,   -37,
     130,   -37,   108,   -37,   139,    92,   -37,   110,   -37,   -37
  };

  const unsigned char
  TigerParser::yydefact_[] =
  {
       0,    17,     0,     0,     0,    61,    44,     0,    43,    42,
      49,     0,     2,     5,     6,     8,     7,     9,    10,    11,
      22,    24,    26,    33,    36,    39,    41,    19,     0,    18,
      40,    41,     0,     0,     0,     0,    57,     0,    53,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    45,     0,     0,     0,     0,
      17,     0,     0,    62,    65,    63,    64,     0,    59,     0,
      58,     0,     0,     0,    54,    23,    25,    34,    35,    27,
      28,    31,    29,    32,    30,    37,    38,     0,    21,    52,
      20,     0,    13,     0,     0,     0,    80,     3,    15,     4,
      46,     0,    50,     0,    47,     0,    51,     0,     0,    75,
      16,     0,     0,     0,    60,     0,    55,     0,     0,    12,
       0,     0,    76,     0,     0,    67,    66,    69,    81,     0,
      48,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      56,    14,    77,     0,     0,     0,     0,    70,     0,    68,
       0,    74,     0,    71,     0,     0,    78,     0,    73,    72
  };

  const signed char
  TigerParser::yypgoto_[] =
  {
     -37,   -37,    19,     0,   -37,   -37,   -37,   -37,   -37,    97,
     -37,   -37,   -37,   113,   117,    24,    57,     4,   -37,    -1,
     -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,
     -37,   -37,   -37,   -37,   -37
  };

  const short int
  TigerParser::yydefgoto_[] =
  {
      -1,    11,    98,    27,    13,    14,    15,    16,    17,    28,
      29,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      73,    74,    69,    70,    34,    63,    64,   126,   127,   137,
      65,   121,   122,    66,   113
  };

  const unsigned char
  TigerParser::yytable_[] =
  {
      12,    31,    56,    59,    33,    60,    30,    35,   143,    55,
      32,    61,    62,   100,   134,    39,   144,    40,    56,   110,
      41,   101,   135,    42,    43,    44,    45,    46,    47,    48,
      49,    36,    52,    37,    57,    38,    68,    71,    54,    31,
      31,    31,    31,    31,    31,    31,    31,    31,    31,    31,
      31,     1,    87,    88,    85,    86,    90,    91,    92,   123,
      52,    50,    51,     1,    53,     2,    54,    99,    79,    80,
      81,    82,    83,    84,    58,    97,   104,     2,   124,     3,
     105,     4,   147,     5,     6,    72,   148,    42,    43,    67,
       7,    89,     8,     9,    10,     1,     6,    93,   125,    77,
      78,   114,    95,   116,     8,     9,    10,   118,   119,     2,
      96,   102,   103,   106,   107,   130,   108,   109,   111,   112,
     115,   129,   117,     3,   120,     4,   128,     5,     6,   139,
     132,   140,    99,   131,     7,   133,     8,     9,    10,   136,
     138,   142,   145,   146,   151,   149,   152,   150,   155,   157,
     153,   141,   154,    75,   156,   158,   159,    94,    76
  };

  const unsigned char
  TigerParser::yycheck_[] =
  {
       0,     2,    13,    32,     4,    34,     2,     7,    10,     4,
      46,    40,    41,     4,     4,     0,    18,    25,    13,    30,
      24,    12,    12,    16,    17,    18,    19,    20,    21,    22,
      23,     3,     5,     5,     9,     7,    36,    37,    11,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     3,    52,    53,    50,    51,    56,    57,    58,     7,
       5,    14,    15,     3,     9,    17,    11,    67,    44,    45,
      46,    47,    48,    49,    38,    27,     8,    17,    26,    31,
      12,    33,     8,    35,    36,    46,    12,    16,    17,    28,
      42,    46,    44,    45,    46,     3,    36,    46,    46,    42,
      43,   101,    46,   103,    44,    45,    46,   107,   108,    17,
      46,     6,    18,     6,    39,   115,    29,     3,    18,    10,
      37,     9,    46,    31,    46,    33,    46,    35,    36,   129,
      28,   131,   132,    18,    42,    10,    44,    45,    46,    46,
      37,    46,    46,    10,   144,    46,    10,    46,    18,    10,
      46,   132,    46,    40,    46,   155,    46,    60,    41
  };

  const unsigned char
  TigerParser::yystos_[] =
  {
       0,     3,    17,    31,    33,    35,    36,    42,    44,    45,
      46,    48,    50,    51,    52,    53,    54,    55,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    50,    56,    57,
      64,    66,    46,    50,    71,    50,     3,     5,     7,     0,
      25,    24,    16,    17,    18,    19,    20,    21,    22,    23,
      14,    15,     5,     9,    11,     4,    13,     9,    38,    32,
      34,    40,    41,    72,    73,    77,    80,    28,    50,    69,
      70,    50,    46,    67,    68,    60,    61,    63,    63,    62,
      62,    62,    62,    62,    62,    64,    64,    50,    50,    46,
      50,    50,    50,    46,    56,    46,    46,    27,    49,    50,
       4,    12,     6,    18,     8,    12,     6,    39,    29,     3,
      30,    18,    10,    81,    50,    37,    50,    46,    50,    50,
      46,    78,    79,     7,    26,    46,    74,    75,    46,     9,
      50,    18,    28,    10,     4,    12,    46,    76,    37,    50,
      50,    49,    46,    10,    18,    46,    10,     8,    12,    46,
      46,    50,    10,    46,    46,    18,    46,    10,    50,    46
  };

  const unsigned char
  TigerParser::yyr1_[] =
  {
       0,    47,    48,    49,    49,    50,    50,    50,    50,    50,
      50,    50,    51,    52,    53,    54,    55,    56,    56,    57,
      57,    58,    59,    59,    60,    60,    61,    61,    61,    61,
      61,    61,    61,    62,    62,    62,    63,    63,    63,    64,
      64,    65,    65,    65,    65,    65,    65,    65,    65,    66,
      66,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    72,    73,    74,    74,    74,
      75,    76,    76,    77,    77,    78,    78,    79,    79,    80,
      81,    81
  };

  const unsigned char
  TigerParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     8,     4,     5,     0,     1,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       2,     1,     1,     1,     1,     3,     4,     4,     6,     1,
       4,     4,     3,     0,     1,     3,     5,     0,     1,     1,
       3,     0,     2,     1,     1,     1,     4,     1,     3,     1,
       3,     3,     5,     9,     7,     0,     1,     3,     5,     5,
       0,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const TigerParser::yytname_[] =
  {
  "$end", "error", "$undefined", "LPAREN", "RPAREN", "LBRACKET",
  "RBRACKET", "LBRACE", "RBRACE", "ASSIGN", "COLON", "DOT", "COMMA",
  "SEMICOLON", "TIMES", "DIVIDE", "PLUS", "MINUS", "EQ", "NEQ", "GT", "LT",
  "GE", "LE", "AND", "OR", "ARRAY", "BREAK", "DO", "ELSE", "END", "FOR",
  "FUNCTION", "IF", "IN", "LET", "NIL", "OF", "THEN", "TO", "TYPE", "VAR",
  "WHILE", "UNKNOWN", "INT", "STRING", "IDENTIFIER", "$accept", "program",
  "breakable-expression", "expression", "if-then-else", "if-then", "for",
  "while", "let-expression", "semicolon-separated-expression",
  "nonempty-semicolon-separated-expression", "assignment-expression",
  "logical-or-expression", "logical-and-expression", "equality-expression",
  "additive-expression", "multiplicative-expression", "unary-expression",
  "postfix-expression", "lvalue-expression",
  "comma-separated-record-field", "nonempty-comma-separated-record-field",
  "comma-separated-expression", "nonempty-comma-separated-expression",
  "space-separated-declaration", "declaration", "type-declaration", "type",
  "record-type", "comma-separated-field", "function-declaration",
  "comma-separated-parameter", "nonempty-comma-separated-parameter",
  "variable-declaration", "nullable-colon-type", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  TigerParser::yyrline_[] =
  {
       0,    85,    85,    91,    92,    95,    96,    97,    98,    99,
     100,   101,   104,   110,   113,   119,   125,   131,   132,   135,
     138,   144,   147,   148,   151,   152,   155,   156,   157,   158,
     159,   160,   161,   164,   165,   166,   169,   170,   171,   174,
     175,   178,   179,   180,   181,   182,   183,   184,   185,   188,
     189,   195,   196,   199,   200,   203,   207,   213,   214,   217,
     218,   221,   222,   228,   229,   230,   233,   236,   237,   238,
     241,   244,   249,   256,   260,   266,   267,   270,   275,   282,
     288,   289
  };

  // Print the state stack on the debug stream.
  void
  TigerParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  TigerParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  TigerParser::token_number_type
  TigerParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
    };
    const unsigned int user_token_number_max_ = 301;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 5 "tiger.yy" // lalr1.cc:1167
} // tiger
#line 2823 "tiger.tab.cc" // lalr1.cc:1167
#line 292 "tiger.yy" // lalr1.cc:1168


/* allows for printing of an error message */
void tiger::TigerParser::error(const location& loc, const std::string& msg)
{
    std::cerr << "Error: " << msg << " at " << loc << "\n";
}
