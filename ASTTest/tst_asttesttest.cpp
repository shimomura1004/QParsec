#include <QString>
#include <QtTest>

#include <../Interpreter/ast/AST.h>
#include <QDebug>

using namespace AST;

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
    qDebug() << LispSymbol::create("hello")->toString();
    qDebug() << LispChar::create('a')->toString();
    qDebug() << LispString::create("hello")->toString();
    qDebug() << LispBool::create(true)->toString();
    qDebug() << LispInt::create(123)->toString();
    qDebug() << LispUndef::create()->toString();
    qDebug() << LispList::create({LispBool::create(true),
                                  LispUndef::create(),
                                  LispInt::create(10)})->toString();
    qDebug() << LispApply::create(LispUndef::create(), {
                                      LispString::create("hello"),
                                      LispBool::create(true)})->toString();
    qDebug() << LispLambda::create({"a", "b"},
                                   LispBool::create(false),
                                   Env())->toString();
    qDebug() << LispDefine::create("plus1",
                                   LispLambda::create({"n"},
                                                      LispList::create({LispSymbol::create("+"),
                                                                        LispInt::create(1)}),
                                                      Env()))->toString();
    qDebug() << LispIf::create(LispBool::create(true), LispString::create("true"), LispString::create("false"))->toString();
    qDebug() << LispLet::create({ QPair<QString, QSharedPointer<LispVal>>("x", LispInt::create(3)),
                                  QPair<QString, QSharedPointer<LispVal>>("y", LispString::create("hello"))
                                },
                                LispSymbol::create("x"))->toString();
    qDebug() << LispSequence::create({LispInt::create(3), LispString::create("hello")})->toString();
}

QTEST_APPLESS_MAIN(ASTTestTest)

#include "tst_asttesttest.moc"
