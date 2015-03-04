#include "string.h"

namespace ast {

String::String(QString s) : val(s) {}

SharedVal String::create(QString s) {
    return QSharedPointer<String>(new String(s));
}

QString String::toString() {
    return QStringLiteral("\"%1\"").arg(val);
}

}
