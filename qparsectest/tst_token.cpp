#include <QObject>
#include <QtTest>
#include "autotest.h"

#include "qparsec.h"
#include "character/character.h"
#include "number/number.h"
#include "token/token.h"

using namespace qparsec;
using namespace character;
using namespace number;
using namespace token;

class TokenTest : public QObject
{
    Q_OBJECT

public:
    TokenTest(){}

private Q_SLOTS:
    void testWhiteSpace();
    void testLexeme();
    void testSymbol();
    void testParens();
    void testBraces();
    void testBrackets();
    void testSquares();
    void testSemiSep();
    void testSemiSep1();
    void testCommaSep();
    void testCommaSep1();
    void testDecimal();
    void testHexadecimal();
    void testOctal();
    void testNatural();
    void testInteger();
    void testDouble();
};

void TokenTest::testWhiteSpace()
{
    Input input("   ");
    WhiteSpace()->parse(input);
    QCOMPARE(input.str(), QString(""));

    Input input2(" hello");
    WhiteSpace()->parse(input2);
    QCOMPARE(input2.str(), QString("hello"));
    WhiteSpace()->parse(input2);
    QCOMPARE(input2.str(), QString("hello"));
    auto hello = Str("hello")->parse(input2);
    QCOMPARE(hello, QString("hello"));
}

void TokenTest::testLexeme()
{
    Input input("a   b");
    auto a = Lexeme(Char('a'))->parse(input);
    QCOMPARE(a, QChar('a'));
    QCOMPARE(input.str(), QString("b"));
    auto b = Lexeme(Char('b'))->parse(input);
    QCOMPARE(b, QChar('b'));
    QVERIFY_EXCEPTION_THROWN(Lexeme(Char('c'))->parse(input), ParserException);
}

void TokenTest::testSymbol()
{
    Input input("+ 1");
    auto plus = Symbol("+")->parse(input);
    QCOMPARE(plus, QString("+"));
    QCOMPARE(input.str(), QString("1"));
    auto one = Symbol("1")->parse(input);
    QCOMPARE(one, QString("1"));
    QVERIFY_EXCEPTION_THROWN(Symbol("2")->parse(input), ParserException);
}

void TokenTest::testParens()
{
    Input input("(hello)");
    auto hello = Parens(Str("hello"))->parse(input);
    QCOMPARE(hello, QString("hello"));

    Input input2("(hello");
    QVERIFY_EXCEPTION_THROWN(Parens(Str("hello"))->parse(input2), ParserException);
    Input input3("hello)");
    QVERIFY_EXCEPTION_THROWN(Parens(Str("hello"))->parse(input3), ParserException);
    Input input4("()");
    QVERIFY_EXCEPTION_THROWN(Parens(Str("hello"))->parse(input4), ParserException);
    Input input5("hello");
    QVERIFY_EXCEPTION_THROWN(Parens(Str("hello"))->parse(input5), ParserException);
}

void TokenTest::testBraces()
{
    Input input("{hello}");
    auto hello = Braces(Str("hello"))->parse(input);
    QCOMPARE(hello, QString("hello"));

    Input input2("{hello");
    QVERIFY_EXCEPTION_THROWN(Braces(Str("hello"))->parse(input2), ParserException);
    Input input3("hello}");
    QVERIFY_EXCEPTION_THROWN(Braces(Str("hello"))->parse(input3), ParserException);
    Input input4("{}");
    QVERIFY_EXCEPTION_THROWN(Braces(Str("hello"))->parse(input4), ParserException);
    Input input5("hello");
    QVERIFY_EXCEPTION_THROWN(Braces(Str("hello"))->parse(input5), ParserException);
}

void TokenTest::testBrackets()
{
    Input input("<hello>");
    auto hello = Brackets(Str("hello"))->parse(input);
    QCOMPARE(hello, QString("hello"));

    Input input2("<hello");
    QVERIFY_EXCEPTION_THROWN(Brackets(Str("hello"))->parse(input2), ParserException);
    Input input3("hello>");
    QVERIFY_EXCEPTION_THROWN(Brackets(Str("hello"))->parse(input3), ParserException);
    Input input4("<>");
    QVERIFY_EXCEPTION_THROWN(Brackets(Str("hello"))->parse(input4), ParserException);
    Input input5("hello");
    QVERIFY_EXCEPTION_THROWN(Brackets(Str("hello"))->parse(input5), ParserException);
}

