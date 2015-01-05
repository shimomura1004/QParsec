#include <QString>
#include <QtTest>

#include <lisp/ast.h>
#include <lisp/parser.h>
#include <QDebug>

using namespace ast;
using namespace qparsec;

class ASTTestTest : public QObject
{
    Q_OBJECT

public:
    ASTTestTest();

private Q_SLOTS:
    void testCreate();
    void testBasic();
};

ASTTestTest::ASTTestTest()
{
}

void ASTTestTest::testCreate()
{
    qDebug() << Symbol::create("hello")->toString();
    qDebug() << Char::create('a')->toString();
    qDebug() << String::create("hello")->toString();
    qDebug() << Bool::create(true)->toString();
    qDebug() << Integer::create(123)->toString();
    qDebug() << Undef::create()->toString();
    qDebug() << List::create({Bool::create(true),
                              Undef::create(),
                              Integer::create(10)})->toString();
    qDebug() << Apply::create(Undef::create(), {
                                  String::create("hello"),
                                  Bool::create(true)})->toString();
    qDebug() << Lambda::create({"a", "b"},
                               Bool::create(false),
                               Env())->toString();
    qDebug() << Define::create("plus1",
                               Lambda::create({"n"},
                                              List::create({Symbol::create("+"),
                                                            Integer::create(1)}),
                                              Env()))->toString();
    qDebug() << If::create(Bool::create(true), String::create("true"), String::create("false"))->toString();
    qDebug() << Let::create({ QPair<QString, QSharedPointer<Val>>("x", Integer::create(3)),
                              QPair<QString, QSharedPointer<Val>>("y", String::create("hello"))
                            },
                            Symbol::create("x"))->toString();
    qDebug() << Sequence::create({Integer::create(3), String::create("hello")})->toString();
}

void ASTTestTest::testBasic()
{
    Input list("'(1  2 3)");
    //qDebug() << parser::Val()->parse(list)->toString();

    Input lambda("(lambda (x) '(123 45))");
    //qDebug() << parser::Val()->parse(lambda)->toString();
}

QTEST_APPLESS_MAIN(ASTTestTest)

#include "tst_asttesttest.moc"
