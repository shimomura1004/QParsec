#ifndef SCHEME_H
#define SCHEME_H

#include <QPair>
#include <cassert>

#include "QParsecChar.h"
#include "QParsecCombinator.h"

namespace qparsec {
namespace tokens {
namespace scheme {

using namespace chars;
using namespace combinators;

struct ParserIdentifier : Parser<QString> {
    Parser<QChar> *Initial() { return Choice({Letter(), SpecialInitial()}); }
    Parser<QChar> *SpecialInitial() { return OneOf("!#$%&|*+-/:<=>?@^_~"); }
    Parser<QChar> *Subsequent() { return Choice({Initial(), Digit(), SpecialSubsequent()}); }
    Parser<QChar> *SpecialSubsequent() { return OneOf("+-.@"); }
    Parser<QString> *PeculiarIdentifier() { return Choice({Str("+"), Str("-"), Str("...")}); }

    ParserIdentifier() : Parser<QString>() {}

    QString parse(Input &input) {
        try {
            auto c = Initial()->parse(input);
            auto cs = Many(Subsequent())->parse(input);
            return c + cs;
        }
        catch (const ParserException &) {
            return PeculiarIdentifier()->parse(input);
        }
    }
};
Parser<QString> *Identifier() { return new ParserIdentifier(); }


Parser<bool> *Boolean() {
    bool(*convertTrue)(QString) = [](QString s){ return s == "#t"; };
    bool(*convertFalse)(QString) = [](QString s){ return s != "#f"; };
    return Choice({Apply(Str("#t"), convertTrue), Apply(Str("#f"), convertFalse)});
}



struct SchemeNumber {
    int64_t gcd(int64_t n, int64_t m){return m==0?n:gcd(m,n%m);}

    enum Type { INTEGER, REAL, RATIONAL, COMPLEX } numtype;
    union {
        int64_t integer;
        double real;
        QPair<int64_t, uint64_t> rational;
        QPair<double, double> complex;
    };

    SchemeNumber(int64_t i) : numtype(INTEGER), integer(i) {}
    SchemeNumber(double r) : numtype(REAL), real(r) {}
    SchemeNumber(QPair<int64_t, uint64_t> r) : numtype(RATIONAL) {
        int64_t n = gcd(r.first, r.second);
        rational = QPair<int64_t, uint64_t>(r.first / n, r.second / n);
    }
    SchemeNumber(QPair<double, double> c) : numtype(COMPLEX), complex(c) {}
    ~SchemeNumber() {
        switch(numtype) {
        case RATIONAL: rational.~QPair(); break;
        case COMPLEX: complex.~QPair(); break;
        default: break;
        }
    }

    SchemeNumber operator*=(int64_t x){
        switch (numtype) {
        case INTEGER:
            integer *= x;
            break;
        case REAL:
            real *= x;
            break;
        case RATIONAL:
            rational.first *= x;
            break;
        case COMPLEX:
            complex.first *= x;
            complex.second *= x;
            break;
        }
        return *this;
    }

    SchemeNumber operator +(SchemeNumber num) {
        switch(this->numtype) {
        case INTEGER: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(integer + num.integer);
            case REAL:     return SchemeNumber(1.0 * integer + num.real);
            case RATIONAL: return SchemeNumber(QPair<int64_t, uint64_t>(integer * num.rational.second + num.rational.first, num.rational.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>(1.0 * integer + num.complex.first, num.complex.second));
            }
        case REAL: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(real + num.integer);
            case REAL:     return SchemeNumber(real + num.real);
            case RATIONAL: return SchemeNumber(QPair<int64_t, uint64_t>(real * num.rational.second + num.rational.first, num.rational.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>(real + num.complex.first, num.complex.second));
            }
        case RATIONAL: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(QPair<int64_t, uint64_t>(rational.first + num.integer * rational.second, rational.second));
            case REAL:     return SchemeNumber(QPair<int64_t, uint64_t>(rational.first + num.real * rational.second, rational.second));
            case RATIONAL: return SchemeNumber(QPair<int64_t, uint64_t>(rational.first * num.rational.second + num.rational.first * rational.second,
                                                                        rational.second * num.rational.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>((1.0 * rational.first / rational.second) + num.complex.first, num.complex.second));
            }
        case COMPLEX: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(QPair<double, double>(complex.first + num.integer, complex.second));
            case REAL:     return SchemeNumber(QPair<double, double>(complex.first + num.real, complex.second));
            case RATIONAL: return SchemeNumber(QPair<double, double>(complex.first + (1.0 * num.rational.first / num.rational.second), complex.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>(complex.first + num.complex.first, complex.second + num.complex.second));
            }
        }
    }

