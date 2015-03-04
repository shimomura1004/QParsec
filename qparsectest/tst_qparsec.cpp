#include <QObject>
#include <QtTest>
#include "autotest.h"

#include "qparsec.h"
#include "character/character.h"
#include "combinator/combinator.h"

using namespace qparsec;
using namespace character;
using namespace combinator;

class QParsecTest : public QObject
{
    Q_OBJECT

public:
    QParsecTest(){}

private Q_SLOTS:
//    void testApply();
////    // apply2
////    // empty
//    void testFail();
//    void testHelp();
////    // ignore
////    void testLazy();
//    void testLeft();
//    void testPair();
//    void testReturn();
//    void testRight();
//    void testSeq();
//    void testTry();
};

//void QParsecTest::testApply()
//{
//    Input input("hello");
//    // you can't omit template arguments
//    auto result = Apply<QString, QString>(Str("hello"), [](QString s){return s.toUpper();})->parse(input);
//    QCOMPARE(result, QString("HELLO"));

//    Input input2("123");
//    auto result2 = Apply<QString, int>(Many1(Digit()), [](QString s){return s.toInt();})->parse(input2);
//    QCOMPARE(result2, 123);

//    Input input3("abc");
//    auto p = Apply<QString, int>(Many1(Digit()), [](QString s){return s.toInt();});
//    QVERIFY_EXCEPTION_THROWN(p->parse(input3), ParserException);
//}

//void QParsecTest::testFail()
//{
//    Input input("a");

//    try {
//        Fail("fail parser")->parse(input);
//        QFAIL("Unreachable");
//    }
//    catch (const ParserException &e) {
//        QCOMPARE(e.reason(), QString("fail parser"));
//    }

//    QChar a = Char('a')->parse(input);
//    QCOMPARE(a, QChar('a'));
//}

//void QParsecTest::testHelp()
//{
//    Input input("a");

//    try {
//        Help(Fail("fail message"), "help message")->parse(input);
//        QFAIL("Unreachable");
//    }
//    catch (const ParserException &e) {
//        QCOMPARE(e.reason(), QString("help message"));
//    }
//}

//void QParsecTest::testLeft()
//{
//    Input input1("123abc");
//    auto num = Left(Many1(Digit()), Str("abc"))->parse(input1);
//    QCOMPARE(num, QStringLiteral("123"));
//}

//void QParsecTest::testPair()
//{
//    Input input("(x 3)");
//    auto pair = Parens(Pair(Lexeme(AnyChar()), Lexeme(Digit())))->parse(input);
//    QCOMPARE(pair.first, QChar('x'));
//    QCOMPARE(pair.second, QChar('3'));
//}

//void QParsecTest::testReturn()
//{
//    Input input("");
//    auto a = Return(QString("a"))->parse(input);
//    QCOMPARE(a, QString("a"));
//}

//void QParsecTest::testRight()
//{
//    Input input("var x = 1;");
//    auto varname = Right(Symbol("var"), ManyTill(AnyChar(), Space()))->parse(input);
//    QString name;
//    for(auto n : varname)
//        name += n;
//    QCOMPARE(name, QString("x"));
//}

//void QParsecTest::testSeq()
//{
//    Input input("abc");

//    Seq(Char('a'), Char('b'), Char('c'))->parse(input);
//    Eof()->parse(input);
//}

//void QParsecTest::testTry()
//{
//    Input input("abc");

//    QVERIFY_EXCEPTION_THROWN(Try(Seq(Char('a'), Char('b'), Char('a')))->parse(input), ParserException);

//    QChar a = Char('a')->parse(input);
//    QChar b = Char('b')->parse(input);
//    QChar c = Char('c')->parse(input);

//    QCOMPARE(a, QChar('a'));
//    QCOMPARE(b, QChar('b'));
//    QCOMPARE(c, QChar('c'));
//}

//Parser<QString> *HelloInNestedParens(){
//    return Choice({Str("hello"), Between(HelloInNestedParens(), Char('('), Char(')'))});
//}
//Parser<QString> *HelloInLazyNestedParens(){
//    Parser<QString>*(*child)() = [](){return Between(HelloInLazyNestedParens(), Char('('), Char(')'));};
//    return Choice({Str("hello"), Lazy(child)});
//}
//void QParsecTest::testLazy()
//{
//    Input input("(((hello)))");
//    // cause stack overflow
//    // auto hello = HelloInNestedParens()->parse(input);
//    auto hello = HelloInLazyNestedParens()->parse(input);
//    QCOMPARE(hello, QString("hello"));
//}

DECLARE_TEST(QParsecTest)

#include "tst_qparsec.moc"
