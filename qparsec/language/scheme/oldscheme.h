#ifndef SCHEME_H
#define SCHEME_H

#include <cassert>
#include <QPair>
#include <QStringList>

#include "qparsec.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

#include "scheme.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

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
