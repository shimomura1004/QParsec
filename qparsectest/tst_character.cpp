#include <QObject>
#include <QtTest>
#include "autotest.h"

#include "parser.h"
#include "character/character.h"

using namespace qparsec;
using namespace character;

class CharacterTest : public QObject
{
    Q_OBJECT

public:
    CharacterTest(){}

private Q_SLOTS:
    void testOneOf();
    void testNoneOf();
    void testChar();
    void testStr();
    void testAnyChar();
    void testSpace();
    void testSpaces();
    void testNewline();
    void testTab();
    void testUpper();
    void testLower();
    void testAlphanum();
    void testLetter();
    void testDigit();
    void testHexdigit();
    void testOctdigit();
    void testSatisfy();
};

void CharacterTest::testOneOf() {
    Input input("a0");

    auto a = OneOf("abc")->parse(input);
    QCOMPARE(a, QChar('a'));

    QVERIFY_EXCEPTION_THROWN(OneOf("abc")->parse(input), ParserException);
}

void CharacterTest::testNoneOf() {
    Input input("a0");

    auto a = NoneOf("0123456789")->parse(input);
    QCOMPARE(a, QChar('a'));

    QVERIFY_EXCEPTION_THROWN(NoneOf("0123456789")->parse(input), ParserException);
}

void CharacterTest::testChar() {
    Input input("abc");
    auto a = Char('a')->parse(input);
    QCOMPARE(a, QChar('a'));

    QChar b = Char('b')->parse(input);
    QCOMPARE(b, QChar('b'));

    QVERIFY_EXCEPTION_THROWN(Char('a')->parse(input), ParserException);

    auto c = Char(QChar('c'))->parse(input);
    QCOMPARE(c, QChar('c'));

    QVERIFY_EXCEPTION_THROWN(Char('a')->parse(input), ParserException);
}

void CharacterTest::testStr() {
    Input input("helloworld");

    auto hello = Str("hello")->parse(input);
    QCOMPARE(hello, QString("hello"));

    QVERIFY_EXCEPTION_THROWN(Str("world!!!")->parse(input), ParserException);
}

void CharacterTest::testAnyChar() {
    Input input("a0*");

    QChar a = AnyChar()->parse(input);
    QChar b = AnyChar()->parse(input);
    QChar c = AnyChar()->parse(input);

    QCOMPARE(a, QChar('a'));
    QCOMPARE(b, QChar('0'));
    QCOMPARE(c, QChar('*'));

    QVERIFY_EXCEPTION_THROWN(AnyChar()->parse(input), ParserException);
}

void CharacterTest::testSpace() {
    Input input(" \ta");

    QChar space = Space()->parse(input);
    QChar tab = Space()->parse(input);
    QCOMPARE(space, QChar(' '));
    QCOMPARE(tab, QChar('\t'));

    QVERIFY_EXCEPTION_THROWN(Space()->parse(input), ParserException);
}

void CharacterTest::testSpaces()
{
    Input input("   ");
    Spaces()->parse(input);
    QCOMPARE(input.str(), QString(""));

    Input input2(" hello");
    Spaces()->parse(input2);
    QCOMPARE(input2.str(), QString("hello"));
    Spaces()->parse(input2);
    QCOMPARE(input2.str(), QString("hello"));
    auto hello = Str("hello")->parse(input2);
    QCOMPARE(hello, QString("hello"));
}

void CharacterTest::testNewline()
{
    Input input("\n");
    auto result1 = Newline()->parse(input);
    QCOMPARE(result1, QChar('\n'));

    Input input2("\nhello");
    auto result2 = Newline()->parse(input2);
    QCOMPARE(result2, QChar('\n'));
    QCOMPARE(input2.str(), QString("hello"));

    QVERIFY_EXCEPTION_THROWN(Newline()->parse(input2), ParserException);
}

void CharacterTest::testTab()
{
    Input input("\t");
    auto result1 = Tab()->parse(input);
    QCOMPARE(result1, QChar('\t'));

    Input input2("\thello");
    auto result2 = Tab()->parse(input2);
    QCOMPARE(result2, QChar('\t'));
    QCOMPARE(input2.str(), QString("hello"));

    QVERIFY_EXCEPTION_THROWN(Tab()->parse(input2), ParserException);
}

void CharacterTest::testUpper()
{
    Input input1("Aa");
    auto A = Upper()->parse(input1);
    QCOMPARE(A, QChar('A'));

    QVERIFY_EXCEPTION_THROWN(Upper()->parse(input1), ParserException);

    Input input2("0");
    QVERIFY_EXCEPTION_THROWN(Upper()->parse(input2), ParserException);
}

void CharacterTest::testLower()
{
    Input input1("aA");
    auto a = Lower()->parse(input1);
    QCOMPARE(a, QChar('a'));

    QVERIFY_EXCEPTION_THROWN(Lower()->parse(input1), ParserException);

    Input input2("0");
    QVERIFY_EXCEPTION_THROWN(Lower()->parse(input2), ParserException);
}

void CharacterTest::testAlphanum()
{
    Input input1("aA0*");
    auto a = Alphanum()->parse(input1);
    QCOMPARE(a, QChar('a'));
    auto A = Alphanum()->parse(input1);
    QCOMPARE(A, QChar('A'));
    auto zero = Alphanum()->parse(input1);
    QCOMPARE(zero, QChar('0'));

    QVERIFY_EXCEPTION_THROWN(Alphanum()->parse(input1), ParserException);
}

void CharacterTest::testLetter()
{
    Input input1("aA0");
    auto a = Letter()->parse(input1);
    QCOMPARE(a, QChar('a'));
    auto A = Letter()->parse(input1);
    QCOMPARE(A, QChar('A'));

    QVERIFY_EXCEPTION_THROWN(Letter()->parse(input1), ParserException);
}

void CharacterTest::testDigit() {
    Input input("1a");

    auto n = Digit()->parse(input);
    QCOMPARE(n, QChar('1'));

    QVERIFY_EXCEPTION_THROWN(Digit()->parse(input), ParserException);
}

void CharacterTest::testHexdigit()
{
    Input input1("aA0G");
    auto a = Hexadigit()->parse(input1);
    QCOMPARE(a, QChar('a'));
    auto A = Hexadigit()->parse(input1);
    QCOMPARE(A, QChar('A'));
    auto zero = Hexadigit()->parse(input1);
    QCOMPARE(zero, QChar('0'));

    QVERIFY_EXCEPTION_THROWN(Hexadigit()->parse(input1), ParserException);
}

void CharacterTest::testOctdigit()
{
    Input input1("078");
    auto zero = Octdigit()->parse(input1);
    QCOMPARE(zero, QChar('0'));
    auto seven = Octdigit()->parse(input1);
    QCOMPARE(seven, QChar('7'));

    QVERIFY_EXCEPTION_THROWN(Octdigit()->parse(input1), ParserException);
}

void CharacterTest::testSatisfy()
{
    Input input1("0a");
    bool(*isDigit)(QChar) = [](QChar c){
        return c.isDigit();
    };
    auto zero = Satisfy(isDigit)->parse(input1);
    QCOMPARE(zero, QChar('0'));

    QVERIFY_EXCEPTION_THROWN(Satisfy(isDigit)->parse(input1), ParserException);
}

DECLARE_TEST(CharacterTest)

#include "tst_character.moc"
