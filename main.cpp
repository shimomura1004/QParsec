#include <QDebug>

#include "QParsec.h"
#include "QParsecCombinator.h"
#include "QParsecChar.h"


void test_Char() {
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

void test_Seq() {
    Input input("abc");

    QChar a, b, c;
    Seq(Char('a', &a), Char('b', &b), Char('c', &c))->parse(input);

    assert(a == 'a');
    assert(b == 'b');
    assert(c == 'c');
}

void test_Try() {
    Input input("abc");

    try {
        Try(Seq(Char('a'), Char('b'), Char('a')))->parse(input);
        assert(false);
    }
    catch (const ParserException &) {}

    Try(Seq(Char('a'), Char('b'), Char('c')))->parse(input);
}

void test_Fail() {
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

void test_Help() {
    Input input("a");

    try {
        Help(Fail("fail message"), "help message")->parse(input);
        assert(false);
    }
    catch (const ParserException &e) {
        assert(e.reason == "help message");
        return;
    }
    assert(false);
}

void test_OneOf() {
    Input input("a0");

    auto a = OneOf("abc")->parse(input);
    assert(a == 'a');

    try {
        OneOf("abc")->parse(input);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_NoneOf() {
    Input input("a0");

    auto a = NoneOf("0123456789")->parse(input);
    assert(a == 'a');

    try {
        NoneOf("0123456789")->parse(input);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_Str() {
    Input input("helloworld");

    auto hello = Str("hello")->parse(input);
    assert(hello == "hello");

    try {
        Str("world!!!")->parse(input);
        assert(false);
    }
    catch (const ParserException &) {}
}

void test_AnyChar() {
    Input input("a0*");

    QChar a, b, c;
    Seq(AnyChar(&a), AnyChar(&b), AnyChar(&c))->parse(input);

    assert(a == 'a');
    assert(b == '0');
    assert(c == '*');

    try {
        AnyChar()->parse(input);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_Digit() {
    Input input("1a");

    auto n = Digit()->parse(input);
    assert(n == '1');

    try {
        Digit()->parse(input);
        assert(false);
    }
    catch(const ParserException &){}
}

void test_Space() {
    Input input(" \ta");

    QChar space, tab;
    Seq(Space(&space), Space(&tab))->parse(input);
    assert(space == ' ');
    assert(tab == '\t');

    try {
        Space()->parse(input);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_Many() {
    Input aaa("aaa");
    auto aaa_ = Many(Char('a'))->parse(aaa);
    assert(aaa_ == "aaa");

    Input num("123");
    auto num_ = Many(Digit())->parse(num);
    assert(num_ == "123");

    Input hellos("hellohellohello");
    auto hellos_ = Many(Str("hello"))->parse(hellos);
    assert(hellos_.length() == 3);
    assert(hellos_[0] == "hello");
    assert(hellos_[1] == "hello");
    assert(hellos_[2] == "hello");

    Input empty("");
    auto empty_ = Many(Char('x'))->parse(empty);
    assert(empty_ == "");

    auto empty2_ = Many(Str("hello"))->parse(empty);
    assert(empty2_.length() == 0);
}

void test_Many1() {
    Input aaa("aaa");
    auto aaa_ = Many1(Char('a'))->parse(aaa);
    assert(aaa_ == "aaa");

    Input num("123");
    auto num_ = Many1(Digit())->parse(num);
    assert(num_ == "123");

    Input hellos("hellohellohello");
    auto hellos_ = Many1(Str("hello"))->parse(hellos);
    assert(hellos_.length() == 3);
    assert(hellos_[0] == "hello");
    assert(hellos_[1] == "hello");
    assert(hellos_[2] == "hello");

    Input empty("");
    try {
        Many1(Char('x'))->parse(empty);
        assert(false);
    }
    catch (const ParserException &){}

    try {
        Many1(Str("hello"))->parse(empty);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_SkipMany() {
    Input input1("   abc");
    QString abc1;
    Seq(SkipMany(Space()), Str("abc", &abc1))->parse(input1);
    assert(abc1 == "abc");

    Input input2("abc");
    QString abc2;
    Seq(SkipMany(Space()), Str("abc", &abc2))->parse(input2);
    assert(abc2 == "abc");
}

void test_SkipMany1() {
    Input input1("   abc");
    QString abc1;
    Seq(SkipMany1(Space()), Str("abc", &abc1))->parse(input1);
    assert(abc1 == "abc");

    Input input2("abc");
    QString abc2;
    try {
        Seq(SkipMany1(Space()), Str("abc", &abc2))->parse(input2);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_Choice() {
    Input input("abc");

    auto a = Choice({Char('a'), Char('b')})->parse(input);
    assert(a == 'a');

    auto b = Choice({Char('a'), Char('b')})->parse(input);
    assert(b == 'b');

    try {
        Choice({Char('a'), Char('b')})->parse(input);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_SepBy() {
    Input input("0 10  2    300");

    auto nums = SepBy(Many1(Digit()), Many1(Space()))->parse(input);
    assert(nums.length() == 4);
    assert(nums[0] == "0");
    assert(nums[1] == "10");
    assert(nums[2] == "2");
    assert(nums[3] == "300");

    Input single("100");
    auto num = SepBy(Many1(Digit()), Many1(Space()))->parse(single);
    assert(num.length() == 1);
    assert(num[0] == "100");

    Input empty("");
    auto empty_ = SepBy(Char('a'), Space())->parse(empty);
    assert(empty_.length() == 0);

    Input endWithSep("10 23  ");
    try {
        SepBy(Many1(Digit()), Many1(Space()))->parse(endWithSep);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_SepBy1() {
    Input input("0 10  2    300");

    auto nums = SepBy1(Many1(Digit()), Many1(Space()))->parse(input);
    assert(nums.length() == 4);
    assert(nums[0] == "0");
    assert(nums[1] == "10");
    assert(nums[2] == "2");
    assert(nums[3] == "300");

    Input single("100");
    auto num = SepBy1(Many1(Digit()), Many1(Space()))->parse(single);
    assert(num.length() == 1);
    assert(num[0] == "100");

    Input empty("");
    try {
        SepBy1(Char('a'), Space())->parse(empty);
        assert(false);
    }
    catch (const ParserException &){}

    Input endWithSep("10 23  ");
    try {
        SepBy1(Many1(Digit()), Many1(Space()))->parse(endWithSep);
        assert(false);
    }
    catch (const ParserException &){}
}

void test_EndBy() {
    Input input("123  32 ");

    auto nums = EndBy(Many1(Digit()), Many1(Space()))->parse(input);
    assert(nums.length() == 2);
    assert(nums[0] == "123");
    assert(nums[1] == "32");

    Input multi_fail("123 32");
    try {
        EndBy(Many1(Digit()), Many1(Space()))->parse(multi_fail);
        assert(false);
    }
    catch (const ParserException &) {}

    Input single("123 ");
    auto single_ = EndBy(Many1(Digit()), Many1(Space()))->parse(single);
    assert(single_.length() == 1);
    assert(single_[0] == "123");

    Input single_fail("123");
    try {
        EndBy(Many1(Digit()), Many1(Space()))->parse(single_fail);
        assert(false);
    }
    catch (const ParserException &) {}

    Input empty("");
    auto empty_ = EndBy(Many1(Digit()), Many1(Space()))->parse(empty);
    assert(empty_.length() == 0);
}

void test_EndBy1() {
    Input input("123  32 ");

    auto nums = EndBy1(Many1(Digit()), Many1(Space()))->parse(input);
    assert(nums.length() == 2);
    assert(nums[0] == "123");
    assert(nums[1] == "32");

    Input multi_fail("123 32");
    try {
        EndBy1(Many1(Digit()), Many1(Space()))->parse(multi_fail);
        assert(false);
    }
    catch (const ParserException &) {}

    Input single("123 ");
    auto single_ = EndBy(Many1(Digit()), Many1(Space()))->parse(single);
    assert(single_.length() == 1);
    assert(single_[0] == "123");

    Input single_fail("123");
    try {
        EndBy1(Many1(Digit()), Many1(Space()))->parse(single_fail);
        assert(false);
    }
    catch (const ParserException &) {}

    Input empty("");
    try {
        EndBy1(Many1(Digit()), Many1(Space()))->parse(empty);
        assert(false);
    }
    catch (const ParserException &) {}
}

void test_Count() {
    Input input("12345");

    auto digits = Count(Digit(), 5)->parse(input);
    assert(digits == "12345");

    auto empty = Count(Digit(), 0)->parse(input);
    assert(empty == "");

    try {
        Count(Digit(), 3)->parse(input);
        assert(false);
    }
    catch (const ParserException &) {}
}

int main() {
    test_Char();
    test_Seq();
    test_Try();
    test_Fail();
    test_Help();
    test_OneOf();
    test_NoneOf();
    test_Str();
    test_AnyChar();
    test_Digit();
    test_Space();
    test_Many();
    test_Many1();
    test_SkipMany();
    test_SkipMany1();
    test_Choice();
    test_SepBy();
    test_SepBy1();
    test_EndBy();
    test_EndBy1();
    test_Count();

    return 0;
}
