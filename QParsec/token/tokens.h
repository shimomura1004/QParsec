#ifndef TOKENS_H
#define TOKENS_H

#include <QParsecChar.h>
#include <QParsecCombinator.h>

namespace qparsec {
namespace tokens {

using namespace qparsec::chars;
using namespace qparsec::combinators;

Parser<void> *WhiteSpace() { return SkipMany(Space()); }

template<typename T>
struct ParserLexeme : Parser<T> {
  Parser<T> *p_;

  ParserLexeme(Parser<T> *p) : Parser<T>(), p_(p) {}
  ~ParserLexeme() { delete p_; }

  T parse(Input &input) {
      auto result = p_->parse(input);
      WhiteSpace()->parse(input);
      return result;
  }
};

template<typename T>
Parser<T> *Lexeme(Parser<T> *p)
{ return new ParserLexeme<T>(p); }

Parser<QString> *Symbol(QString str)
{ return Lexeme(Str(str)); }

template<typename T>
Parser<T> *Parens(Parser<T> *p)
{ return Between(p, Symbol("("), Symbol(")")); }

template<typename T>
Parser<T> *Braces(Parser<T> *p)
{ return Between(p, Symbol("{"), Symbol("}")); }

template<typename T>
Parser<T> *Brackets(Parser<T> *p)
{ return Between(p, Symbol("<"), Symbol(">")); }

template<typename T>
Parser<T> *Squares(Parser<T> *p)
{ return Between(p, Symbol("["), Symbol("]")); }

Parser<QChar> *Semi() { return Lexeme(Char(';')); }

Parser<QChar> *Comma() { return Lexeme(Char(',')); }

Parser<QChar> *Colon() { return Lexeme(Char(':')); }

Parser<QChar> *Dot() { return Lexeme(Char('.')); }

template<typename T>
Parser<QList<T>> *SemiSep(Parser<T> *p)
{ return SepBy(p, Semi()); }

template<typename T>
Parser<QList<T>> *SemiSep1(Parser<T> *p)
{ return SepBy1(p, Semi()); }

template<typename T>
Parser<QList<T>> *CommaSep(Parser<T> *p)
{ return SepBy(p, Comma()); }

template<typename T>
Parser<QList<T>> *CommaSep1(Parser<T> *p)
{ return SepBy1(p, Comma()); }

}
}

#endif // TOKENS_H