    SchemeNumber operator *(SchemeNumber num) {
        switch(this->numtype) {
        case INTEGER: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(integer * num.integer);
            case REAL:     return SchemeNumber(1.0 * integer * num.real);
            case RATIONAL: return SchemeNumber(QPair<int64_t, uint64_t>(integer * num.rational.first, num.rational.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>(1.0 * integer * num.complex.first, 1.0 * integer * num.complex.second));
            }
        case REAL: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(real * num.integer);
            case REAL:     return SchemeNumber(real * num.real);
            case RATIONAL: return SchemeNumber(QPair<int64_t, uint64_t>(real * num.rational.first, num.rational.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>(real * num.complex.first, real * num.complex.second));
            }
        case RATIONAL: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(QPair<int64_t, uint64_t>(rational.first * num.integer, rational.second));
            case REAL:     return SchemeNumber(QPair<int64_t, uint64_t>(rational.first * num.real, rational.second));
            case RATIONAL: return SchemeNumber(QPair<int64_t, uint64_t>(rational.first * num.rational.first, rational.second * num.rational.second));
            case COMPLEX:  return SchemeNumber(QPair<double, double>((1.0 * rational.first / rational.second) * num.complex.first,
                                                                     (1.0 * rational.first / rational.second) * num.complex.second));
            }
        case COMPLEX: switch(num.numtype) {
            case INTEGER:  return SchemeNumber(QPair<double, double>(complex.first * num.integer, complex.second * num.integer));
            case REAL:     return SchemeNumber(QPair<double, double>(complex.first * num.real, complex.second * num.real));
            case RATIONAL: return SchemeNumber(QPair<double, double>(complex.first * (1.0 * num.rational.first / num.rational.second),
                                                                     complex.second * (1.0 * num.rational.first / num.rational.second)));
            case COMPLEX:  return SchemeNumber(QPair<double, double>(complex.first * num.complex.first - complex.second * num.complex.second,
                                                                     complex.first * num.complex.second + complex.second * num.complex.first));
            }
        }
    }
};

Parser<QChar> *Sign() {
    return Option(Choice({ Char('+'), Char('-') }), QChar('+'));
}

template<int n> Parser<void> *Radix();
template<> Parser<void> *Radix<2>(){ return Ignore(Str("#b")); }
template<> Parser<void> *Radix<8>(){ return Ignore(Str("#o")); }
template<> Parser<void> *Radix<10>(){ return Option_(Str("#d")); }
template<> Parser<void> *Radix<16>(){ return Ignore(Str("#x")); }

template<int n> Parser<QChar> *SDigit();
template<> Parser<QChar> *SDigit<2>(){ return OneOf("01"); }
template<> Parser<QChar> *SDigit<8>(){ return Octdigit(); }
template<> Parser<QChar> *SDigit<10>(){ return Digit(); }
template<> Parser<QChar> *SDigit<16>(){ return Hexadigit(); }

struct ParserSuffix : Parser<QString> {
    Parser<QChar> *ExponentMarker() { return OneOf("esfdl"); }

    QString parse(Input &input) {
        try {
            auto exp = ExponentMarker()->parse(input);
            auto sign = Sign()->parse(input);
            auto digit = Many1(SDigit<10>())->parse(input);

            // short, float, double and long precision specifiers are all converted to default(double)
            Q_UNUSED(exp);
            return QStringLiteral("e%1%2").arg(sign, digit);
        }
        catch (const ParserException &) {}
        return QStringLiteral("");
    }
};
Parser<QString> *Suffix() { return new ParserSuffix(); }

template<int n>
struct ParserUReal : Parser<SchemeNumber> {
    SchemeNumber parse(Input &input) {}
};
template<>
struct ParserUReal<10> : Parser<SchemeNumber> {
    Parser<QString> *SchemeDigit(){return Many1(SDigit<10>());}
    Parser<QString> *Placeholders(){
        // '#' is just replaced to '0'
        QString(*sharpToZero)(QString) = [](QString str){return str.replace('#', '0');};
        return Apply(Many(Char('#')), sharpToZero);
    }

