#ifndef SCHEME_H
#define SCHEME_H

#include "QParsecChar.h"
#include "QParsecCombinator.h"

namespace qparsec {
namespace tokens {

using namespace chars;
using namespace combinators;

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



namespace scheme {

struct ParserIdentifier : Parser<QString> {
    Parser<QChar> *Initial() { return Choice({Letter(), SpecialInitial()}); }
    Parser<QChar> *SpecialInitial() { return OneOf("!#$%&|*+-/:<=>?@^_~"); }
    Parser<QChar> *Subsequent() { return Choice({Initial(), Digit(), SpecialSubsequent()}); }
    Parser<QChar> *SpecialSubsequent() { return OneOf("+-.@"); }
    Parser<QString> *PeculiarIdentifier() { return Choice({Str("+"), Str("-"), Str("...")}); }

    ParserIdentifier(QString *out) : Parser<QString>(out) {}

    QString parse(Input &input) {
        try {
            auto c = Initial()->parse(input);
            auto cs = Many(Subsequent())->parse(input);
            QString result = c + cs;
            return setOut(result);
        }
        catch (const ParserException &) {
            auto cs = PeculiarIdentifier()->parse(input);
            return setOut(cs);
        }
    }
};
Parser<QString> *Identifier(QString *out = nullptr) { return new ParserIdentifier(out); }

Parser<bool> *Boolean(bool *out = nullptr) {
    bool(*convertTrue)(QString) = [](QString s){ return s == "#t"; };
    bool(*convertFalse)(QString) = [](QString s){ return s != "#f"; };
    return Choice({Apply(Str("#t"), convertTrue), Apply(Str("#f"), convertFalse)}, out);
}


template<int n> Parser<void> *Radix();
template<> Parser<void> *Radix<2>(){ return Ignore(Str("#b")); }
template<> Parser<void> *Radix<8>(){ return Ignore(Str("#o")); }
template<> Parser<void> *Radix<10>(){ return Option_(Str("#d")); }
template<> Parser<void> *Radix<16>(){ return Ignore(Str("#x")); }

Parser<QChar> *Sign() {
    return Option(Choice({ Char('+'), Char('-') }), QChar('+'));
}

template<int n> Parser<QChar> *SDigit();
template<> Parser<QChar> *SDigit<2>(){ return OneOf("01"); }
template<> Parser<QChar> *SDigit<8>(){ return Octdigit(); }
template<> Parser<QChar> *SDigit<10>(){ return Digit(); }
template<> Parser<QChar> *SDigit<16>(){ return Hexadigit(); }

Parser<int64_t> *Decimal10() {
}

template<int n>
Parser<int64_t> *UInteger() {
    int64_t(*f)(QString) = [](QString s){return s.toLongLong(0, n);};
    return Apply(Many1(SDigit<n>()), f);
}

template<int n>
Parser<int64_t> *UReal() {
    return Choice({ UInteger<n>(),
                    // UInteger<n>() '/' UInteger<n>(),
                  });
}
template<>
Parser<int64_t> *UReal<10>() {
    return Choice({ UInteger<10>(),
                    // UInteger<n>() '/' UInteger<n>(),
                    Decimal10()
                  });
}

template<int n>
Parser<int64_t> *Real(int64_t *out = nullptr) {
    int64_t(*f)(QChar, int64_t) = [](QChar sign, int64_t num){return (sign == '+' ? 1 : -1) * num;};
    return Apply2(Sign(), UReal<n>(), f, out);
}

template<int n>
Parser<int64_t> *Complex() {
    return Choice({ Real<n>()
                  // ...
                  });
}

template<int n>
Parser<int64_t> *Num(int64_t *out = nullptr) {
    return Right(Radix<n>(), Complex<n>(), out);
}

Parser<int64_t> *Num (int64_t *out = nullptr) {
    return Choice({Num<2>(), Num<8>(), Num<10>(), Num<16>()}, out);
}


}
}
}

#endif // SCHEME_H
