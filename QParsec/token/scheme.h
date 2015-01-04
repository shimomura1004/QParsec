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
    enum Type { INTEGER, REAL, RATIONAL, COMPLEX } numtype;
    union {
        int64_t integer;
        double real;
        QPair<int64_t, uint64_t> rational;
        QPair<double, double> complex;
    };

    SchemeNumber(int64_t i) : numtype(INTEGER), integer(i) {}
    SchemeNumber(double r) : numtype(REAL), real(r) {}
    SchemeNumber(QPair<int64_t, uint64_t> r) : numtype(RATIONAL), rational(r) {}
    SchemeNumber(QPair<double, double> c) : numtype(COMPLEX), complex(c) {}
    ~SchemeNumber() {
        switch(numtype) {
        case RATIONAL: rational.~QPair(); break;
        case COMPLEX: complex.~QPair(); break;
        default: break;
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

struct ParserSuffix : Parser<QPair<QString, QChar>> {
    Parser<QChar> *ExponentMarker() { return OneOf("esfdl"); }

    QPair<QString, QChar> parse(Input &input) {
        try {
            auto exp = ExponentMarker()->parse(input);
            auto sign = Sign()->parse(input);
            auto digit = Many1(SDigit<10>())->parse(input);
            return QPair<QString, QChar>(QStringLiteral("") + sign + digit, exp);
        }
        catch (const ParserException &) {}
        return QPair<QString, QChar>(QStringLiteral(""), QChar());
    }
};
Parser<QPair<QString, QChar>> *Suffix() { return new ParserSuffix(); }

template<int n>
Parser<SchemeNumber> *UInteger() {
    SchemeNumber(*f)(QString) = [](QString s){
        SchemeNumber num(static_cast<int64_t>(s.toLongLong(0, n)));
        return num;
    };
    return Apply(Many1(SDigit<n>()), f);
}

struct ParserDecimal10 : Parser<SchemeNumber>  {
    SchemeNumber parse(Input &input) {
        try {
            auto integral = UInteger<10>()->parse(input);
            auto suffix = Suffix()->parse(input);
        }
        catch (const ParserException &) {}

        try {
            Char('.')->parse(input);
            Many1(SDigit<10>())->parse(input);
            Many(Char('#'))->parse(input);
            Suffix()->parse(input);
        }
        catch (const ParserException &) {}

        try {
            Many1(SDigit<10>())->parse(input);
            Char('.')->parse(input);
            Many(SDigit<10>())->parse(input);
            Many(Char('#'))->parse(input);
            Suffix()->parse(input);
        }
        catch (const ParserException &) {}

        Many1(SDigit<10>())->parse(input);
        Many1(Char('#'))->parse(input);
        Char('.')->parse(input);
        Many(Char('#'))->parse(input);
        Suffix()->parse(input);
    }
};
Parser<SchemeNumber> *Decimal10(){ return new ParserDecimal10(); }

template<int n>
Parser<SchemeNumber> *UReal() {
    return Choice({ UInteger<n>(),
                    // UInteger<n>() '/' UInteger<n>(),
                  });
}
template<>
Parser<SchemeNumber> *UReal<10>() {
    return Choice({ UInteger<10>(),
                    // UInteger<n>() '/' UInteger<n>(),
                    Decimal10()
                  });
}

template<int n>
Parser<SchemeNumber> *Real() {
    SchemeNumber(*f)(QChar, SchemeNumber) = [](QChar sign, SchemeNumber num){
        assert(num.numtype != SchemeNumber::COMPLEX);

        switch(num.numtype) {
        case SchemeNumber::INTEGER:
            num.integer *= (sign == '+') ? 1 : -1;
            break;
        case SchemeNumber::REAL:
            num.real *= (sign == '+') ? 1 : -1;
            break;
        case SchemeNumber::RATIONAL:
            num.rational.first *= (sign == '+') ? 1 : -1;
            break;
        case SchemeNumber::COMPLEX:
            break;
        }

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