void TokenTest::testSquares()
{
    Input input("[hello]");
    auto hello = Squares(Str("hello"))->parse(input);
    QCOMPARE(hello, QString("hello"));

    Input input2("[hello");
    QVERIFY_EXCEPTION_THROWN(Squares(Str("hello"))->parse(input2), ParserException);
    Input input3("hello]");
    QVERIFY_EXCEPTION_THROWN(Squares(Str("hello"))->parse(input3), ParserException);
    Input input4("[]");
    QVERIFY_EXCEPTION_THROWN(Squares(Str("hello"))->parse(input4), ParserException);
    Input input5("hello");
    QVERIFY_EXCEPTION_THROWN(Squares(Str("hello"))->parse(input5), ParserException);
}

void TokenTest::testSemiSep()
{
    Input input("0; 10;  2;  300");

    auto nums = SemiSep(Many1(Digit()))->parse(input);
    QCOMPARE(nums.length(), 4);
    QCOMPARE(nums[0], QString("0"));
    QCOMPARE(nums[1], QString("10"));
    QCOMPARE(nums[2], QString("2"));
    QCOMPARE(nums[3], QString("300"));

    Input single("100");
    auto num = SemiSep(Many1(Digit()))->parse(single);
    QCOMPARE(num.length(), 1);
    QCOMPARE(num[0], QString("100"));

    Input empty("");
    auto empty_ = SemiSep(Many1(Digit()))->parse(empty);
    QCOMPARE(empty_.length(), 0);

    Input endWithSep("10; 23;  ");
    QVERIFY_EXCEPTION_THROWN(SemiSep(Many1(Digit()))->parse(endWithSep), ParserException);
}

void TokenTest::testSemiSep1()
{
    Input input("0; 10;  2;  300");

    auto nums = SemiSep1(Many1(Digit()))->parse(input);
    QCOMPARE(nums.length(), 4);
    QCOMPARE(nums[0], QString("0"));
    QCOMPARE(nums[1], QString("10"));
    QCOMPARE(nums[2], QString("2"));
    QCOMPARE(nums[3], QString("300"));

    Input single("100");
    auto num = SemiSep1(Many1(Digit()))->parse(single);
    QCOMPARE(num.length(), 1);
    QCOMPARE(num[0], QString("100"));

    Input empty("");
    QVERIFY_EXCEPTION_THROWN(SemiSep1(Many1(Digit()))->parse(empty), ParserException);

    Input endWithSep("10; 23;  ");
    QVERIFY_EXCEPTION_THROWN(SemiSep1(Many1(Digit()))->parse(endWithSep), ParserException);
}

void TokenTest::testCommaSep()
{
    Input input("0, 10,  2,  300");

    auto nums = CommaSep(Many1(Digit()))->parse(input);
    QCOMPARE(nums.length(), 4);
    QCOMPARE(nums[0], QString("0"));
    QCOMPARE(nums[1], QString("10"));
    QCOMPARE(nums[2], QString("2"));
    QCOMPARE(nums[3], QString("300"));

    Input single("100");
    auto num = CommaSep(Many1(Digit()))->parse(single);
    QCOMPARE(num.length(), 1);
    QCOMPARE(num[0], QString("100"));

    Input empty("");
    auto empty_ = CommaSep(Many1(Digit()))->parse(empty);
    QCOMPARE(empty_.length(), 0);

    Input endWithSep("10, 23,  ");
    QVERIFY_EXCEPTION_THROWN(CommaSep(Many1(Digit()))->parse(endWithSep), ParserException);
}

void TokenTest::testCommaSep1()
{
    Input input("0, 10,  2,  300");

    auto nums = CommaSep1(Many1(Digit()))->parse(input);
    QCOMPARE(nums.length(), 4);
    QCOMPARE(nums[0], QString("0"));
    QCOMPARE(nums[1], QString("10"));
    QCOMPARE(nums[2], QString("2"));
    QCOMPARE(nums[3], QString("300"));

    Input single("100");
    auto num = CommaSep1(Many1(Digit()))->parse(single);
    QCOMPARE(num.length(), 1);
    QCOMPARE(num[0], QString("100"));

    Input empty("");
    QVERIFY_EXCEPTION_THROWN(CommaSep1(Many1(Digit()))->parse(empty), ParserException);

    Input endWithSep("10, 23,  ");
    QVERIFY_EXCEPTION_THROWN(CommaSep1(Many1(Digit()))->parse(endWithSep), ParserException);
}

