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

  ParserLexeme(Parser<T> *p, T *out) : Parser<T>(out), p_(p) {}
  ~ParserLexeme() { delete p_; }

  T parse(Input &input) {
      auto result = p_->parse(input);
      WhiteSpace()->parse(input);
      return Parser<T>::setOut(result);
  }
};

template<typename T>
Parser<T> *Lexeme(Parser<T> *p, T *out = nullptr)
{ return new ParserLexeme<T>(p, out); }

Parser<QString> *Symbol(QString str, QString *out = nullptr)
{ return Lexeme(Str(str), out); }

template<typename T>
Parser<T> *Parens(Parser<T> *p, T *out = nullptr)
{ return Between(p, Symbol("("), Symbol(")"), out); }

template<typename T>
Parser<T> *Braces(Parser<T> *p, T *out = nullptr)
{ return Between(p, Symbol("{"), Symbol("}"), out); }

template<typename T>
Parser<T> *Brackets(Parser<T> *p, T *out = nullptr)
{ return Between(p, Symbol("<"), Symbol(">"), out); }

template<typename T>
Parser<T> *Squares(Parser<T> *p, T *out = nullptr)
{ return Between(p, Symbol("["), Symbol("]"), out); }

Parser<QChar> *Semi(QChar *out = nullptr) { return Lexeme(Char(';'), out); }

Parser<QChar> *Comma(QChar *out = nullptr) { return Lexeme(Char(','), out); }

Parser<QChar> *Colon(QChar *out = nullptr) { return Lexeme(Char(':'), out); }

Parser<QChar> *Dot(QChar *out = nullptr) { return Lexeme(Char('.'), out); }

template<typename T>
Parser<QList<T>> *SemiSep(Parser<T> *p, QList<T> *out = nullptr)
{ return SepBy(p, Semi(), out); }

template<typename T>
Parser<QList<T>> *SemiSep1(Parser<T> *p, QList<T> *out = nullptr)
{ return SepBy1(p, Semi(), out); }

template<typename T>
Parser<QList<T>> *CommaSep(Parser<T> *p, QList<T> *out = nullptr)
{ return SepBy(p, Comma(), out); }

template<typename T>
Parser<QList<T>> *CommaSep1(Parser<T> *p, QList<T> *out = nullptr)
{ return SepBy1(p, Comma(), out); }

}
}

#endif // TOKENS_H
