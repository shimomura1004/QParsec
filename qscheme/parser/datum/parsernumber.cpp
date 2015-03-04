#include "parsernumber.h"
#include "prim/prim.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {
namespace datum {

using namespace qparsec;
using namespace prim;
using namespace language;

qparsec::Parser<ast::SharedVal> *Number() {
    ast::SharedVal(*f)(scheme::SchemeNumber) = [](scheme::SchemeNumber num){
        switch(num.numtype) {
        case scheme::SchemeNumber::INTEGER:
            return ast::Integer::create(num.integer);
        case scheme::SchemeNumber::REAL:
            return ast::Real::create(num.real);
        case scheme::SchemeNumber::RATIONAL:
            return ast::Rational::create(num.rational.first, num.rational.second);
        case scheme::SchemeNumber::COMPLEX:
            return ast::Complex::create(num.complex.first, num.complex.second);
        }
    };

    return Apply(scheme::Number(), f);
}

}
}
}