void TokenTest::testDecimal()
{
    Input input1("123");
    auto result1 = Decimal()->parse(input1);
    QCOMPARE(result1, 123);

    Input input2("-123");
    QVERIFY_EXCEPTION_THROWN(Decimal()->parse(input2), ParserException);

    Input input3("");
    QVERIFY_EXCEPTION_THROWN(Decimal()->parse(input3), ParserException);

    Input input4("deadbeaf");
    QVERIFY_EXCEPTION_THROWN(Decimal()->parse(input4), ParserException);
}

void TokenTest::testHexadecimal()
{
    Input input1("0x123");
    auto result1 = Hexadecimal()->parse(input1);
    QCOMPARE(result1, 0x123);

    Input input2("0x-123");
    QVERIFY_EXCEPTION_THROWN(Hexadecimal()->parse(input2), ParserException);

    Input input3("");
    QVERIFY_EXCEPTION_THROWN(Hexadecimal()->parse(input3), ParserException);

    Input input4("0xbeaf");
    auto result4 = Hexadecimal()->parse(input4);
    QCOMPARE(result4, 0xbeaf);
}

void TokenTest::testOctal()
{
    Input input1("0o123");
    auto result1 = Octal()->parse(input1);
    QCOMPARE(result1, 0123);

    Input input2("Oo-123");
    QVERIFY_EXCEPTION_THROWN(Octal()->parse(input2), ParserException);

    Input input3("");
    QVERIFY_EXCEPTION_THROWN(Octal()->parse(input3), ParserException);

    Input input4("0o9");
    QVERIFY_EXCEPTION_THROWN(Octal()->parse(input4), ParserException);
}

void TokenTest::testNatural()
{
    Input dec("12345");
    QCOMPARE(Natural()->parse(dec), 12345);

    Input hex("0x123");
    QCOMPARE(Natural()->parse(hex), 0x123);

    Input oct("0o123");
    QCOMPARE(Natural()->parse(oct), 0123);

    Input empty("");
    QVERIFY_EXCEPTION_THROWN(Natural()->parse(empty), ParserException);

    Input err("hello");
    QVERIFY_EXCEPTION_THROWN(Natural()->parse(err), ParserException);
}

void TokenTest::testInteger()
{
    Input input1("123");
    QCOMPARE(Integer()->parse(input1), 123);
    Input input2("+123");
    QCOMPARE(Integer()->parse(input2), 123);
    Input input3("-123");
    QCOMPARE(Integer()->parse(input3), -123);
    Input input4("+0x123");
    QCOMPARE(Integer()->parse(input4), 0x123);
    Input input5("-0x123");
    QCOMPARE(Integer()->parse(input5), -0x123);
}

void TokenTest::testDouble()
{
    Input input1("123.45");
    QCOMPARE(Double()->parse(input1), 123.45);
    Input input2("-123.45");
    QCOMPARE(Double()->parse(input2), -123.45);
    Input input3("123e2");
    QCOMPARE(Double()->parse(input3), 123e2);
    Input input4("-123e2");
    QCOMPARE(Double()->parse(input4), -123e2);
    Input input5("123e-2");
    QCOMPARE(Double()->parse(input5), 123e-2);
    Input input6("-123E-2");
    QCOMPARE(Double()->parse(input6), -123E-2);
    Input input7("123.");
    QVERIFY_EXCEPTION_THROWN(Double()->parse(input7), ParserException);
    Input input8("123E");
    QVERIFY_EXCEPTION_THROWN(Double()->parse(input8), ParserException);
    Input input9("123.e2");
    QVERIFY_EXCEPTION_THROWN(Double()->parse(input9), ParserException);
    Input input10("123.-2");
    QVERIFY_EXCEPTION_THROWN(Double()->parse(input10), ParserException);
    Input input11(".5");
    QCOMPARE(Double()->parse(input11), .5);
    Input input12("-.5");
    QCOMPARE(Double()->parse(input12), -.5);
}

DECLARE_TEST(TokenTest)

#include "tst_token.moc"
