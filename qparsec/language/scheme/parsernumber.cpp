#include "parsernumber.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

int64_t SchemeNumber::gcd(int64_t n, int64_t m){return m==0?n:gcd(m,n%m);}

SchemeNumber::SchemeNumber(int64_t i) : numtype(INTEGER), integer(i) {}
SchemeNumber::SchemeNumber(double r) : numtype(REAL), real(r) {}
SchemeNumber::SchemeNumber(QPair<int64_t, uint64_t> r) : numtype(RATIONAL) {
    int64_t n = gcd(r.first, r.second);
    rational = QPair<int64_t, uint64_t>(r.first / n, r.second / n);
}
SchemeNumber::SchemeNumber(QPair<double, double> c) : numtype(COMPLEX), complex(c) {}

SchemeNumber::~SchemeNumber() {
    switch(numtype) {
    case RATIONAL: rational.~QPair(); break;
    case COMPLEX: complex.~QPair(); break;
    default: break;
    }
}

SchemeNumber SchemeNumber::operator +(SchemeNumber num) {
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

SchemeNumber SchemeNumber::operator *(SchemeNumber num) {
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


Parser<QChar> *Sign() {
    return Option(Choice({ Char('+'), Char('-') }), QChar('+'));
}

template<> Parser<void> *Radix<2>(){ return Ignore(Str("#b")); }
template<> Parser<void> *Radix<8>(){ return Ignore(Str("#o")); }
template<> Parser<void> *Radix<10>(){ return Option_(Str("#d")); }
template<> Parser<void> *Radix<16>(){ return Ignore(Str("#x")); }

template<> Parser<QChar> *SDigit<2>(){ return OneOf("01"); }
template<> Parser<QChar> *SDigit<8>(){ return Octdigit(); }
template<> Parser<QChar> *SDigit<10>(){ return Digit(); }
template<> Parser<QChar> *SDigit<16>(){ return Hexadigit(); }

Parser<QChar> *ParserSuffix::ExponentMarker() { return OneOf("esfdl"); }

QString ParserSuffix::parse(Input &input) {
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

Parser<QString> *Suffix() { return new ParserSuffix(); }

Parser<QString> *ParserUReal<10>::SchemeDigit(){return Many1(SDigit<10>());}

Parser<QString> *ParserUReal<10>::Placeholders(){
    // '#' is just replaced to '0'
    QString(*sharpToZero)(QString) = [](QString str){return str.replace('#', '0');};
    return Apply(Many(Char('#')), sharpToZero);
}

SchemeNumber ParserUReal<10>::parse(Input &input) {
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


Parser<SchemeNumber> *Number() {
    return Choice({Num<2>(), Num<8>(), Num<10>(), Num<16>()});
}

}
}
}
