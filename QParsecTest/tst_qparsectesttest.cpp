#include <QString>
#include <QtTest>
#include <QDebug>

#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>

class QParsecTestTest : public QObject
{
    Q_OBJECT

public:
    QParsecTestTest();

private Q_SLOTS:
    void testChar();
    void testSeq();
    void testTry();
    void testFail();
    void testHelp();
    void testOneOf();
    void testNoneOf();
    void testStr();
    void testAnyChar();
    void testDigit();
    void testSpace();
    void testMany();
    void testMany1();
    void testSkipMany();
    void testSkipMany1();
    void testChoice();
    void testSepBy();
    void testSepBy1();
    void testEndBy();
    void testEndBy1();
    void testCount();
    void testBetween();
    void testOption();
};

QParsecTestTest::QParsecTestTest()
{
}

void QParsecTestTest::testChar() {
    Input input("abc");
    auto a = Char('a')->parse(input);
    QCOMPARE(a, QChar('a'));

    QChar b;
    Char('b', &b)->parse(input);
    QCOMPARE(b, QChar('b'));

    QVERIFY_EXCEPTION_THROWN(Char('a')->parse(input), ParserException);

    auto c = Char(QChar('c'))->parse(input);
    QCOMPARE(c, QChar('c'));

    QVERIFY_EXCEPTION_THROWN(Char('a')->parse(input), ParserException);
}

void QParsecTestTest::testSeq() {
    Input input("abc");

    QChar a, b, c;
    Seq(Char('a', &a), Char('b', &b), Char('c', &c))->parse(input);

    QCOMPARE(a, QChar('a'));
    QCOMPARE(b, QChar('b'));
    QCOMPARE(c, QChar('c'));
}

void QParsecTestTest::testTry() {
    Input input("abc");

    QVERIFY_EXCEPTION_THROWN(Try(Seq(Char('a'), Char('b'), Char('a')))->parse(input), ParserException);

    QChar a, b, c;
    Try(Seq(Char('a', &a), Char('b', &b), Char('c', &c)))->parse(input);

    QCOMPARE(a, QChar('a'));
    QCOMPARE(b, QChar('b'));
    QCOMPARE(c, QChar('c'));
}

void QParsecTestTest::testFail() {
    Input input("a");

    try {
        Fail("fail parser")->parse(input);
        QFAIL("Unreachable");
    }
    catch (const ParserException &e) {
        QCOMPARE(e.reason, QString("fail parser"));
    }

    QChar a;
    Char('a', &a)->parse(input);
    QCOMPARE(a, QChar('a'));
}

void QParsecTestTest::testHelp() {
    Input input("a");

    try {
        Help(Fail("fail message"), "help message")->parse(input);
        QFAIL("Unreachable");
    }
    catch (const ParserException &e) {
        QCOMPARE(e.reason, QString("help message"));
    }
}

void QParsecTestTest::testOneOf() {
    Input input("a0");

    auto a = OneOf("abc")->parse(input);
    QCOMPARE(a, QChar('a'));

    QVERIFY_EXCEPTION_THROWN(OneOf("abc")->parse(input), ParserException);
}

void QParsecTestTest::testNoneOf() {
    Input input("a0");

    auto a = NoneOf("0123456789")->parse(input);
    QCOMPARE(a, QChar('a'));

    QVERIFY_EXCEPTION_THROWN(NoneOf("0123456789")->parse(input), ParserException);
}

void QParsecTestTest::testStr() {
    Input input("helloworld");

    auto hello = Str("hello")->parse(input);
    QCOMPARE(hello, QString("hello"));

    QVERIFY_EXCEPTION_THROWN(Str("world!!!")->parse(input), ParserException);
}

void QParsecTestTest::testAnyChar() {
    Input input("a0*");

    QChar a, b, c;
    Seq(AnyChar(&a), AnyChar(&b), AnyChar(&c))->parse(input);

    QCOMPARE(a, QChar('a'));
    QCOMPARE(b, QChar('0'));
    QCOMPARE(c, QChar('*'));

    QVERIFY_EXCEPTION_THROWN(AnyChar()->parse(input), ParserException);
}

void QParsecTestTest::testDigit() {
    Input input("1a");

    auto n = Digit()->parse(input);
    QCOMPARE(n, QChar('1'));

    QVERIFY_EXCEPTION_THROWN(Digit()->parse(input), ParserException);
}

