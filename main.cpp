#include <QDebug>

#include "QParsec.h"
#include "QParsecCombinator.h"
#include "QParsecChar.h"


void test_char() {
    Input input("abc");
    auto a = Char('a')->parse(input);
    assert(a == 'a');

    QChar b;
    Char('b', &b)->parse(input);
    assert(b == 'b');

    try {
        Char('a')->parse(input);
        assert(false);
    }
    catch (const ParserException &) {}

    auto c = Char(QChar('c'))->parse(input);
    assert(c == 'c');

    try {
        Char('a')->parse(input);
        assert(false);
    }
    catch (const ParserException &) {}
}

void test_seq() {
    Input input("abc");

    QChar a, b, c;
    Seq(Char('a', &a), Char('b', &b), Char('c', &c))->parse(input);

    assert(a == 'a');
    assert(b == 'b');
    assert(c == 'c');
}

void test_try() {
    Input input("abc");

    try {
        Try(Seq(Char('a'), Char('b'), Char('a')))->parse(input);
        assert(false);
    }
    catch (const ParserException &) {}

    Try(Seq(Char('a'), Char('b'), Char('c')))->parse(input);
}

void test_fail() {
    Input input("a");

    try {
        Fail("fail parser")->parse(input);
        assert(false);
    }
    catch (const ParserException &e) {
        assert(e.reason == "fail parser");
    }

    Char('a')->parse(input);
}

void test_help() {
    Input input("a");

    try {
        Help(Fail("fail message"), "help message")->parse(input);
        assert(false);
    }
    catch (const ParserException &e) {
        assert(e.reason == "help message");
    }
}


struct ParserStruct : Parser<void> {
  void parse(Input &input) {
      S(SkipMany(Space()))->parse(input);
      S(Str("struct"))->parse(input);
      S(SkipMany(Space()))->parse(input);
      S(Char('{'));
      S(Char('}'));
      S(SkipMany(Space()))->parse(input);
  }
};

int main() {
    test_char();
    test_seq();
    test_try();
    test_fail();
    test_help();

    return 0;
}