    SchemeNumber parse(Input &input) {
        try {
            Try(Char('.'))->parse(input);

            // e.g. ".123##e-23"
            auto digit = SchemeDigit()->parse(input);
            auto placeholders = Placeholders()->parse(input);
            auto suffix = Suffix()->parse(input);
            double result = QStringLiteral(".%1%2%3").arg(digit, placeholders, suffix).toDouble();
            return SchemeNumber(result);
        }
        catch (const ParserException &) {}

        auto digit = SchemeDigit()->parse(input);
        auto placeholders = Placeholders()->parse(input);

        try {
            Char('.')->parse(input);

            // e.g. "123##.23"
            auto decimal = SchemeDigit()->parse(input);
            auto decimalplaceholder = Placeholders()->parse(input);
            auto suffix = Suffix()->parse(input);

            double result = QStringLiteral("%1%2.%3%4%5").arg(digit, placeholders, decimal, decimalplaceholder, suffix).toDouble();
            return SchemeNumber(result);
        }
        catch (const ParserException &) {}

        try {
            Char('/')->parse(input);

            // e.g. "12/34"
            auto digitDenom = SchemeDigit()->parse(input);
            auto placeholdersDenom = Placeholders()->parse(input);

            int64_t num = QStringLiteral("%1%2").arg(digit, placeholders).toLongLong();
            uint64_t denom = QStringLiteral("%1%2").arg(digitDenom, placeholdersDenom).toLongLong();
            return SchemeNumber(QPair<int64_t, uint64_t>(num, denom));
        }
        catch (const ParserException &) {}

        // e.g. 123e45
        auto suffix = Suffix()->parse(input);

        double result = QStringLiteral("%1%2%3").arg(digit, placeholders, suffix).toDouble();
        return SchemeNumber(result);
    }
};
template<int n> Parser<SchemeNumber> *UReal() { return new ParserUReal<n>(); }

template<int n>
Parser<SchemeNumber> *Real() {
    SchemeNumber(*f)(QChar, SchemeNumber) = [](QChar sign, SchemeNumber num){
        assert(num.numtype != SchemeNumber::COMPLEX);

        num *= (sign == '+') ? 1 : -1;
        return num;
    };
    return Apply2(Sign(), UReal<n>(), f);
}

template<int n>
Parser<SchemeNumber> *Complex() {
    return Choice({ Real<n>()
                  // ...
                  });
}

template<int n>
Parser<SchemeNumber> *Num() {
    return Right(Radix<n>(), Complex<n>());
}

Parser<SchemeNumber> *Number () {
    return Choice({Num<2>(), Num<8>(), Num<10>(), Num<16>()});
}


struct ParserCharacter : Parser<QChar> {
    Parser<QString> *CharacterName() { return Choice({Str("space"), Str("newline")}); }

    QChar parse(Input &input) {
        Str("#\\")->parse(input);

        try {
            auto name = Try(CharacterName())->parse(input);
            if (name == "space")
                return QChar(' ');
            if (name == "newline")
                return QChar('\n');
        }
        catch (const ParserException &) {}

        auto c = AnyChar()->parse(input);
        return QChar(c);
    }
};
Parser<QChar> *Character() { return new ParserCharacter(); }


Parser<QString> *String() {
    QString(*charToStr)(QChar) = [](QChar c){return QString(c);};
    QString(*join)(QList<QString>) = [](QList<QString> strs){
        QString result;
        Q_FOREACH(const QString &str, strs)
            result += str;
        return result;
    };

    return Apply( Between( Many(Choice({ Apply(NoneOf("\\\""), charToStr),
                                         Str("\\\""),
                                         Str("\\\\") })),
                           Char('"'),
                           Char('"')),
                  join);
}


}
}
}

#endif // SCHEME_H
