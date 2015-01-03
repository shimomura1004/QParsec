#ifndef SCHEME_H
#define SCHEME_H

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

Parser<int64_t> *Decimal10() {
    // todo
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
Parser<int64_t> *Real() {
    int64_t(*f)(QChar, int64_t) = [](QChar sign, int64_t num){return (sign == '+' ? 1 : -1) * num;};
    return Apply2(Sign(), UReal<n>(), f);
}

template<int n>
Parser<int64_t> *Complex() {
    return Choice({ Real<n>()
                  // ...
                  });
}

template<int n>
Parser<int64_t> *Num() {
    return Right(Radix<n>(), Complex<n>());
}

Parser<int64_t> *Num () {
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
