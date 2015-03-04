#ifndef PARSERNUMBER_H
#define PARSERNUMBER_H

#include <QChar>
#include <cassert>

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

struct SchemeNumber {
    int64_t gcd(int64_t n, int64_t m);

    enum Type { INTEGER = 0, REAL, RATIONAL, COMPLEX } numtype;
    union {
        int64_t integer;
        double real;
        QPair<int64_t, uint64_t> rational;
        QPair<double, double> complex;
    };

    SchemeNumber(int64_t i);
    SchemeNumber(double r);
    SchemeNumber(QPair<int64_t, uint64_t> r);
    SchemeNumber(QPair<double, double> c);
    ~SchemeNumber();

    SchemeNumber operator +(SchemeNumber num);
    SchemeNumber operator *(SchemeNumber num);
};

Parser<QChar> *Sign();

template<int n> Parser<void> *Radix();
template<> Parser<void> *Radix<2>();
template<> Parser<void> *Radix<8>();
template<> Parser<void> *Radix<10>();
template<> Parser<void> *Radix<16>();

template<int n> Parser<QChar> *SDigit();
template<> Parser<QChar> *SDigit<2>();
template<> Parser<QChar> *SDigit<8>();
template<> Parser<QChar> *SDigit<10>();
template<> Parser<QChar> *SDigit<16>();

class ParserSuffix : public Parser<QString> {
protected:
    Parser<QChar> *ExponentMarker();

public:
    QString parse(Input &input);
};
Parser<QString> *Suffix();

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

Parser<SchemeNumber> *Number();

}
}
}

#endif // PARSERNUMBER_H
