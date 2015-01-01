#include <QString>
#include <QtTest>
#include <QDebug>

#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>
#include <QParsecToken.h>

using namespace qparsec;

class QParsecTestTest : public QObject
{
    Q_OBJECT

    struct ParserPlus : Parser<int(*)(int,int)> {
        int (*parse(Input &input))(int, int) {
            Char('+')->parse(input);
            return [](int x, int y){ return x + y; };
        }
    };
    struct ParserDiv : Parser<int(*)(int,int)> {
        int (*parse(Input &input))(int, int) {
            Char('/')->parse(input);
            return [](int x, int y){ return x / y; };
        }
    };

public:
    QParsecTestTest();

private Q_SLOTS:
    void testSeq();
    void testTry();
    void testLeft();
    void testRight();
    void testFail();
    void testHelp();
    void testApply();
    void testLazy();

    void testOneOf();
    void testNoneOf();
    void testChar();
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
    void testManyTill();
    void testChainl();
    void testChaninl1();
    void testChainr();
    void testChainr1();

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

QParsecTestTest::QParsecTestTest()
{
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

void QParsecTestTest::testLeft()
{
    Input input1("123abc");
    auto num = Left(Decimal(), Str("abc"))->parse(input1);
    QCOMPARE(num, 123);
}

void QParsecTestTest::testRight()
{
    Input input("var x = 1;");
    auto varname = Right(Symbol("var"), ManyTill(AnyChar(), Space()))->parse(input);
    QString name;
    Q_FOREACH(auto n, varname)
        name += n;
    QCOMPARE(name, QString("x"));
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

void QParsecTestTest::testApply()
{
    Input input("hello");
    // you can't omit template arguments
    auto result = Apply<QString, QString>(Str("hello"), [](QString s){return s.toUpper();})->parse(input);
    QCOMPARE(result, QString("HELLO"));

    Input input2("123");
    auto result2 = Apply<QString, int>(Many1(Digit()), [](QString s){return s.toInt();})->parse(input2);
    QCOMPARE(result2, 123);

    Input input3("abc");
    auto p = Apply<QString, int>(Many1(Digit()), [](QString s){return s.toInt();});
    QVERIFY_EXCEPTION_THROWN(p->parse(input3), ParserException);
}

Parser<QString> *HelloInNestedParens(){
    return Choice({Str("hello"), Parens(HelloInNestedParens())});
}
Parser<QString> *HelloInLazyNestedParens(){
    Parser<QString>*(*child)() = [](){return Parens(HelloInLazyNestedParens());};
    return Choice({Str("hello"), Lazy(child)});
}
void QParsecTestTest::testLazy()
{
    Input input("(((hello)))");
    // cause stack overflow
    // auto hello = HelloInNestedParens()->parse(input);
    auto hello = HelloInLazyNestedParens()->parse(input);
    QCOMPARE(hello, QString("hello"));
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

void QParsecTestTest::testManyTill()
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

void QParsecTestTest::testChainl()
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

void QParsecTestTest::testChaninl1()
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

void QParsecTestTest::testChainr()
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

void QParsecTestTest::testChainr1()
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

void QParsecTestTest::testWhiteSpace()
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

void QParsecTestTest::testLexeme()
{
    Input input("a   b");
    auto a = Lexeme(Char('a'))->parse(input);
    QCOMPARE(a, QChar('a'));
    QCOMPARE(input.str(), QString("b"));
    auto b = Lexeme(Char('b'))->parse(input);
    QCOMPARE(b, QChar('b'));
    QVERIFY_EXCEPTION_THROWN(Lexeme(Char('c'))->parse(input), ParserException);
}

void QParsecTestTest::testSymbol()
{
    Input input("+ 1");
    auto plus = Symbol("+")->parse(input);
    QCOMPARE(plus, QString("+"));
    QCOMPARE(input.str(), QString("1"));
    auto one = Symbol("1")->parse(input);
    QCOMPARE(one, QString("1"));
    QVERIFY_EXCEPTION_THROWN(Symbol("2")->parse(input), ParserException);
}

void QParsecTestTest::testParens()
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

void QParsecTestTest::testBraces()
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

void QParsecTestTest::testBrackets()
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

void QParsecTestTest::testSquares()
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

void QParsecTestTest::testSemiSep()
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

void QParsecTestTest::testSemiSep1()
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

void QParsecTestTest::testCommaSep()
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

void QParsecTestTest::testCommaSep1()
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

void QParsecTestTest::testDecimal()
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

void QParsecTestTest::testHexadecimal()
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

void QParsecTestTest::testOctal()
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

void QParsecTestTest::testNatural()
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

void QParsecTestTest::testInteger()
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

void QParsecTestTest::testDouble()
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

QTEST_APPLESS_MAIN(QParsecTestTest)

#include "tst_qparsectesttest.moc"