void QParsecTestTest::testSpace() {
    Input input(" \ta");

    QChar space, tab;
    Seq(Space(&space), Space(&tab))->parse(input);
    QCOMPARE(space, QChar(' '));
    QCOMPARE(tab, QChar('\t'));

    QVERIFY_EXCEPTION_THROWN(Space()->parse(input), ParserException);
}

void QParsecTestTest::testMany() {
    Input aaa("aaa");
    auto aaa_ = Many(Char('a'))->parse(aaa);
    QCOMPARE(aaa_, QString("aaa"));

    Input num("123");
    auto num_ = Many(Digit())->parse(num);
    QCOMPARE(num_, QString("123"));

    Input hellos("hellohellohello");
    auto hellos_ = Many(Str("hello"))->parse(hellos);
    QCOMPARE(hellos_.length(), 3);
    QCOMPARE(hellos_[0], QString("hello"));
    QCOMPARE(hellos_[1], QString("hello"));
    QCOMPARE(hellos_[2], QString("hello"));

    Input empty("");
    auto empty_ = Many(Char('x'))->parse(empty);
    QCOMPARE(empty_, QString(""));

    auto empty2_ = Many(Str("hello"))->parse(empty);
    QCOMPARE(empty2_.length(), 0);
}

void QParsecTestTest::testMany1() {
    Input aaa("aaa");
    auto aaa_ = Many1(Char('a'))->parse(aaa);
    QCOMPARE(aaa_, QString("aaa"));

    Input num("123");
    auto num_ = Many1(Digit())->parse(num);
    QCOMPARE(num_, QString("123"));

    Input hellos("hellohellohello");
    auto hellos_ = Many1(Str("hello"))->parse(hellos);
    QCOMPARE(hellos_.length(), 3);
    QCOMPARE(hellos_[0], QString("hello"));
    QCOMPARE(hellos_[1], QString("hello"));
    QCOMPARE(hellos_[2], QString("hello"));

    Input empty("");
    QVERIFY_EXCEPTION_THROWN(Many1(Char('x'))->parse(empty), ParserException);
    QVERIFY_EXCEPTION_THROWN(Many1(Str("hello"))->parse(empty), ParserException);
}

void QParsecTestTest::testSkipMany() {
    Input input1("   abc");
    QString abc1;
    Seq(SkipMany(Space()), Str("abc", &abc1))->parse(input1);
    QCOMPARE(abc1, QString("abc"));

    Input input2("abc");
    QString abc2;
    Seq(SkipMany(Space()), Str("abc", &abc2))->parse(input2);
    QCOMPARE(abc2, QString("abc"));
}

void QParsecTestTest::testSkipMany1() {
    Input input1("   abc");
    QString abc1;
    Seq(SkipMany1(Space()), Str("abc", &abc1))->parse(input1);
    QCOMPARE(abc1, QString("abc"));

    Input input2("abc");
    QString abc2;
    QVERIFY_EXCEPTION_THROWN(Seq(SkipMany1(Space()), Str("abc", &abc2))->parse(input2), ParserException);
}

void QParsecTestTest::testChoice() {
    Input input("abc");

    auto a = Choice({Char('a'), Char('b')})->parse(input);
    QCOMPARE(a, QChar('a'));

    auto b = Choice({Char('a'), Char('b')})->parse(input);
    QCOMPARE(b, QChar('b'));

    QVERIFY_EXCEPTION_THROWN(Choice({Char('a'), Char('b')})->parse(input), ParserException);
}

void QParsecTestTest::testSepBy() {
    Input input("0 10  2    300");

    auto nums = SepBy(Many1(Digit()), Many1(Space()))->parse(input);
    QCOMPARE(nums.length(), 4);
    QCOMPARE(nums[0], QString("0"));
    QCOMPARE(nums[1], QString("10"));
    QCOMPARE(nums[2], QString("2"));
    QCOMPARE(nums[3], QString("300"));

    Input single("100");
    auto num = SepBy(Many1(Digit()), Many1(Space()))->parse(single);
    QCOMPARE(num.length(), 1);
    QCOMPARE(num[0], QString("100"));

    Input empty("");
    auto empty_ = SepBy(Char('a'), Space())->parse(empty);
    QCOMPARE(empty_.length(), 0);

    Input endWithSep("10 23  ");
    QVERIFY_EXCEPTION_THROWN(SepBy(Many1(Digit()), Many1(Space()))->parse(endWithSep), ParserException);
}

