#include <QObject>
#include <QtTest>
#include "autotest.h"

#include "parser.h"
#include "combinator/combinator.h"
#include "prim/prim.h"
#include "character/character.h"
#include "number/number.h"

using namespace qparsec;
using namespace combinator;
using namespace prim;
using namespace character;
using namespace number;

class CombinatorTest : public QObject
{
    Q_OBJECT

public:
    CombinatorTest(){}

private Q_SLOTS:
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
    void testEof();
    void testManyTill();
    void testChainl();
    void testChaninl1();
    void testChainr();
    void testChainr1();
    void testLookAhead();
};

void CombinatorTest::testMany() {
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

void CombinatorTest::testMany1() {
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

void CombinatorTest::testSkipMany() {
    Input input1("   abc");

    SkipMany(Space())->parse(input1);
    QString abc1 = Str("abc")->parse(input1);
    QCOMPARE(abc1, QString("abc"));

    Input input2("abc");
    SkipMany(Space())->parse(input2);
    QString abc2 = Str("abc")->parse(input2);
    QCOMPARE(abc2, QString("abc"));
}

void CombinatorTest::testSkipMany1() {
    Input input1("   abc");
    SkipMany1(Space())->parse(input1);
    QString abc1 = Str("abc")->parse(input1);
    QCOMPARE(abc1, QString("abc"));

    Input input2("abc");
    QVERIFY_EXCEPTION_THROWN(Seq(SkipMany1(Space()), Str("abc"))->parse(input2), ParserException);
}

void CombinatorTest::testChoice() {
    Input input("abc");

    auto a = Choice({Char('a'), Char('b')})->parse(input);
    QCOMPARE(a, QChar('a'));

    auto b = Choice({Char('a'), Char('b')})->parse(input);
    QCOMPARE(b, QChar('b'));

    QVERIFY_EXCEPTION_THROWN(Choice({Char('a'), Char('b')})->parse(input), ParserException);
}

void CombinatorTest::testSepBy() {
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

void CombinatorTest::testSepBy1() {
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

void CombinatorTest::testEndBy() {
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

void CombinatorTest::testEndBy1() {
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

void CombinatorTest::testCount() {
    Input input("12345");

    auto digits = Count(Digit(), 5)->parse(input);
    QCOMPARE(digits, QString("12345"));

    auto empty = Count(Digit(), 0)->parse(input);
    QCOMPARE(empty, QString(""));

    QVERIFY_EXCEPTION_THROWN(Count(Digit(), 3)->parse(input), ParserException);
}

void CombinatorTest::testBetween()
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

void CombinatorTest::testOption()
{
    Input input("123");
    auto digit = Option(Many1(Digit()), QString("100"))->parse(input);
    QCOMPARE(digit, QString("123"));

    Input input2("hello");
    auto digit2 = Option(Many1(Digit()), QString("100"))->parse(input2);
    QCOMPARE(digit2, QString("100"));
}

void CombinatorTest::testEof()
{
    Input input("a");
    QVERIFY_EXCEPTION_THROWN(Eof()->parse(input), ParserException);

    auto a = AnyChar()->parse(input);
    QCOMPARE(a, QChar('a'));

    Eof()->parse(input);
}

void CombinatorTest::testManyTill()
{
    Input input("<!-- this is comment -->body");

    Str("<!--")->parse(input);
    auto rawcomment = ManyTill(AnyChar(), Str("-->"))->parse(input);
    QString comment;
    Q_FOREACH(QChar c, rawcomment)
        comment.push_back(c);
    QCOMPARE(comment, QString(" this is comment "));

    Input input2("<!---->");
    Str("<!--")->parse(input2);
    auto rawcomment2 = ManyTill(AnyChar(), Str("-->"))->parse(input2);
    QCOMPARE(rawcomment2.length(), 0);

    Input input3("<!-- hello");
    QVERIFY_EXCEPTION_THROWN(ManyTill(AnyChar(), Str("-->"))->parse(input3), ParserException);
}

void CombinatorTest::testChainl()
{
    Input input("12+34+56");
    auto result = Chainl(Decimal(), new ParserPlus(), 0)->parse(input);
    QCOMPARE(result, 12 + 34 + 56);

    Input input2("");
    auto result2 = Chainl(Decimal(), new ParserPlus(), 0)->parse(input2);
    QCOMPARE(result2, 0);

    Input input3("12");
    auto result3 = Chainl(Decimal(), new ParserPlus(), 0)->parse(input3);
    QCOMPARE(result3, 12);

    Input input4("12+");
    QVERIFY_EXCEPTION_THROWN(Chainl(Decimal(), new ParserPlus(), 0)->parse(input4), ParserException);

    Input input5("8/4/2");
    auto result5 = Chainl(Decimal(), new ParserDiv(), 0)->parse(input5);
    QCOMPARE(result5, (8 / 4) / 2);
}

void CombinatorTest::testChaninl1()
{
    Input input("12+34+56");
    auto result = Chainl1(Decimal(), new ParserPlus())->parse(input);
    QCOMPARE(result, 12 + 34 + 56);

    Input input2("");
    QVERIFY_EXCEPTION_THROWN(Chainl1(Decimal(), new ParserPlus())->parse(input2), ParserException);

    Input input3("12");
    auto result3 = Chainl1(Decimal(), new ParserPlus())->parse(input3);
    QCOMPARE(result3, 12);

    Input input4("12+");
    QVERIFY_EXCEPTION_THROWN(Chainl1(Decimal(), new ParserPlus())->parse(input4), ParserException);

    Input input5("8/4/2");
    auto result5 = Chainl1(Decimal(), new ParserDiv())->parse(input5);
    QCOMPARE(result5, (8 / 4) / 2);
}

void CombinatorTest::testChainr()
{
    Input input("12+34+56");
    auto result = Chainr(Decimal(), new ParserPlus(), 0)->parse(input);
    QCOMPARE(result, 12 + 34 + 56);

    Input input2("");
    auto result2 = Chainr(Decimal(), new ParserPlus(), 0)->parse(input2);
    QCOMPARE(result2, 0);

    Input input3("12");
    auto result3 = Chainr(Decimal(), new ParserPlus(), 0)->parse(input3);
    QCOMPARE(result3, 12);

    Input input4("12+");
    QVERIFY_EXCEPTION_THROWN(Chainl(Decimal(), new ParserPlus(), 0)->parse(input4), ParserException);

    Input input5("8/4/2");
    auto result5 = Chainr(Decimal(), new ParserDiv(), 0)->parse(input5);
    QCOMPARE(result5, 8 / (4 / 2));
}

void CombinatorTest::testChainr1()
{
    Input input("12+34+56");
    auto result = Chainr1(Decimal(), new ParserPlus())->parse(input);
    QCOMPARE(result, 12 + 34 + 56);

    Input input2("");
    QVERIFY_EXCEPTION_THROWN(Chainr1(Decimal(), new ParserPlus())->parse(input2), ParserException);

    Input input3("12");
    auto result3 = Chainr1(Decimal(), new ParserPlus())->parse(input3);
    QCOMPARE(result3, 12);

    Input input4("12+");
    QVERIFY_EXCEPTION_THROWN(Chainr1(Decimal(), new ParserPlus())->parse(input4), ParserException);

    Input input5("8/4/2");
    auto result5 = Chainr1(Decimal(), new ParserDiv())->parse(input5);
    QCOMPARE(result5, 8 / (4 / 2));
}

void CombinatorTest::testLookAhead()
{
    Input input("abc123");

    auto abc = LookAhead(Str("abc"))->parse(input);
    QCOMPARE(abc, QStringLiteral("abc"));

    QVERIFY_EXCEPTION_THROWN(Str("123")->parse(input), ParserException);

    auto abc2 = Str("abc")->parse(input);
    QCOMPARE(abc2, QStringLiteral("abc"));

    QVERIFY_EXCEPTION_THROWN(Str("abc")->parse(input), ParserException);

    auto onetwothree = LookAhead(Str("123"))->parse(input);
    QCOMPARE(onetwothree, QStringLiteral("123"));
}

DECLARE_TEST(CombinatorTest)

#include "tst_combinator.moc"
