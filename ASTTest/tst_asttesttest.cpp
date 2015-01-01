#include <QString>
#include <QtTest>

#include <lisp/Ast.h>
#include <QDebug>

using namespace lisp::ast;

class ASTTestTest : public QObject
{
    Q_OBJECT

public:
    ASTTestTest();

private Q_SLOTS:
    void testCreate();
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
    qDebug() << Int::create(123)->toString();
    qDebug() << Undef::create()->toString();
    qDebug() << List::create({Bool::create(true),
                              Undef::create(),
                              Int::create(10)})->toString();
    qDebug() << Apply::create(Undef::create(), {
                                  String::create("hello"),
                                  Bool::create(true)})->toString();
    qDebug() << Lambda::create({"a", "b"},
                               Bool::create(false),
                               Env())->toString();
    qDebug() << Define::create("plus1",
                               Lambda::create({"n"},
                                              List::create({Symbol::create("+"),
                                                            Int::create(1)}),
                                              Env()))->toString();
    qDebug() << If::create(Bool::create(true), String::create("true"), String::create("false"))->toString();
    qDebug() << Let::create({ QPair<QString, QSharedPointer<Val>>("x", Int::create(3)),
                              QPair<QString, QSharedPointer<Val>>("y", String::create("hello"))
                            },
                            Symbol::create("x"))->toString();
    qDebug() << Sequence::create({Int::create(3), String::create("hello")})->toString();
}

QTEST_APPLESS_MAIN(ASTTestTest)

#include "tst_asttesttest.moc"