void QParsecTestTest::testSepBy1() {
    Input input("0 10  2    300");

    auto nums = SepBy1(Many1(Digit()), Many1(Space()))->parse(input);
    QCOMPARE(nums.length(), 4);
    QCOMPARE(nums[0], QString("0"));
    QCOMPARE(nums[1], QString("10"));
    QCOMPARE(nums[2], QString("2"));
    QCOMPARE(nums[3], QString("300"));

    Input single("100");
    auto num = SepBy1(Many1(Digit()), Many1(Space()))->parse(single);
    QCOMPARE(num.length(), 1);
    QCOMPARE(num[0], QString("100"));

    Input empty("");
    QVERIFY_EXCEPTION_THROWN(SepBy1(Char('a'), Space())->parse(empty), ParserException);

    Input endWithSep("10 23  ");
    QVERIFY_EXCEPTION_THROWN(SepBy1(Many1(Digit()), Many1(Space()))->parse(endWithSep), ParserException);
}

void QParsecTestTest::testEndBy() {
    Input input("123  32 ");

    auto nums = EndBy(Many1(Digit()), Many1(Space()))->parse(input);
    QCOMPARE(nums.length(), 2);
    QCOMPARE(nums[0], QString("123"));
    QCOMPARE(nums[1], QString("32"));

    Input multi_fail("123 32");
    QVERIFY_EXCEPTION_THROWN(EndBy(Many1(Digit()), Many1(Space()))->parse(multi_fail), ParserException);

    Input single("123 ");
    auto single_ = EndBy(Many1(Digit()), Many1(Space()))->parse(single);
    QCOMPARE(single_.length(), 1);
    QCOMPARE(single_[0], QString("123"));

    Input single_fail("123");
    QVERIFY_EXCEPTION_THROWN(EndBy(Many1(Digit()), Many1(Space()))->parse(single_fail), ParserException);

    Input empty("");
    auto empty_ = EndBy(Many1(Digit()), Many1(Space()))->parse(empty);
    QCOMPARE(empty_.length(), 0);
}

void QParsecTestTest::testEndBy1() {
    Input input("123  32 ");

    auto nums = EndBy1(Many1(Digit()), Many1(Space()))->parse(input);
    QCOMPARE(nums.length(), 2);
    QCOMPARE(nums[0], QString("123"));
    QCOMPARE(nums[1], QString("32"));

    Input multi_fail("123 32");
    QVERIFY_EXCEPTION_THROWN(EndBy1(Many1(Digit()), Many1(Space()))->parse(multi_fail), ParserException);

    Input single("123 ");
    auto single_ = EndBy(Many1(Digit()), Many1(Space()))->parse(single);
    QCOMPARE(single_.length(), 1);
    QCOMPARE(single_[0], QString("123"));

    Input single_fail("123");
    QVERIFY_EXCEPTION_THROWN(EndBy1(Many1(Digit()), Many1(Space()))->parse(single_fail), ParserException);

    Input empty("");
    QVERIFY_EXCEPTION_THROWN(EndBy1(Many1(Digit()), Many1(Space()))->parse(empty), ParserException);
}

void QParsecTestTest::testCount() {
    Input input("12345");

    auto digits = Count(Digit(), 5)->parse(input);
    QCOMPARE(digits, QString("12345"));

    auto empty = Count(Digit(), 0)->parse(input);
    QCOMPARE(empty, QString(""));

    QVERIFY_EXCEPTION_THROWN(Count(Digit(), 3)->parse(input), ParserException);
}

void QParsecTestTest::testBetween()
{
    Input input("(123)");
    auto digits = Between(Many1(Digit()), Char('('), Char(')'))->parse(input);
    QCOMPARE(digits, QString("123"));

    Input left("(123");
    QVERIFY_EXCEPTION_THROWN(Between(Many1(Digit()), Char('('), Char(')'))->parse(left), ParserException);

    Input right("123)");
    QVERIFY_EXCEPTION_THROWN(Between(Many1(Digit()), Char('('), Char(')'))->parse(right), ParserException);

    Input hello("(hello)");
    QVERIFY_EXCEPTION_THROWN(Between(Many1(Digit()), Char('('), Char(')'))->parse(hello), ParserException);
}

void QParsecTestTest::testOption()
{
    Input input("123");
    auto digit = Option(Many1(Digit()), QString("100"))->parse(input);
    QCOMPARE(digit, QString("123"));

    Input input2("hello");
    auto digit2 = Option(Many1(Digit()), QString("100"))->parse(input2);
    QCOMPARE(digit2, QString("100"));
}

QTEST_APPLESS_MAIN(QParsecTestTest)

#include "tst_qparsectesttest.moc"