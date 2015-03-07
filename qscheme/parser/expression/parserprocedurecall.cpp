#include "parserprocedurecall.h"
#include "expression.h"
#include "prim/prim.h"
#include "combinator/combinator.h"
#include "token/token.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace combinator;
using namespace token;

Parser<ast::SharedVal> *ProcedureCall() {
    ast::SharedVal(*f)(QList<ast::SharedVal>) = [](QList<ast::SharedVal> ops){
        return ast::Apply::create(ops.first(), ops.mid(1));
    };
    return Apply(Parens(Many1(Lazy(Expression))), f);
}

}
}
}
