#ifndef SCHEME_H
#define SCHEME_H

#include <cassert>
#include <QPair>
#include <QStringList>

#include "qparsec.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

class ParserIdentifier : public Parser<QString> {
protected:
    Parser<QChar> *Initial() { return Choice({Letter(), SpecialInitial()}); }
    Parser<QChar> *SpecialInitial() { return OneOf("!#$%&|*+-/:<=>?@^_~"); }
    Parser<QChar> *Subsequent() { return Choice({Initial(), Digit(), SpecialSubsequent()}); }
    Parser<QChar> *SpecialSubsequent() { return OneOf("+-.@"); }
    Parser<QString> *PeculiarIdentifier() { return Choice({Str("+"), Str("-"), Str("...")}); }

public:
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

const QStringList ExpressionKeyword = {
    "quote", "lambda", "if", "set!", "begin", "cond", "and", "or", "case",
    "let", "let*", "letrec", "do", "delay", "quasiquote"
};
const QStringList SyntacticKeyword = {
    "else", "=>", "define", "unquote", "unquote-splicing"
};

class ParserVariable : public Parser<QString> {
public:
    QString parse(Input &input) {
        input.preserve();

        auto ident = Identifier()->parse(input);
        if (SyntacticKeyword.contains(ident) || ExpressionKeyword.contains(ident)) {
            input.restore();
            throw ParserException(input.index(), QStringLiteral("%1 is reserved identifier").arg(ident));
        }
        return ident;
    }
};
Parser<QString> *Variable() { return new ParserVariable(); }

Parser<bool> *Boolean() {
    bool(*convertTrue)(QString) = [](QString s){ return s == "#t"; };
    bool(*convertFalse)(QString) = [](QString s){ return s != "#f"; };
    return Choice({Apply(Str("#t"), convertTrue), Apply(Str("#f"), convertFalse)});
}

struct SchemeNumber {
    int64_t gcd(int64_t n, int64_t m){return m==0?n:gcd(m,n%m);}

    enum Type { INTEGER = 0, REAL, RATIONAL, COMPLEX } numtype;
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

class ParserSuffix : public Parser<QString> {
protected:
    Parser<QChar> *ExponentMarker() { return OneOf("esfdl"); }

public:
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
class ParserUReal : public Parser<SchemeNumber> {
protected:
    Parser<QString> *SchemeDigit(){return Many1(SDigit<n>());}

public:
    SchemeNumber parse(Input &input) {
        auto real = SchemeDigit()->parse(input);

        try {
            Char('/')->parse(input);
            auto denom = SchemeDigit()->parse(input);
            return SchemeNumber(QPair<int64_t, uint64_t>(real.toLongLong(0, n), denom.toLongLong(0, n)));
        } catch (const ParserException &) {}

        return SchemeNumber(real.toLongLong(0, n));
    }
};
template<>
class ParserUReal<10> : public Parser<SchemeNumber> {
protected:
    Parser<QString> *SchemeDigit(){return Many1(SDigit<10>());}
    Parser<QString> *Placeholders(){
        // '#' is just replaced to '0'
        QString(*sharpToZero)(QString) = [](QString str){return str.replace('#', '0');};
        return Apply(Many(Char('#')), sharpToZero);
    }

public:
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

        auto suffix = Suffix()->parse(input);

        if (suffix.isEmpty()) {
            // e.g. "123", "-12"
            return SchemeNumber(QStringLiteral("%1%2").arg(digit, placeholders).toLongLong());
        }
        else {
            // e.g. 123e45
            return SchemeNumber(QStringLiteral("%1%2%3").arg(digit, placeholders, suffix).toDouble());
        }
    }
};
template<int n> Parser<SchemeNumber> *UReal() { return new ParserUReal<n>(); }

template<int n>
Parser<SchemeNumber> *Real() {
    SchemeNumber(*f)(QChar, SchemeNumber) = [](QChar sign, SchemeNumber num){
        assert(num.numtype != SchemeNumber::COMPLEX);

        return num * static_cast<int64_t>((sign == '+') ? 1 : -1);
    };
    return Apply2(Sign(), UReal<n>(), f);
}

template<int n>
class ParserComplex : public Parser<SchemeNumber> {
protected:
    Parser<SchemeNumber> *ImaginaryNum() {
        SchemeNumber(*f)(QChar, SchemeNumber) = [](QChar c, SchemeNumber num){
            assert(num.numtype != SchemeNumber::RATIONAL && num.numtype != SchemeNumber::COMPLEX);

            int sign = (c == '+') ? 1 : -1;
            switch(num.numtype) {
            case SchemeNumber::INTEGER:
                return SchemeNumber(QPair<double, double>(0, num.integer * sign));
            case SchemeNumber::REAL:
                return SchemeNumber(QPair<double, double>(0, num.real * sign));
            default:
                return num;
            }
        };
        return Left(Apply2(OneOf("+-"), Option(UReal<n>(), SchemeNumber(1.0)), f), Char('i'));
    }

public:
    SchemeNumber parse(Input &input) {
        try {
            // e.g. "+2.3i", "-3i", "+i"
            return Try(ImaginaryNum())->parse(input);
        } catch (const ParserException &) {}

        auto real = Real<n>()->parse(input);

        try {
            // what the '@' means?
            Char('@')->parse(input);
            auto real2 = Real<n>()->parse(input);
            Q_UNUSED(real2);
            return real;
        } catch (const ParserException &) {}

        try {
            // e.g. "12.3+4i"
            auto imaginary = Try(ImaginaryNum())->parse(input);

            assert(real.numtype != SchemeNumber::COMPLEX);

            return real + imaginary;
        } catch (const ParserException &) {}

        // e.g. "12.3", "-23"
        return real;
    }
};
template<int n>
Parser<SchemeNumber> *Complex() { return new ParserComplex<n>(); }

template<int n>
Parser<SchemeNumber> *Num() {
    return Right(Radix<n>(), Complex<n>());
}

Parser<SchemeNumber> *Number () {
    return Choice({Num<2>(), Num<8>(), Num<10>(), Num<16>()});
}


class ParserCharacter : public Parser<QChar> {
protected:
    Parser<QString> *CharacterName() { return Choice({Str("space"), Str("newline")}); }

public:
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
