#include "parserstring.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

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
